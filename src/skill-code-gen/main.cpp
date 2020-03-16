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

#include "iostream"
#include "vector"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTextStream>

using namespace std;

struct TranslationUnit
{
    TranslationUnit()
        : stateMethods(false)
        , mainDocument(nullptr)
    {}

    QString scxmlFileName;
    QString outHFileName,
            outCppFileName,
            outCppDataModelFileName,
            outHDataModelFileName; // 4 output files names - not used
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

string to_upper(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::toupper));
    return str;
}

string to_lower(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::tolower));
    return str;
}

void ReplaceKeyInsideTemplate (QString path_new_skill, QFile& input_file, QFile& output_file, QRegularExpression key, QString value){
    // open and read the template
    input_file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = input_file.readAll();
    // replace
    dataText.replace(key, value);
    // write the output file
    if(output_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file);
        out << dataText;
    }
    output_file.close();
}

struct State{
    QString id;
    int ReturnStatusEnum;
    string ReturnStatus;
};

struct Service{
    QString name_instance;
    QString service_type;
    QString component_type;
    QString client_port_name;
    };

struct Attribute{
    QString data_type;
    QString name_instance;
    QString value;
};

struct ClientPort{ // not really needed
    QString client_port_name;
};

struct SkillDescription {
    QString skill_name;
    vector<State> ListStates;
    vector<string> UsedComponents;
    vector<Service> UsedServices;
    vector<Attribute> UsedAttributes;
    vector<ClientPort> UsedClientPorts;
};

string DecoderEnum (int id){  // enum BT_Status { Undefined, Idle, Success, Failure };
    switch (id)
    {
    case 0:
        return "UNDEFINED";
    case 1:
        return "IDLE";
    case 2:
        return "SUCCESS";
    case 3:
        return "FAILURE";
    default:
        return "IDLE";
    }
}

//constexpr unsigned int str2int(const char* str, int h = 0)
//{
//    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
//}

//string CustomSwitchStatus (string state_string){                    // TOGLIERE ED UTILIZZARE IL TAG DANIELE / ATTRIBUTO DEL tipo state VEDI
//    const char* c = state_string.c_str();
//    switch (str2int(c))
//    {
//    case str2int("idle"):
//        return "IDLE";
//    case str2int("halted"):
//        return "RUNNING";
//    case str2int("success"):
//        return "SUCCESS";
//    case str2int("failure"):
//        return "FAILURE";
//    case str2int("sendrequest"):
//        return "RUNNING";
//    case str2int("getstatus"):
//        return "RUNNING";
//    case str2int("get"):
//        return "IDLE";
//    default:
//        return "IDLE";
//    }
//}

//vector<string> CheckUsedComponents ( QString dataTextXML ){ // modifica t.c. lista dei used components fornita da fuori

//    vector<string> result;

//    // search for "Connector" per capire se crea connessioni
//    if ( dataTextXML.contains( "Connector",  Qt::CaseInsensitive ) ){

//        // manual check for all the components names

//        string search = "GoToComponent";
//        string service_name = "GoTo"; // VA BENE SOLO SE SINGOLO SERVIZIO
//        QString qstr = QString::fromStdString(search);
//        if( dataTextXML.contains( qstr ,  Qt::CaseInsensitive ) ){
//            cout << " \n\n\n DETECTED " + search + " \n";
//            result.push_back(service_name); // NB !
//        }

//        search = "BatteryComponent";
//        service_name = "BatteryReader";
//        qstr = QString::fromStdString(search);
//        if( dataTextXML.contains( qstr ,  Qt::CaseInsensitive ) ){
//            cout << " \n\n\n DETECTED " + search + " \n";
//            result.push_back(service_name);
//        }

//        search = "Component_XXXXX";
//        service_name = "Service_XXXX";
//        qstr = QString::fromStdString(search);
//        if( dataTextXML.contains( qstr ,  Qt::CaseInsensitive ) ){
//            cout << " \n\n\n DETECTED " + search + " \n";
//            result.push_back(service_name);
//        }

