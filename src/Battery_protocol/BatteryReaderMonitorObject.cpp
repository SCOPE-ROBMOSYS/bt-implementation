/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include "BatteryReaderMonitorObject.h"

#include "BatteryReader.h"
#include "ChargingStatus.h"
// Include also the .cpp file in order to use the internal types
#include "BatteryReader.cpp"
#include "ChargingStatus.cpp"

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Things.h>

namespace {
YARP_LOG_COMPONENT(BATTERYREADERMONITOR,
                   "scope.carrier.portmonitor.BatteryReader",
                   yarp::os::Log::minimumPrintLevel(),
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)
}

bool BatteryReaderMonitorObject::create(const yarp::os::Property& options)
{
    sender = options.find("sender_side").asBool();
    if (!sender) {
        yCError(BATTERYREADERMONITOR) << "Attaching on receiver side is not supported yet.";
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

yarp::os::Things& BatteryReaderMonitorObject::update(yarp::os::Things& thing)
{
    yCTrace(BATTERYREADERMONITOR) << "update()";

    yarp::os::Bottle b;
    b.addDouble(yarp::os::SystemClock::nowSystem());
    b.addString(source);
    b.addString(destination);
    b.addString("command");
    //b.addBool(sender);

#if 0
    if (!sender) {
        yCTrace(BATTERYREADERMONITOR) << "update() -> receiver";
        const auto* cmd = thing.cast_as<yarp::os::Bottle>();
        if (cmd) {
            yCInfo(BATTERYREADERMONITOR) << "Received command:" << cmd->toString();
        }
        return thing;
    }
# endif
    yCTrace(BATTERYREADERMONITOR) << "update() -> sender";
//     yarp::os::Portable::copyPortable(*(thing.getPortWriter()), data);
//     yCInfo(BATTERYREADERMONITOR) << "Sending command:" << data.toString();

    if (const auto* reply = thing.cast_as<BatteryReader_level_helper>()) {
        yCDebug(BATTERYREADERMONITOR) << "Sending command 'level'";
        b.addString("level");
    } else if (const auto* reply = thing.cast_as<BatteryReader_charging_status_helper>()) {
        yCDebug(BATTERYREADERMONITOR) << "Sending command 'charging_status'";
        b.addString("charging_status");
    } else {
        yCWarning(BATTERYREADERMONITOR) << "Sending unknown command";
        b.addString("[unknown]");
    }

    port.write(b);

    return thing;
}



yarp::os::Things& BatteryReaderMonitorObject::updateReply(yarp::os::Things& thing)
{
    yCTrace(BATTERYREADERMONITOR) << "updateReply()";

    yarp::os::Bottle b;
    b.addDouble(yarp::os::SystemClock::nowSystem());
    b.addString(source);
    b.addString(destination);
    b.addString("reply");
    //b.addBool(sender);

#if 0
    if (!sender) {
        yCTrace(BATTERYREADERMONITOR) << "update() -> sender";
        yAssert(false); // It doesn't work on receiver side yet
//         yarp::os::Portable::copyPortable(*(thing.getPortWriter()), data);
//         yCInfo(BATTERYREADERMONITOR) << "Sending reply:" << data.toString();
        return thing;
    }
#endif

    yCTrace(BATTERYREADERMONITOR) << "update() -> receiver";

    if (const auto* reply = thing.cast_as<BatteryReader_level_helper>()) {
        yCDebug(BATTERYREADERMONITOR) << "Received reply to 'level':" << reply->m_return_helper;
        b.addString("level");
        b.addFloat64(reply->m_return_helper);
    } else if (const auto* reply = thing.cast_as<BatteryReader_charging_status_helper>()) {
        // FIXME ChargingStatusVocab::toString should be static.
        yCDebug(BATTERYREADERMONITOR) << "Received reply to 'charging_status'" << ChargingStatusVocab().toString(reply->m_return_helper);
        b.addString("send_stop");
        b.addInt32(reply->m_return_helper);
    } else {
        yCWarning(BATTERYREADERMONITOR) << "Received unknown reply";
        b.addString("[unknown]");
    }
    port.write(b);

    return thing;
}
