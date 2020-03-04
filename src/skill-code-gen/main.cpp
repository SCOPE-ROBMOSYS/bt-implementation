/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "qscxmlcompiler_p.h"
#include <QTextCodec>
#include <QCommandLineParser>
#include <QCommandLineOption>

struct TranslationUnit
{
    TranslationUnit()
        : stateMethods(false)
        , mainDocument(nullptr)
    {}

    QString scxmlFileName;
    QString outHFileName, outCppFileName;
    QString namespaceName;
    bool stateMethods;
    DocumentModel::ScxmlDocument *mainDocument;
    QList<DocumentModel::ScxmlDocument *> allDocuments;
    QHash<DocumentModel::ScxmlDocument *, QString> classnameForDocument;
    QList<TranslationUnit *> dependencies;
};

enum {
    NoError = 0,
    CommandLineArgumentsError = -1,
    NoInputFilesError = -2,
    CannotOpenInputFileError = -3,
    ParseError = -4,
    CannotOpenOutputHeaderFileError = -5,
    CannotOpenOutputCppFileError = -6,
    ScxmlVerificationError = -7,
    NoTextCodecError = -8
};

int write(TranslationUnit *tu)
{
    QTextStream errs(stderr, QIODevice::WriteOnly);

    QFile outH(tu->outHFileName);
    if (!outH.open(QFile::WriteOnly)) {
        errs << QStringLiteral("Error: cannot open '%1': %2").arg(outH.fileName(), outH.errorString()) << endl;
        return CannotOpenOutputHeaderFileError;
    }

    QFile outCpp(tu->outCppFileName);
    if (!outCpp.open(QFile::WriteOnly)) {
        errs << QStringLiteral("Error: cannot open '%1': %2").arg(outCpp.fileName(), outCpp.errorString()) << endl;
        return CannotOpenOutputCppFileError;
    }

    // Make sure it outputs UTF-8, as that is what C++ expects.
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    if (!utf8) {
        errs << QStringLiteral("Error: cannot find a QTextCodec for generating UTF-8.");
        return NoTextCodecError;
    }

    QTextStream h(&outH);
    h.setCodec(utf8);
    h.setGenerateByteOrderMark(true);
    QTextStream c(&outCpp);
    c.setCodec(utf8);
    c.setGenerateByteOrderMark(true);
//     CppDumper dumper(h, c);
//     dumper.dump(tu);

    auto docs = tu->allDocuments;
//     tables.resize(docs.size());
//     metaDataInfos.resize(tables.size());
//     dataModelInfos.resize(tables.size());
//     factories.resize(tables.size());
    auto classnameForDocument = tu->classnameForDocument;

    for (int i = 0, ei = docs.size(); i != ei; ++i) {
        auto doc = docs.at(i);

        for (DocumentModel::AbstractState* astate : doc->allStates) {
            auto state = astate->asState();
            if (!state) {
                qDebug() << astate->id;
                continue;
            }
            qDebug() << state->id << state->xmlLocation.line << state->xmlLocation.column << state->dataElements.size() << state->bt_status;
        }



//         auto metaDataInfo = &metaDataInfos[i];
//         GeneratedTableData::build(doc, &tables[i], metaDataInfo, &dataModelInfos[i],
    }



    h.flush();
    outH.close();
    c.flush();
    outCpp.close();
    return NoError;
}

