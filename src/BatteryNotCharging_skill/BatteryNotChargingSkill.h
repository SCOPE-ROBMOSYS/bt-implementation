/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#ifndef BATTERYNOTCHARGING_SKILL_BATTERYNOTCHARGINGSKILL_H
#define BATTERYNOTCHARGING_SKILL_BATTERYNOTCHARGINGSKILL_H

#include <Skill_request.h>

#include <yarp/os/RpcServer.h>
#include <yarp/os/Network.h>

#include "BatteryNotChargingSkillStateMachine.h"
#include "BatteryNotChargingSkillDataModel.h"

class BatteryNotChargingSkill:
        public Skill_request
{
public:
    BatteryNotChargingSkill(std::string name);

    bool start();

    SkillAck request_ack() override;
    void send_start() override;
    void send_stop() override;

private:
    std::string name;
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    BatteryNotChargingSkillDataModel dataModel;
    BatteryNotChargingSkillStateMachine stateMachine;
};

#endif // BATTERYNOTCHARGING_SKILL_BATTERYNOTCHARGINGSKILL_H
