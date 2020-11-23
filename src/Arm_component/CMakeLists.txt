################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################


add_executable(ArmComponent)
target_sources(ArmComponent
  PRIVATE
    ArmComponent.cpp)
target_link_libraries(ArmComponent
  PRIVATE
    Arm_protocol
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_dev)
