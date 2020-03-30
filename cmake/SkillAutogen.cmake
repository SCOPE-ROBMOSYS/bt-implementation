set(RE_GENERATE_SKILLS TRUE)
set(suffix SkillStateMachine.scxml)

#function(GENERATE_SKILLS var)

#if(RE_GENERATE_SKILLS)
#    add_custom_command(
#       COMMENT "################# Skills auto-generation commands inside SkillAutogen.cmake."
#       DEPENDS skill-code-gen

#       OUTPUT dummy.txt

#       MESSAGE "------------------- Start skills auto-generation, inside SkillAutogen.cmake FIRST"
#       #generate skill files
##       COMMAND skill-code-gen  ../src/${SKILL_NAME}_skill/BatteryLevelSkillStateMachine.scxml
##       COMMAND skill-code-gen  ../src/${SKILL_NAME}_skill/BatteryNotChargingSkillStateMachine.scxml
#       COMMAND skill-code-gen  ../src/${SKILL_NAME}_skill/GoToSkillStateMachine.scxml
#       )
#    # necessary otherwise cmake does not execute add_custom_command above
#    add_custom_target(dummy_app ALL
#        DEPENDS dummy.txt)
#endif(RE_GENERATE_SKILLS)

#endfunction()

set(LIST_OF_SKILLS BatteryLevel BatteryNotCharging GoTo)

#set(LIST_OF_INPUT_FILES BatteryLevelSkillStateMachine.scxml BatteryNotChargingSkillStateMachine.scxml GoToSkillStateMachine.scxml)
set(LIST_OF_INPUT_FILES GoToSkillStateMachine.scxml)


set(EXECUTABLE skill-code-gen)
set(SKILL_NAME GoTo)

message("------------------- outside function in SkillAutogen.cmake")

function(generate_skills outfiles)
    if(RE_GENERATE_SKILLS)
        MESSAGE ("------------------- Start skills auto-generation, inside SkillAutogen.cmake")

        set(scxml_files ${LIST_OF_INPUT_FILES})

        foreach(it ${scxml_files})

            message ("------------------- inside foreach")

            get_filename_component(outfilename ${it} NAME_WE)
            message ("------------------- 1 outfilename :  ${outfilename} " )
            get_filename_component(infile ${it} ABSOLUTE)
            message ("------------------- 2 infile :  ${infile} " )

#            set(out_main main.cpp)
#            set(out_cpp GoToSkill.cpp)
#            set(out_h GoToSkill.h)
#            set(out_DataModel_cpp GoToSkillDataModel.cpp)
#            set(out_DataModel_h GoToSkillDataModel.h)

            set(out_main ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
            set(out_cpp ${CMAKE_CURRENT_BINARY_DIR}/GoToSkill.cpp)
            set(out_h ${CMAKE_CURRENT_BINARY_DIR}/GoToSkill.h)
            set(out_DataModel_cpp ${CMAKE_CURRENT_BINARY_DIR}/GoToSkillDataModel.cpp)
            set(out_DataModel_h ${CMAKE_CURRENT_BINARY_DIR}/GoToSkillDataModel.h)

            set(path_output /home/scope/bt-implementation/src/GoTo_skill)

#            set(out_main ${path_output}/main.cpp)
#            set(out_cpp ${path_output}/GoToSkill.cpp)
#            set(out_h ${path_output}/GoToSkill.h)
#            set(out_DataModel_cpp ${path_output}/GoToSkillDataModel.cpp)
#            set(out_DataModel_h ${path_output}/GoToSkillDataModel.h)

            message ("------------------- 3 out_main :  ${out_main} " )

#            unset(outfiles)
            set(${outfiles} ${out_main} ${out_cpp} ${out_h} ${out_DataModel_cpp} ${out_DataModel_h})
#            list(APPEND ${outfiles} ${out_main})
#            list(APPEND ${outfiles} ${out_cpp})
#            list(APPEND ${outfiles} ${out_h})
#            list(APPEND ${outfiles} ${out_DataModel_cpp})
#            list(APPEND ${outfiles} ${out_DataModel_h})

            message ("------------------- 4 outfiles :  ${${outfiles}} " )

            message ("------------------- 4 __END__ : EXECUTABLE -> ${EXECUTABLE}" )

#            set(options)
#            set(oneValueArgs)
#            set(multiValueArgs OPTIONS)

#            cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

            add_custom_command(OUTPUT ${${outfiles}}
                               MAIN_DEPENDENCY ${EXECUTABLE}
                               #DEPENDS ${EXECUTABLE}
                               MESSAGE ("------------------- inside add_custom_command")
                               COMMAND ${EXECUTABLE} ${infile}
                               #ARGS ${ARGS_OPTIONS} --output ${outfile} ${infile}
                               VERBATIM)


#            add_custom_target(dummy_app ALL
#                  DEPENDS ${outfiles}) # ${out_DataModel_h})

        endforeach()

#        set_source_files_properties(${outfiles} PROPERTIES SKIP_AUTOMOC TRUE)
        set(${outfiles} ${${outfiles}} PARENT_SCOPE)

    endif(RE_GENERATE_SKILLS)
endfunction()

function(qt5_add_statecharts outfiles)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(scxml_files ${ARGS_UNPARSED_ARGUMENTS})
    foreach(it ${scxml_files})
        get_filename_component(outfilename ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfilename})
        set(outfile_cpp ${CMAKE_CURRENT_BINARY_DIR}/${outfilename}.cpp)
        set(outfile_h ${CMAKE_CURRENT_BINARY_DIR}/${outfilename}.h)
        add_custom_command(OUTPUT ${outfile_cpp} ${outfile_h}
                           COMMAND ${Qt5Scxml_QSCXMLC_EXECUTABLE}
                           ARGS ${ARGS_OPTIONS} --output ${outfile} ${infile}
                           MAIN_DEPENDENCY ${infile}
                           VERBATIM)
        list(APPEND ${outfiles} ${outfile_cpp})
    endforeach()
    set_source_files_properties(${outfiles} PROPERTIES SKIP_AUTOMOC TRUE)
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()
