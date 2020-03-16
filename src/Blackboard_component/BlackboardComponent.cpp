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


using namespace yarp::os;

class BlackboardComponent : public Blackboard
{
public:
    BlackboardComponent() = default;

    bool init(yarp::os::ResourceFinder &rf)
    {
        yInfo() << "Initializing Blackboard";

        Bottle p(rf.toString());
        yDebug() << p.toString();

        // Cycle for each group, i.e. names in square brackets -> [group]
        for(int i=0; i< p.size(); i++)
        {
            Bottle group = *(p.get(i).asList());
            yDebug() << "Group " << group.toString();

            Property prop;
            std::string mapKey = group.get(0).toString();
            yDebug() <<"Key " << mapKey;

            // Cycle for each line in the group
            for(int j=0; j<group.size(); j++)
            {
                // Rows are supposed to be key/data (where data may contains multiple elements)
                if(group.get(j).isList())
                {                    
                    Value row = group.get(j);
                    std::string propKey = row.asList()->get(0).toString();
                    Bottle data = row.asList()->tail();
                    yDebug() << "data " << data.toString();

                    Value a;
                    if(data.size() == 1)
                    {
                        a = data.get(0);
                       // yDebug() << "adding datum " << a.toString();
                    }
                    else
                    {
                        Bottle *abl = a.asList();
                        for(int k=0; k<data.size(); k++)
                        {
                            abl->add(data.get(k));
                            //yDebug() << "adding datum " << (data.get(k)).toString();
                        }
                    }
                    prop.put(propKey, a);
                    m_initialization_values[mapKey] = prop;
                }
            }
        }
        resetData();
        return true;
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

    yarp::os::Property getData(const std::string& target) override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        Property p = m_storage[target];
        yInfo() << "getData with target " << p.toString();
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

    bool setData(const std::string& target, const yarp::os::Property& datum) override
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

        m_storage[target].fromString(datum.toString(), false);
        return true;
    }

private:
    yarp::os::RpcServer m_server_port;
    std::map<std::string, Property> m_storage;
    std::map<std::string, Property> m_initialization_values;
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

    if(!blackboardComponent.init(rf))
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

