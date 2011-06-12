cmake_minimum_required(VERSION 2.8.4)

project(EASYIMAGESIZER3_GUI)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})

include_directories( ${EASYIMAGESIZER3_GUI_BINARY_DIR} ${QT_INCLUDE_DIR} )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )

set(EASYIMAGESIZER3_GUI_SRCS
    main.cpp
    mainwindow.cpp
	tipdialog.cpp
	form.cpp
)

set(EASYIMAGESIZER3_GUI_HEADERS
	mainwindow.h
	tipdialog.h
	form.h
)

set(EASYIMAGESIZER3_GUI_UIS
    mainwindow.ui
	tipdialog.ui
	form.ui
)

set( EASYIMAGESIZER3_GUI_RCS
       easyimagesizer3-gui.qrc
  )

# enable warnings
add_definitions( -Wall )

QT4_WRAP_CPP(EASYIMAGESIZER3_GUI_MOC_SRCS ${EASYIMAGESIZER3_GUI_HEADERS})
QT4_ADD_RESOURCES( EASYIMAGESIZER3_GUI_RC_SRCS ${EASYIMAGESIZER3_GUI_RCS} )
QT4_WRAP_UI(EASYIMAGESIZER3_GUI_UIS_H ${EASYIMAGESIZER3_GUI_UIS})

add_executable(easyimagesizer3-gui ${EASYIMAGESIZER3_GUI_SRCS} ${EASYIMAGESIZER3_GUI_MOC_SRCS}
							${EASYIMAGESIZER3_GUI_RC_SRCS} ${EASYIMAGESIZER3_GUI_UIS_H})
target_link_libraries(easyimagesizer3-gui easyimagesizer3 ${QT_LIBRARIES})