#version 330

/*
 * Orthographic wireframe fragment shader
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 03/30/2013
 */

// input: fragment color
in vec4 f_color;

void main() {
    gl_FragColor = f_color;
}

