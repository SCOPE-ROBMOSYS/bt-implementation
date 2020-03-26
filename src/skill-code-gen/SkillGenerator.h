/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#ifndef SKILLGENERATOR_H
#define SKILLGENERATOR_H

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

#include "SkillConfigReader.h"

using namespace std;

struct TranslationUnit
{
    TranslationUnit()
        : stateMethods(false)
        , mainDocument(nullptr)
    {}

    QString scxmlFileName;
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

struct State{
    QString id;
    int ReturnStatusEnum;
    string ReturnStatus;
};

struct Service{
    QString name_instance;
    QString service_type;
    QString thrift_protocol;
    QString client_port_name;
    QString connect_type;
};

struct Attribute{
    QString data_type;
    QString name_instance;
    QString value;
    QString init_source; // if "define_in_skill_class_constructor" then it will be added in the constructor
};

struct ClientPort{ // not really needed
    QString client_port_name;
};

struct SkillDescription {

//    SkillDescription()
//    {}

    QString skill_name;
    vector<State> ListStates;
    vector<string> UsedComponents;
    vector<Service> UsedServices;
    vector<Attribute> UsedAttributes;
    vector<ClientPort> UsedClientPorts;
    bool specify_port_name_attribute;
    bool add_constructor; //for not default constructor
    vector<Attribute> ListAttributesInitWithConstructor;
};



class SkillGenerator
{
public:
    SkillGenerator (TranslationUnit *tu);

    TranslationUnit *tu_;

    //methods

    int write(); // the former

    int init();

    string to_upper(string str);
    string to_lower(string str);
    void ReplaceKeyInsideTemplate (QString path_new_skill, QFile& input_file, QFile& output_file, QRegularExpression key, QString value);
    string DecoderEnum (int id);

    vector<string> GenerateStringList_name_instance (vector<Attribute> ListAttributes);
    vector<string> GenerateStringList_data_type (vector<Attribute> ListAttributes);

    string GenerateListConstructorParameters (vector<string> ListParamToAssign_data_type, vector<string> ListParamToAssign_name_instance);
    string GenerateListConstructorParametersAssign (vector<string> ListMemberAttributes, vector<string> ListParamToAssign);
    string GenerateListConstructorParametersPassArgs (vector<string> ListParamToAssign_name_instance);


private:

};

#endif // SKILLGENERATOR_H
