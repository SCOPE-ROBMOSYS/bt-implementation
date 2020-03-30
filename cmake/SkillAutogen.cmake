set(EXECUTABLE skill-code-gen)

function(generate_skills outfiles outlibs)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs OPTIONS)

  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(scxml_files ${ARGN})

  unset(protocol_libs)
  foreach(it ${scxml_files})

    file(STRINGS ${it} strings REGEX "scope_yarp:thrift_protocol")
    foreach(string IN LISTS strings)
      if("${string}" MATCHES "scope_yarp:thrift_protocol=[\"'](.+)[\"']")
        list(APPEND protocol_libs "${CMAKE_MATCH_1}_protocol")
      endif()
    endforeach()

    get_filename_component(outfilename ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)

    set(out_main "${CMAKE_CURRENT_BINARY_DIR}/main.cpp")
    set(out_cpp "${CMAKE_CURRENT_BINARY_DIR}/GoToSkill.cpp")
    set(out_h "${CMAKE_CURRENT_BINARY_DIR}/GoToSkill.h")
    set(out_DataModel_cpp "${CMAKE_CURRENT_BINARY_DIR}/GoToSkillDataModel.cpp")
    set(out_DataModel_h "${CMAKE_CURRENT_BINARY_DIR}/GoToSkillDataModel.h")

    unset(${outfiles})
    list(APPEND ${outfiles} ${out_main})
    list(APPEND ${outfiles} ${out_cpp})
    list(APPEND ${outfiles} ${out_h})
    list(APPEND ${outfiles} ${out_DataModel_cpp})
    list(APPEND ${outfiles} ${out_DataModel_h})

    add_custom_command(OUTPUT ${${outfiles}}
                       COMMAND ${EXECUTABLE} ${infile}
                       MAIN_DEPENDENCY ${infile}
                       DEPENDS ${EXECUTABLE}
                       COMMENT "Generating code from ${it}"
                       VERBATIM)

  endforeach()

  qt5_add_statecharts(STATECHARTS_FILES GoToSkillStateMachine.scxml OPTIONS ${ARGS_OPTIONS})
  set_source_files_properties(${STATECHARTS_FILES} PROPERTIES SKIP_AUTOMOC TRUE)
  list(APPEND ${outfiles} ${STATECHARTS_FILES})

  set_source_files_properties(${${outfiles}} PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})

  set(${outfiles} ${${outfiles}} PARENT_SCOPE)
  set(${outlibs} Qt5::Scxml
                 YARP::YARP_os
                 YARP::YARP_init
                 BT_protocol
                 ${protocol_libs}
                 PARENT_SCOPE)

endfunction()
