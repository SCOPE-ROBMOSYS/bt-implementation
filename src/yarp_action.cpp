#include <yarp_condition.h>
#include <yarp/os/LogStream.h>
YARPCondition::YARPCondition(string name, string server_port_name) : YARPNode(name,server_port_name)
{

}




NodeType YARPCondition::type() const
{
    return NodeType::CONDITION;
}

void YARPCondition::halt()
{
    yWarning() << "The condition " << name() << " received an halt. You may want to check your code.";
    // just in case, but it should not be needed
    setStatus(NodeStatus::IDLE);
}


