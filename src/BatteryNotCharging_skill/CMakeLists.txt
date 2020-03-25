################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the skill (state machine executable)
add_executable(BatteryNotChargingSkill)
target_include_directories(BatteryNotChargingSkill
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})
target_sources(BatteryNotChargingSkill
  PRIVATE
    main.cpp
    BatteryNotChargingSkill.cpp
    BatteryNotChargingSkill.h
    BatteryNotChargingSkillDataModel.cpp
    BatteryNotChargingSkillDataModel.h)

qt5_add_statecharts(STATECHARTS_FILES BatteryNotChargingSkillStateMachine.scxml)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/BatteryNotChargingSkillStateMachine.cpp PROPERTIES SKIP_AUTOMOC TRUE)

target_sources(BatteryNotChargingSkill
  PRIVATE
    ${STATECHARTS_FILES})

target_link_libraries(BatteryNotChargingSkill
  PRIVATE
    Qt5::Scxml
    YARP::YARP_os
    YARP::YARP_init
    GoTo_protocol
    Battery_protocol
    BT_protocol)
