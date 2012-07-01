cmake_minimum_required(VERSION 2.8.0)

project(BLUR)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${BLUR_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(BLUR_SRCS
    blur.cpp
    previewimagelabel.cpp
)

set(BLUR_HEADERS
    blur.h
    previewimagelabel.h
)

set(BLUR_UIS
    blur.ui
)

set(BLUR_RCS
       blur.qrc
)

QT4_WRAP_CPP(BLUR_MOC_SRCS ${BLUR_HEADERS})
QT4_ADD_RESOURCES( BLUR_RC_SRCS ${BLUR_RCS} )
QT4_WRAP_UI(BLUR_UIS_H ${BLUR_UIS})

add_library(blur SHARED ${BLUR_SRCS}
                                ${BLUR_MOC_SRCS}
                                ${BLUR_RC_SRCS}
                                ${BLUR_UIS_H})
target_link_libraries(blur easyimagesizer3 ${QT_LIBRARIES})
