/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

//#ifndef KEY_SKILL_NAME_UPPERCASE__SKILL_KEY_SKILL_NAME_UPPERCASE_DATAMODEL_H
//#define KEY_SKILL_NAME_UPPERCASE__SKILL_KEY_SKILL_NAME_UPPERCASE_DATAMODEL_H
#pragma once

#include <QScxmlCppDataModel>
#include <QVariantMap>

#include <QTime>
#include <QTimer>
#include <QDebug>

#include "BatteryReader.h"
#include <yarp/os/Network.h>
#include <yarp/os/RpcClient.h>

struct Connector
{
    Connector(std::string from, std::string to, std::string carrier) :
            m_from(std::move(from)),
            m_to(std::move(to)),
            m_carrier(std::move(carrier))
    {
        if (!yarp::os::Network::connect(m_from, m_to, carrier)) {
            qFatal("Error! Could not connect to server /fakeBattery");
        }
    }

    ~Connector()
    {
        if (!yarp::os::Network::disconnect(m_from, m_to)) {
            qFatal("Error! Could not disconnect from server /fakeBattery");
        }
    }

    std::string m_from;
    std::string m_to;
    std::string m_carrier;
};

class BatteryLevelSkillDataModel: public QScxmlCppDataModel
{
    Q_OBJECT
    Q_SCXML_DATAMODEL

public:
    BatteryLevelSkillDataModel() = default;

    bool setup(const QVariantMap& initialDataValues) override;

    yarp::os::Network yarp;
    yarp::os::RpcClient client_port;

    BatteryReader batteryReader;
    double level { 0.0 }; // added using DATAMODEL 
    
};

Q_DECLARE_METATYPE(::BatteryLevelSkillDataModel*)

//#endif // KEY_SKILL_NAME_UPPERCASE__SKILL_BatteryLevelUPPERCASE_DATAMODEL_H
