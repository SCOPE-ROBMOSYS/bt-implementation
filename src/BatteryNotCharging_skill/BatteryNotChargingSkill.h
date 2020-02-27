#ifndef BATTERYNOTCHARGING_SKILL_BATTERYNOTCHARGINGSKILL_H
#define BATTERYNOTCHARGING_SKILL_BATTERYNOTCHARGINGSKILL_H

#include <BT_request.h>

#include <yarp/os/RpcServer.h>
#include <yarp/os/Network.h>

#include "BatteryNotChargingSkillStateMachine.h"
#include "BatteryNotChargingSkillDataModel.h"

class BatteryNotChargingSkill:
        public BT_request
{
public:
    BatteryNotChargingSkill(std::string name);

    bool start();

    ReturnStatus request_status() override;
    ReturnStatus request_tick() override;
    void request_halt() override;

private:
    std::string name;
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    BatteryNotChargingSkillDataModel dataModel;
    BatteryNotChargingSkillStateMachine stateMachine;
};

#endif // BATTERYNOTCHARGING_SKILL_BATTERYNOTCHARGINGSKILL_H
