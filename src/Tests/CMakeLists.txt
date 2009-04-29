project(rsstests)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# find all source files
file(GLOB_RECURSE RSSTESTS_SRCS *.cc)

# find all header files
file(GLOB_RECURSE RSSTESTS_HDRS *.h)

# find/configure boost
set(Boost_ADDITIONAL_VERSIONS 1.37)
find_package(Boost 1.37 REQUIRED filesystem system thread unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# find/configure Lua (statically linked for release builds)
reverse_link_preference_in_release()
find_package(Lua51 REQUIRED)
reverse_link_preference_in_release()
include_directories(${LUA_INCLUDE_DIR})

# include directory for Luabind
include_directories(${luabind_SOURCE_DIR})

# find/configure CGAL
find_package(CGAL)
include(${CGAL_USE_FILE})

# find/configure LAPACK
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
	endif(LAPACK_LIBRARIES)
endif(APPLE)

# generate executable of the above source files and link it
add_executable(RSSUnitTests ${RSSTESTS_SRCS})
target_link_libraries(RSSUnitTests
	RSSimulator
	${Boost_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLUT_glut_LIBRARY}
	${LUA_LIBRARIES}
	${CGAL_LIBRARIES}
	${LAPACK_LIBRARIES}
	${BLAS_LIBRARIES}
	luabind
)

# provide a custom target for boost style unit test call
add_custom_target(boost_test RSSUnitTests)

# filter tests from source files
# For this to work, any unit test should be in a file '<rel_path>/<test_name>_test.cc'.
# Furthermore, all testcases inside such files should be enclosed in a test suite called
# '<rel_path>_<test_name>', where any '/' in '<rel_path>' is substituted by an '_'.
foreach(SOURCE_FILE ${RSSTESTS_SRCS})
	file(RELATIVE_PATH SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}" "${SOURCE_FILE}")
	string(REGEX MATCH ".+_test\\.cc$" TEST_FILE ${SOURCE_FILE})
	list(APPEND TEST_FILES ${TEST_FILE})
endforeach(SOURCE_FILE)
foreach(TEST_FILE ${TEST_FILES})
	string(REGEX REPLACE "_test\\.cc$" "" TEST_NAME ${TEST_FILE})
	string(REGEX REPLACE "/" "_" TEST_NAME ${TEST_NAME})
	list(APPEND TEST_NAMES ${TEST_NAME})
endforeach(TEST_FILE)

# add tests
foreach(TEST_NAME ${TEST_NAMES})
	add_test(${TEST_NAME} RSSUnitTests "--run_test=${TEST_NAME}")
endforeach(TEST_NAME)
