################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the skill (state machine executable)
add_executable(GoToSkill)
target_include_directories(GoToSkill
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})
target_sources(GoToSkill
  PRIVATE
    main.cpp
    GoToSkill.cpp
    GoToSkill.h
    GoToSkillDataModel.cpp
    GoToSkillDataModel.h)

qt5_add_statecharts(STATECHARTS_FILES GoToSkillStateMachine.scxml)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/GoToSkillStateMachine.cpp PROPERTIES SKIP_AUTOMOC TRUE)

target_sources(GoToSkill
  PRIVATE
    ${STATECHARTS_FILES})

target_link_libraries(GoToSkill
  PRIVATE
    Qt5::Scxml
    YARP::YARP_os
    YARP::YARP_init
    GoTo_protocol
    BT_protocol
    Blackboard_protocol)
