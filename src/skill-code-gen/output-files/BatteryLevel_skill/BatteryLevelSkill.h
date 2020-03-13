/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

//#ifndef KEY_SKILL_NAME_UPPERCASE__SKILL_KEY_SKILL_NAME_UPPERCASE_SKILL_H
//#define KEY_SKILL_NAME_UPPERCASE__SKILL_KEY_SKILL_NAME_UPPERCASE_SKILL_H
# pragma once

#include <BT_request.h>

#include <yarp/os/RpcServer.h>
#include <yarp/os/Network.h>

#include "BatteryLevelSkillStateMachine.h"
#include "BatteryLevelSkillDataModel.h"

class BatteryLevelSkill:
        public BT_request
{
public:
    BatteryLevelSkill(std::string name);

    bool start();

    ReturnStatus request_status() override;
    ReturnStatus request_tick() override;
    void request_halt() override;

private:
    std::string name;
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    BatteryLevelSkillDataModel dataModel;
    BatteryLevelSkillStateMachine stateMachine;
};

//#endif // KEY_SKILL_NAME_UPPERCASE__SKILL_KEY_SKILL_NAME_UPPERCASE_SKILL_H
