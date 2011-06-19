	CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)
	
 	########### Project name ###########
 	PROJECT(EasyImageSizer)
 	SET(EASYIMAGESIZER_MAJOR "3")
 	SET(EASYIMAGESIZER_MINOR "0")
 	SET(EASYIMAGESIZER_PATCH "1")
 	SET(VERSION "${EASYIMAGESIZER_MAJOR}.${EASYIMAGESIZER_MINOR}.${EASYIMAGESIZER_PATCH}")
 	SET(PACKAGE easyimagesizer)
   
  ########### Subdirectories ###############

	set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )
	set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )
	set( CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/bin" )
	include_directories("${PROJECT_SOURCE_DIR}/easyimagesizer3/include")
	
	IF(NOT CMAKE_BUILD_TYPE)
		SET(CMAKE_BUILD_TYPE Release CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
	ENDIF(NOT CMAKE_BUILD_TYPE)

	ADD_SUBDIRECTORY( easyimagesizer3 )
	ADD_SUBDIRECTORY( easyimagesizer3_gui )
	ADD_SUBDIRECTORY( plugins )