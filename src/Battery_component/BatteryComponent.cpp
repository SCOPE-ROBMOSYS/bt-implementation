/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>

#include <BatteryReader.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/IBattery.h>

using namespace yarp::dev;

class BatteryComponent : public BatteryReader
{
public:
    BatteryComponent() = default;

    bool open()
    {
        yarp::os::Property pbatteryclient_cfg;
        pbatteryclient_cfg.put("device", "batteryClient");
        pbatteryclient_cfg.put("remote", "/fakeBattery");
        pbatteryclient_cfg.put("local", "/batteryClient");


        if (!ddbatteryclient.open(pbatteryclient_cfg)) {
            yError("Could not open batteryClient");
            return false;
        }

        if (!ddbatteryclient.view(ibattery)) {
            yError("Could not open batteryClient");
            return false;
        }

        this->yarp().attachAsServer(server_port);
        if (!server_port.open("/BatteryComponent")) {
            yError("Could not open ");
            return false;
        }

        return true;
    }

    void close()
    {
        server_port.close();
    }

    ChargingStatus charging_status() override
    {
        yWarning("charging status called");
        ibattery->getBatteryCurrent(battery_current);

        return  (battery_current < 0) ? BATTERY_CHARGING : BATTERY_NOT_CHARGING;
    }

    double level() override
    {
        yWarning("battery level called");
        ibattery->getBatteryCharge(battery_charge);

        return battery_charge;
    }


private:
    yarp::dev::PolyDriver ddbatteryclient;
    yarp::dev::IBattery * ibattery { nullptr };

    yarp::os::RpcServer server_port;

    double battery_current = 0;
    double battery_charge  = 0;

};

int main()
{
    yarp::os::Network yarp;

    BatteryComponent batteryComponent;
    if (!batteryComponent.open()) {
        return 1;
    }

    while (true) {
        yInfo("Server running happily");
        yarp::os::Time::delay(10);
    }

    batteryComponent.close();

    return 0;
}
