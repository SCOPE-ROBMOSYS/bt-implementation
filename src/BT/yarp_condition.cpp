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

}

NodeStatus YARPCondition::tick()
{
    return YARPNode::tick();
}

PortsList YARPCondition::providedPorts()
{
    // This action has a single input port called "message"
    // Any port must have a name. The type is optional.
    return { InputPort<std::string>("port_name") };
}
