################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the skill (state machine executable)

include(SkillAutogen)

add_executable(BatteryLevelSkill)
generate_skills(SOURCES_FILES LIBRARIES BatteryLevelSkillStateMachine.scxml)
target_sources(BatteryLevelSkill PRIVATE ${SOURCES_FILES})
target_link_libraries(BatteryLevelSkill PRIVATE ${LIBRARIES})
