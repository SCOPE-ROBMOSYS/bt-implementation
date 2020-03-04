################################################################################
#                                                                              #
# Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

set(SOURCES main.cpp)
qt5_add_resources(SOURCES list_templates.qrc)

add_executable(skill-code-gen ${SOURCES})

target_sources(skill-code-gen
  PRIVATE
    main.cpp
    qscxmlcompiler.cpp
    qscxmlcompiler.h
    qscxmlcompiler_p.h
    SkillGenerator.cpp
    SkillGenerator.h
    SkillConfigReader.cpp
    SkillConfigReader.h
)
target_link_libraries(skill-code-gen
  PRIVATE
    Qt5::Core
    Qt5::Scxml
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/qscxmlcompiler.cpp PROPERTIES SKIP_AUTOMOC TRUE)

# BEGIN FIXME This should not be here
target_include_directories(skill-code-gen
  PRIVATE
    /usr/include/x86_64-linux-gnu/qt5/QtScxml/5.12.5/QtScxml/private/
    /usr/include/x86_64-linux-gnu/qt5/QtScxml/5.12.5/QtScxml/
    /usr/include/x86_64-linux-gnu/qt5/QtScxml/5.12.5/
    /usr/include/x86_64-linux-gnu/qt5/QtCore/5.12.5/QtCore/private/
    /usr/include/x86_64-linux-gnu/qt5/QtCore/5.12.5/QtCore/
    /usr/include/x86_64-linux-gnu/qt5/QtCore/5.12.5/
)
# END FIXME This should not be here

target_compile_definitions(skill-code-gen
  PRIVATE
    BUILD_QSCXMLC) # Used in qscxmlcompiler.cpp

target_compile_options(skill-code-gen
  PRIVATE
    -Wno-overloaded-virtual)
