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

#include "GoToSkillStateMachine.h"
#include "GoToSkillDataModel.h"

class GoToSkill:
        public BT_request
{
public:
    GoToSkill(std::string name , const std::string location);

    bool start();

    ReturnStatus request_status() override;
    ReturnStatus request_tick() override;
    void request_halt() override;

private:
    std::string name;
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    GoToSkillDataModel dataModel;
    GoToSkillStateMachine stateMachine;
};
