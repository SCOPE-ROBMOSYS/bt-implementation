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
#include <yarp_action.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <yarp/os/LogStream.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/actions/always_failure_node.h>
#include <behaviortree_cpp_v3/actions/always_success_node.h>

using namespace std;
using namespace BT;

//class FlipFlopSuccessCondition : public ConditionNode
//{

//public:
//    FlipFlopSuccessCondition(const std::string& name) :
//        ConditionNode(name, {} )
//    {
//        setRegistrationID("AlwaysSuccess");
//    }

//private:
//    int n = 0;
//    virtual BT::NodeStatus tick() override
//    {
//        if(n++ < 3)
//        {
//            cout << "condition true" << endl;
//            return NodeStatus::SUCCESS;
//        }
//        else
//        {
//            n = 0;
//            cout << "condition false" << endl;
//            return NodeStatus::FAILURE;
//        }
//    }
//};


int main()
{
    BehaviorTreeFactory bt_factory;
    bt_factory.registerNodeType<YARPAction>("YARPAction");
    bt_factory.registerNodeType<YARPCondition>("YARPCondition");

    BT::Tree tree = bt_factory.createTreeFromFile("./test_action_BT.xml");

    vector<TreeNode::Ptr> all_nodes_prt = tree.nodes;

    for (TreeNode::Ptr node_prt : all_nodes_prt)
    {
        TreeNode* node = node_prt.get();
        yInfo() << node->name();

        YARPAction* as_yarp_action = static_cast<YARPAction*>(node);
        YARPCondition* as_yarp_condition = static_cast<YARPCondition*>(node);

        if(as_yarp_action = nullptr)
        {
            as_yarp_action->init();
        }
    }

    while(true)
    {
        tree.root_node->executeTick();
        std::this_thread::sleep_for (std::chrono::milliseconds(1000));
    }

    return 0;
}




