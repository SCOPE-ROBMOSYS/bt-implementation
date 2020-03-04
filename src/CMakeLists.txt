################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# Protocol used between BT and Skill
add_subdirectory(BT_protocol)

# Protocols used between Skill and Component
add_subdirectory(GoTo_protocol)
add_subdirectory(Battery_protocol)

# Code Generator
add_subdirectory(skill-code-gen)

add_subdirectory(BT)
add_subdirectory(GoTo_skill)
add_subdirectory(GoTo_component)
add_subdirectory(BatteryLevel_skill)
add_subdirectory(BatteryNotCharging_skill)
add_subdirectory(Battery_component)

# will be in YARP
add_subdirectory(fakeBatteryChargingStation)
