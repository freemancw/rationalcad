#version 330

/*
 * Order Independent Transparency with Depth Peeling
 * Copyright (c) NVIDIA Corporation. All rights reserved.
 * @author Louis Bavoil <sdkfeedback@nvidia.com>
 * @date 12/18/2013
 */

uniform samplerRECT DepthTex;

vec4 ShadeFragment();

void main(void) {
    // Bit-exact comparison between FP32 z-buffer and fragment depth
    float frontDepth = textureRect(DepthTex, gl_FragCoord.xy).r;
    if (gl_FragCoord.z <= frontDepth) {
        discard;
    }
	
    // Shade all the fragments behind the z-buffer
    vec4 color = ShadeFragment();
    gl_FragColor = vec4(color.rgb * color.a, color.a);
}
