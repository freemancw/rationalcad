#version 330

/*
 * Order Independent Transparency with Depth Peeling
 * Copyright (c) NVIDIA Corporation. All rights reserved.
 * @author Louis Bavoil <sdkfeedback@nvidia.com>
 * @date 12/18/2013
 */

vec3 ShadeVertex();

void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0].xyz = ShadeVertex();
}
