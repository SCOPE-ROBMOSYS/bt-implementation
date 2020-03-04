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

SkillGenerator::SkillGenerator (TranslationUnit *tu) :
    tu_(tu)
{
    //    init(); called from main
}

int SkillGenerator::init(){

    ConfigGeneration();
    int result = write();

    return result;
}

void SkillGenerator::ConfigGeneration(){

    SD_.add_constructor = false; // default constructor

    // used only for .conf files
    // **********************************++ !path & new folder **********************************++
        //detect root path
        string ss_whole = tu_->Path.toUtf8().constData(); //receive from outside
        /* I need to
        1) search the word "skill" inside the string  -->  std::string::find()
            test_str.find("Skill") contains the pointer to the sub-string initial position
        2) fetch string before that. */
        string path_root_package = ss_whole.substr (0, ss_whole.rfind("src") ); // i.e. /home/scope/bt-implementation/
        //cout << "\n\n\nRoot --> " << path_root_package << "\n\n";

        // locate folder with template files
        QString Qpath_root_package = QString::fromStdString(path_root_package);
        string end_path_template = "src/skill-code-gen/skill-template-files/";
        string path_template_str = path_root_package + end_path_template; // e.g. /home/scope/bt-implementation/src/skill-code-gen/skill-template-files/

        path_template_ = QString::fromStdString(path_template_str);

        //detect skill name
        string ss = tu_->scxmlFileName.toUtf8().constData();
        //bool found = ss.find("SkillStateMachine.scxml") != ss.npos; //    std::cout << "\n\n\nTEST\n\n\n" << found << " ||||| " << ss.find("SkillStateMachine.scxml") << "\n\n\nEND TEST\n\n\n";
        string str_first_part = ss.substr (0, ss.find("SkillStateMachine.scxml") );
        //cout << "Skill name --> " << str_first_part << "\n\n";
        SD_.skill_name = QString::fromStdString(str_first_part);

        // create new folder
    //    SD_.path_skill_folder = Qpath_root_package + "src/" + SD_.skill_name + "_skill/";
    //    QDir dir_new_skill;
    //    if(!dir_new_skill.exists(SD_.path_skill_folder))
    //    {
    //        dir_new_skill.mkpath(SD_.path_skill_folder); // qDebug()<<"\ndirectory now exists\n\n";
    //    }

    // **********************************++ !read config **********************************++
    string file_with_path = path_root_package + "src/skill-code-gen/input-files/config-skills/";
    file_with_path = file_with_path + str_first_part + "Skill.conf";
    SkillConfigReader Skill_Config_Reader(file_with_path);
    Skill_Config_ = Skill_Config_Reader.ReturnConfig();
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
    for(unsigned int i=0; i<ListAttributes.size(); i++){
        output.push_back(ListAttributes[i].name_instance.toStdString());
    }
    return output;
}

vector<string> SkillGenerator::GenerateStringList_data_type (vector<Attribute> ListAttributes){
    vector<string> output;
    for(unsigned int i=0; i<ListAttributes.size(); i++){
        output.push_back(ListAttributes[i].data_type.toStdString());
    }
    return output;
}

string SkillGenerator::GenerateListConstructorParametersPassArgs (vector<string> ListParamToAssign_name_instance){
    string output ="";
    for(unsigned int i=0; i<ListParamToAssign_name_instance.size(); i++){
        output = output + ListParamToAssign_name_instance[i];
        if(i!= (ListParamToAssign_name_instance.size()-1) ){
            output = output + ", ";
        }
    }
    return output;
}

string SkillGenerator::GenerateListConstructorParameters (vector<string> ListParamToAssign_data_type, vector<string> ListParamToAssign_name_instance){
    string output ="";
    for(unsigned int i=0; i<ListParamToAssign_data_type.size(); i++){
        output = output + ListParamToAssign_data_type[i] + " " + ListParamToAssign_name_instance[i];
        if(i!= (ListParamToAssign_data_type.size()-1) ){
            output = output + ", ";
        }
    }
    return output;
}

