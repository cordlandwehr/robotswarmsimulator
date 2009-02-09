################################################################################
# Sections that have to be edited:                                             #
#   - configuration                                                            #
#   - sources/headers                                                          #
#   - external libraries (if any)                                              #
#   - executables/libraries                                                    #
################################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


# ------------------------------- configuration --------------------------------

# project name
PROJECT(rsstests)

# names/flags of executables/libraries
# (you have to adjust the executables/libraries section at the end of the file)
SET(PROJECT_EXE1 RSSUnitTests)
SET(PROJECT_EXE1_FLAGS)

# link with the following libraries build by this cmake configuration
SET(INTERNAL_LIBRARIES
	RSS
)


## custom variables
#...


# ----------------------------- standard variables -----------------------------

SET(PROJECT_SRCS_LST ${PROJECT_NAME}_SRCS)
SET(PROJECT_HDRS_LST ${PROJECT_NAME}_HDRS)
SET(NON_PROJECT_SRCS_LST non_${PROJECT_NAME}_SRCS)
SET(NON_PROJECT_HDRS_LST non_${PROJECT_NAME}_HDRS)
SET(PROJECT_LIBRARIES_LST ${PROJECT_NAME}_LIBRARIES)


# ------------------------------ sources/headers -------------------------------

# use these if you're too lazy to maintain the file list (but make sure to
# rerun cmake after adding any files)
FILE(GLOB_RECURSE ${PROJECT_SRCS_LST} *.cc)
#FILE(GLOB_RECURSE ${NON_PROJECT_SRCS_LST} <glob_expression>)
#LIST(REMOVE_ITEM ${PROJECT_SRCS_LST} ${${NON_PROJECT_SRCS_LST}})

FILE(GLOB_RECURSE ${PROJECT_HDRS_LST} *.h)
#FILE(GLOB_RECURSE ${NON_PROJECT_HDRS_LST} <glob_expression>)
#LIST(REMOVE_ITEM ${PROJECT_HDRS_LST} ${${NON_PROJECT_HDRS_LST}})

## prefered way: manage sources/headers manually
#SET(${PROJECT_SRCS_LST}
#	<file_list>
#)
#
#SET(${PROJECT_HDRS_LST}
#	<file_list>
#)

# more custom sources/headers/resources (e.g. for Qt moc/ui/res files)
#...

# add source directory to include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


# ----------------------------- external libraries -----------------------------

# find/configure boost
SET(Boost_ADDITIONAL_VERSIONS 1.37)
FIND_PACKAGE(Boost 1.37 COMPONENTS unit_test_framework)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LIST(APPEND ${PROJECT_LIBRARIES_LST} ${Boost_LIBRARIES})


# ----------------------------- internal libraries -----------------------------

LIST(APPEND ${PROJECT_LIBRARIES_LST} ${INTERNAL_LIBRARIES})


# ------------------------------------ IDE -------------------------------------

# gathers all header files in a group for Xcode
IF(CMAKE_GENERATOR MATCHES "Xcode")
	SOURCE_GROUP("Header Files" FILES ${${PROJECT_HDRS_LST}})
ENDIF(CMAKE_GENERATOR MATCHES "Xcode")


# --------------------------- executables/libraries ----------------------------

# add/link main executable
ADD_EXECUTABLE(${PROJECT_EXE1} ${PROJECT_EXE1_FLAGS} ${${PROJECT_SRCS_LST}} ${${PROJECT_HDRS_LST}})
TARGET_LINK_LIBRARIES(${PROJECT_EXE1} ${${PROJECT_LIBRARIES_LST}})
