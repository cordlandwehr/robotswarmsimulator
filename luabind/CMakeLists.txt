project(luabind)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# find all source files
file(GLOB_RECURSE LUABIND_SRCS *.cpp)

# find all header files
#file(GLOB_RECURSE LUABIND_HDRS *.h)

# find/configure boost
set(Boost_ADDITIONAL_VERSIONS 1.37)
find_package(Boost 1.37 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# find/configure Lua
find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# generate library of the above source files
add_library(luabind STATIC ${LUABIND_SRCS})
