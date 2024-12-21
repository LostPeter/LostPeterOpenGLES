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

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTexCoord;


///////////////////////////////// PassConstants /////////////////////////////////
//TransformConstants
struct TransformConstants
{
    mat4 mat4View;
    mat4 mat4View_Inv;
    mat4 mat4Proj;
    mat4 mat4Proj_Inv;
    mat4 mat4ViewProj;
    mat4 mat4ViewProj_Inv;
};

//CameraConstants
struct CameraConstants
{
    vec3 posEyeWorld;    
    float fNearZ;
    float fFarZ;
    float fReserved1;
    float fReserved2;
    float fReserved3;
};

//LightConstants
struct LightConstants
{
    vec4 lightCommon; // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
    vec3 position; // directional/point/spot
    float falloffStart; // point/spot light only
    vec3 direction; // directional/spot light only
    float falloffEnd; // point/spot light only
    vec4 ambient; // ambient
    vec4 diffuse; // diffuse
    vec4 specular; // specular

    mat4 depthMVP; // depthMVP
};

//PassConstants
#define MAX_LIGHT_COUNT                 16
layout (std140) uniform PassConstants
{
    //TransformConstants
    TransformConstants g_Transforms[2]; //0: Eye Left(Main); 1: Eye Right
    //CameraConstants
    CameraConstants g_Cameras[2]; //0: Eye Left(Main); 1: Eye Right
    
    //TimeConstants
    float g_TotalTime;
    float g_DeltaTime;
    float g_Pad1;
    float g_Pad2;

    //RenderTarget
    vec2 g_RenderTargetSize;
    vec2 g_RenderTargetSize_Inv;

    //Material
    vec4 g_AmbientLight;
    
    //Light
    LightConstants g_MainLight;
    LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
} passConsts;


///////////////////////////////// ObjectConstants ///////////////////////////////
//ObjectConstants
layout (std140) uniform ObjectConstants
{
    mat4 g_MatWorld;
} objectConsts;


out vec4 fragColor;
out vec2 fragTexCoord;

void main()
{
    gl_Position = passConsts.g_Transforms[0].mat4Proj * passConsts.g_Transforms[0].mat4View * objectConsts.g_MatWorld * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
