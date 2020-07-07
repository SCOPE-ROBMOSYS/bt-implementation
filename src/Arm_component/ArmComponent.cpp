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

using namespace yarp::dev;

class ArmComponent : public ArmService
{
public:
    ArmComponent() = default;

    bool open()
    {

        this->yarp().attachAsServer(server_port);
        if (!server_port.open("/ArmComponent")) {
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
        return true;
    }

    bool extractHand() override
    {
        yInfo("extractHand");
        return true;
    }

    bool retractHand() override
    {
        yInfo("retractHand");
        return true;
    }

    bool closeHand() override
    {
        yInfo("closeHand");
        return true;
    }

    bool home() override
    {
        yInfo("home");
        return true;
    }

private:

    yarp::os::RpcServer server_port;
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