string SkillGenerator::GenerateListConstructorParametersAssign (vector<string> ListMemberAttributes, vector<string> ListParamToAssign){
    string output ="";
    for(unsigned int i=0; i<ListMemberAttributes.size(); i++){
        output = output +  ListMemberAttributes[i]   + "(std::move(" +    ListParamToAssign[i] + "))";
        if(i!= (ListMemberAttributes.size()-1) ){
            output = output + ", ";
        }
    }
    return output;
}

void SkillGenerator::Generate_Main(){

    QFile template_file(":/skill-template-files/main_cpp.t");   //    QFile template_file (path_template_ + "main_cpp.t");

    // 1: open and read the template
    template_file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = template_file.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(K_.key_skill_name, SD_.skill_name);

    // 2.2 @KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS@
    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS("@KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS@");
    string attrib_1_pass_args = "";
    if( SD_.add_constructor == true ){
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD_.ListAttributesInitWithConstructor);
        attrib_1_pass_args = GenerateListConstructorParametersPassArgs (ListParamToAssign_name_instance);
    }
    string attrib_1_pass_args_with_comma = ", " + attrib_1_pass_args;
    QString value_CONSTRUCTOR_ATTRIBUTES_p1_PASS_ARGS_with_comma = QString::fromStdString(attrib_1_pass_args_with_comma);
    if(SD_.ListAttributesInitWithConstructor.size()>0){ // otherwise insert a not needed comma
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS, value_CONSTRUCTOR_ATTRIBUTES_p1_PASS_ARGS_with_comma);
    }else{
        dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p1_PASSED_ARGS, V_.empty);
    }

    // 2.4:  KEY_LIST_PUBLIC_ATTRIBUTES_main
    QRegularExpression  KEY_LIST_PUBLIC_ATTRIBUTES_main("@KEY_LIST_PUBLIC_ATTRIBUTES_main@");

    string all_instances_main = "";

    // list of attributes (without value assigned)
    for(unsigned int i=0; i<SD_.UsedAttributes.size(); i++){
        string single_instance = "";
        if( SD_.UsedAttributes[i].init_source.toStdString() != "initialize_inside_header"){
            single_instance = "" + SD_.UsedAttributes[i].data_type.toStdString() + " " + SD_.UsedAttributes[i].name_instance.toStdString() + ";\n    " ;
        }
         all_instances_main =  all_instances_main + single_instance;
    }

//    cout << "\n\n\PRINT COMPONENTS -->  all_instances_main : " <<  all_instances_main << "\n\n\ " ;
    QString value_LIST_PUBLIC_ATTRIBUTES_main = QString::fromStdString( all_instances_main);
    dataText.replace( KEY_LIST_PUBLIC_ATTRIBUTES_main, value_LIST_PUBLIC_ATTRIBUTES_main);

    // 3: create new file and insert the dataText
    QFile output_file(SD_.path_skill_folder + "main.cpp");
    if( output_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file);
        out << dataText;
    }
    output_file.close();
}

void SkillGenerator::Generate_Skill_h(){

    QFile template_file(":/skill-template-files/Name_Skill_h.t");

    // 1: open and read the template
    template_file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = template_file.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(K_.key_skill_name, SD_.skill_name);

    // 2.2 @KEY_CONSTRUCTOR_ATTRIBUTES_p1@
//    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p1("@KEY_CONSTRUCTOR_ATTRIBUTES_p1@");
    string attrib_1 = "";
    if( SD_.add_constructor == true ){
        vector<string> ListParamToAssign_data_type     = GenerateStringList_data_type (SD_.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD_.ListAttributesInitWithConstructor);
        attrib_1 = GenerateListConstructorParameters (ListParamToAssign_data_type, ListParamToAssign_name_instance);
    }
    string attrib_1_with_comma = ", " + attrib_1;
    QString value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma = QString::fromStdString(attrib_1_with_comma);
    V_.value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma = value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma; // assign the global value to exploit the value globally
    if(SD_.ListAttributesInitWithConstructor.size()>0){ // otherwise insert a not needed comma
        dataText.replace(K_.KEY_CONSTRUCTOR_ATTRIBUTES_p1, value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma);
    }else{
        dataText.replace(K_.KEY_CONSTRUCTOR_ATTRIBUTES_p1, V_.empty);
    }

    // 3: create new file and insert the dataText
    QFile output_file(SD_.path_skill_folder + SD_.skill_name + "Skill.h");
    if( output_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file);
        out << dataText;
    }
    output_file.close();
}