//    }
//    return result;
//}

int write(TranslationUnit *tu)
{

    SkillDescription SD;

    auto docs = tu->allDocuments;
//     tables.resize(docs.size());
//     metaDataInfos.resize(tables.size());
//     dataModelInfos.resize(tables.size());
//     factories.resize(tables.size());
    auto classnameForDocument = tu->classnameForDocument;

    for (int i = 0, ei = docs.size(); i != ei; ++i) {
        auto doc = docs.at(i);

        for (DocumentModel::AbstractState* astate : doc->allStates) { // doc->allStates funzione che identifica tutti gli stati nel scxml
            auto state = astate->asState();
            if (!state) {
                qDebug() << astate->id;
                continue;
            }
            qDebug() << state->id << state->xmlLocation.line << state->xmlLocation.column << state->dataElements.size() << state->bt_status;

            // need to control the gerarchy of the states, s.t. the external "wrapper" is not tackled into the list
            // not easy because cannot control the exact location (names have different length), maybe control on TAG ANNIDATION

            State stato;
            stato.id = state->id;
            stato.ReturnStatusEnum = state->bt_status;
            stato.ReturnStatus = DecoderEnum(state->bt_status);
            SD.ListStates.push_back(stato);
        }

        // <datamodel> interpreter

        for (DocumentModel::Node* anode : doc->allNodes) {
            auto data = anode->asDataElement();
            if (!data) {
                // not a <data> element
                continue;
            }
            cout << "\n\n ************ ACTUAL TEST************  \n\n ";
            qDebug() << data->id << data->expr << data->cpp_type << data->component_type << data->service_type  << data->client_port_name;
            cout << "\n\n ************ end ACTUAL TEST************  \n\n ";

            // access to data element
            if (data->component_type != ""){
                Service service;
                service.name_instance = data->id;
                service.service_type = data->service_type;
                service.component_type = data->component_type;
                service.client_port_name = data->client_port_name;
                SD.UsedServices.push_back(service);
            }else if(data->cpp_type != ""){
                Attribute attribute;
                attribute.data_type = data->cpp_type;
                attribute.name_instance = data->id;
                attribute.value = data->expr;
                SD.UsedAttributes.push_back(attribute);
            }else if(data->client_port_name != ""){
                ClientPort port;
                port.client_port_name = data->client_port_name;
                SD.UsedClientPorts.push_back(port);
            }
        }

//         auto metaDataInfo = &metaDataInfos[i];
//         GeneratedTableData::build(doc, &tables[i], metaDataInfo, &dataModelInfos[i],
    }

    std::cout << "\n\n\n SKILL_DESCRIPTION SD : \n      ListStates size : " << SD.ListStates.size() << "\n\n" ;
    qDebug() << " Print: " << SD.ListStates[0].id << " " << SD.ListStates[1].id << " " << SD.ListStates[2].id << " " << SD.ListStates[3].id << " .\n\n";

    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################

    // locate TEMPLATE-folder with template-files
    QString path_template ="/home/scope/bt-implementation/src/skill-code-gen/skill-template-files/";

    // template files
    QFile file_template_NAMESkill_h             (path_template + "Name_Skill_h.t");
    QFile file_template_NAMESkill_cpp           (path_template + "Name_Skill_cpp.t");
    QFile file_template_NAMESkill_DataModel_h   (path_template + "Name_SkillDataModel_h.t");
    QFile file_template_NAMESkill_DataModel_cpp (path_template + "Name_SkillDataModel_cpp.t");
    QFile file_template_CMakeLists              (path_template + "CMakeLists_txt.t");
    QFile file_template_main                    (path_template + "main_cpp.t");
//    QFile file_template_NAMESkill_Manifest_xml  (path_template + "NAMESkillManifest.xml");

    //detect skill name
    string ss = tu->scxmlFileName.toUtf8().constData();

    cout << "\n\n\ #########################################################"
            "#########################################################"
            "#########################################################"
            "######################################################### \n\n START DEBUG SKILL --> " << ss << "\n\n\ " ;

    /*
    I need to
    1) search the word "skill" inside the string  -->  std::string::find()
        test_str.find("Skill") contiene puntatore alla posizione di inizio substring
    2) fetch string before that.
    */
    bool found = ss.find("SkillStateMachine.scxml") != ss.npos;
    std::cout << "\n\n\nTEST\n\n\n" << found << " ||||| " << ss.find("SkillStateMachine.scxml") << "\n\n\nEND TEST\n\n\n";
    // 2) split
    string str_first_part = ss.substr (0, ss.find("SkillStateMachine.scxml") );
    cout << "\n\n\nPRINT: str_first_part --> " << str_first_part << "\n\n";

    QString skill_name = QString::fromStdString(str_first_part);
    QRegularExpression key_skill_name("@KEY_SKILL_NAME@");
    QString value_skill_name(skill_name);

    // create new folder
    QString path_new_skill = "/home/scope/bt-implementation/src/skill-code-gen/output-files/" + skill_name + "_skill/";
    QDir dir_new_skill;
    if(!dir_new_skill.exists(path_new_skill))
    {
        dir_new_skill.mkpath(path_new_skill);
        qDebug()<<"\ndirectory now exists\n\n";
    }

    // **********************************++ "CmakeLists.txt" **********************************++
    // create new file
    QFile output_file_0(path_new_skill + "CMakeLists.txt"); // e.g. /home/scope/bt-implementation/build/bin/DIR_NEW_SKILL/BatteryLevelSkill.cpp
    ReplaceKeyInsideTemplate (path_new_skill, file_template_CMakeLists, output_file_0, key_skill_name, value_skill_name);

    // **********************************++ "main.cpp" **********************************++
    // create new file
    QFile output_file_main(path_new_skill + "main.cpp"); // e.g. /home/scope/bt-implementation/build/bin/DIR_NEW_SKILL/BatteryLevelSkill.cpp
    ReplaceKeyInsideTemplate (path_new_skill, file_template_main, output_file_main, key_skill_name, value_skill_name);

    // **********************************++ "Skill.h" **********************************++
    // create new file
    QFile output_file_1_H(path_new_skill + skill_name + "Skill.h"); // e.g. /home/scope/bt-implementation/build/bin/DIR_NEW_SKILL/BatteryLevelSkill.cpp
    ReplaceKeyInsideTemplate (path_new_skill, file_template_NAMESkill_h, output_file_1_H, key_skill_name, value_skill_name);


    // **********************************++ "Skill.cpp" **********************************++

    // 1: open and read the template
    file_template_NAMESkill_cpp.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = file_template_NAMESkill_cpp.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.2: states
    QRegularExpression key_states("@KEY_SKILL_STATES@");
    string all = "";
    for (int i=0; i<SD.ListStates.size(); i++){
        auto actual_state = SD.ListStates[i].id;
        string actual_state_string = actual_state.toUtf8().constData();
        string single_state_condit = "       if (state == \"" + actual_state_string + "\") {\n           return BT_" + SD.ListStates[i].ReturnStatus + ";\n       }\n" ;
        all = all + single_state_condit;
    }
    cout << all;
    QString value_all_states_condit = QString::fromStdString(all);
    dataText.replace(key_states, value_all_states_condit);


    // 3: create new file and insert the dataText
    QFile output_file_2_cpp(path_new_skill + skill_name + "Skill.cpp"); // e.g. /home/scope/bt-implementation/build/bin/DIR_NEW_SKILL/BatteryLevelSkill.cpp
    if(output_file_2_cpp.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file_2_cpp);
        out << dataText;
    }
    output_file_2_cpp.close();


    // ********************************** open scxml (not needed in last version) **********************************++

