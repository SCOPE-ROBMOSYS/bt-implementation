# Create the battery_commands C++ Library
add_library(battery_protocol STATIC)
yarp_add_idl(IDL_GEN_FILES battery_commands.thrift)
target_sources(battery_protocol
  PRIVATE
    ${IDL_GEN_FILES})
target_include_directories(battery_protocol
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(battery_protocol
  PRIVATE
    YARP::YARP_os)
