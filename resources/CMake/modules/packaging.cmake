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

# enable packaging
include(CPack)
