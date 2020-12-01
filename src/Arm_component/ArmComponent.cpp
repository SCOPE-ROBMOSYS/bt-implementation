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
#include <yarp/os/LogComponent.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

using namespace yarp::dev;
using namespace yarp::os;
using namespace std;
using namespace std::chrono_literals;

namespace {
YARP_LOG_COMPONENT(ARMCOMPONENT,
                   "scope.component.Arm",
                   yarp::os::Log::minimumPrintLevel(),
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)
}

#define FAKE_ARM
#define FAKE_ARM_FAILURES 3

class ArmComponent : public ArmService
{
public:
    ArmComponent() = default;

    bool open()
    {
        Network yarp;

#if !defined(FAKE_ARM)
        m_client_port_arm.open(m_client_name_arm);
        m_client_port_wrist.open(m_client_name_wrist);
        m_client_port_hand.open(m_client_name_hand);

        m_client_port_arm.open(m_client_name_arm);
        m_client_port_wrist.open(m_client_name_wrist);
        m_client_port_hand.open(m_client_name_hand);

        yarp.connect(m_client_name_arm, m_server_name_arm);
        yarp.connect(m_client_name_wrist, m_server_name_wrist);
        yarp.connect(m_client_name_hand, m_server_name_hand);
#endif

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
        yCInfo(ARMCOMPONENT, "preGrasp");

#if !defined(FAKE_ARM)
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
            //yCInfo(ARMCOMPONENT) << "Waiting shuoulder";

        }
#endif
        return true;
    }

    bool extractHand() override
    {
        yCInfo(ARMCOMPONENT, "extractHand");
#if !defined(FAKE_ARM)
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
#else
        std::lock_guard<std::mutex> lock(mutex);
        std::this_thread::sleep_for(8s);
        extracted = true;
#endif

        return true;
    }

    bool retractHand() override
    {
        yCInfo(ARMCOMPONENT, "retractHand");

#if !defined(FAKE_ARM)
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
#else
        std::lock_guard<std::mutex> lock(mutex);
        std::this_thread::sleep_for(8s);
        extracted = false;
#endif

        return true;
    }

    bool closeHand() override
    {
        yCInfo(ARMCOMPONENT, "closeHand");

#if !defined(FAKE_ARM)
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

#else
        std::lock_guard<std::mutex> lock(mutex);
        std::this_thread::sleep_for(4s);
        if (extracted && opened) {
            if (failures >= FAKE_ARM_FAILURES) {
                grasped = true;
            } else {
                ++failures;
            }
        }
        opened = false;
#endif
        return true;
    }

    bool openHand() override
    {
        yCInfo(ARMCOMPONENT, "openHand");

#if !defined(FAKE_ARM)
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
#else
        std::lock_guard<std::mutex> lock(mutex);
        std::this_thread::sleep_for(4s);
        opened = true;
        if (grasped) {
            failures = 0;
        }
        grasped = false;
#endif

        return true;
    }

    bool hasGrasped() override
    {
      yCInfo(ARMCOMPONENT, "hasGrasped");

#if !defined(FAKE_ARM)
      // checks 3 times
      bool has_grasped = false;
      unsigned int i = 0;
      while (i++<3)
      {

        Bottle cmd, response;
        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(0);

        m_client_port_hand.write(cmd,response);
        double value_0 = response.get(2).asDouble();

        // response.clear();
        // cmd.addString("get");
        // cmd.addString("vel");
        // cmd.addInt32(0);
        //
        // m_client_port_hand.write(cmd,response);
        //
        // double value_1 = response.get(2).asDouble();

        // std::cout << "ARM "<< value_0 <<  " "<< value_1<< std::endl;
        has_grasped = ( value_0 > 40 && value_0 < 75);
        if (has_grasped)
        {
            yCInfo(ARMCOMPONENT, "something in the hand, checking again");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        else
        {
            yCInfo(ARMCOMPONENT, "nothing in the hand");
            return false;

        }
      }

        return has_grasped;
#else
        std::lock_guard<std::mutex> lock(mutex);
        return grasped;
#endif

    }

    bool home() override
    {
        yCInfo(ARMCOMPONENT, "home");
        retractHand();
        closeHand();

#if !defined(FAKE_ARM)
        // move arm home
        Bottle cmd, response;
        cmd.addString("set");
        cmd.addString("pos");
        cmd.addInt32(0);
        cmd.addDouble(0);

        m_client_port_arm.write(cmd,response);

        // checking with in actually gets there
        cmd.clear();
        response.clear();

        cmd.addString("get");
        cmd.addString("enc");
        cmd.addInt32(0);

        m_client_port_arm.write(cmd,response);

        double real_value = response.get(2).asDouble();

        while (abs(real_value) > 2)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cmd.clear();
            response.clear();

            cmd.addString("get");
            cmd.addString("enc");
            cmd.addInt32(0);

            m_client_port_arm.write(cmd,response);
            real_value = response.get(2).asDouble();
            //yInfo() << "Waiting shoulder";

        }
#else
        std::lock_guard<std::mutex> lock(mutex);
        std::this_thread::sleep_for(10s);
#endif

        return true;
    }



private:

    yarp::os::RpcServer server_port;

#if !defined(FAKE_ARM)
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
#else
    std::mutex mutex;
    bool extracted {false};
    bool opened {false};
    bool grasped {false};
    int failures {0};
#endif

};

int main()
{
    yarp::os::Network yarp;

    ArmComponent armComponent;
    if (!armComponent.open()) {
        return 1;
    }

    while (true) {
        yCInfo(ARMCOMPONENT, "Server running happily");
        yarp::os::Time::delay(10);
    }

    armComponent.close();

    return 0;
}
