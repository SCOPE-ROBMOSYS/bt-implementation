################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Create the skill (state machine executable)

include(SkillAutogen)

add_executable(FetchSkill)
generate_skills(SOURCES_FILES LIBRARIES FetchSkillStateMachine.scxml)
target_sources(FetchSkill PRIVATE ${SOURCES_FILES})
target_link_libraries(FetchSkill PRIVATE ${LIBRARIES})