//    QString path_SCXML = "/home/scope/bt-implementation/build/bin/";

//        QFile XML( path_SCXML + tu->scxmlFileName);
//        // open and read
//        XML.open(QIODevice::Text | QIODevice::ReadOnly);
//        QString dataTextXML = XML.readAll();

//        SD.UsedComponents = CheckUsedComponents ( dataTextXML );


    // **********************************++ "SkillDataModel.h" **********************************++

    // 1: open and read the template
    file_template_NAMESkill_DataModel_h.open(QIODevice::Text | QIODevice::ReadOnly);
    dataText.clear();
    dataText = file_template_NAMESkill_DataModel_h.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.2: KEY_LIST_PUBLIC_ATTRIBUTES
    QRegularExpression KEY_LIST_PUBLIC_ATTRIBUTES("@KEY_LIST_PUBLIC_ATTRIBUTES@");

    string all_instances = "";

    // not used in last version
//    for (int i=0; i<SD.UsedComponents.size(); i++){
//        auto component = SD.UsedComponents[i]; //string
//        string instance_name = to_lower(component);
//        string single_instance = "" + component + " " + instance_name + "; // added by MY AUTOMATIC DETECTION \n    " ;
//        all_instances = all_instances + single_instance;
//    }

    // list of services
    for (int i=0; i<SD.UsedServices.size(); i++){
        string single_instance = "" + SD.UsedServices[i].service_type.toStdString() + " " + SD.UsedServices[i].name_instance.toStdString() + ";\n    "; // "; // added using DATAMODEL \n    " ;
        all_instances = all_instances + single_instance;
    }

    // list of attributes (with value assigned)
    for (int i=0; i<SD.UsedAttributes.size(); i++){
        string single_instance = "" + SD.UsedAttributes[i].data_type.toStdString() + " " + SD.UsedAttributes[i].name_instance.toStdString() + " { " + SD.UsedAttributes[i].value.toStdString() + " }; // added using DATAMODEL \n    " ;
        all_instances = all_instances + single_instance;
    }

    cout << "\n\n\PRINT COMPONENTS --> all_instances : " << all_instances << "\n\n\ " ;
    QString value_LIST_PUBLIC_ATTRIBUTES = QString::fromStdString(all_instances);
    dataText.replace(KEY_LIST_PUBLIC_ATTRIBUTES, value_LIST_PUBLIC_ATTRIBUTES);


    // 3: create new file and insert the dataText
    QFile output_file_3_DataModel_h(path_new_skill + skill_name + "SkillDataModel.h"); // e.g. /home/scope/bt-implementation/build/bin/DIR_NEW_SKILL/BatteryLevelSkill.cpp
    if(output_file_3_DataModel_h.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file_3_DataModel_h);
        out << dataText;
    }
    output_file_3_DataModel_h.close();

    // **********************************++ "SkillDataModel.cpp" **********************************++

    // 1: open and read the template
    file_template_NAMESkill_DataModel_cpp.open(QIODevice::Text | QIODevice::ReadOnly);
    dataText.clear();
    dataText = file_template_NAMESkill_DataModel_cpp.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.2      @OPEN_PORTS@

