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

#include "SkillGenerator.h"

using namespace std;

//SkillGenerator::SkillGenerator (TranslationUnit *tu) :
//    tu_(tu)
//{

//}

SkillGenerator::SkillGenerator (TranslationUnit *tu)
{
    tu_ = tu;
}


int SkillGenerator::init(){

    int result = write();

    return result;
}

string SkillGenerator::to_upper(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::toupper));
    return str;
}

string SkillGenerator::to_lower(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::tolower));
    return str;
}

void SkillGenerator::ReplaceKeyInsideTemplate (QString path_new_skill, QFile& input_file, QFile& output_file, QRegularExpression key, QString value){
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


string SkillGenerator::DecoderEnum (int id){  // enum BT_Status { Undefined, Idle, Success, Failure };
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

vector<string> SkillGenerator::GenerateStringList_name_instance (vector<Attribute> ListAttributes){
    vector<string> output;
    for(int i=0; i<ListAttributes.size(); i++){
        output.push_back(ListAttributes[i].name_instance.toStdString());
    }
    return output;
}

vector<string> SkillGenerator::GenerateStringList_data_type (vector<Attribute> ListAttributes){
    vector<string> output;
    for(int i=0; i<ListAttributes.size(); i++){
        output.push_back(ListAttributes[i].data_type.toStdString());
    }
    return output;
}

string SkillGenerator::GenerateListConstructorParametersPassArgs (vector<string> ListParamToAssign_name_instance){
    string output ="";
    for(int i=0; i<ListParamToAssign_name_instance.size(); i++){
        output = output + ListParamToAssign_name_instance[i];
        if(i!= (ListParamToAssign_name_instance.size()-1) ){
            output = output + ", ";
        }
    }
    return output;
}

string SkillGenerator::GenerateListConstructorParameters (vector<string> ListParamToAssign_data_type, vector<string> ListParamToAssign_name_instance){
    string output ="";
    for(int i=0; i<ListParamToAssign_data_type.size(); i++){
        output = output + ListParamToAssign_data_type[i] + " " + ListParamToAssign_name_instance[i];
        if(i!= (ListParamToAssign_data_type.size()-1) ){
            output = output + ", ";
        }
    }
    return output;
}

string SkillGenerator::GenerateListConstructorParametersAssign (vector<string> ListMemberAttributes, vector<string> ListParamToAssign){
    string output ="";
    for(int i=0; i<ListMemberAttributes.size(); i++){
        output = output +  ListMemberAttributes[i]   + "(std::move(" +    ListParamToAssign[i] + "))";
        if(i!= (ListMemberAttributes.size()-1) ){
            output = output + ", ";
        }
    }
    return output;
}

int SkillGenerator::write()
{
    bool debug = false;

    SkillDescription SD;
    SD.add_constructor = false; // default constructor

    auto docs = tu_->allDocuments;

    for (int i = 0, ei = docs.size(); i != ei; ++i) {
        auto doc = docs.at(i);

        for (DocumentModel::AbstractState* astate : doc->allStates) { // doc->allStates identifies all the states inside the scxml input file
            auto state = astate->asState();
            if (!state) {
                if(debug){
                    qDebug() << astate->id;
                }
                continue;
            }
            if(debug){
                qDebug() << state->id << state->xmlLocation.line << state->xmlLocation.column << state->dataElements.size() << state->bt_status;
            }
            // need to control the gerarchy of the states, s.t. the external "wrapper" is not tackled into the list

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
            if (debug){
                cout << "\n\n ************ ACTUAL TEST************  \n\n ";
                qDebug() << data->id << data->expr << data->cpp_type << data->thrift_protocol << data->service_type  << data->client_port_name << data->init_source << data->connect_type;
                cout << "\n\n ************ end ACTUAL TEST************  \n\n ";
            }

            // access to data element
            if (data->thrift_protocol != ""){ // means that the data represents a service!
                Service service;
                service.name_instance = data->id;
                service.service_type = data->service_type;
                service.thrift_protocol = data->thrift_protocol;
                service.client_port_name = data->client_port_name;
                service.connect_type = data->connect_type;
                SD.UsedServices.push_back(service);
            }else if(data->cpp_type != ""){ // means that the data represents an attribute!
                Attribute attribute;
                attribute.data_type = data->cpp_type;
                attribute.name_instance = data->id;
                attribute.value = data->expr;
                attribute.init_source = data->init_source;
                SD.UsedAttributes.push_back(attribute);
                if(attribute.init_source.toStdString() == "define_in_class_constructor"){
                   SD.add_constructor = true;
                   SD.ListAttributesInitWithConstructor.push_back(attribute);
                }
            }else if(data->client_port_name != ""){ // not needed
                ClientPort port;
                port.client_port_name = data->client_port_name;
                SD.UsedClientPorts.push_back(port);
            }
        }
    }

    if (debug){
        std::cout << "\n\n\n SKILL_DESCRIPTION SD : \n      ListStates size : " << SD.ListStates.size() << "\n\n" ;
        qDebug() << " Print: " << SD.ListStates[0].id << " " << SD.ListStates[1].id << " " << SD.ListStates[2].id << " " << SD.ListStates[3].id << " .\n\n";
    }

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
    QFile file_template_NAMESkill_Manifest_xml  (path_template + "NAMESkillManifest_yml.t");

    //detect skill name
    string ss = tu_->scxmlFileName.toUtf8().constData();
    /* I need to
    1) search the word "skill" inside the string  -->  std::string::find()
        test_str.find("Skill") contains the pointer to the sub-string initial position
    2) fetch string before that. */
    bool found = ss.find("SkillStateMachine.scxml") != ss.npos;
//    std::cout << "\n\n\nTEST\n\n\n" << found << " ||||| " << ss.find("SkillStateMachine.scxml") << "\n\n\nEND TEST\n\n\n";
    // 2) split
    string str_first_part = ss.substr (0, ss.find("SkillStateMachine.scxml") );
    cout << "\n\n\nGENERATED SKILL : str_first_part --> " << str_first_part << "\n\n";

    QString skill_name = QString::fromStdString(str_first_part);
    QRegularExpression key_skill_name("@KEY_SKILL_NAME@");
    QString value_skill_name(skill_name);

    // create new folder
    QString path_new_skill = "/home/scope/bt-implementation/src/" + skill_name + "_skill/"; // "/home/scope/bt-implementation/src/skill-code-gen/output-files/" + skill_name + "_skill/";
    QDir dir_new_skill;
    if(!dir_new_skill.exists(path_new_skill))
    {
        dir_new_skill.mkpath(path_new_skill); // qDebug()<<"\ndirectory now exists\n\n";
    }

    // **********************************++ !read config **********************************++

    string file_with_path = "/home/scope/bt-implementation/src/skill-code-gen/input-files/config-skills/";
    file_with_path = file_with_path + str_first_part + "Skill.conf";
    SkillConfigReader Skill_Config_Reader(file_with_path);
    SkillConfig Skill_Config = Skill_Config_Reader.ReturnConfig();

    // **********************************++ "CmakeLists.txt" **********************************++
    // create new file
//    QFile output_file_0(path_new_skill + "CMakeLists.txt");
//    ReplaceKeyInsideTemplate (path_new_skill, file_template_CMakeLists, output_file_0, key_skill_name, value_skill_name);

    QString value_NULL = QString::fromStdString("");

    // 1: open and read the template
    file_template_CMakeLists.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = file_template_CMakeLists.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.2:  ADDITIONAL_THRIFT_PROTOCOLS
    QRegularExpression KEY_ADDITIONAL_THRIFT_PROTOCOLS("@ADDITIONAL_THRIFT_PROTOCOLS@");
    string all_protocols = "";
    // list of protocols
    for (int i=0; i<SD.UsedServices.size(); i++){
        all_protocols =  all_protocols + "\n    " + SD.UsedServices[i].thrift_protocol.toStdString() + "_protocol";
    }
    QString value_ADDITIONAL_THRIFT_PROTOCOLS = QString::fromStdString(all_protocols);
    dataText.replace( KEY_ADDITIONAL_THRIFT_PROTOCOLS, value_ADDITIONAL_THRIFT_PROTOCOLS);

    // 3: create new file and insert the dataText
    QFile output_file_0(path_new_skill + "CMakeLists.txt");
    if( output_file_0.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file_0);
        out << dataText;
    }
     output_file_0.close();

    // **********************************++ "main.cpp" **********************************++

    // 1: open and read the template
    file_template_main.open(QIODevice::Text | QIODevice::ReadOnly);
    dataText.clear();
    dataText = file_template_main.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.3 @KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS@
    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS("@KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS@");
    string attrib_1_pass_args = "";
    if( SD.add_constructor == true ){
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD.ListAttributesInitWithConstructor);
        attrib_1_pass_args = GenerateListConstructorParametersPassArgs (ListParamToAssign_name_instance);
    }
    string attrib_1_pass_args_with_comma = ", " + attrib_1_pass_args;
    QString value_CONSTRUCTOR_ATTRIBUTES_p1_PASS_ARGS_with_comma = QString::fromStdString(attrib_1_pass_args_with_comma);
    if(SD.ListAttributesInitWithConstructor.size()>0){ // otherwise insert a not needed comma
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS, value_CONSTRUCTOR_ATTRIBUTES_p1_PASS_ARGS_with_comma);
    }else{
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS, value_NULL);
    }

    // 2.4:  KEY_LIST_PUBLIC_ATTRIBUTES_main
    QRegularExpression  KEY_LIST_PUBLIC_ATTRIBUTES_main("@KEY_LIST_PUBLIC_ATTRIBUTES_main@");

    string all_instances_main = "";

    // list of attributes (without value assigned)
    for (int i=0; i<SD.UsedAttributes.size(); i++){
        string single_instance = "";
        if( SD.UsedAttributes[i].init_source.toStdString() != "initialize_inside_header"){
            single_instance = "" + SD.UsedAttributes[i].data_type.toStdString() + " " + SD.UsedAttributes[i].name_instance.toStdString() + ";\n    " ;
        }
         all_instances_main =  all_instances_main + single_instance;
    }

