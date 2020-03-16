/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "GoToSkillDataModel.h"
#include <QDebug>
#include <QScxmlStateMachine>

bool GoToSkillDataModel::setup(const QVariantMap &initialDataValues)
{
    Q_UNUSED(initialDataValues)

    if (!yarp::os::NetworkBase::checkNetwork()) {
        qWarning("Error! YARP Network is not initialized");
        return false;
    }

    if (!client_port.open("//goToClient/location")) {
       qWarning("Error! Cannot open YARP port");
       return false;
    }
    if(!goTo.yarp().attachAsClient(client_port)) {
       qWarning("Error! Could not attach as client");
       return false;
    }


    return true;
}
