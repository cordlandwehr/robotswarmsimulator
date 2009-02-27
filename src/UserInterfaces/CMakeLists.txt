project(rssmain)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# internal include directories
include_directories("${rsslib_SOURCE_DIR}")
include_directories("${rsslib_BINARY_DIR}")

# find all source files
set(RSSMAIN_SRCS
	robot_swarm_simulator_cmdline.cc
	glut_visualizer.cc
)

# find/configure boost (statically linked for release builds)
if(CMAKE_BUILD_TYPE MATCHES Release)
	set(Boost_USE_STATIC_LIBS TRUE)
endif(CMAKE_BUILD_TYPE MATCHES Release)
set(Boost_ADDITIONAL_VERSIONS 1.37)
find_package(Boost 1.37 REQUIRED program_options thread)
include_directories(${Boost_INCLUDE_DIRS})

# find/configure OpenGL
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

# find/configure glut
find_package(GLUT)
include_directories(${GLUT_INCLUDE_DIR})

# find/configure Lua (statically linked for release builds)
reverse_link_preference_in_release()
find_package(Lua51 REQUIRED)
reverse_link_preference_in_release()
include_directories(${LUA_INCLUDE_DIR})

# include directory for Luabind
include_directories(${luabind_SOURCE_DIR})

# generate executable of the above source files and link it
add_executable(RobotSwarmSimulator ${RSSMAIN_SRCS})
target_link_libraries(RobotSwarmSimulator
	RSSimulator
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLUT_glut_LIBRARY}
	${LUA_LIBRARIES}
	luabind
)

# add executable to install target
install(TARGETS RobotSwarmSimulator
        RUNTIME DESTINATION bin
                COMPONENT mainprogram
)

