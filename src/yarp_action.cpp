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

YARPAction::YARPAction(string name, string server_port_name) : YARPNode(name,server_port_name) {}

NodeType YARPAction::type() const
{
    return NodeType::ACTION;
}

void YARPAction::halt()
{
    yDebug() << "Node" << name() << "sending halt to skill";
    m_bt_request.request_halt();
    // send halt request to server
}
