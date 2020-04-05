/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "GoToSkillDataModel.h"
#include <QDebug>
#include <QTimer>
#include <QScxmlStateMachine>

GoToSkillDataModel::GoToSkillDataModel(std::string location) :
        location(std::move(location)),
        currVal(0),
        resourceCount(0)
{
    qDebug() << "GoToSkillDataModel::GoToSkillDataModel() called";
}

bool GoToSkillDataModel::setup(const QVariantMap &initialDataValues)
{
    Q_UNUSED(initialDataValues)

    if (!yarp::os::NetworkBase::checkNetwork()) {
        qWarning("Error! YARP Network is not initialized");
        return false;
    }

    if (!id_port.open("...")) {
        qWarning("Error! Cannot open YARP port");
        return false;
    }

    if(!idService.yarp().attachAsClient(id_port)) {
        qWarning("Error! Could not attach as client");
        return false;
    }

    skillID = idService.request_id("goToClient_" + location);

    if (!client_port.open("/goToClient/" + location)) {
        qWarning("Error! Cannot open YARP port");
        return false;
    }

    if(!goTo.yarp().attachAsClient(client_port)) {
        qWarning("Error! Could not attach as client");
        return false;
    }

    if (!blackboard_port.open("/blackboardClient/" + location)) {
        qWarning("Error! Cannot open YARP port");
        return false;
    }

    if(!blackboard.yarp().attachAsClient(blackboard_port)) {
        qWarning("Error! Could not attach as client");
        return false;
    }


    return true;
}
