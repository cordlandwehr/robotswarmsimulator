project(rssmain)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# find all source files
set(RSSMAIN_SRCS
	SampleGUI.cc
)

# find/configure boost
set(Boost_ADDITIONAL_VERSIONS 1.37)
find_package(Boost 1.37 REQUIRED thread)
include_directories(${Boost_INCLUDE_DIRS})

# find/configure OpenGL
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

# find/configure glut
find_package(GLUT)
include_directories(${GLUT_INCLUDE_DIR})

# generate executable of the above source files and link it
add_executable(RobotSwarmSimulator ${RSSMAIN_SRCS})
target_link_libraries(RobotSwarmSimulator
	RSSimulator
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLUT_glut_LIBRARY}
)

# add executable to install target
install(TARGETS RobotSwarmSimulator
        RUNTIME DESTINATION bin
                COMPONENT mainprogram
)
