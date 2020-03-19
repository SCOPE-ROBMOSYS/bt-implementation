/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
#include <iostream>

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Property.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>

#include <Blackboard.h>

#include <fstream>
#include <algorithm>
#include <string>
using namespace yarp::os;

class BlackboardComponent : public Blackboard
{
public:
    BlackboardComponent() = default;

    bool init(std::string filename)
    {
        // The file has entry of the type key : value1, value2, value3,...
        yInfo() << "Initializing Blackboard";

        std::ifstream cFile (filename);
        if (cFile.is_open())
        {
            std::string line;
            while(getline(cFile, line)){//parse each single line
                line.erase(std::remove_if(line.begin(), line.end(), isspace),
                                     line.end());
                if(line[0] == '#' || line.empty()) // skypping comments and empylined
                    continue;
                auto delimiterPos = line.find(":"); // delimiter is :
                auto key = line.substr(0, delimiterPos);
                auto allvalues = line.substr(delimiterPos + 1);
                std::string word = "";
                std::stringstream s(allvalues);
                Bottle b_values;
                while (getline(s, word, ',')) {// values are Comma Separated Values
                    b_values.addString(word);
                }
                m_initialization_values[key] = b_values;
            }
            resetData();
            return true;

        }
        else {
            std::cerr << "Couldn't open config file for reading.\n";
            return false;
        }

    }

    bool open()
    {
        this->yarp().attachAsServer(m_server_port);
        if (!m_server_port.open("/BlackboardComponent")) {
            yError("Could not open /BlackboardComponent");
            return false;
        }
        return true;
    }

    void close()
    {
        m_server_port.close();
    }

    yarp::os::Bottle getData(const std::string& target) override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        Bottle b = m_storage[target];
        yInfo() << "getData with target " << b.toString();
        return m_storage[target];
    }

    std::vector<std::string> listAll() override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::string> ret;
        for(auto entry : m_storage)
        {
            yInfo() <<  entry.first  << " : " << entry.second.toString();
            ret.emplace_back(entry.first + " : " + entry.second.toString());
        }
        return ret;
    }

    // erase a single entry
    void clearData(const std::string &target)  override
    {
        m_storage.erase (m_storage.find(target));
    }

    // erase all the memory
    void clearAll() override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_storage.clear();
    }

    void resetData() override
    {
        m_storage.clear();
        m_storage = m_initialization_values;
    }

    bool setData(const std::string& target, const yarp::os::Bottle& datum) override
    {
        /* Using fromString(toString) is the only way known to merge two properties together.
         * In case the pair <key, value> exists only in the lhs, it'll be kept as is
         * In case the pair <key, value> exists only in the rhs, it'll be copied into the lhs
         * In case the pair <key, value> exists in both lhs and rhs, the one in the lhs will
         * overwrite the one in lhs.
         *
         * See https://www.yarp.it/classyarp_1_1os_1_1Property.html for documentation
         */
        std::lock_guard<std::mutex> lock(m_mutex);

        yInfo() << "setData with target " << target << " and params " << datum.toString();

        m_storage[target] = datum;
        return true;
    }

private:
    yarp::os::RpcServer m_server_port;
    std::map<std::string, Bottle> m_storage;
    std::map<std::string, Bottle> m_initialization_values;
    std::mutex                      m_mutex;

};

int main(int argc, char *argv[])
{
    Network yarp;

    BlackboardComponent blackboardComponent;
    if (!blackboardComponent.open()) {
        return 1;
    }

    ResourceFinder rf;
    rf.setVerbose();
    std::string default_cong_filename = "entries.ini";
    bool file_ok = rf.setDefaultConfigFile(default_cong_filename);
    if (!file_ok)
    {
        yWarning() << " Default config file " << default_cong_filename << " not found";
    }
    rf.configure(argc, argv);


    if(!blackboardComponent.init(default_cong_filename))
    {
        return 1;
    }

    while (true) {
        yInfo("Server running happily");
        yarp::os::Time::delay(10);
    }

    blackboardComponent.close();

    return 0;
}

