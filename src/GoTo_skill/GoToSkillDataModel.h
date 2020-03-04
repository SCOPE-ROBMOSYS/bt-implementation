/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#ifndef GOTO_SKILL_GOTODATAMODEL_H
#define GOTO_SKILL_GOTODATAMODEL_H

#include <QScxmlCppDataModel>
#include <QVariantMap>

#include <QTime>
#include <QTimer>
#include <QDebug>

#include "GoTo.h"
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
            qFatal("Error! Could not connect to server");
        }
    }

    ~Connector()
    {
        if (!yarp::os::Network::disconnect(m_from, m_to)) {
            qFatal("Error! Could not disconnect from server");
        }
    }

    std::string m_from;
    std::string m_to;
    std::string m_carrier;
};

class GoToSkillDataModel: public QScxmlCppDataModel
{
    Q_OBJECT
    Q_SCXML_DATAMODEL

public:
    GoToSkillDataModel(std::string location);

    bool setup(const QVariantMap& initialDataValues) override;

    yarp::os::Network yarp;
    yarp::os::RpcClient client_port;
    GoTo goTo;

    const std::string location;
};

Q_DECLARE_METATYPE(::GoToSkillDataModel*)

#endif // GOTO_SKILL_GOTODATAMODEL_H
