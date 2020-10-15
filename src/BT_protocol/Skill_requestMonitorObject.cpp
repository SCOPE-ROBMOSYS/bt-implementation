/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include "Skill_requestMonitorObject.h"

#include <Skill_request.h>
#include <SkillAck.h>
// Include also the .cpp file in order to use the internal types
#include <Skill_request.cpp>
#include <SkillAck.cpp>

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Things.h>

namespace {
YARP_LOG_COMPONENT(SKILLREQUESTMONITOR,
                   "scope.carrier.portmonitor.Skill_request",
                   yarp::os::Log::minimumPrintLevel(),
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)
}

bool Skill_requestMonitorObject::create(const yarp::os::Property& options)
{
    sender = options.find("sender_side").asBool();
    if (!sender) {
        yCError(SKILLREQUESTMONITOR) << "Attaching on receiver side is not supported yet.";
        return false;
    }

    source = options.find("source").asString();
    destination = options.find("destination").asString();

    if (!port.openFake(source + "/monitor")) {
        return false;
    }

    if (!port.addOutput("/monitor")) {
        return false;
    }

    return true;
}

yarp::os::Things& Skill_requestMonitorObject::update(yarp::os::Things& thing)
{
    yCTrace(SKILLREQUESTMONITOR) << "update()";

    yarp::os::Bottle b;
    b.addDouble(yarp::os::SystemClock::nowSystem());
    b.addString(source);
    b.addString(destination);
    b.addString("command");
    //b.addBool(sender);

#if 0
    if (!sender) {
        yCTrace(SKILLREQUESTMONITOR) << "update() -> receiver";
        const auto* cmd = thing.cast_as<yarp::os::Bottle>();
        if (cmd) {
            yCInfo(SKILLREQUESTMONITOR) << "Received command:" << cmd->toString();
        }
        return thing;
    }
# endif
    yCTrace(SKILLREQUESTMONITOR) << "update() -> sender";
//     yarp::os::Portable::copyPortable(*(thing.getPortWriter()), data);
//     yCInfo(SKILLREQUESTMONITOR) << "Sending command:" << data.toString();

    if (/*const auto* cmd = */thing.cast_as<Skill_request_request_ack_helper>()) {
        yCDebug(SKILLREQUESTMONITOR) << "Sending command 'request_ack'";
        b.addString("request_ack");
    } else if (/*const auto* cmd = */thing.cast_as<Skill_request_send_start_helper>()) {
        yCDebug(SKILLREQUESTMONITOR) << "Sending command 'send_start'";
        b.addString("send_start");
    } else if (/*const auto* cmd = */thing.cast_as<Skill_request_send_stop_helper>()) {
        yCDebug(SKILLREQUESTMONITOR) << "Sending command 'send_stop'";
        b.addString("send_stop");
    } else {
        yCWarning(SKILLREQUESTMONITOR) << "Sending unknown command";
        b.addString("[unknown]");
    }

    port.write(b);

    return thing;
}



yarp::os::Things& Skill_requestMonitorObject::updateReply(yarp::os::Things& thing)
{
    yCTrace(SKILLREQUESTMONITOR) << "updateReply()";

    yarp::os::Bottle b;
    b.addDouble(yarp::os::SystemClock::nowSystem());
    b.addString(source);
    b.addString(destination);
    b.addString("reply");
    //b.addBool(sender);

#if 0
    if (!sender) {
        yCTrace(SKILLREQUESTMONITOR) << "update() -> sender";
        yAssert(false); // It doesn't work on receiver side yet
//         yarp::os::Portable::copyPortable(*(thing.getPortWriter()), data);
//         yCInfo(SKILLREQUESTMONITOR) << "Sending reply:" << data.toString();
        return thing;
    }
#endif

    yCTrace(SKILLREQUESTMONITOR) << "update() -> receiver";

    // FIXME SkillAckVocab::toString should be static.
    if (const auto* reply = thing.cast_as<Skill_request_request_ack_helper>()) {
        yCDebug(SKILLREQUESTMONITOR) << "Received reply to 'request_ack':" << SkillAckVocab().toString(reply->m_return_helper);
        b.addString("request_ack");
        b.addInt32(static_cast<int32_t>(reply->m_return_helper));
    } else if (/*const auto* reply = */thing.cast_as<Skill_request_send_start_helper>()) {
        yCDebug(SKILLREQUESTMONITOR) << "Received reply to 'send_start'";
        b.addString("send_stop");
    } else if (/*const auto* reply = */thing.cast_as<Skill_request_send_stop_helper>()) {
        yCDebug(SKILLREQUESTMONITOR) << "Received reply to 'send_stop'";
        b.addString("send_stop");
    } else {
        yCWarning(SKILLREQUESTMONITOR) << "Received unknown reply";
        b.addString("[unknown]");
    }

    port.write(b);

    return thing;
}
