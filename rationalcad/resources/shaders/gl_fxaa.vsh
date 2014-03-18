/*
 * FXAA vertex shader
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 05/18/2013
 */

// input: vertex attributes
attribute highp vec4 v_position;
attribute highp vec4 v_multi_tex_coord_0;

// output: fragment color
varying highp vec4 f_multi_tex_coord_0;

void main(void) {
    gl_Position = v_position;
    f_multi_tex_coord_0 = v_multi_tex_coord_0;
}
