/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#include "GoToSkill.h"

#include <QTimer>
#include <QDebug>
#include <QTime>
#include <qstatemachine.h>

#define DEBUG_STATE_MACHINE

GoToSkill::GoToSkill(std::string name, std::string location) :
        dataModel(std::move(location)),
        name(std::move(name))
{
    stateMachine.setDataModel(&dataModel);

#ifdef DEBUG_STATE_MACHINE
    stateMachine.connectToState("wrapper", [](bool active) {
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the wrapper state";
    });

    stateMachine.connectToState("idle", [](bool active) {
        if(active) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        }
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the idle state";
        if(!active) {
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }
    });

    stateMachine.connectToState("wrapperLock", [](bool active) {
    if(active) {
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    }
    qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the wrapperLock state";
    if(!active) {
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    }
    });

    stateMachine.connectToState("wrapperUnlock_1", [](bool active) {
    if(active) {
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    }
    qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the wrapperUnlock_1 state";
    if(!active) {
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    }
    });

    stateMachine.connectToState("wrapperUnlock_2", [](bool active) {
    if(active) {
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    }
    qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the wrapperUnlock_2 state";
    if(!active) {
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    }
    });

    stateMachine.connectToState("wrapperUnlock_3", [](bool active) {
    if(active) {
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    }
    qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the wrapperUnlock_3 state";
    if(!active) {
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
    }
    });

    stateMachine.connectToState("sendrequest", [](bool active) {
        if(active) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        }
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the sendrequest state";
        if(!active) {
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }
    });

    stateMachine.connectToState("getstatus", [](bool active) {
        if(active) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        }
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the getstatus state";
        if(!active) {
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }
    });

    stateMachine.connectToState("success", [](bool active) {
        if(active) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        }
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the success state";
        if(!active) {
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }
    });

    stateMachine.connectToState("failure", [](bool active) {
        if(active) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        }
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the failure state";
        if(!active) {
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }
    });

    stateMachine.connectToState("halted", [](bool active) {
        if(active) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        }
        qDebug() << QTime::currentTime().toString() << (active ? "entered" : "exited") << "the halted state";
        if(!active) {
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }
    });

    stateMachine.connectToEvent("TICK", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> TICK!";
    });

    stateMachine.connectToEvent("HALT", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> HALT!";
    });

    stateMachine.connectToEvent("STATUS_READY", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> STATUS_READY!";
    });

    stateMachine.connectToEvent("STATUS_RUNNING", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> STATUS_RUNNING!";
    });

    stateMachine.connectToEvent("STATUS_SUCCESS", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> STATUS_SUCCESS!";
    });

    stateMachine.connectToEvent("STATUS_FAILED", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> STATUS_FAILED!";
    });

    stateMachine.connectToEvent("RESET", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> RESET!";
    });
#endif
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

//     auto timer = new QTimer(&stateMachine);
//     QObject::connect(timer, &QTimer::timeout, &stateMachine, [&](){ qDebug() << QTime::currentTime().toString() << "Event TICK submitted"; stateMachine.submitEvent("TICK"); });
//     timer->start(1000);
//
//
//     auto timer2 = new QTimer(&stateMachine);
//     QObject::connect(timer2, &QTimer::timeout, &stateMachine, [&](){ qDebug() << QTime::currentTime().toString() << "Event HALT submitted"; stateMachine.submitEvent("HALT"); });
//     timer2->start(17000);

    return true;
}

ReturnStatus GoToSkill::request_status()
{
    while (true) {
        auto states = stateMachine.activeStateNames();

        for (const auto& state : states) {
            if (state == "idle") {
                return BT_IDLE;
            }
            if (state == "halted") {
                return BT_RUNNING;
            }
            if (state == "success") {
                return BT_SUCCESS;
            }
            if (state == "failure") {
                return BT_FAILURE;
            }
            if (state == "sendrequest") {
                return BT_RUNNING;
            }
            if (state == "getstatus") {
                return BT_RUNNING;
            }
            if (state == "lockKey" || state == "lockKey_1" || state == "lockKey_2" || state == "lockKey_3" || state == "checkResource") {
                return BT_RUNNING;
            }
            if (state == "readData" || state == "readData_1" || state == "readData_2" || state == "readData_3") {
                return BT_RUNNING;
            }
            if (state == "writeData" || state == "writeData_1" || state == "writeData_2" || state == "writeData_3") {
                return BT_RUNNING;
            }
            if (state == "unlockKey" || state == "unlockKey_1" || state == "unlockKey_2" || state == "unlockKey_3" || state == "unlockBusy") {
                return BT_RUNNING;
            }
            if (state == "wrongKey") {
                return BT_FAILURE;
            }
        }
    }
}

ReturnStatus GoToSkill::request_tick()
{
#ifdef DEBUG_STATE_MACHINE
    qDebug() << QTime::currentTime().toString() << "Event TICK submitted";
#endif
    stateMachine.submitEvent("TICK");
    return request_status();
}

void GoToSkill::request_halt()
{
#ifdef DEBUG_STATE_MACHINE
    qDebug() << QTime::currentTime().toString() << "Event HALT submitted";
#endif
    stateMachine.submitEvent("HALT",  QStateMachine::HighPriority);
}
