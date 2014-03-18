/*
 * FXAA fragment shader
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 05/18/2013
 */

// input:
varying highp vec4 f_multi_tex_coord_0;

// input:
uniform sampler2D f_tex_0;

void main(void) {
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = texture2D(f_tex_0, f_multi_tex_coord_0.st);
}
