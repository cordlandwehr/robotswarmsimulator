cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE false)

# project name and version
project("Robot Swarm Simulator")
set(PROJECT_VERSION 0.7)

# module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/resources/CMake/modules)

# our code uses min/max at some point --> this clashes with a macro definition of MSVC
add_definitions(-DNOMINMAX)

# this is needed on some systems due to a bug in current CGAL CMake scripts
add_definitions(-frounding-math)

# if we're building an eclipse project, create a dummy 'source project'
# (this is needed for SVN to work)
set(ECLIPSE_CDT4_GENERATE_SOURCE_PROJECT true)

# enable testing
enable_testing()

# the following macro allows easy switching of link preferences (shared or static)
# for release builds. Initially, shared libraries are prefered by CMake.
macro(reverse_link_preference_in_release)
	if(CMAKE_BUILD_TYPE MATCHES Release)
		list(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
	endif(CMAKE_BUILD_TYPE MATCHES Release)
endmacro(reverse_link_preference_in_release)


# fixes eclipse (linux) problem that indicates STD/STL include files as not being present (although the compiler finds them)
include_directories(/usr/include/c++/4.3)

# descend into subprojects
add_subdirectory(luabind)
add_subdirectory(src)
add_subdirectory(src/UserInterfaces)
add_subdirectory(src/Tests)
add_subdirectory(documentation)

# package projectfiles in src/Projectfiles for release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
	install(DIRECTORY src/ProjectFiles/
	        DESTINATION ProjectFiles
	        CONFIGURATIONS Release
	        COMPONENT projectfiles
	        FILES_MATCHING
	        PATTERN *.swarm
	        PATTERN *.robot
	        PATTERN *.obstacle
	        PATTERN *.lua
	        PATTERN .svn EXCLUDE
	)
endif(CMAKE_BUILD_TYPE MATCHES Release)

# package Lua algorithms in src/LuaAlgorithms for release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
	install(DIRECTORY src/LuaAlgorithms/
	        DESTINATION LuaAlgorithms
	        CONFIGURATIONS Release
	        COMPONENT projectfiles
	        PATTERN .svn EXCLUDE
	)
endif(CMAKE_BUILD_TYPE MATCHES Release)

# package logo
if(CMAKE_BUILD_TYPE MATCHES Release)
	install(DIRECTORY resources/Textures/
	        DESTINATION bin/resources/Textures
	        CONFIGURATIONS Release
	        COMPONENT mainprogram
	        PATTERN .svn EXCLUDE
	)
endif(CMAKE_BUILD_TYPE MATCHES Release)

# packaging
include(packaging)
