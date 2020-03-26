################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

#BT parsing and execution
add_subdirectory(BT)

# Protocol used between BT and Skill
add_subdirectory(BT_protocol)

# Protocols used between Skill and Component
add_subdirectory(GoTo_protocol)
add_subdirectory(Battery_protocol)
add_subdirectory(Blackboard_protocol)

# Code Generator
add_subdirectory(skill-code-gen)

# need to include .cmake file to generate the skills, launching the skill-code-gen executable
#include(SkillAutogen)

#Skills
add_subdirectory(GoTo_skill)
add_subdirectory(BatteryLevel_skill)
add_subdirectory(BatteryNotCharging_skill)

#components
add_subdirectory(Battery_component)
add_subdirectory(GoTo_component)
add_subdirectory(Blackboard_component)

# will be in YARP
add_subdirectory(fakeBatteryChargingStation)
