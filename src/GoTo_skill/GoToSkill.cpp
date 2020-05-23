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
    qRegisterMetaType<QScxmlEvent>("QScxmlEvent");

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

    stateMachine.connectToEvent("CMD_START", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> CMD_START!";
    });

    stateMachine.connectToEvent("CMD_STOP", [](const QScxmlEvent &){
        qDebug() << QTime::currentTime().toString() << "----> CMD_STOP!";
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
//     QObject::connect(timer, &QTimer::timeout, &stateMachine, [&](){ qDebug() << QTime::currentTime().toString() << "Event CMD_START submitted"; stateMachine.submitEvent("CMD_START"); });
//     timer->start(1000);
//
//
//     auto timer2 = new QTimer(&stateMachine);
//     QObject::connect(timer2, &QTimer::timeout, &stateMachine, [&](){ qDebug() << QTime::currentTime().toString() << "Event CMD_STOP submitted"; stateMachine.submitEvent("CMD_STOP"); });
//     timer2->start(17000);

    return true;
}

SkillAck GoToSkill::request_ack()
{
  stateMachine.submitEvent("CMD_OK");

    while (true) {
        auto states = stateMachine.activeStateNames();

        for (const auto& state : states) {
            if (state == "idle") {
                return SKILL_IDLE;
            }
            if (state == "halted") {
                return SKILL_RUNNING;
            }
            if (state == "success") {
                return SKILL_SUCCESS;
            }
            if (state == "failure") {
                return SKILL_FAILURE;
            }
            if (state == "sendrequest") {
                return SKILL_RUNNING;
            }
            if (state == "getstatus") {
                return SKILL_RUNNING;
            }
            if (state == "lockKey" || state == "lockKey_1" || state == "lockKey_2" || state == "lockKey_3" || state == "internal_lockKey" || state == "checkResource") {
                return BT_RUNNING;
            }
            if (state == "readData" || state == "readData_1" || state == "readData_2" || state == "readData_3" || state == "internal_readData" || state == "internal_halted") {
                return BT_RUNNING;
            }
            if (state == "writeData" || state == "writeData_1" || state == "writeData_2" || state == "writeData_3" || state == "internal_writeData" || state == "rollback_writeData") {
                return BT_RUNNING;
            }
            if (state == "unlockKey" || state == "unlockKey_1" || state == "unlockKey_2" || state == "unlockKey_3" || state == "internal_unlockKey" || state == "unlockBusy") {
                return BT_RUNNING;
            }
            if (state == "wrongKey") {
                return BT_FAILURE;
            }
        }
    }
}

void GoToSkill::send_start()
{
#ifdef DEBUG_STATE_MACHINE
    qDebug() << QTime::currentTime().toString() << "Event CMD_START submitted";
#endif
    stateMachine.submitEvent("CMD_START");
  //  return request_ack();
}

void GoToSkill::send_stop()
{
#ifdef DEBUG_STATE_MACHINE
    qDebug() << QTime::currentTime().toString() << "Event CMD_STOP submitted";
#endif
    stateMachine.submitEvent("CMD_STOP",  QStateMachine::HighPriority);
}