static void collectAllDocuments(DocumentModel::ScxmlDocument *doc,
                                QList<DocumentModel::ScxmlDocument *> *docs)
{
    docs->append(doc);
    for (DocumentModel::ScxmlDocument *subDoc : qAsConst(doc->allSubDocuments)) {
        collectAllDocuments(subDoc, docs);
    }
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationVersion(QString::fromLatin1("%1 (Qt %2)").arg(
                              QString::number(/*Q_QSCXMLC_OUTPUT_REVISION*/2),
                              QString::fromLatin1(QT_VERSION_STR)));

    QCommandLineParser cmdParser;
    QTextStream errs(stderr, QIODevice::WriteOnly);

    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.setApplicationDescription(QCoreApplication::translate("main",
                       "Compiles the given input.scxml file to a header and a cpp file."));

    QCommandLineOption optionNamespace(QLatin1String("namespace"),
                       QCoreApplication::translate("main", "Put generated code into <namespace>."),
                       QCoreApplication::translate("main", "namespace"));
    QCommandLineOption optionOutputBaseName(QStringList() << QLatin1String("o") << QLatin1String("output"),
                       QCoreApplication::translate("main", "Generate <name>.h and <name>.cpp files."),
                       QCoreApplication::translate("main", "name"));
    QCommandLineOption optionOutputHeaderName(QLatin1String("header"),
                       QCoreApplication::translate("main", "Generate <name> for the header file."),
                       QCoreApplication::translate("main", "name"));
    QCommandLineOption optionOutputSourceName(QLatin1String("impl"),
                       QCoreApplication::translate("main", "Generate <name> for the source file."),
                       QCoreApplication::translate("main", "name"));
    QCommandLineOption optionClassName(QLatin1String("classname"),
                       QCoreApplication::translate("main", "Generate <name> for state machine class name."),
                       QCoreApplication::translate("main", "name"));
    QCommandLineOption optionStateMethods(QLatin1String("statemethods"),
                       QCoreApplication::translate("main", "Generate read and notify methods for states"));

    cmdParser.addPositionalArgument(QLatin1String("input"),
                       QCoreApplication::translate("main", "Input SCXML file."));
    cmdParser.addOption(optionNamespace);
    cmdParser.addOption(optionOutputBaseName);
    cmdParser.addOption(optionOutputHeaderName);
    cmdParser.addOption(optionOutputSourceName);
    cmdParser.addOption(optionClassName);
    cmdParser.addOption(optionStateMethods);

    cmdParser.process(QCoreApplication::arguments());

    const QStringList inputFiles = cmdParser.positionalArguments();

    if (inputFiles.count() < 1) {
        errs << QCoreApplication::translate("main", "Error: no input file.") << endl;
        cmdParser.showHelp(NoInputFilesError);
    }

    if (inputFiles.count() > 1) {
        errs << QCoreApplication::translate("main", "Error: unexpected argument(s): %1")
                .arg(inputFiles.mid(1).join(QLatin1Char(' '))) << endl;
        cmdParser.showHelp(NoInputFilesError);
    }

    const QString scxmlFileName = inputFiles.at(0);

    TranslationUnit options;
    options.stateMethods = cmdParser.isSet(optionStateMethods);
    if (cmdParser.isSet(optionNamespace))
        options.namespaceName = cmdParser.value(optionNamespace);
    QString outFileName = cmdParser.value(optionOutputBaseName);
    QString outHFileName = cmdParser.value(optionOutputHeaderName);
    QString outCppFileName = cmdParser.value(optionOutputSourceName);
    QString mainClassName = cmdParser.value(optionClassName);

    if (outFileName.isEmpty())
        outFileName = QFileInfo(scxmlFileName).baseName();
    if (outHFileName.isEmpty())
        outHFileName = outFileName + QLatin1String(".h");
    if (outCppFileName.isEmpty())
        outCppFileName = outFileName + QLatin1String(".cpp");
    qDebug() << __LINE__;

    QFile file(scxmlFileName);
    if (!file.open(QFile::ReadOnly)) {
        errs << QStringLiteral("Error: cannot open input file %1").arg(scxmlFileName);
        return CannotOpenInputFileError;
    }
    qDebug() << __LINE__;

    QXmlStreamReader reader(&file);
    QScxmlCompiler compiler(&reader);
    compiler.setFileName(file.fileName());
    compiler.compile();
    if (!compiler.errors().isEmpty()) {
        const auto errors = compiler.errors();
        for (const QScxmlError &error : errors) {
            errs << error.toString() << endl;
        }
        return ParseError;
    }
    qDebug() << __LINE__;

    auto mainDoc = QScxmlCompilerPrivate::get(&compiler)->scxmlDocument();
    if (mainDoc == nullptr) {
        Q_ASSERT(!compiler.errors().isEmpty());
        const auto errors = compiler.errors();
        for (const QScxmlError &error : errors) {
            errs << error.toString() << endl;
        }
        return ScxmlVerificationError;
    }

    qDebug() << __LINE__;
    if (mainClassName.isEmpty())
        mainClassName = mainDoc->root->name;
    if (mainClassName.isEmpty()) {
        mainClassName = QFileInfo(scxmlFileName).fileName();
        int dot = mainClassName.lastIndexOf(QLatin1Char('.'));
        if (dot != -1)
            mainClassName = mainClassName.left(dot);
    }
    qWarning() << mainClassName;
    qDebug() << __LINE__;

    QList<DocumentModel::ScxmlDocument *> docs;
    collectAllDocuments(mainDoc, &docs);

    TranslationUnit tu = options;
    tu.allDocuments = docs;
    tu.scxmlFileName = QFileInfo(file).fileName();
    tu.mainDocument = mainDoc;
    tu.outHFileName = outHFileName;
    tu.outCppFileName = outCppFileName;
    tu.classnameForDocument.insert(mainDoc, mainClassName);

    docs.pop_front();

    for (DocumentModel::ScxmlDocument *doc : qAsConst(docs)) {
        auto name = doc->root->name;
        auto prefix = name;
        if (name.isEmpty()) {
            prefix = QStringLiteral("%1_StateMachine").arg(mainClassName);
            name = prefix;
        }

        int counter = 1;
        while (tu.classnameForDocument.key(name) != nullptr)
            name = QStringLiteral("%1_%2").arg(prefix).arg(++counter);

        tu.classnameForDocument.insert(doc, name);
    }

    return write(&tu);
}
