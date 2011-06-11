cmake_minimum_required(VERSION 2.8.4)

project(ROTATE)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(ROTATE_SRCS
    rotate.cpp
)

set(ROTATE_HEADERS
	rotate.h
)

set(ROTATE_UIS
    rotate.ui
)

QT4_WRAP_CPP(ROTATE_MOC_SRCS ${ROTATE_HEADERS})
QT4_WRAP_UI(ROTATE_UIS_H ${ROTATE_UIS})

add_library(rotate SHARED ${ROTATE_SRCS}
							${ROTATE_MOC_SRCS}
							${ROTATE_UIS_H})
target_link_libraries(rotate easyimagesizer3 ${QT_LIBRARIES})