/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#ifndef GOTO_SKILL_GOTOSKILL_H
#define GOTO_SKILL_GOTOSKILL_H

#include <Skill_request.h>

#include <yarp/os/RpcServer.h>
#include <yarp/os/Network.h>

#include "GoToSkillStateMachine.h"
#include "GoToSkillDataModel.h"

class GoToSkill:
        public Skill_request
{
public:
    GoToSkill(std::string name, std::string location);

    bool start();

    SkillAck request_ack() override;
    void send_start() override;
    void send_stop() override;

private:
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    GoToSkillDataModel dataModel;
    GoToSkillStateMachine stateMachine;

    std::string name;
};

#endif // GOTO_SKILL_GOTOSKILL_H
