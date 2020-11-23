/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>

#include <ArmService.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/IBattery.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/Network.h>

#include <thread>
#include <chrono>
#include <iostream>
using namespace yarp::dev;
using namespace yarp::os;
using namespace std;

class ArmComponent : public ArmService
{
public:
    ArmComponent() = default;

    bool open()
    {
        Network yarp;

        RpcClient port;
        m_client_port_arm.open(m_client_name_arm);
        m_client_port_wrist.open(m_client_name_wrist);
        m_client_port_hand.open(m_client_name_hand);

        m_client_port_arm.open(m_client_name_arm);
        m_client_port_wrist.open(m_client_name_wrist);
        m_client_port_hand.open(m_client_name_hand);

        yarp.connect(m_client_name_arm, m_server_name_arm);
        yarp.connect(m_client_name_wrist, m_server_name_wrist);
        yarp.connect(m_client_name_hand, m_server_name_hand);

        this->yarp().attachAsServer(server_port);
        if (!server_port.open("/ArmComponent"))
        {
            yError("Could not open /ArmComponent");
            return false;
        }

        return true;
    }

    void close()
    {
        server_port.close();
    }

    bool preGrasp() override
    {
        yInfo("preGrasp");

        Bottle cmd, response;
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(0);
        cmd.addDouble(m_arm_target);

        m_client_port_arm.write(cmd,response);

        // checking with in actually gets there
        cmd.clear();
        response.clear();

        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(0);

        m_client_port_arm.write(cmd,response);

        double real_value = response.get(2).asDouble();

        while (abs(real_value - m_arm_target) > 2)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cmd.clear();
            response.clear();

            cmd.addString("get");
            cmd.addString("enc");
            cmd.addInt32(0);

            m_client_port_arm.write(cmd,response);
            real_value = response.get(2).asDouble();
            //yInfo() << "Waiting shuoulder";

        }
        return true;
    }

    bool extractHand() override
    {
        yInfo("extractHand");
        Bottle cmd, response;

        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(5);
        cmd.addDouble(m_wrist_target);

        m_client_port_wrist.write(cmd,response);


        cmd.clear();
        response.clear();

        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(5);

        m_client_port_wrist.write(cmd,response);

        double real_value = response.get(2).asDouble();

        while (abs(real_value - m_wrist_target) > 0.01)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cmd.clear();
            response.clear();

            cmd.addString("get");
            cmd.addString("enc");
            cmd.addInt32(5);

            m_client_port_wrist.write(cmd,response);
            real_value = response.get(2).asDouble();
        }

        return true;
    }

    bool retractHand() override
    {
        yInfo("retractHand");

        Bottle cmd, response;
        cmd.clear();
        response.clear();
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(5);
        cmd.addDouble(0.0);

        m_client_port_wrist.write(cmd,response);

        cmd.clear();
        response.clear();

        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(5);

        m_client_port_wrist.write(cmd,response);

        double real_value = response.get(2).asDouble();

        while (abs(real_value) > 0.01)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cmd.clear();
            response.clear();

            cmd.addString("get");
            cmd.addString("enc");
            cmd.addInt32(5);

            m_client_port_wrist.write(cmd,response);
            real_value = response.get(2).asDouble();
        }
        return true;
    }

    bool closeHand() override
    {
        yInfo("closeHand");

        Bottle cmd, response;
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(0);
        cmd.addDouble(m_hand_closed_target);

        m_client_port_hand.write(cmd,response);


        cmd.clear();
        response.clear();
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(1);
        cmd.addDouble(m_hand_closed_target);

        m_client_port_hand.write(cmd,response);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        return true;
    }

    bool openHand() override
    {
        yInfo("openHand");

        Bottle cmd, response;
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(0);
        cmd.addDouble(35);

        m_client_port_hand.write(cmd,response);


        cmd.clear();
        response.clear();
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(1);
        cmd.addDouble(20);

        m_client_port_hand.write(cmd,response);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        return true;
    }

    bool hasGrasped() override
    {
        yInfo("hasGrasped");

        Bottle cmd, response;
        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(0);

        m_client_port_hand.write(cmd,response);
        double value_0 = response.get(2).asDouble();

        response.clear();
        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(1);

        m_client_port_hand.write(cmd,response);

        double value_1 = response.get(2).asDouble();

        std::cout <<  "left thumb " << value_1<< std::endl;
        bool has_grasped = value_1 < 70 && value_1 > 40;

        if (has_grasped)
        {
          yInfo("something in the hand");
        }
        else
        {
          yInfo("nothing in the hand");
        }


        return has_grasped;

    }

    bool home() override
    {
        yInfo("home");
        return true;
    }



private:

    yarp::os::RpcServer server_port;

    string m_client_name_arm = "/ArmComponent/arm/rpc:o";
    string m_client_name_wrist = "/ArmComponent/wrist/rpc:o";
    string m_client_name_hand = "/ArmComponent/hand/rpc:o";

    string m_server_name_arm = "/cer/left_arm/rpc:i";
    string m_server_name_wrist = "/cer/left_arm/rpc:i";
    string m_server_name_hand = "/cer/left_hand/rpc:i";

    RpcClient m_client_port_arm;
    RpcClient m_client_port_wrist;
    RpcClient m_client_port_hand;

    double m_arm_target = 40;
    double m_wrist_target = 0.1;
    double m_hand_closed_target = 80;
    double m_hand_open_target = 20;

};

int main()
{
    yarp::os::Network yarp;

    ArmComponent armComponent;
    if (!armComponent.open()) {
        return 1;
    }

    while (true) {
        yInfo("Server running happily");
        yarp::os::Time::delay(10);
    }

    armComponent.close();

    return 0;
}