//    QRegularExpression KEY_OPEN_PORTS("@OPEN_PORTS@");

//    string all_ports ="";

//    if ( SD.UsedClientPorts.size() != 0 ){ // if specified in datamodel
//        for (int i=0; i<SD.UsedClientPorts.size(); i++){
//            string single_port = "if (!client_port.open(" + SD.UsedClientPorts[i].client_port_name.toStdString() +")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n" ;
//            all_ports = all_ports + single_port;
//        }
//    }else  // if not specified in datamodel, then use the default name "/@KEY_SKILL_NAME@Client"
//        all_ports = all_ports + "if (!client_port.open(/" + value_skill_name.toStdString() + "Client" + ")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n" ;

//    QString value_OPEN_PORTS = QString::fromStdString(all_ports);
//    dataText.replace(KEY_OPEN_PORTS, value_OPEN_PORTS);


//    // 2.3  @OPEN_PORTS_AND_ATTACH_CLIENTS@

//    QRegularExpression KEY_ATTACH_CLIENTS("@ATTACH_CLIENTS@");

//    string all_clients ="";

//    for (int i=0; i<SD.UsedServices.size(); i++){
//        string single_client = "if(!" + SD.UsedServices[i].name_instance.toStdString() + ".yarp().attachAsClient(client_port)) {\n       qWarning(\"Error! Could not attach as client\");\n       return false;\n    }\n";
//        all_clients = all_clients + single_client;
//    }

