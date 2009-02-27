# general packaging information
set(CPACK_PACKAGE_VENDOR              "PG Schlaue Schwaerme, Universitaet Paderborn")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TODO: DESCRIPTION_SUMMARY")
set(CPACK_PACKAGE_VERSION             ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY   ${PROJECT_NAME})

# for Mac OS X bundles
set(MACOSX_INFO_PLIST_IN    ${CMAKE_SOURCE_DIR}/resources/MacOSX/Info.plist.in)
set(MACOSX_STARTUPSCRIPT_IN ${CMAKE_SOURCE_DIR}/resources/MacOSX/startup_script.sh.in)
set(MACOSX_INFO_PLIST       ${CMAKE_BINARY_DIR}/resources/MacOSX/Info.plist)
set(MACOSX_BUNDLE_ICON      ${CMAKE_SOURCE_DIR}/resources/MacOSX/bundle_icon.icns)
set(MACOSX_STARTUPSCRIPT    ${CMAKE_BINARY_DIR}/resources/MacOSX/startup_script.sh)
set(CPACK_BUNDLE_NAME            ${PROJECT_NAME})
set(CPACK_BUNDLE_ICON            ${MACOSX_BUNDLE_ICON})
set(CPACK_BUNDLE_PLIST           ${MACOSX_INFO_PLIST})
set(CPACK_BUNDLE_STARTUP_COMMAND ${MACOSX_STARTUPSCRIPT})

# copy/configure some resources
configure_file(${MACOSX_INFO_PLIST_IN} ${MACOSX_INFO_PLIST})
configure_file(${MACOSX_STARTUPSCRIPT_IN} ${MACOSX_STARTUPSCRIPT})

# installs some needed shared runtime libraries on windows systems
include(InstallRequiredSystemLibraries)

# component informations
set(CPACK_COMPONENT_MAINPROGRAM_DISPLAY_NAME "Robot Swarm Simulator")
set(CPACK_COMPONENT_MAINPROGRAM_DESCRIPTION "The main program (and libraries).")
set(CPACK_COMPONENT_GLUTLIBRARIES_DISPLAY_NAME "GLUT Libraries")
set(CPACK_COMPONENT_GLUTLIBRARIES_DESCRIPTION
    "OpenGL Utility Toolkit (dynamic libraries).")
set(CPACK_COMPONENT_LUALIBRARIES_DISPLAY_NAME "Lua Libraries")
set(CPACK_COMPONENT_LUALIBRARIES_DESCRIPTION "Dynamic libraries for Lua.")
set(CPACK_COMPONENT_PROJECTFILES_DISPLAY_NAME "Project Files")
set(CPACK_COMPONENT_PROJECTFILES_DESCRIPTION
    "Some example project files that can be loaded into the simulator.")
set(CPACK_COMPONENT_USERSGUIDE_DISPLAY_NAME "Users Guide")
set(CPACK_COMPONENT_USERSGUIDE_DESCRIPTION
    "Manual of the simulator. Read this to get started.")

# installs some needed shared runtime libraries on windows systems
include(InstallRequiredSystemLibraries)

# enable stripping
# note: disabled on Mac OS X, where standard stripping causes an "Bus Error"
if(NOT APPLE)
	set(CPACK_STRIP_FILES TRUE)
endif(NOT APPLE)

# enable packaging
include(CPack)
