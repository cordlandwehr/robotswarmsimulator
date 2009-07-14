project(luabind)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# find all source files
file(GLOB_RECURSE LUABIND_SRCS *.cpp)

# find all header files
#file(GLOB_RECURSE LUABIND_HDRS *.h)

# find/configure boost
set(Boost_ADDITIONAL_VERSIONS 1.37 1.37.0 1.38 1.38.0 1.39 1.39.0)
find_package(Boost 1.37 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# find/configure Lua (statically linked for release builds)
reverse_link_preference_in_release()
find_package(Lua51 REQUIRED)
reverse_link_preference_in_release()
include_directories(${LUA_INCLUDE_DIR})

# generate library of the above source files
add_library(luabind STATIC ${LUABIND_SRCS})
