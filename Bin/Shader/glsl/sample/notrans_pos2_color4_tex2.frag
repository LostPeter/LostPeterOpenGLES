/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-12-17
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#version 300 es
precision mediump float;

out vec4 outColor;

in vec4 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texSampler;

void main() 
{
    vec3 color = texture(texSampler, fragTexCoord).xyz * fragColor.xyz;
    outColor = vec4(color, 1.0);
}