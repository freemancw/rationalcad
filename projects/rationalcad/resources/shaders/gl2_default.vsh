#version 330

/*
 * Orthographic wireframe fragment shader
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 03/30/2013
 */

// input: vertex attributes
layout (location = 0) in vec4 v_position;
layout (location = 1) in vec4 v_mat_ambient;

// input: transformation matrices
uniform mat4 m_modelview;
uniform mat4 m_projection;

// output: fragment color
out vec4 f_color;

void main(void) {
    f_color = v_mat_ambient;
    gl_PointSize = 4.0;
    gl_Position = m_projection * m_modelview * v_position;
}
