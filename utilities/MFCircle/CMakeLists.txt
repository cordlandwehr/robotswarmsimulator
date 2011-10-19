cmake_minimum_required(VERSION 2.8)
project(MFCircle)

option(USE_OPENMP "(De-)activate usage of OpenMP (default = ON)" ON)

# find static boost libraries
find_package(Boost COMPONENTS random date_time REQUIRED)
if (CMAKE_BUILD_TYPE MATCHES Release)
  set(Boost_USE_STATIC_LIBS ON)
endif()

# option to utilize OpenMP set?
if (USE_OPENMP)
  # does the system support OpenMP?
  find_package(OpenMP)
  if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
endif()

# include Boost and if possible OpenMP headers
include_directories(${BOOST_INCLUDE_DIRS} ${OPENMP_INCLUDE_DIRS})

# create MFCircle executable
add_executable(MFCircle move_and_forget_circle.cc)
target_link_libraries(MFCircle ${BOOST_LIBRARIES})
