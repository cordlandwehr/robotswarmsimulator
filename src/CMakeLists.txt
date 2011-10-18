project(rsslib)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# process config header
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# find all source files
file(GLOB_RECURSE RSSLIB_SRCS *.cc)
if(RSS_NO_VISUALIZATION)
	file(GLOB_RECURSE NON_RSSLIB_SRCS testheader*.cc Playground/*.cc Tests/*.cc UserInterfaces/*.cc Gui/*.cc OpenGL/*.cc Visualization/*.cc)
else(RSS_NO_VISUALIZATION)
	file(GLOB_RECURSE NON_RSSLIB_SRCS testheader*.cc Playground/*.cc Tests/*.cc UserInterfaces/*.cc)
endif(RSS_NO_VISUALIZATION)
list(REMOVE_ITEM RSSLIB_SRCS ${NON_RSSLIB_SRCS})

# find all header files
file(GLOB_RECURSE RSSLIB_HDRS *.h)
if(RSS_NO_VISUALIZATION)
	file(GLOB_RECURSE NON_RSSLIB_HDRS testheader*.h Playground/*.h Tests/*.h UserInterfaces/*.h Gui/*.h OpenGL/*.h Visualization/*.h)
else(RSS_NO_VISUALIZATION)
	file(GLOB_RECURSE NON_RSSLIB_HDRS testheader*.h Playground/*.h Tests/*.h UserInterfaces/*.h)
endif(RSS_NO_VISUALIZATION)
list(REMOVE_ITEM RSSLIB_HDRS ${NON_RSSLIB_HDRS})

# find/configure boost
if(CMAKE_BUILD_TYPE MATCHES Release)
        set(Boost_USE_STATIC_LIBS TRUE)
endif(CMAKE_BUILD_TYPE MATCHES Release)
set(Boost_ADDITIONAL_VERSIONS 1.37 1.37.0 1.38 1.38.0 1.39 1.39.0)
find_package(Boost 1.37 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# find/configure Lua (statically linked for release builds)
reverse_link_preference_in_release()
find_package(Lua51 REQUIRED)
reverse_link_preference_in_release()
include_directories(${LUA_INCLUDE_DIR})

# find/configure LAPACK
if(APPLE)
	#find_library(LAPACK_LIBRARIES Accelerate)
	add_definitions(-DUSE_LAPACK)
	# force dynamic string support on Snow Leopard
	set(CMAKE_CXX_FLAGS "-D_GLIBCXX_FULLY_DYNAMIC_STRING")
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
	if(CMAKE_BUILD_TYPE MATCHES Release)
		# we can't use find_package(LAPACK) here (as for non-release builds), due to a bug #354463 of the lapack .deb package. Therefor, find the correct libraries manually.
		reverse_link_preference_in_release()
		find_library(LAPACK_LIBRARY lapack-3)
		find_library(LAPACK_BLAS_LIBRARY blas-3)
		find_library(LAPACK_GFORTRAN_LIBRARY gfortran)
		reverse_link_preference_in_release()
		if (LAPACK_LIBRARY AND LAPACK_BLAS_LIBRARY AND LAPACK_GFORTRAN_LIBRARY)
			add_definitions(-DUSE_LAPACK)
			set(LAPACK_LIBRARIES ${LAPACK_LIBRARY} ${LAPACK_BLAS_LIBRARY} ${LAPACK_GFORTRAN_LIBRARY})
		endif(LAPACK_LIBRARY AND LAPACK_BLAS_LIBRARY AND LAPACK_GFORTRAN_LIBRARY)
	else(CMAKE_BUILD_TYPE MATCHES Release)
		find_package(LAPACK) # do not rely on this for other OS, because it needs a working Fortran environment
		if(LAPACK_LIBRARIES)
			add_definitions(-DUSE_LAPACK)
			set(LAPACK_LIBRARIES ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES})
		endif(LAPACK_LIBRARIES)	
	endif(CMAKE_BUILD_TYPE MATCHES Release)
endif(APPLE)

# include directory for Luabind
include_directories(${luabind_SOURCE_DIR})

if(RSS_NO_VISUALIZATION)
else(RSS_NO_VISUALIZATION)
	# find/configure Qt4
	set(QT_USE_QTOPENGL TRUE)
	find_package(Qt4 REQUIRED)
	
	include(${QT_USE_FILE})
	
	file(GLOB_RECURSE RSSLIB_QT4_SRCS Gui/*.h)
	file(GLOB_RECURSE RSSLIB_QT4_UIS Gui/*.ui)
	file(GLOB_RECURSE RSSLIB_QT4_RES ${CMAKE_SOURCE_DIR}/resources/*.qrc)
	
	QT4_WRAP_UI(RSSLIB_UI_CPP ${RSSLIB_QT4_UIS})
	QT4_WRAP_CPP(RSSLIB_MOC_CPP ${RSSLIB_QT4_SRCS})
	QT4_ADD_RESOURCES(RSSLIB_RES_H ${RSSLIB_QT4_RES})
	
	list(APPEND RSSLIB_SRCS ${RSSLIB_MOC_CPP} ${RSSLIB_UI_CPP} ${RSSLIB_RES_H})
endif(RSS_NO_VISUALIZATION)

find_package(Threads REQUIRED)

# generate library of the above source files
add_library(RSSimulator STATIC ${RSSLIB_SRCS})
target_link_libraries(RSSimulator
	${CMAKE_THREAD_LIBS_INIT}
)
