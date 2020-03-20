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

#Skills
add_subdirectory(GoTo_skill)
add_subdirectory(BatteryLevel_skill)
add_subdirectory(BatteryNotCharging_skill)

set(BAR_FOUND FALSE)
if(BAR_FOUND)
    add_subdirectory(GoTo_skill)
    add_subdirectory(BatteryLevel_skill)
    add_subdirectory(BatteryNotCharging_skill)
endif(BAR_FOUND)

#components
add_subdirectory(Battery_component)
add_subdirectory(GoTo_component)
add_subdirectory(Blackboard_component)

# will be in YARP
add_subdirectory(fakeBatteryChargingStation)
