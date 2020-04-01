/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "@KEY_SKILL_NAME@Skill.h"

#include <QTimer>
#include <QDebug>
#include <QTime>
#include <qstatemachine.h>

@KEY_SKILL_NAME@Skill::@KEY_SKILL_NAME@Skill(std::string name @KEY_CONSTRUCTOR_ATTRIBUTES_p1@) :
        name(std::move(name))@KEY_CONSTRUCTOR_ATTRIBUTES_p2@
{
    stateMachine.setDataModel(&dataModel);
}

bool @KEY_SKILL_NAME@Skill::start()
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

ReturnStatus @KEY_SKILL_NAME@Skill::request_status()
{
    auto states = stateMachine.activeStateNames();

    for (const auto& state : states) {
@KEY_SKILL_STATES@
    }
    // error
    return BT_FAILURE;
}

ReturnStatus @KEY_SKILL_NAME@Skill::request_tick()
{
    stateMachine.submitEvent("TICK");
    return request_status();
}

void @KEY_SKILL_NAME@Skill::request_halt()
{
    stateMachine.submitEvent("HALT",  QStateMachine::HighPriority);
}
