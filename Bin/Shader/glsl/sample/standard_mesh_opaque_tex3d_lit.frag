/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-05-20
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#version 300 es
precision highp float;
precision mediump sampler3D;

in vec4 fragWorldPos;
in vec4 fragColor;
in vec3 fragWorldNormal;
in vec2 fragTexCoord;

uniform sampler3D texSampler0;

out vec4 outColor;


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
    vec4 position; // xyz: directional/point/spot, w: falloffStart
    vec4 direction; // xyz: directional/spot light only, w: falloffEnd
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
#define MAX_OBJECT_COUNT  				1024
//ObjectConstants
struct ObjectConstant
{
	mat4 g_MatWorld;
};	
layout (std140) uniform ObjectConstants
{
    ObjectConstant objs[MAX_OBJECT_COUNT];
} objectConsts;


///////////////////////////////// MaterialConstants /////////////////////////////
//TextureConstants
struct TextureConstants
{
    //x: texWidth; y: texHeight; z: texDepth; w: indexTextureArray
    vec4 texSize; 

    //x: texSpeedU; y: texSpeedV; z: texSpeedW; w: reserve0
    vec4 texSpeed;

    //x: texChunkMaxX; y: texChunkMaxY; z: texChunkIndexX; w: texChunkIndexY
    vec4 texChunk;
};

//MaterialConstants
#define MAX_TEXTURE_COUNT               16
struct MaterialConstant
{
    vec4 factorAmbient;
    vec4 factorDiffuse;
    vec4 factorSpecular;

    //x: shininess; y: alpha; z: islighting; w: reserve0
    vec4 lighting;

    //x: castshadow; y: receiveshadow; z: reserve0; w: reserve1
    vec4 shadow;

    TextureConstants aTexLayers[MAX_TEXTURE_COUNT];
};
#define MAX_MATERIAL_COUNT              64
layout (std140) uniform MaterialConstants
{
	MaterialConstant mats[MAX_MATERIAL_COUNT];
} materialConsts;


///////////////////////////////// Lighting_Lambert /////////////////////////////
vec3 calculate_Light_Ambient(vec3 ambientGlobal, 
                             vec3 ambientMaterial, 
                             vec3 ambientLight)
{
    return ambientGlobal * ambientMaterial * ambientLight;
}
vec3 calculate_Light_Diffuse_Lambert(vec3 diffuseMaterial, 
                                     vec3 diffuseLight,
                                     vec3 L,
                                     vec3 N)
{
    return diffuseMaterial * diffuseLight * max(dot(N, L), 0.0);
}
vec3 calculate_Specular_Phong(vec3 specularMaterial, 
                              vec3 specularLight,
                              float shininess,
                              vec3 posWorld,
                              vec3 posEye,
                              vec3 L,
                              vec3 N)
{
    vec3 V = normalize(posEye - posWorld);
    vec3 R = normalize(2.0 * max(dot(N, L), 0.0) * N - L);
    
    return specularMaterial * specularLight * pow(max(dot(V, R), 0.0), shininess);
}
vec3 calculate_Specular_BlinnPhong(vec3 specularMaterial, 
                                   vec3 specularLight,
                                   float shininess,
                                   vec3 posWorld,
                                   vec3 posEye,
                                   vec3 L,
                                   vec3 N)
{
    vec3 V = normalize(posEye - posWorld);
    vec3 H = normalize(L + V);

    return specularMaterial * specularLight * pow(max(dot(N, H), 0.0), shininess);
}

vec3 calculate_Light(vec3 ambientGlobal,
                     LightConstants lightCB,
                     MaterialConstant matCB,
                     vec3 posWorld,
                     vec3 posEye,
                     vec3 N)
{
    int type = int(lightCB.lightCommon.x + 0.5);
    int mode = int(lightCB.lightCommon.z + 0.5);
    int isLighting = int(matCB.lighting.z + 0.5);
    if (mode == 0 || isLighting != 1)
        return vec3(1.0, 1.0, 1.0);
    
    vec3 L;
    if (type == 0)
    {
        L = - lightCB.direction.xyz;
    }
    else
    {
        vec3 posLight = lightCB.position.xyz;
        L = normalize(posLight - posWorld);
    }

    //Ambient
    vec3 colorAmbient = vec3(0.0,0.0,0.0); 
    if (mode == 1 ||
        mode == 5 ||
        mode == 6 ||
        mode == 7 ||
        mode == 10 ||
        mode == 11)
    {
        colorAmbient = calculate_Light_Ambient(ambientGlobal,
                                               matCB.factorAmbient.rgb,
                                               lightCB.ambient.rgb);
    }
    
    //Diffuse
    vec3 colorDiffuse = vec3(0.0,0.0,0.0); 
    if (mode == 2 ||
        mode == 5 ||
        mode == 8 ||
        mode == 9 ||
        mode == 10 ||
        mode == 11)
    {
        colorDiffuse = calculate_Light_Diffuse_Lambert(matCB.factorDiffuse.rgb,
                                                       lightCB.diffuse.rgb,
                                                       L,
                                                       N);
    }
    
    //Specular
    vec3 colorSpecular = vec3(0.0,0.0,0.0);
    if (mode == 3 ||
        mode == 6 ||
        mode == 8 ||
        mode == 10)
    {
        //Phong
        colorSpecular = calculate_Specular_Phong(matCB.factorSpecular.rgb, 
                                                 lightCB.specular.rgb,
                                                 matCB.lighting.x,
                                                 posWorld,
                                                 posEye,
                                                 L,
                                                 N);
    }
    else if (mode == 4 ||
             mode == 7 ||
             mode == 9 ||
             mode == 11)
    {
        //BlinnPhong
        colorSpecular = calculate_Specular_BlinnPhong(matCB.factorSpecular.rgb,
                                                      lightCB.specular.rgb,
                                                      matCB.lighting.x,
                                                      posWorld,
                                                      posEye,
                                                      L,
                                                      N);
    }

    return colorAmbient + colorDiffuse + colorSpecular;
}


void main()
{
    vec3 color;

    MaterialConstant mat = materialConsts.mats[int(fragWorldPos.w + 0.5)];
    vec3 N = normalize(fragWorldNormal);

    vec3 colorLight;
    //Main Light
    int viewIndex = 0;
    CameraConstants cam = passConsts.g_Cameras[viewIndex];
    vec3 colorMainLight = calculate_Light(passConsts.g_AmbientLight.rgb,
                                          passConsts.g_MainLight,
                                          mat,
                                          fragWorldPos.xyz,
                                          cam.posEyeWorld, 
                                          N);
    colorLight = colorMainLight;

    //Additional Light



    //Texture
    vec3 colorTexture = texture(texSampler0, vec3(fragTexCoord, mat.aTexLayers[0].texSize.w)).xyz;
    //VertexColor
    vec3 colorVertex = fragColor.xyz;

    //Final Color
    color = colorLight * colorTexture * colorVertex;

    outColor = vec4(color.xyz, 1.0);
}