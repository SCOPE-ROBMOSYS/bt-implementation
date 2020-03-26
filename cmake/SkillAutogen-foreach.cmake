set(RE-GENERATE-SKILLS TRUE)
set(suffix SkillStateMachine.scxml)
set(SKILL_NAME BatteryLevel)

set(LIST_OF_SKILLS BatteryLevel BatteryNotCharging GoTo)

foreach(_ARG ${LIST_OF_SKILLS})
    MESSAGE ("-------------------DEBUG foreach")
endforeach()

if(RE-GENERATE-SKILLS)
    add_custom_command(
       COMMENT "################# Skills auto-generation commands inside SkillAutogen.cmake."
       DEPENDS skill-code-gen
       OUTPUT dummy.txt #../GoTo_skill/SomeSource.h SomeSource.cpp #

#       foreach(_ARG ${LIST_OF_SKILLS})
#           MESSAGE ("-------------------DEBUG foreach")
#       endforeach()

       #remove old dirs
#       COMMAND rm -r  ../src/BatteryLevel_skill
#       COMMAND sudo rm -r  ../src/BatteryNotCharging_skill
#       COMMAND sudo rm -r  ../src/GoTo_skill
       #create new dirs
       MESSAGE "------------------- Start skills auto-generation, inside SkillAutogen.cmake"
       COMMAND mkdir -p ../src/BatteryLevel_skill
       COMMAND cd  ../src/BatteryLevel_skill
       COMMAND echo ' ' > CMakeLists.txt
       COMMAND mkdir -p  ../BatteryNotCharging_skill
       COMMAND cd  ../BatteryNotCharging_skill
       COMMAND echo ' ' > CMakeLists.txt
       COMMAND mkdir -p  ../GoTo_skill
       COMMAND cd  ../GoTo_skill
       COMMAND echo ' ' > CMakeLists.txt
       #generate skill files
       COMMAND skill-code-gen  ${SKILL_SCXML_FILES_DIR}/BatteryLevelSkillStateMachine.scxml
       COMMAND skill-code-gen  ${SKILL_SCXML_FILES_DIR}/BatteryNotChargingSkillStateMachine.scxml
       COMMAND skill-code-gen  ${SKILL_SCXML_FILES_DIR}/GoToSkillStateMachine.scxml
       )
    # necessary otherwise cmake does not execute add_custom_command above
    add_custom_target(dummy_app ALL
        DEPENDS dummy.txt)
endif(RE-GENERATE-SKILLS)

