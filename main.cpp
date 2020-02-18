/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/
/**
 * @file main.cpp
 * @authors: Michele Colledanchise <michele.colledanchise@iit.it>
 */




#include <iostream>
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <yarp_condition.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;
using namespace BT;


class AlwaysSuccessCondition : public ConditionNode
{
  public:
    AlwaysSuccessCondition(const std::string& name) :
        ConditionNode(name, {} )
    {
        setRegistrationID("AlwaysSuccess");
    }

  private:
    int n = 0;
    virtual BT::NodeStatus tick() override
    {
        if(n++ < 3)
        {
            cout << "condition true" << endl;
            return NodeStatus::SUCCESS;
        }
        else
        {
            n = 0;
            cout << "condition false" << endl;
            return NodeStatus::FAILURE;
        }
    }
};


int main()
{
    ReactiveSequence seq("ciao");
    AlwaysSuccessCondition cond("Cond");
    YARPCondition act("name", "/goto_skill/BT_request/server");

    act.init();


    seq.addChild(&cond);
    seq.addChild(&act);




    while(true)
    {

    seq.executeTick();
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));

    }
    return 0;
}