//    cout << "\n\n\PRINT COMPONENTS -->  all_instances_main : " <<  all_instances_main << "\n\n\ " ;
    QString value_LIST_PUBLIC_ATTRIBUTES_main = QString::fromStdString( all_instances_main);
    dataText.replace( KEY_LIST_PUBLIC_ATTRIBUTES_main, value_LIST_PUBLIC_ATTRIBUTES_main);


    // 3: create new file and insert the dataText
    QFile  output_file_main(path_new_skill + "main.cpp");
    if( output_file_main.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file_main);
        out << dataText;
    }
     output_file_main.close();

    // **********************************++ "Skill.h" **********************************++

    // 1: open and read the template
    file_template_NAMESkill_h.open(QIODevice::Text | QIODevice::ReadOnly);
    dataText.clear();
    dataText = file_template_NAMESkill_h.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.3 @KEY_CONSTRUCTOR_ATTRIBUTES_p1@
    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p1("@KEY_CONSTRUCTOR_ATTRIBUTES_p1@");
    string attrib_1 = "";
    if( SD.add_constructor == true ){
        vector<string> ListParamToAssign_data_type     = GenerateStringList_data_type (SD.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD.ListAttributesInitWithConstructor);
        attrib_1 = GenerateListConstructorParameters (ListParamToAssign_data_type, ListParamToAssign_name_instance);
    }
    string attrib_1_with_comma = ", " + attrib_1;
    QString value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma = QString::fromStdString(attrib_1_with_comma);
    if(SD.ListAttributesInitWithConstructor.size()>0){ // otherwise insert a not needed comma
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1, value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma);
    }else{
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1, value_NULL);
    }

    // 3: create new file and insert the dataText
    QFile output_file_1_h(path_new_skill + skill_name + "Skill.h");
    if(output_file_1_h.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file_1_h);
        out << dataText;
    }
    output_file_1_h.close();

    // **********************************++ "Skill.cpp" **********************************++

    // 1: open and read the template
    file_template_NAMESkill_cpp.open(QIODevice::Text | QIODevice::ReadOnly);
    dataText.clear();
    dataText = file_template_NAMESkill_cpp.readAll();

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
    if(debug){
        cout << all;
    }
    QString value_all_states_condit = QString::fromStdString(all);
    dataText.replace(key_states, value_all_states_condit);

    // 2.3 @KEY_CONSTRUCTOR_ATTRIBUTES_p1@ + @KEY_CONSTRUCTOR_ATTRIBUTES_p2@

    // KEY_CONSTRUCTOR_ATTRIBUTES_p1 is already defined for .h
    if(SD.ListAttributesInitWithConstructor.size()>0){
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1, value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma);
    }else{
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1, value_NULL);
    }

    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p2("@KEY_CONSTRUCTOR_ATTRIBUTES_p2@");
    string attrib_2 = "";
    if( SD.add_constructor == true ){
        string space = ",\n        ";
        attrib_2 = attrib_2 + space;
        vector<string> ListParamToAssign = GenerateStringList_name_instance(SD.ListAttributesInitWithConstructor);
        int dim = ListParamToAssign.size();
        vector<string> ListMemberAttributes;
        for(int i=0; i<dim; i++){
            ListMemberAttributes.push_back("dataModel"); // to check!
        }
        attrib_2 = attrib_2 + GenerateListConstructorParametersAssign (ListMemberAttributes, ListParamToAssign);
    }
    QString value_CONSTRUCTOR_ATTRIBUTES_p2 = QString::fromStdString(attrib_2);
    dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p2, value_CONSTRUCTOR_ATTRIBUTES_p2);

    // 3: create new file and insert the dataText
    QFile output_file_2_cpp(path_new_skill + skill_name + "Skill.cpp");
    if(output_file_2_cpp.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file_2_cpp);
        out << dataText;
    }
    output_file_2_cpp.close();

    // **********************************++ "SkillDataModel.h" **********************************++

    // 1: open and read the template
    file_template_NAMESkill_DataModel_h.open(QIODevice::Text | QIODevice::ReadOnly);
    dataText.clear();
    dataText = file_template_NAMESkill_DataModel_h.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(key_skill_name, value_skill_name);

    // 2.1.2 #include @INCLUDE_THRIFT_SERVICE@
    QRegularExpression KEY_INCLUDE_THRIFT_SERVICE("@INCLUDE_THRIFT_SERVICE@");
    string str_include ="";
    for (int i=0; i<SD.UsedServices.size(); i++){
        str_include = str_include + "#include \"" + SD.UsedServices[i].service_type.toStdString()  + ".h\" \n ";
        // with < >
        //str_include = str_include + "#include <" + SD.UsedServices[i].service_type.toStdString()  + ".h> \n ";
    }
    QString value_INCLUDE_THRIFT_SERVICE = QString::fromStdString(str_include);
    dataText.replace(KEY_INCLUDE_THRIFT_SERVICE, value_INCLUDE_THRIFT_SERVICE);

    // 2.2: KEY_LIST_PUBLIC_ATTRIBUTES
    QRegularExpression KEY_LIST_PUBLIC_ATTRIBUTES("@KEY_LIST_PUBLIC_ATTRIBUTES@");

    string all_instances = "";

    // list of services
    for (int i=0; i<SD.UsedServices.size(); i++){
        string single_instance = "" + SD.UsedServices[i].service_type.toStdString() + " " + SD.UsedServices[i].name_instance.toStdString() + ";\n    ";
        all_instances = all_instances + single_instance;
    }

    // list of attributes (with value assigned)
    for (int i=0; i<SD.UsedAttributes.size(); i++){
        string single_instance = "";
        if( SD.UsedAttributes[i].init_source.toStdString() == "initialize_inside_header"){
            single_instance = "" + SD.UsedAttributes[i].data_type.toStdString() + " " + SD.UsedAttributes[i].name_instance.toStdString() + " { " + SD.UsedAttributes[i].value.toStdString() + " };\n    " ;
        }
        else{
            single_instance = "" + SD.UsedAttributes[i].data_type.toStdString() + " " + SD.UsedAttributes[i].name_instance.toStdString() + ";\n    " ;
        }
        all_instances = all_instances + single_instance;
    }

