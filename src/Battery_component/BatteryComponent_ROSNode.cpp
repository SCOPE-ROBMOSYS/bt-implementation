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

#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h" // service battery protocol
#include <cstdlib>

using namespace yarp::dev;

class BatteryComponent //: public BatteryReader
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
        yarp::dev::IBattery::Battery_status status;
        ibattery->getBatteryStatus(status);
        return  (status == yarp::dev::IBattery::BATTERY_OK_IN_CHARGE) ? BATTERY_CHARGING : BATTERY_NOT_CHARGING;
    }

    double level() override
    {
        yWarning("battery level called");
        double battery_charge  = 0;
        ibattery->getBatteryCharge(battery_charge);
        return battery_charge;
    }


private:
    yarp::dev::PolyDriver ddbatteryclient;
    yarp::dev::IBattery * ibattery { nullptr };
    yarp::os::RpcServer server_port;
};

int main() // old
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

// ROS srv definition

bool BatteryReader(beginner_tutorials::AddTwoInts::Request  &req, // QUALE DELLE FUNZIONI?
                   beginner_tutorials::AddTwoInts::Response &res)
{
  res.sum = req.a + req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  return true;
}

int main(int argc, char **argv) // new
{
  ros::init(argc, argv, "BatteryComponent_server");  // server = battery component; client = batteryLevel_skill
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("BatteryReader", BatteryReader);
  ROS_INFO("Ready to call: BatteryReader. Server running happily");
  ros::spin();

  return 0;
}
