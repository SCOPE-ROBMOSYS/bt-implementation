project(BT-examples)

cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(BehaviorTree REQUIRED)
find_package(YARP REQUIRED)

set(generated_libs_dir "${CMAKE_CURRENT_SOURCE_DIR}")

yarp_idl_to_dir(INPUT_FILES     "${CMAKE_CURRENT_SOURCE_DIR}/thrift/BT_wrappers.thrift"
                OUTPUT_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/generated_files"
                SOURCES_VAR      BT_WRAP_SOURCES
                HEADERS_VAR      BT_WRAP_HEADERS
                INCLUDE_DIRS_VAR BT_WRAP_INCLUDE_DIRS
                )

include_directories(include/ ${BT_WRAP_INCLUDE_DIRS})

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/yarp_action.cpp
)

add_executable(bt_example main.cpp ${SRC_FILES} ${BT_WRAP_SOURCES} )
target_link_libraries(bt_example YARP::YARP_OS YARP::YARP_init BehaviorTree::behaviortree_cpp_v3)
