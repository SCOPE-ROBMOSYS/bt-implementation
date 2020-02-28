# Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

add_executable(fakeBatteryChargingStation)

yarp_add_idl(IDL_GEN_FILES FakeBatteryService.thrift)

target_sources(fakeBatteryChargingStation PRIVATE fakeBatteryChargingStation.cpp
                                                  ${IDL_GEN_FILES})

target_link_libraries(fakeBatteryChargingStation PRIVATE YARP::YARP_os
                                                         YARP::YARP_sig
                                                         YARP::YARP_dev
                                                         YARP::YARP_init)
