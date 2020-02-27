/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
/**
 * @file yarp_condition.h
 * @authors: Michele Colledanchise <michele.colledanchise@iit.it>
 */


#include <yarp_condition.h>
#include <yarp/os/LogStream.h>
YARPCondition::YARPCondition(string name, const NodeConfiguration& config) :  ConditionNode(name, config), YARPNode(name,name)

{
    bool ok = init();
    if(!ok)
    {
       yError() << "Something went wrong in the node init() of " << name;
    }
}

NodeStatus YARPCondition::tick()
{
    yDebug() << "Node" << YARPNode::name << "sending tick to skill";
    return YARPNode::tick();
}

PortsList YARPCondition::providedPorts()
{
    // This action has a single input port called "message"
    // Any port must have a name. The type is optional.
    return { InputPort<std::string>("port_name") };
}
