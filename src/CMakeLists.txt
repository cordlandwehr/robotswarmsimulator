project(rsslib)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# process config header
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

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

# find/configure Lua (statically linked for release builds)
reverse_link_preference_in_release()
find_package(Lua51 REQUIRED)
reverse_link_preference_in_release()
include_directories(${LUA_INCLUDE_DIR})

# find/configure CGAL
find_package(CGAL)
if(CGAL_FOUND)
	include(${CGAL_USE_FILE})
	add_definitions(-DUSE_CGAL)
endif(CGAL_FOUND)

# find/configure LAPACK
if(APPLE)
	add_definitions(-DUSE_LAPACK)
elseif(WIN32)
	# currently tested only for MinGW
	find_library(LAPACK_LIBRARY lapack)
	find_library(LAPACK_BLAS_LIBRARY blas)
	find_library(LAPACK_F77_LIBRARY F77)
	find_library(LAPACK_I77_LIBRARY I77)
	if(LAPACK_LIBRARY AND LAPACK_BLAS_LIBRARY AND LAPACK_F77_LIBRARY AND LAPACK_I77_LIBRARY)
		add_definitions(-DUSE_LAPACK)
	endif(LAPACK_LIBRARY AND LAPACK_BLAS_LIBRARY AND LAPACK_F77_LIBRARY AND LAPACK_I77_LIBRARY)
endif(APPLE)

# include directory for Luabind
include_directories(${luabind_SOURCE_DIR})

# generate library of the above source files
add_library(RSSimulator STATIC ${RSSLIB_SRCS})
