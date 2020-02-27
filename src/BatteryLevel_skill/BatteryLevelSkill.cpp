#include "BatteryLevelSkill.h"

#include <QDebug>

BatteryLevelSkill::BatteryLevelSkill(std::string name) :
        name(std::move(name))
{
    stateMachine.setDataModel(&dataModel);
}

bool BatteryLevelSkill::start()
{
    if (!yarp::os::NetworkBase::checkNetwork()) {
        qWarning("Error! YARP Network is not initialized");
        return false;
    }

    if (!port.open("/" + name + "/BT_rpc/server")) {
        qWarning("Error! Cannot open YARP port");
        return false;
    }

    if(!this->yarp().attachAsServer(port)) {
        qWarning("Error! Could not attach as server");
        return false;
    }

    stateMachine.start();

    return true;
}

ReturnStatus BatteryLevelSkill::request_status()
{
    auto states = stateMachine.activeStateNames();

    for (const auto& state : states) {
        if (state == "idle") {
            return BT_IDLE;
        }
        if (state == "get") {
            return BT_RUNNING;
        }
        if (state == "success") {
            return BT_SUCCESS;
        }
        if (state == "failure") {
            return BT_FAILURE;
        }
    }

    // error
    return BT_FAILURE;
}


ReturnStatus BatteryLevelSkill::request_tick()
{
    stateMachine.submitEvent("TICK");
    return request_status();
}

void BatteryLevelSkill::request_halt()
{
    stateMachine.submitEvent("HALT");
}
