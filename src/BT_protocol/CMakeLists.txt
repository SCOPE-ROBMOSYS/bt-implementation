# Create the BT_protocol C++ Library
add_library(BT_protocol STATIC)
yarp_add_idl(IDL_GEN_FILES BT_wrappers.thrift)
target_sources(BT_protocol
  PRIVATE
    ${IDL_GEN_FILES})
target_include_directories(BT_protocol
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(BT_protocol
  PRIVATE
    YARP::YARP_os)
