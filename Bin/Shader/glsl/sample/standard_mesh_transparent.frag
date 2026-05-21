/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2025-05-04
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#version 300 es
precision mediump float;
precision mediump sampler2D;

in vec4 fragColor;
in vec2 fragTexCoord;
in float fragAlpha;

uniform sampler2D texSampler0;

out vec4 outColor;

void main()
{
    vec3 color = texture(texSampler0, fragTexCoord).xyz * fragColor.xyz;
    outColor = vec4(color.xyz, fragAlpha);
}