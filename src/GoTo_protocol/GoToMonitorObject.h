/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GOTOMONITOROBJECT_H
#define GOTOMONITOROBJECT_H

#include <yarp/os/MonitorObject.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Things.h>
#include <yarp/os/Port.h>

class GoToMonitorObject :
        public yarp::os::MonitorObject
{
public:
    bool create(const yarp::os::Property& options) override;
    yarp::os::Things& update(yarp::os::Things& thing) override;
    yarp::os::Things& updateReply(yarp::os::Things& thing) override;

private:
    bool sender;
    std::string source;
    std::string destination;
    yarp::os::Bottle data;
    yarp::os::Things th;
    yarp::os::Port port;
};

#endif // GOTOMONITOROBJECT_H
