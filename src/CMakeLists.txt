project(rsslib)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# find all source files
file(GLOB_RECURSE RSSLIB_SRCS *.cc)
file(GLOB_RECURSE NON_RSSLIB_SRCS testheader*.cc Playground/*.cc Tests/*.cc UserInterfaces/*.cc)
list(REMOVE_ITEM RSSLIB_SRCS ${NON_RSSLIB_SRCS})

# find all header files
file(GLOB_RECURSE RSSLIB_HDRS *.h)
file(GLOB_RECURSE NON_RSSLIB_HDRS testheader*.h Playground/*.h Tests/*.h UserInterfaces/*.h)
list(REMOVE_ITEM RSSLIB_HDRS ${NON_RSSLIB_HDRS})

# find/configure boost
set(Boost_ADDITIONAL_VERSIONS 1.37)
find_package(Boost 1.37 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# generate library of the above source files
add_library(RSSimulator ${RSSLIB_SRCS})

# if we build a shared library add it to the install target
if(BUILD_SHARED_LIBS)
	install(TARGETS RSSimulator
	        COMPONENT mainprogram
	        LIBRARY DESTINATION lib
	        RUNTIME DESTINATION bin
	)
endif(BUILD_SHARED_LIBS)