//    QString value_ATTACH_CLIENTS = QString::fromStdString(all_clients);
//    dataText.replace(KEY_ATTACH_CLIENTS, value_ATTACH_CLIENTS);


    // 2.3  @OPEN_PORTS_AND_ATTACH_CLIENTS@

    QRegularExpression KEY_OPEN_PORTS_AND_ATTACH_CLIENTS("@OPEN_PORTS_AND_ATTACH_CLIENTS@");

    string merge ="";
    string all_clients ="";
    string all_ports ="";

    for (int i=0; i<SD.UsedServices.size(); i++){
        string single_port = "if (!client_port.open(\"/" + SD.UsedServices[i].client_port_name.toStdString() +"\")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n" ;
        all_ports = all_ports + single_port;

        string single_client = "    if(!" + SD.UsedServices[i].name_instance.toStdString() + ".yarp().attachAsClient(client_port)) {\n       qWarning(\"Error! Could not attach as client\");\n       return false;\n    }\n";
        all_clients = all_clients + single_client;


        merge = all_ports + all_clients;
    }

    QString value_OPEN_PORTS_AND_ATTACH_CLIENTS = QString::fromStdString(merge);
    dataText.replace(KEY_OPEN_PORTS_AND_ATTACH_CLIENTS, value_OPEN_PORTS_AND_ATTACH_CLIENTS);


    // 3: create new file and insert the dataText
    QFile output_file_3_DataModel_cpp(path_new_skill + skill_name + "SkillDataModel.cpp"); // e.g. /home/scope/bt-implementation/build/bin/DIR_NEW_SKILL/BatteryLevelSkill.cpp
    if(output_file_3_DataModel_cpp.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file_3_DataModel_cpp);
        out << dataText;
    }
    output_file_3_DataModel_cpp.close();


    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################
    // ##########################################################################################################################################################################################

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
    cout << "start main ";
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
    QCommandLineOption optionOutputHeaderDMName(QLatin1String("headerDM"),
                       QCoreApplication::translate("main", "Generate <name> for the header file."),
                       QCoreApplication::translate("main", "name"));
    QCommandLineOption optionOutputSourceDMName(QLatin1String("implDM"),
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
    cmdParser.addOption(optionOutputHeaderDMName);
    cmdParser.addOption(optionOutputSourceDMName);

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
    QString outHDataModelFileName = cmdParser.value(optionOutputHeaderDMName);
    QString outCppDataModelFileName = cmdParser.value(optionOutputSourceDMName);

    QString mainClassName = cmdParser.value(optionClassName);

    if (outFileName.isEmpty())
        outFileName = QFileInfo(scxmlFileName).baseName();

    if (outHFileName.isEmpty())
        outHFileName = outFileName + QLatin1String(".h");
    if (outCppFileName.isEmpty())
        outCppFileName = outFileName + QLatin1String(".cpp");
    if (outHDataModelFileName.isEmpty())
        outHDataModelFileName = outFileName + QLatin1String("DataModel.h");
    if (outCppDataModelFileName.isEmpty())
        outCppDataModelFileName = outFileName + QLatin1String("DataModel.cpp");

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
    tu.outHDataModelFileName = outHDataModelFileName;
    tu.outCppDataModelFileName = outCppDataModelFileName;

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

