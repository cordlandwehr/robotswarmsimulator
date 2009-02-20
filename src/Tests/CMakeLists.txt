project(rsstests)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# find all source files
file(GLOB_RECURSE RSSTESTS_SRCS *.cc)

# find all header files
file(GLOB_RECURSE RSSTESTS_HDRS *.h)

# find/configure boost
set(Boost_ADDITIONAL_VERSIONS 1.37)
find_package(Boost 1.37 REQUIRED thread unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# generate executable of the above source files and link it
add_executable(RSSUnitTests ${RSSTESTS_SRCS})
target_link_libraries(RSSUnitTests
	RSSimulator
	${Boost_LIBRARIES}
#	${OPENGL_LIBRARIES}
#	${GLUT_glut_LIBRARY}
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
