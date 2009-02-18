cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE false)

# project name and version
project("Robot Swarm Simulator")
set(PROJECT_VERSION 0.1)

# module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/resources/CMake/modules)

# if we're building an eclipse project, create a dummy 'source project'
# (this is needed for SVN to work)
set(ECLIPSE_CDT4_GENERATE_SOURCE_PROJECT true)

# enable testing
enable_testing()

# descend into subprojects
add_subdirectory(src)
add_subdirectory(src/UserInterfaces)
add_subdirectory(src/Tests)
add_subdirectory(documentation)

# packaging
include(packaging)
