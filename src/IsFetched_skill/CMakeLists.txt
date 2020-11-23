################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the skill (state machine executable)

include(SkillAutogen)

add_executable(IsFetchedSkill)
generate_skills(SOURCES_FILES LIBRARIES IsFetchedSkillStateMachine.scxml)
target_sources(IsFetchedSkill PRIVATE ${SOURCES_FILES})
target_link_libraries(IsFetchedSkill PRIVATE ${LIBRARIES})
