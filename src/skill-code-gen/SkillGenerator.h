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

//#define _DEBUG // alternatively : cmake -DCMAKE_BUILD_TYPE=Debug ..

struct TranslationUnit
{
    TranslationUnit()
        : stateMethods(false)
        , mainDocument(nullptr)
    {}

    QString Path;
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
    int SkillAckEnum;
    string SkillAck;
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
    QString path_skill_folder;
    vector<State> ListStates;
    vector<string> UsedComponents;
    vector<Service> UsedServices;
    vector<Attribute> UsedAttributes;
    vector<ClientPort> UsedClientPorts;
    bool specify_port_name_attribute;
    bool add_constructor; //for not default constructor
    vector<Attribute> ListAttributesParsedAsOption;
};

struct Keys {
    QString key_skill_name;
    QString KEY_CONSTRUCTOR_ATTRIBUTES_p1;

    Keys()
        : key_skill_name("@KEY_SKILL_NAME@")
        , KEY_CONSTRUCTOR_ATTRIBUTES_p1("@KEY_CONSTRUCTOR_ATTRIBUTES_p1@")
    {}

};

struct Values {
    QString empty;
    QString value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma;

    Values()
        : empty("")
    {}

};

class SkillGenerator
{
public:
    SkillGenerator (TranslationUnit *tu);
    int init();

private:
    QString path_template_;
    TranslationUnit *tu_;
    SkillDescription SD_;
    Keys K_;
    Values V_;
    SkillConfig Skill_Config_;

    string to_upper(string str);
    string to_lower(string str);
    void ReplaceKeyInsideTemplate (QString path_new_skill, QFile& input_file, QFile& output_file, QRegularExpression key, QString value);
    string DecoderEnum (int id);

    vector<string> GenerateStringList_name_instance (vector<Attribute> ListAttributes);
    vector<string> GenerateStringList_data_type (vector<Attribute> ListAttributes);

    string GenerateListConstructorParameters (vector<string> ListParamToAssign_data_type, vector<string> ListParamToAssign_name_instance);
    string GenerateListConstructorParametersAssign (vector<string> ListMemberAttributes, vector<string> ListParamToAssign);
    string GenerateListConstructorParametersPassArgs (vector<string> ListParamToAssign_name_instance);

    int write();
    void ConfigGeneration();
    void Generate_Main();
    void Generate_Skill_h();
    void Generate_Skill_cpp();
    void Generate_Skill_DataModel_h();
    void Generate_Skill_DataModel_cpp();
};

#endif // SKILLGENERATOR_H
