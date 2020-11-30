/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include "ArmServiceMonitorObject.h"

#include "ArmService.h"
// Include also the .cpp file in order to use the internal types
#include "ArmService.cpp"

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Things.h>

namespace {
YARP_LOG_COMPONENT(ARMSERVICEMONITOR,
                   "scope.carrier.portmonitor.ArmService",
                   yarp::os::Log::minimumPrintLevel(),
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)

constexpr yarp::conf::vocab32_t VOCAB_OK = yarp::os::createVocab('o', 'k');
constexpr yarp::conf::vocab32_t VOCAB_FAIL = yarp::os::createVocab('f', 'a', 'i', 'l');
}

bool ArmServiceMonitorObject::create(const yarp::os::Property& options)
{
    sender = options.find("sender_side").asBool();
    if (!sender) {
        yCError(ARMSERVICEMONITOR) << "Attaching on receiver side is not supported yet.";
        return false;
    }

    source = options.find("source").asString();
    destination = options.find("destination").asString();

    if (!port.open/*Fake*/(source + "/monitor")) {
        return false;
    }

    if (!port.addOutput("/monitor")) {
        return false;
    }

    return true;
}

yarp::os::Things& ArmServiceMonitorObject::update(yarp::os::Things& thing)
{
    std::lock_guard<std::mutex> lock(mutex);

    yCTrace(ARMSERVICEMONITOR) << "update()";

    yarp::os::Bottle msg;
    msg.addDouble(yarp::os::SystemClock::nowSystem());
    msg.addString(source);
    msg.addString(destination);
    msg.addString("command");
    msg.addString("ArmService");
    //msg.addBool(sender);
    auto& bcmd = msg.addList();
    auto& bargs [[maybe_unused]] = msg.addList();
    auto& breply [[maybe_unused]] = msg.addList();

#if 0
    if (!sender) {
        yCTrace(ARMSERVICEMONITOR) << "update() -> receiver";
        const auto* cmd = thing.cast_as<yarp::os::Bottle>();
        if (cmd) {
            yCInfo(ARMSERVICEMONITOR) << "Received command:" << cmd->toString();
        }
        return thing;
    }
# endif
    yCTrace(ARMSERVICEMONITOR) << "update() -> sender";
//     yarp::os::Portable::copyPortable(*(thing.getPortWriter()), data);
//     yCInfo(ARMSERVICEMONITOR) << "Sending command:" << data.toString();

    if (/*const auto* cmd = */thing.cast_as<ArmService_preGrasp_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'preGrasp'";
        bcmd.addString("preGrasp");
    } else if (/*const auto* cmd = */thing.cast_as<ArmService_extractHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'extractHand'";
        bcmd.addString("extractHand");
    } else if (/*const auto* cmd = */thing.cast_as<ArmService_retractHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'retractHand'";
        bcmd.addString("retractHand");
    } else if (/*const auto* cmd = */thing.cast_as<ArmService_closeHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'closeHand'";
        bcmd.addString("closeHand");
    } else if (/*const auto* cmd = */thing.cast_as<ArmService_openHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'openHand'";
        bcmd.addString("openHand");
    } else if (/*const auto* cmd = */thing.cast_as<ArmService_hasGrasped_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'hasGrasped'";
        bcmd.addString("hasGrasped");
    } else if (/*const auto* cmd = */thing.cast_as<ArmService_home_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Sending command 'home'";
        bcmd.addString("home");
    } else {
        yCWarning(ARMSERVICEMONITOR) << "Sending unknown command";
        bcmd.addString("[unknown]");
    }

    yCDebug(ARMSERVICEMONITOR, "Writing: %s", msg.toString().c_str());
    port.write(msg);


    yCTrace(ARMSERVICEMONITOR) << "update() returning";

    return thing;
}



yarp::os::Things& ArmServiceMonitorObject::updateReply(yarp::os::Things& thing)
{
    std::lock_guard<std::mutex> lock(mutex);

    yCTrace(ARMSERVICEMONITOR) << "updateReply()";
    yarp::os::Bottle msg;
    msg.addDouble(yarp::os::SystemClock::nowSystem());
    msg.addString(source);
    msg.addString(destination);
    msg.addString("reply");
    //b.addBool(sender);
    msg.addString("ArmService");
    auto& bcmd = msg.addList();
    auto& bargs [[maybe_unused]] = msg.addList();
    auto& breply [[maybe_unused]] = msg.addList();

#if 0
    if (!sender) {
        yCTrace(ARMSERVICEMONITOR) << "update() -> sender";
        yAssert(false); // It doesn't work on receiver side yet
//         yarp::os::Portable::copyPortable(*(thing.getPortWriter()), data);
//         yCInfo(ARMSERVICEMONITOR) << "Sending reply:" << data.toString();
        return thing;
    }
#endif

    yCTrace(ARMSERVICEMONITOR) << "update() -> receiver";

    if (const auto* reply = thing.cast_as<ArmService_preGrasp_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'preGrasp'";
        bcmd.addString("preGrasp");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else if (const auto* reply = thing.cast_as<ArmService_extractHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'extractHand'";
        bcmd.addString("extractHand");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else if (const auto* reply = thing.cast_as<ArmService_retractHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'retractHand'";
        bcmd.addString("retractHand");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else if (const auto* reply = thing.cast_as<ArmService_closeHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'closeHand'";
        bcmd.addString("closeHand");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else if (const auto* reply = thing.cast_as<ArmService_openHand_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'openHand'";
        bcmd.addString("openHand");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else if (const auto* reply = thing.cast_as<ArmService_hasGrasped_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'hasGrasped'";
        bcmd.addString("hasGrasped");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else if (const auto* reply = thing.cast_as<ArmService_home_helper>()) {
        yCDebug(ARMSERVICEMONITOR) << "Received reply to 'home'";
        bcmd.addString("home");
        breply.addInt32(reply->m_return_helper ? VOCAB_OK : VOCAB_FAIL);
    } else {
        yCWarning(ARMSERVICEMONITOR) << "Received unknown reply";
        bcmd.addString("[unknown]");
    }

    yCDebug(ARMSERVICEMONITOR, "Writing: %s", msg.toString().c_str());
    port.write(msg);

    yCTrace(ARMSERVICEMONITOR) << "updateReply() returning";

    return thing;
}
