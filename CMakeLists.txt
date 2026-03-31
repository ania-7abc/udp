cmake_minimum_required(VERSION 3.14)
project(udp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_library(udp src/udp.cpp)
set_target_properties(udp PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
target_include_directories(udp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(udp PRIVATE Boost::system)

install(TARGETS udp EXPORT udpTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(EXPORT udpTargets
    FILE udpTargets.cmake
    NAMESPACE ania-7abc::
    DESTINATION lib/cmake/udp
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/udpConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/udpConfig.cmake
    INSTALL_DESTINATION lib/cmake/udp
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/udpConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/udpConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/udpConfigVersion.cmake
    DESTINATION lib/cmake/udp
)
