#include <BT_request.h>

#include <yarp/os/RpcServer.h>

#include "GoToSkillStateMachine.h"
#include "GoToSkillDataModel.h"

class GoToSkill:
        public BT_request
{
public:
    GoToSkill();

    bool start();

    ReturnStatus request_status() override;
    ReturnStatus request_tick() override;
    void request_halt() override;

private:
    yarp::os::Network yarpnet;
    yarp::os::RpcServer port;
    GoToSkillDataModel dataModel;
    GoToSkillStateMachine stateMachine;
};
