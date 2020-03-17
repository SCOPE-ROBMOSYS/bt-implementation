/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "BatteryNotChargingSkillDataModel.h"
#include <QDebug>
#include <QTimer>
#include <QScxmlStateMachine>



bool BatteryNotChargingSkillDataModel::setup(const QVariantMap &initialDataValues)
{
    Q_UNUSED(initialDataValues)

    if (!yarp::os::NetworkBase::checkNetwork()) {
        qWarning("Error! YARP Network is not initialized");
        return false;
    }

    if (!client_port.open("batteryReaderClient")) {
       qWarning("Error! Cannot open YARP port");
       return false;
    }
    if(!batteryReader.yarp().attachAsClient(client_port)) {
       qWarning("Error! Could not attach as client");
       return false;
    }


    if (!yarp::os::Network::connect(client_port.getName(), "/BatteryComponent", "tcp")) {
        qWarning("Error! Could not connect to server /fakeBattery");
        return false;
    }


    return true;
}
