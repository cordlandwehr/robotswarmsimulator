# if we find doxygen, install a documentation target
find_package(Doxygen)
if(DOXYGEN_FOUND)
	# our doxygen documentation uses latex at some points
	find_package(LATEX)

	# doxygen configuration file
	SET(DOXYGEN_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in")
	SET(DOXYGEN_FILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf")
	configure_file("${DOXYGEN_FILE_IN}" "${DOXYGEN_FILE_OUT}" @ONLY)
	
	# create target 'documentation'
	add_custom_target(documentation COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_FILE_OUT}")
endif(DOXYGEN_FOUND)
