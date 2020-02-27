# Create the battery_commands C++ Library
add_library(Battery_protocol STATIC)
yarp_add_idl(IDL_GEN_FILES battery_commands.thrift)
target_sources(Battery_protocol
  PRIVATE
    ${IDL_GEN_FILES})
target_include_directories(Battery_protocol
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(Battery_protocol
  PRIVATE
    YARP::YARP_os)
