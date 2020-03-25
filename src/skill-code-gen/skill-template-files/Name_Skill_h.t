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

#include "@KEY_SKILL_NAME@SkillStateMachine.h"  // not found!!
#include "@KEY_SKILL_NAME@SkillDataModel.h"

class @KEY_SKILL_NAME@Skill:
        public BT_request
{
public:
    @KEY_SKILL_NAME@Skill(std::string name @KEY_CONSTRUCTOR_ATTRIBUTES_p1@);

    bool start();

    ReturnStatus request_status() override;
    ReturnStatus request_tick() override;
    void request_halt() override;

private:
    std::string name;
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    @KEY_SKILL_NAME@SkillDataModel dataModel;
    @KEY_SKILL_NAME@SkillStateMachine stateMachine;
};
