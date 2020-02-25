# Create the GoTo_protocol C++ Library
add_library(GoTo_protocol STATIC)
yarp_add_idl(IDL_GEN_FILES GoTo.thrift)
target_sources(GoTo_protocol
  PRIVATE
    ${IDL_GEN_FILES})
target_include_directories(GoTo_protocol
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(GoTo_protocol
  PRIVATE
    YARP::YARP_os)
