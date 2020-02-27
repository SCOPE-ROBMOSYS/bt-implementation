#ifndef GOTO_SKILL_GOTOSKILL_H
#define GOTO_SKILL_GOTOSKILL_H

#include <BT_request.h>

#include <yarp/os/RpcServer.h>
#include <yarp/os/Network.h>

#include "GoToSkillStateMachine.h"
#include "GoToSkillDataModel.h"

class GoToSkill:
        public BT_request
{
public:
    GoToSkill(std::string name, std::string location);

    bool start();

    ReturnStatus request_status() override;
    ReturnStatus request_tick() override;
    void request_halt() override;

private:
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    GoToSkillDataModel dataModel;
    GoToSkillStateMachine stateMachine;

    std::string name;
};

#endif // GOTO_SKILL_GOTOSKILL_H
