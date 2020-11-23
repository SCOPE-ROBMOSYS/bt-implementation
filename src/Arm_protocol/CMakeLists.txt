################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the battery_commands C++ Library
add_library(Arm_protocol STATIC)
yarp_add_idl(IDL_GEN_FILES arm_commands.thrift)
target_sources(Arm_protocol
  PRIVATE
    ${IDL_GEN_FILES})
target_include_directories(Arm_protocol
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(Arm_protocol
  PRIVATE
    YARP::YARP_os)
