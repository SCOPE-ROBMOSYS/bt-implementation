/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "BatteryLevelSkillDataModel.h"
#include <QDebug>
#include <QTimer>
#include <QScxmlStateMachine>

BatteryLevelSkillDataModel::BatteryLevelSkillDataModel(int i) :
                        i(std::move(i))
{
}

bool BatteryLevelSkillDataModel::setup(const QVariantMap &initialDataValues)
{
    Q_UNUSED(initialDataValues)

    if (!yarp::os::NetworkBase::checkNetwork()) {
        qWarning("Error! YARP Network is not initialized");
        return false;
    }

    // open ports

    if (!client_port.open("/batteryReaderClient/" + not_default)) {
       qWarning("Error! Cannot open YARP port");
       return false;
    }

    if (!client_port.open("/bb_instance_nameClient/" + not_default)) {
       qWarning("Error! Cannot open YARP port");
       return false;
    }

    // attach as clients

    if(!batteryReader.yarp().attachAsClient(client_port)) {
       qWarning("Error! Could not attach as client");
       return false;
    }
    if(!bb_instance_name.yarp().attachAsClient(client_port)) {
       qWarning("Error! Could not attach as client");
       return false;
    }


    // open connections to components

    if (!yarp::os::Network::connect(client_port.getName(), "/BatteryComponent", "tcp")) {
        qWarning("Error! Could not connect to server /fakeBattery");
        return false;
    }
    if (!yarp::os::Network::connect(client_port.getName(), "/Blackboard", "tcp")) {
        qWarning("Error! Could not connect to server /fakeBattery");
        return false;
    }


    return true;
}
