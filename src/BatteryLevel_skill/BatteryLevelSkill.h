/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

# pragma once

#include <BT_request.h>

#include <yarp/os/RpcServer.h>
#include <yarp/os/Network.h>

#include "BatteryLevelSkillStateMachine.h"  // not found!!
#include "BatteryLevelSkillDataModel.h"

class BatteryLevelSkill:
        public BT_request
{
public:
    BatteryLevelSkill(std::string name , int i);

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
