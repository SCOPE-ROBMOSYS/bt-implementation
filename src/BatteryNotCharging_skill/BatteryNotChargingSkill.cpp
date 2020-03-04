/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "BatteryNotChargingSkill.h"

#include <QDebug>

BatteryNotChargingSkill::BatteryNotChargingSkill(std::string name) :
        name(std::move(name))
{
    stateMachine.setDataModel(&dataModel);
}

bool BatteryNotChargingSkill::start()
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

ReturnStatus BatteryNotChargingSkill::request_status()
{
    auto states = stateMachine.activeStateNames();

    for (const auto& state : states) {
        if (state == "idle") {
            return BT_IDLE;
        }
        if (state == "get") {
            return BT_IDLE;
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

ReturnStatus BatteryNotChargingSkill::request_tick()
{
    stateMachine.submitEvent("TICK");
    return request_status();
}

void BatteryNotChargingSkill::request_halt()
{
    stateMachine.submitEvent("HALT");
}
