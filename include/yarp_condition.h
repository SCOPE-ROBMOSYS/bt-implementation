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


#pragma once

#include <yarp_node.h>
#include <string>

class YARPCondition : public YARPNode
{
public:
    YARPCondition(string name, string server_port_name);
    virtual NodeType type() const override final;
    virtual void halt() override final;
};