void SkillGenerator::Generate_Skill_cpp(){

    QFile template_file(":/skill-template-files/Name_Skill_cpp.t");

    // 1: open and read the template
    template_file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = template_file.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(K_.key_skill_name, SD_.skill_name);

    // 2.2: states
    QRegularExpression key_states("@KEY_SKILL_STATES@");
    string all = "";
    for(unsigned int i=0; i<SD_.ListStates.size(); i++){
        auto actual_state = SD_.ListStates[i].id;
        string actual_state_string = actual_state.toUtf8().constData();
        string single_state_condit = "       if (state == \"" + actual_state_string + "\") {\n           return BT_" + SD_.ListStates[i].ReturnStatus + ";\n       }\n" ;
        all = all + single_state_condit;
    }
    #ifdef _DEBUG
        cout << all;
    #endif
    QString value_all_states_condit = QString::fromStdString(all);
    dataText.replace(key_states, value_all_states_condit);

    // 2.3 @KEY_CONSTRUCTOR_ATTRIBUTES_p1@ + @KEY_CONSTRUCTOR_ATTRIBUTES_p2@

//    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p1("@KEY_CONSTRUCTOR_ATTRIBUTES_p1@");
    if(SD_.ListAttributesInitWithConstructor.size()>0){
        dataText.replace(K_.KEY_CONSTRUCTOR_ATTRIBUTES_p1, V_.value_CONSTRUCTOR_ATTRIBUTES_p1_with_comma);
    }else{
        dataText.replace(K_.KEY_CONSTRUCTOR_ATTRIBUTES_p1, V_.empty);
    }

    QRegularExpression KEY_CONSTRUCTOR_ATTRIBUTES_p2("@KEY_CONSTRUCTOR_ATTRIBUTES_p2@");
    string attrib_2 = "";
    if( SD_.add_constructor == true ){
        string space = ",\n        ";
        attrib_2 = attrib_2 + space;
        vector<string> ListParamToAssign = GenerateStringList_name_instance(SD_.ListAttributesInitWithConstructor);
        vector<string> ListMemberAttributes;
        for(unsigned int i=0; i<ListParamToAssign.size(); i++){
            ListMemberAttributes.push_back("dataModel"); // to check!
        }
        attrib_2 = attrib_2 + GenerateListConstructorParametersAssign (ListMemberAttributes, ListParamToAssign);
    }
    QString value_CONSTRUCTOR_ATTRIBUTES_p2 = QString::fromStdString(attrib_2);
    dataText.replace(KEY_CONSTRUCTOR_ATTRIBUTES_p2, value_CONSTRUCTOR_ATTRIBUTES_p2);

    // 3: create new file and insert the dataText
    QFile output_file(SD_.path_skill_folder + SD_.skill_name + "Skill.cpp");
    if( output_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file);
        out << dataText;
    }
    output_file.close();
}

