/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
/**
 * @file yarp_action.cpp
 * @authors: Michele Colledanchise <michele.colledanchise@iit.it>
 */


#include <yarp_action.h>
#include <yarp/os/LogStream.h>

YARPAction::YARPAction(string name, const NodeConfiguration& config) :  ActionNodeBase(name, config), YARPNode(name,name)
{

    bool ok = init();

    if(!ok)
    {
        yError() << "Something went wront";
    }
}

NodeStatus YARPAction::tick()
{
    return YARPNode::tick();
}

PortsList YARPAction::providedPorts()
{
    // This action has a single input port called "message"
    // Any port must have a name. The type is optional.
    return { InputPort<std::string>("port_name") };
}

void YARPAction::halt()
{
    yDebug() << "Node" << ActionNodeBase::name() << "sending halt to skill";
    m_bt_request.request_halt();
    // send halt request to server
}

