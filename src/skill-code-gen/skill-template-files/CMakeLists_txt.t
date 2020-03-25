################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the skill (state machine executable)
add_executable(@KEY_SKILL_NAME@Skill)
target_include_directories(@KEY_SKILL_NAME@Skill
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})
target_sources(@KEY_SKILL_NAME@Skill
  PRIVATE
    main.cpp
    @KEY_SKILL_NAME@Skill.cpp
    @KEY_SKILL_NAME@Skill.h
    @KEY_SKILL_NAME@SkillDataModel.cpp
    @KEY_SKILL_NAME@SkillDataModel.h)

qt5_add_statecharts(STATECHARTS_FILES @KEY_SKILL_NAME@SkillStateMachine.scxml)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/@KEY_SKILL_NAME@SkillStateMachine.cpp PROPERTIES SKIP_AUTOMOC TRUE)

target_sources(@KEY_SKILL_NAME@Skill
  PRIVATE
    ${STATECHARTS_FILES})

target_link_libraries(@KEY_SKILL_NAME@Skill
  PRIVATE
    Qt5::Scxml
    YARP::YARP_os
    YARP::YARP_init
    GoTo_protocol
    Battery_protocol
    BT_protocol)
