/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
/**
 * @file yarp_action.h
 * @authors: Michele Colledanchise <michele.colledanchise@iit.it>
 */


#pragma once

#include <BT_request.h>
#include <behaviortree_cpp_v3/action_node.h>
#include <yarp/os/RpcClient.h>
#include <BT_request.h>

using namespace std;
using namespace BT;

class YARP_Action : public ActionNodeBase
{
public:
    YARP_Action(string name, string server_port_name);
    void halt() override;
    NodeStatus tick() override;
    bool init();
private:
    string m_client_port_name, m_server_port_name;
    yarp::os::RpcClient m_rpc_client;
    BT_request m_bt_request;


};