//    cout << "\n\n\PRINT COMPONENTS --> all_instances : " << all_instances << "\n\n\ " ;
    QString value_LIST_PUBLIC_ATTRIBUTES = QString::fromStdString(all_instances);
    dataText.replace(KEY_LIST_PUBLIC_ATTRIBUTES, value_LIST_PUBLIC_ATTRIBUTES);


    // 2.3 @CONSTRUCTOR@
    QRegularExpression KEY_CONSTRUCTOR("@CONSTRUCTOR@");
    string construct = "";
    if( SD.add_constructor == false ){ // it is also used in .cpp to insert the not default constructor
        construct = construct + value_skill_name.toStdString() + "SkillDataModel() = default;";
    }else{ // e.g.  GoToSkillDataModel(std::string location);
        vector<string> ListParamToAssign_data_type     = GenerateStringList_data_type (SD.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD.ListAttributesInitWithConstructor);
        string params = GenerateListConstructorParameters (ListParamToAssign_data_type, ListParamToAssign_name_instance);
        construct = construct + value_skill_name.toStdString() + "SkillDataModel(" + params +");";
    }

    QString value_CONSTRUCTOR = QString::fromStdString(construct);
    dataText.replace(KEY_CONSTRUCTOR, value_CONSTRUCTOR);


    // 3: create new file and insert the dataText
    QFile output_file_3_DataModel_h(path_new_skill + skill_name + "SkillDataModel.h");
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

    // 2.2  @OPEN_PORTS_AND_ATTACH_CLIENTS@
    QRegularExpression KEY_OPEN_PORTS_AND_ATTACH_CLIENTS("@OPEN_PORTS_AND_ATTACH_CLIENTS@");
    string merge ="";
    string all_clients ="";
    string all_ports ="";

    for (int i=0; i<SD.UsedServices.size(); i++){

        string single_port = "";

        if(Skill_Config.specify_port_name_attribute){
            // additional name spec if needed
            string port_name_specific = Skill_Config.port_name_list[0];
            single_port = "    if (!client_port.open(\"/" + SD.UsedServices[i].name_instance.toStdString() + "Client/" + port_name_specific + "\")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n\n" ;

        }else{
            single_port = "    if (!client_port.open(\"/" + SD.UsedServices[i].name_instance.toStdString() + "Client\")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n\n" ;
        }

        all_ports = all_ports + single_port;
        string single_client = "    if(!" + SD.UsedServices[i].name_instance.toStdString() + ".yarp().attachAsClient(client_port)) {\n       qWarning(\"Error! Could not attach as client\");\n       return false;\n    }\n";
        all_clients = all_clients + single_client;
        merge = "    // open ports\n\n" + all_ports + "    // attach as clients\n\n" + all_clients;
    }
    QString value_OPEN_PORTS_AND_ATTACH_CLIENTS = QString::fromStdString(merge);
    dataText.replace(KEY_OPEN_PORTS_AND_ATTACH_CLIENTS, value_OPEN_PORTS_AND_ATTACH_CLIENTS);

    // 2.3  @OPEN_CONNECTIONS_TO_COMPONENTS@
    QRegularExpression KEY_OPEN_CONNECTIONS_TO_COMPONENTS("@OPEN_CONNECTIONS_TO_COMPONENTS@");
    string all_components ="";
    for (int i=0; i<SD.UsedServices.size(); i++){
        all_components = all_components + "    if (!yarp::os::Network::connect(client_port.getName(), \"/" + SD.UsedServices[i].thrift_protocol.toStdString()  + "\", \"" + SD.UsedServices[i].connect_type.toStdString() + "\")) {\n        qWarning(\"Error! Could not connect to server /fakeBattery\");\n        return false;\n    }\n" ;
    }
    all_components = "    // open connections to components\n\n" + all_components;
    QString value_OPEN_CONNECTIONS_TO_COMPONENTS = QString::fromStdString(all_components);
    dataText.replace(KEY_OPEN_CONNECTIONS_TO_COMPONENTS, value_OPEN_CONNECTIONS_TO_COMPONENTS);

    // 2.4 @ADD_CONSTRUCTOR@ if needed (case not default in .h)
    QRegularExpression KEY_ADD_CONSTRUCTOR("@ADD_CONSTRUCTOR@");
    if( SD.add_constructor == true ){

        string attributes_init_string_intro = value_skill_name.toStdString() + "SkillDataModel::" + value_skill_name.toStdString() +"SkillDataModel("; // GoToSkillDataModel::GoToSkillDataModel

        vector<string> ListParamToAssign_data_type     = GenerateStringList_data_type (SD.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD.ListAttributesInitWithConstructor);
        string attributes_init_string_part_1 = GenerateListConstructorParameters (ListParamToAssign_data_type, ListParamToAssign_name_instance);

        string attributes_init_string_mid =") :\n                        ";

        vector<string> ListMemberAttributes = GenerateStringList_name_instance(SD.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign = ListMemberAttributes;
        string attributes_init_string_part_2 = GenerateListConstructorParametersAssign (ListMemberAttributes, ListParamToAssign);

        string attributes_init_string_end ="\n{\n}";

        string constructor_string = attributes_init_string_intro + attributes_init_string_part_1 + attributes_init_string_mid + attributes_init_string_part_2 + attributes_init_string_end;
        QString value_ADD_CONSTRUCTOR = QString::fromStdString(constructor_string);
        dataText.replace(KEY_ADD_CONSTRUCTOR, value_ADD_CONSTRUCTOR);

    }else{
        dataText.replace(KEY_ADD_CONSTRUCTOR, "");
    }

    // 3: create new file and insert the dataText
    QFile output_file_3_DataModel_cpp(path_new_skill + skill_name + "SkillDataModel.cpp");
    if(output_file_3_DataModel_cpp.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&output_file_3_DataModel_cpp);
        out << dataText;
    }
    output_file_3_DataModel_cpp.close();

    // end skill-auto-gen

    return NoError;
}
