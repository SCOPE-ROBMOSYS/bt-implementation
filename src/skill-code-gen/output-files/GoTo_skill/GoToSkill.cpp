/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "GoToSkill.h"

#include <QDebug>

GoToSkill::GoToSkill(std::string name) :
        name(std::move(name)) // KEY_CONSTRUCTOR_ATTRIBUTES
{
    stateMachine.setDataModel(&dataModel);
}

bool GoToSkill::start()
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

ReturnStatus GoToSkill::request_status()
{
    auto states = stateMachine.activeStateNames();

    for (const auto& state : states) {
       if (state == "wrapper") {
           return BT_UNDEFINED;
       }
       if (state == "idle") {
           return BT_UNDEFINED;
       }
       if (state == "sendrequest") {
           return BT_UNDEFINED;
       }
       if (state == "getstatus") {
           return BT_UNDEFINED;
       }
       if (state == "success") {
           return BT_UNDEFINED;
       }
       if (state == "failure") {
           return BT_UNDEFINED;
       }
       if (state == "halted") {
           return BT_UNDEFINED;
       }

    }
    // error
    return BT_FAILURE;
}

ReturnStatus GoToSkill::request_tick()
{
    stateMachine.submitEvent("TICK");
    return request_status();
}

void GoToSkill::request_halt()
{
    stateMachine.submitEvent("HALT");
}
