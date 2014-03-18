#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================

include_directories(${RATIONALCAD_SOURCE_DIR}/logging)
include_directories(${RATIONALCAD_SOURCE_DIR}/mathlib)

link_directories(${RATIONALCAD_BINARY_DIR}/logging)
link_directories(${RATIONALCAD_BINARY_DIR}/mathlib)

add_executable(rcad 
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

target_link_libraries(rcad logging mathlib)