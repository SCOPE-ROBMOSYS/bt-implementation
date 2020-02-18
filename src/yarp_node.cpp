/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
/**
 * @file YARPNode.cpp
 * @authors: Michele Colledanchise <michele.colledanchise@iit.it>
 */


#include <yarp_node.h>
#include <behaviortree_cpp_v3/leaf_node.h>
#include <BT_request.h>
#include <iostream>
#include <yarp/os/RpcClient.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;
using namespace BT;

YARPNode::YARPNode(string name, string server_port_name) : LeafNode(name, {}), m_client_port_name("/"+name+"/BT_rpc/client"), m_server_port_name(server_port_name)
{}

bool YARPNode::init()
{
    yarp::os::Network yarp;


    if (!m_rpc_client.open(m_client_port_name))
    {
        yError() << "Could not open port " << m_client_port_name;
        return false;
    }


    if (!yarp.connect(m_client_port_name, m_server_port_name))
    {
        yError() << "Could not connect to port " << m_server_port_name;
        return false;
    }


    if (!m_bt_request.yarp().attachAsClient(m_rpc_client))
    {
        yError() << "Could not attach as client to " << m_server_port_name;
        return false;
    }
    yDebug() << "Node" << name() << "initialized correctly";

    return true;
}

NodeStatus YARPNode::tick()
{
    yDebug() << "Node" << name() << "ticked";

    ReturnStatus status = m_bt_request.request_tick();

    while(status==BT_IDLE)
    {
        status = m_bt_request.request_status();
        std::this_thread::sleep_for (std::chrono::milliseconds(100));

    }

    switch (status) {
    case BT_RUNNING:
        yDebug() << "Node" << name() << "returns running";
        return NodeStatus::RUNNING;// may be two different enums (thrift and BT library). Making sure that the return status are the correct ones
    case BT_SUCCESS:
        yDebug() << "Node" << name() << "returns success";
        return NodeStatus::SUCCESS;
    case BT_FAILURE:
        yDebug() << "Node" << name() << "returns failure";
        return NodeStatus::FAILURE;
    default:
        yError() << "Invalid return status for received by node " << name();
        break;
    }
    return NodeStatus::RUNNING;
}


NodeStatus YARPNode::status() const
{

    yDebug() << "Node" << name() << "getting status";
    ReturnStatus status = m_bt_request.request_status();
    switch (status) {
    case BT_RUNNING:
        yDebug() << "Node" << name() << "returns running";
        return NodeStatus::RUNNING;// may be two different enums (thrift and BT library). Making sure that the return status are the correct ones
    case BT_SUCCESS:
        yDebug() << "Node" << name() << "returns success";
        return NodeStatus::SUCCESS;
    case BT_FAILURE:
        yDebug() << "Node" << name() << "returns failure";
        return NodeStatus::FAILURE;
    default:
        yError() << "Invalid return status for received by node " << name();
        break;
    }
}


