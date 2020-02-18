#ifndef YARP_CONDITION_H
#define YARP_CONDITION_H

#include <yarp_node.h>
#include <string>
class YARPCondition : public YARPNode
{
public:
    YARPCondition(string name, string server_port_name);

    virtual NodeType type() const override final;

    virtual void halt() override final;

};

#endif // YARP_CONDITION_H
