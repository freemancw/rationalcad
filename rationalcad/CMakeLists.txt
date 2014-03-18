#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================

set(CMAKE_AUTOMOC ON)

set(QT_INSTALL_DIR "" CACHE PATH "Path to Qt installation directory.")
set(WINSDK_DIR "" CACHE PATH "Path to Windows SDK directory.")
set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR}; ${WINSDK_DIR})

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

include_directories(${RATIONALCAD_SOURCE_DIR}/logging)
include_directories(${RATIONALCAD_SOURCE_DIR}/mathlib)

link_directories(${RATIONALCAD_BINARY_DIR}/logging)
link_directories(${RATIONALCAD_BINARY_DIR}/mathlib)

add_executable(rationalcad 
	rc_config.cpp
	rc_grid.cpp
	rc_logger.cpp
	rc_main.cpp
	rc_opengl.cpp
	rc_qt_dialog_about.cpp
	rc_qt_dialog_preferences.cpp
	rc_qt_widget_orthographic.cpp
	rc_qt_widget_perspective.cpp
	rc_qt_window_main.cpp
	rc_scene.cpp
)

qt5_use_modules(rationalcad Core Gui Widgets OpenGL)
target_link_libraries(rationalcad logging mathlib)