void SkillGenerator::Generate_Skill_DataModel_h(){

    QFile template_file(":/skill-template-files/Name_SkillDataModel_h.t");

    // 1: open and read the template
    template_file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = template_file.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(K_.key_skill_name, SD_.skill_name);

    // 2.1.2 #include @INCLUDE_THRIFT_SERVICE@
    QRegularExpression KEY_INCLUDE_THRIFT_SERVICE("@INCLUDE_THRIFT_SERVICE@");
    string str_include ="";
    for(unsigned int i=0; i<SD_.UsedServices.size(); i++){
        str_include = str_include + "#include \"" + SD_.UsedServices[i].service_type.toStdString()  + ".h\" \n ";
        // with < >
        //str_include = str_include + "#include <" + SD_.UsedServices[i].service_type.toStdString()  + ".h> \n ";
    }
    QString value_INCLUDE_THRIFT_SERVICE = QString::fromStdString(str_include);
    dataText.replace(KEY_INCLUDE_THRIFT_SERVICE, value_INCLUDE_THRIFT_SERVICE);

    // 2.2: KEY_LIST_PUBLIC_ATTRIBUTES
    QRegularExpression KEY_LIST_PUBLIC_ATTRIBUTES("@KEY_LIST_PUBLIC_ATTRIBUTES@");
    string all_instances = "";
    // list of services
    for(unsigned int i=0; i<SD_.UsedServices.size(); i++){
        string single_instance = "" + SD_.UsedServices[i].service_type.toStdString() + " " + SD_.UsedServices[i].name_instance.toStdString() + ";\n    ";
        all_instances = all_instances + single_instance;
    }

    // list of attributes (with value assigned)
    for(unsigned int i=0; i<SD_.UsedAttributes.size(); i++){
        string single_instance = "";
        if( SD_.UsedAttributes[i].init_source.toStdString() == "initialize_inside_header"){
            single_instance = "" + SD_.UsedAttributes[i].data_type.toStdString() + " " + SD_.UsedAttributes[i].name_instance.toStdString() + " { " + SD_.UsedAttributes[i].value.toStdString() + " };\n    " ;
        }
        else{
            single_instance = "" + SD_.UsedAttributes[i].data_type.toStdString() + " " + SD_.UsedAttributes[i].name_instance.toStdString() + ";\n    " ;
        }
        all_instances = all_instances + single_instance;
    }

//    cout << "\n\n\PRINT COMPONENTS --> all_instances : " << all_instances << "\n\n\ " ;
    QString value_LIST_PUBLIC_ATTRIBUTES = QString::fromStdString(all_instances);
    dataText.replace(KEY_LIST_PUBLIC_ATTRIBUTES, value_LIST_PUBLIC_ATTRIBUTES);

    // 2.3 @CONSTRUCTOR@
    QRegularExpression KEY_CONSTRUCTOR("@CONSTRUCTOR@");
    string construct = "";
    if( SD_.add_constructor == false ){ // it is also used in .cpp to insert the not default constructor
        construct = construct + SD_.skill_name.toStdString() + "SkillDataModel() = default;";
    }else{ // e.g.  GoToSkillDataModel(std::string location);
        vector<string> ListParamToAssign_data_type     = GenerateStringList_data_type (SD_.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD_.ListAttributesInitWithConstructor);
        string params = GenerateListConstructorParameters (ListParamToAssign_data_type, ListParamToAssign_name_instance);
        construct = construct + SD_.skill_name.toStdString() + "SkillDataModel(" + params +");";
    }

    QString value_CONSTRUCTOR = QString::fromStdString(construct);
    dataText.replace(KEY_CONSTRUCTOR, value_CONSTRUCTOR);

    // 3: create new file and insert the dataText
    QFile output_file(SD_.path_skill_folder + SD_.skill_name + "SkillDataModel.h");
    if( output_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file);
        out << dataText;
    }
    output_file.close();
}

