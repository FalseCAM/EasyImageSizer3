	CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)
	
 	########### Project name ###########
 	PROJECT(EasyImageSizer3)
 	SET(EASYIMAGESIZER_MAJOR "3")
 	SET(EASYIMAGESIZER_MINOR "0")
 	SET(EASYIMAGESIZER_PATCH "2")
 	SET(VERSION "${EASYIMAGESIZER_MAJOR}.${EASYIMAGESIZER_MINOR}.${EASYIMAGESIZER_PATCH}")
 	SET(PACKAGE easyimagesizer3)
   
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
	
	install(FILES "bin/libeasyimagesizer3.so"
			DESTINATION lib)
	install(FILES "bin/easyimagesizer3-gui"
			DESTINATION "share/easyimagesizer3")
	install(FILES "bin/plugins/libexif.so"
			DESTINATION "share/easyimagesizer3/plugins")
	install(FILES "bin/plugins/liboverlay.so"
			DESTINATION "share/easyimagesizer3/plugins")
	install(FILES "bin/plugins/liboverlay-text.so"
			DESTINATION "share/easyimagesizer3/plugins")
	install(FILES "bin/plugins/librename.so"
			DESTINATION "share/easyimagesizer3/plugins")
	install(FILES "bin/plugins/libresize.so"
			DESTINATION "share/easyimagesizer3/plugins")
	install(FILES "bin/plugins/librotate.so"
			DESTINATION "share/easyimagesizer3/plugins")
	install(DIRECTORY "easyimagesizer3/include/easyimagesizer3"
			DESTINATION "include"
			FILES_MATCHING PATTERN "*.h")
	install (FILES "data/easyimagesizer3.xpm"
			DESTINATION "share/pixmaps")
	install (FILES "data/easyimagesizer3.sh"
			DESTINATION bin)