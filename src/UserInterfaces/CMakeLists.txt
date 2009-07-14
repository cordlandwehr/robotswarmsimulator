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
set(Boost_ADDITIONAL_VERSIONS 1.37 1.37.0 1.38 1.38.0 1.39 1.39.0)
find_package(Boost 1.37 REQUIRED filesystem program_options system thread)
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

# find/configure CGAL
# TODO(peter) packaging of CGAL libraries for release builds
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

# find/configure LAPACK
# TODO(peter) packaging of LAPACK libraries for release builds
if(APPLE)
	find_library(LAPACK_LIBRARIES Accelerate)
	add_definitions(-DUSE_LAPACK)
elseif(WIN32)
	# currently tested only for MinGW
	find_library(LAPACK_LIBRARY lapack)
	find_library(LAPACK_BLAS_LIBRARY blas)
	find_library(LAPACK_F77_LIBRARY F77)
	find_library(LAPACK_I77_LIBRARY I77)
	if(LAPACK_LIBRARY AND LAPACK_BLAS_LIBRARY AND LAPACK_F77_LIBRARY AND LAPACK_I77_LIBRARY)
		add_definitions(-DUSE_LAPACK)
		set(LAPACK_LIBRARIES ${LAPACK_LIBRARY} ${LAPACK_BLAS_LIBRARY} ${LAPACK_F77_LIBRARY} ${LAPACK_I77_LIBRARY})
	endif(LAPACK_LIBRARY AND LAPACK_BLAS_LIBRARY AND LAPACK_F77_LIBRARY AND LAPACK_I77_LIBRARY)
else(APPLE)
	find_package(LAPACK) # do not rely on this for other OS, because it needs a working Fortran environment
	if(LAPACK_LIBRARIES)
		add_definitions(-DUSE_LAPACK)
		set(LAPACK_LIBRARIES ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES})
	endif(LAPACK_LIBRARIES)
endif(APPLE)

# generate executable of the above source files and link it
add_executable(RobotSwarmSimulator ${RSSMAIN_SRCS})
target_link_libraries(RobotSwarmSimulator
	RSSimulator
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLUT_glut_LIBRARY}
	${LUA_LIBRARIES}
	${CGAL_LIBRARIES}
	${LAPACK_LIBRARIES}
	luabind
)

# add executable to install target
install(TARGETS RobotSwarmSimulator
        RUNTIME DESTINATION bin
                COMPONENT mainprogram
)

# for Windows release builds, we need to pack glut and Lua
if(WIN32)
	string(REGEX REPLACE "\\.lib$" ".dll" LUA_LIBRARIES_DLL ${LUA_LIBRARIES})
	string(REGEX REPLACE "\\.lib$" ".dll" GLUT_LIBRARY_DLL ${GLUT_glut_LIBRARY})
	
	if(NOT LUA_LIBRARIES_DLL MATCHES ".*.a$")
		install(FILES ${LUA_LIBRARIES_DLL}
		        DESTINATION bin
		        CONFIGURATIONS Release
		        COMPONENT lualibraries
		)
	endif(NOT LUA_LIBRARIES_DLL MATCHES ".*.a$")
	
	if(NOT GLUT_LIBRARY_DLL MATCHES ".*.a$")
		install(FILES ${GLUT_LIBRARY_DLL}
		        DESTINATION bin
		        CONFIGURATIONS Release
		        COMPONENT glutlibraries
		)
	endif(NOT GLUT_LIBRARY_DLL MATCHES ".*.a$")
endif(WIN32)