void SkillGenerator::Generate_Skill_DataModel_cpp(){

    QFile template_file(":/skill-template-files/Name_SkillDataModel_cpp.t");

    // 1: open and read the template
    template_file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = template_file.readAll();

    // 2: replace

    // 2.1: name
    dataText.replace(K_.key_skill_name, SD_.skill_name);

    // 2.2  @OPEN_PORTS_AND_ATTACH_CLIENTS@
    QRegularExpression KEY_OPEN_PORTS_AND_ATTACH_CLIENTS("@OPEN_PORTS_AND_ATTACH_CLIENTS@");
    string merge ="";
    string all_clients ="";
    string all_ports ="";
    for(unsigned int i=0; i<SD_.UsedServices.size(); i++){
        string single_port = "";
        if(Skill_Config_.specify_port_name_attribute){
            // additional name spec if needed
            string port_name_specific = Skill_Config_.port_name_list[0];
            single_port = "    if (!client_port.open(\"/" + SD_.UsedServices[i].name_instance.toStdString() + "Client/" + port_name_specific + "\")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n\n" ;

        }else{
            single_port = "    if (!client_port.open(\"/" + SD_.UsedServices[i].name_instance.toStdString() + "Client\")) {\n       qWarning(\"Error! Cannot open YARP port\");\n       return false;\n    }\n\n" ;
        }
        all_ports = all_ports + single_port;
        string single_client = "    if(!" + SD_.UsedServices[i].name_instance.toStdString() + ".yarp().attachAsClient(client_port)) {\n       qWarning(\"Error! Could not attach as client\");\n       return false;\n    }\n";
        all_clients = all_clients + single_client;
        merge = "    // open ports\n\n" + all_ports + "    // attach as clients\n\n" + all_clients;
    }
    QString value_OPEN_PORTS_AND_ATTACH_CLIENTS = QString::fromStdString(merge);
    dataText.replace(KEY_OPEN_PORTS_AND_ATTACH_CLIENTS, value_OPEN_PORTS_AND_ATTACH_CLIENTS);

    // 2.3  @OPEN_CONNECTIONS_TO_COMPONENTS@
    QRegularExpression KEY_OPEN_CONNECTIONS_TO_COMPONENTS("@OPEN_CONNECTIONS_TO_COMPONENTS@");
    string all_components ="";
    for(unsigned int i=0; i<SD_.UsedServices.size(); i++){
        all_components = all_components + "    if (!yarp::os::Network::connect(client_port.getName(), \"/" + SD_.UsedServices[i].thrift_protocol.toStdString()  + "\", \"" + SD_.UsedServices[i].connect_type.toStdString() + "\")) {\n        qWarning(\"Error! Could not connect to server /fakeBattery\");\n        return false;\n    }\n" ;
    }
    all_components = "    // open connections to components\n\n" + all_components;
    QString value_OPEN_CONNECTIONS_TO_COMPONENTS = QString::fromStdString(all_components);
    dataText.replace(KEY_OPEN_CONNECTIONS_TO_COMPONENTS, value_OPEN_CONNECTIONS_TO_COMPONENTS);

    // 2.4 @ADD_CONSTRUCTOR@ if needed (case not default in .h)
    QRegularExpression KEY_ADD_CONSTRUCTOR("@ADD_CONSTRUCTOR@");
    if( SD_.add_constructor == true ){

        string attributes_init_string_intro = SD_.skill_name.toStdString() + "SkillDataModel::" + SD_.skill_name.toStdString() +"SkillDataModel("; // GoToSkillDataModel::GoToSkillDataModel

        vector<string> ListParamToAssign_data_type     = GenerateStringList_data_type (SD_.ListAttributesInitWithConstructor);
        vector<string> ListParamToAssign_name_instance = GenerateStringList_name_instance (SD_.ListAttributesInitWithConstructor);
        string attributes_init_string_part_1 = GenerateListConstructorParameters (ListParamToAssign_data_type, ListParamToAssign_name_instance);

        string attributes_init_string_mid =") :\n                        ";

        vector<string> ListMemberAttributes = GenerateStringList_name_instance(SD_.ListAttributesInitWithConstructor);
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
    QFile output_file(SD_.path_skill_folder + SD_.skill_name + "SkillDataModel.cpp");
    if( output_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(& output_file);
        out << dataText;
    }
    output_file.close();
}

//void SkillGenerator::Generate_Format(){

//    QFile template_file (path_template_ + "format.t");

//    // 1: open and read the template
//    template_file.open(QIODevice::Text | QIODevice::ReadOnly);
//    QString dataText = template_file.readAll();

//    // 2: replace

//    // 2.1: name
//    dataText.replace(K_.key_skill_name, SD_.skill_name);

//    // 2.2:  ....

//    // 3: create new file and insert the dataText
//    QFile output_file(SD_.path_skill_folder + "format.extension");
//    if( output_file.open(QFile::WriteOnly | QFile::Truncate)) {
//        QTextStream out(& output_file);
//        out << dataText;
//    }
//    output_file.close();
//}

int SkillGenerator::write()
{
    auto docs = tu_->allDocuments;

    for (int i = 0, ei = docs.size(); i != ei; ++i) {
        auto doc = docs.at(i);
        for (DocumentModel::AbstractState* astate : doc->allStates) { // doc->allStates identifies all the states inside the scxml input file
            auto state = astate->asState();
            if (!state) {
                #ifdef _DEBUG
                    qDebug() << astate->id;
                #endif
                continue;
            }
            #ifdef _DEBUG
                qDebug() << state->id << state->xmlLocation.line << state->xmlLocation.column << state->dataElements.size() << state->bt_status;
            #endif
            // need to control the gerarchy of the states, s.t. the external "wrapper" is not tackled into the list
            State stato;
            stato.id = state->id;
            stato.ReturnStatusEnum = state->bt_status;
            stato.ReturnStatus = DecoderEnum(state->bt_status);
            SD_.ListStates.push_back(stato);
        }

        // <datamodel> interpreter
        for (DocumentModel::Node* anode : doc->allNodes) {
            auto data = anode->asDataElement();
            if (!data) {
                // not a <data> element
                continue;
            }
            #ifdef _DEBUG
                cout << "\n\n ************ ACTUAL TEST************  \n\n ";
                qDebug() << data->id << data->expr << data->cpp_type << data->thrift_protocol << data->service_type  << data->client_port_name << data->init_source << data->connect_type;
                cout << "\n\n ************ end ACTUAL TEST************  \n\n ";
            #endif
            // access to data element
            if (data->thrift_protocol != ""){ // means that the data represents a service!
                Service service;
                service.name_instance = data->id;
                service.service_type = data->service_type;
                service.thrift_protocol = data->thrift_protocol;
                service.client_port_name = data->client_port_name;
                service.connect_type = data->connect_type;
                SD_.UsedServices.push_back(service);
            }else if(data->cpp_type != ""){ // means that the data represents an attribute!
                Attribute attribute;
                attribute.data_type = data->cpp_type;
                attribute.name_instance = data->id;
                attribute.value = data->expr;
                attribute.init_source = data->init_source;
                SD_.UsedAttributes.push_back(attribute);
                if(attribute.init_source.toStdString() == "define_in_class_constructor"){
                   SD_.add_constructor = true;
                   SD_.ListAttributesInitWithConstructor.push_back(attribute);
                }
            }else if(data->client_port_name != ""){ // not needed
                ClientPort port;
                port.client_port_name = data->client_port_name;
                SD_.UsedClientPorts.push_back(port);
            }
        }
    }

    #ifdef _DEBUG
        std::cout << "\n\n\n SKILL_DESCRIPTION SD_ : \n      ListStates size : " << SD_.ListStates.size() << "\n\n" ;
        qDebug() << " Print: " << SD_.ListStates[0].id << " " << SD_.ListStates[1].id << " " << SD_.ListStates[2].id << " " << SD_.ListStates[3].id << " .\n\n";
    #endif

    // **********************************++ !generate skill files **********************************++
    Generate_Main();
    Generate_Skill_h();
    Generate_Skill_cpp();
    Generate_Skill_DataModel_h();
    Generate_Skill_DataModel_cpp();

    return NoError;
}
