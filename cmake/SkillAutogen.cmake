set(RE-GENERATE-SKILLS TRUE)
if(RE-GENERATE-SKILLS)
    add_custom_command(
       OUTPUT dummy.txt #../GoTo_skill/SomeSource.h SomeSource.cpp #
       #remove old dirs
       COMMAND sudo rm -r /home/scope/bt-implementation/src/BatteryLevel_skill
       COMMAND sudo rm -r /home/scope/bt-implementation/src/BatteryNotCharging_skill
       COMMAND sudo rm -r /home/scope/bt-implementation/src/GoTo_skill
       #create new dirs
       COMMAND mkdir /home/scope/bt-implementation/src/BatteryLevel_skill
       COMMAND cd /home/scope/bt-implementation/src/BatteryLevel_skill
       COMMAND echo ' ' > CMakeLists.txt
       COMMAND mkdir /home/scope/bt-implementation/src/BatteryNotCharging_skill
       COMMAND cd /home/scope/bt-implementation/src/BatteryNotCharging_skill
       COMMAND echo ' ' > CMakeLists.txt
       COMMAND mkdir /home/scope/bt-implementation/src/GoTo_skill
       COMMAND cd /home/scope/bt-implementation/src/GoTo_skill
       COMMAND echo ' ' > CMakeLists.txt
       #generate skill files
       COMMAND skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/BatteryLevelSkillStateMachine.scxml
       COMMAND skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/BatteryNotChargingSkillStateMachine.scxml
       COMMAND skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/GoToSkillStateMachine.scxml
       COMMENT "########################################### SkillAutogen.cmake COMMENT skills generation"
       DEPENDS skill-code-gen
       )
    # necessary otherwise cmake does not execute add_custom_command above
    add_custom_target(dummy_app ALL
        DEPENDS dummy.txt)
endif(RE-GENERATE-SKILLS)

