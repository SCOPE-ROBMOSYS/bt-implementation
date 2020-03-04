/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
#include "SkillConfigReader.h"

#include <iostream>
#include <fstream> //file stream classes
#include <algorithm>
#include <string>

using namespace std;

SkillConfigReader::SkillConfigReader(string file):
    file_(file)
{
}

SkillConfig SkillConfigReader::ReturnConfig(){ //used to return the value outside
    if (!init()){
        //
    }
    ReadConfig();
    return SC_;
}

bool SkillConfigReader::init(){

    return true;
}

bool SkillConfigReader::ReadConfig(){

    std::ifstream text (file_);

    if (text.is_open())
    {
        std::string line;
        while(getline(text, line)){//parse each single line
            if (line.find("port_name_attribute")!=std::string::npos){ // only if specified
                SC_.specify_port_name_attribute=true;

                auto detectedPosition = line.find(":"); // detect only the delimiter because it returns the start position of the searched string
                if (detectedPosition!=std::string::npos){
                    auto key_port_name = line.substr(0, detectedPosition);
                    auto value_port_name = line.substr(detectedPosition + 1); // remaining part
                    SC_.port_name_list.push_back(value_port_name);
                    int i = SC_.port_name_list.size()-1;
                    cout << "\n\nCONFIG READER DEBUG:\n SC_.port_name -> " << SC_.port_name_list[i] << "\n\n";
                }

            }else{
                SC_.specify_port_name_attribute=false;
//                cout << "\n\nCONFIG READER DEBUG:\n SC_.port_name -> " << "NOT FOUND" << "\n\n";
            }

        }
        return true;
    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
        return false;
    }
}



