#version 330

/*
 * Perspective fragment shader
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-03-30
 */

// input: vertex attributes
layout (location = 0) in vec4 v_position;
layout (location = 1) in vec4 v_normal;
layout (location = 2) in vec4 v_mat_ambient;

// input: transformation matrices
uniform mat4 m_modelview;
uniform mat4 m_projection;

// output: fragment color
out vec4 f_color;

void main(void) {
    gl_PointSize = 4.0;
    gl_Position = m_projection * m_modelview * v_position;
    f_color = v_mat_ambient * abs(dot(v_normal, vec4(0.267, 0.535, 0.802, 0)));
    f_color.a = 1;
}
