################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the Arm_protocol C++ Library
add_library(Arm_protocol STATIC)
#yarp_add_idl(IDL_GEN_FILES arm_commands.thrift)
set(IDL_GEN_FILES
  ArmService.cpp
  ArmService.h
)
target_sources(Arm_protocol
  PRIVATE
    ${IDL_GEN_FILES})
target_include_directories(Arm_protocol
#   INTERFACE
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(Arm_protocol
  PRIVATE
    YARP::YARP_os)


yarp_prepare_plugin(ArmService
  TYPE ArmServiceMonitorObject
  INCLUDE ArmServiceMonitorObject.h
  CATEGORY portmonitor
  INTERNAL ON
)

yarp_add_plugin(yarp_pm_ArmServiceMonitor)

target_sources(yarp_pm_ArmServiceMonitor
  PRIVATE ArmServiceMonitorObject.cpp
          ArmServiceMonitorObject.h
)

target_link_libraries(yarp_pm_ArmServiceMonitor
  PRIVATE YARP::YARP_os
)

target_include_directories(yarp_pm_ArmServiceMonitor
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

yarp_install(
  TARGETS yarp_pm_ArmServiceMonitor
  EXPORT ArmServiceMonitor
  COMPONENT ArmServiceMonitor
  LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
  ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
  YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR}
)
