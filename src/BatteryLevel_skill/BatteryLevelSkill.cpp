/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

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

SkillAck BatteryLevelSkill::request_ack()
{
    while (true) {
        auto states = stateMachine.activeStateNames();

        for (const auto& state : states) {
            if (state == "idle") {
              stateMachine.submitEvent("REQUEST_ACK");
                return SKILL_IDLE;
            }
            if (state == "get") {
              stateMachine.submitEvent("REQUEST_ACK");
                return SKILL_IDLE;
            }
            if (state == "success") {
              stateMachine.submitEvent("REQUEST_ACK");
                return SKILL_SUCCESS;
            }
            if (state == "failure") {
              stateMachine.submitEvent("REQUEST_ACK");
                return SKILL_FAILURE;
            }
        }
    }
}


void BatteryLevelSkill::send_start()
{
    stateMachine.submitEvent("TICK");
    //return request_ack();
}

void BatteryLevelSkill::send_stop()
{
    stateMachine.submitEvent("HALT");
}
