/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

struct Data { }
(
  yarp.name = "yarp::os::Property"
  yarp.includefile="yarp/os/Property.h"
)

service Blackboard {
    Data getData(1: string target)
    bool setData(1: string target, 2: Data datum)
    void clearData(1: string target)
    void clearAll()
    void resetData()
    list<string> listAll()
}
