	CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
	
 	########### Project name ###########
 	PROJECT(EasyImageSizer)
 	#SET(EASYIMAGESIZER_MAJOR "3")
 	#SET(EASYIMAGESIZER_MINOR "0")
 	#SET(EASYIMAGESIZER_PATCH "1")
 	#SET(VERSION "${EASYIMAGESIZER_MAJOR}.${EASYIMAGESIZER_MINOR}.${EASYIMAGESIZER_PATCH}")
 	#SET(PACKAGE easyimagesizer)
   
  ########### Subdirectories ###############

	set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )
	set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )
	set( CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/bin" )
	include_directories("${PROJECT_SOURCE_DIR}/easyimagesizer3/include")

	ADD_SUBDIRECTORY( easyimagesizer3 )
	ADD_SUBDIRECTORY( easyimagesizer3-gui )
	ADD_SUBDIRECTORY( plugins )