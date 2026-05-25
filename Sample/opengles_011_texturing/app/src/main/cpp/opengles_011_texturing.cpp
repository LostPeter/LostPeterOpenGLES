/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-05-19
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_011_texturing.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


/////////////////////////// Type ////////////////////////////////
enum BumpMappingType
{
    BumpMapping_DiffuseColor = 0,       //0: DiffuseColor
    BumpMapping_BumpMapping,            //1: NormalMapping

};
struct EnumBumpMappingDesc { BumpMappingType Value; const char* Name; const char* Tooltip; };
static const EnumBumpMappingDesc s_aBumpMappingDescs[] =
{
    { BumpMapping_DiffuseColor,                   "DiffuseColor",                     "Diffuse Color" },
    { BumpMapping_BumpMapping,                    "BumpMapping",                      "Bump Mapping" },
};


enum NormalMappingType
{
    NormalMapping_DiffuseColor = 0,     //0: DiffuseColor
    NormalMapping_NormalColor,          //1: NormalColor
    NormalMapping_NormalMapping,        //2: NormalMapping

};
struct EnumNormalMappingDesc { NormalMappingType Value; const char* Name; const char* Tooltip; };
static const EnumNormalMappingDesc s_aNormalMappingDescs[] =
{
    { NormalMapping_DiffuseColor,                 "DiffuseColor",                     "Diffuse Color" },
    { NormalMapping_NormalColor,                  "NormalColor",                      "Normal Color" },
    { NormalMapping_NormalMapping,                "NormalMapping",                    "Normal Mapping" },
};


enum ParallaxMappingType
{
    ParallaxMapping_DiffuseColor = 0,           //0: DiffuseColor
    ParallaxMapping_NormalColor,                //1: NormalColor
    ParallaxMapping_NormalMapping,              //2: NormalMapping
    ParallaxMapping_CommonParallaxMapping,      //3: CommonParallaxMapping
    ParallaxMapping_SteepParallaxMapping,       //4: SteepParallaxMapping
    ParallaxMapping_OcclusionParallaxMapping,   //5: OcclusionParallaxMapping

};
struct EnumParallaxMappingDesc { ParallaxMappingType Value; const char* Name; const char* Tooltip; };
static const EnumParallaxMappingDesc s_aParallaxMappingDescs[] =
{
    { ParallaxMapping_DiffuseColor,                 "DiffuseColor",                     "Diffuse Color" },
    { ParallaxMapping_NormalColor,                  "NormalColor",                      "Normal Color" },
    { ParallaxMapping_NormalMapping,                "NormalMapping",                    "Normal Mapping" },
    { ParallaxMapping_CommonParallaxMapping,        "CommonParallaxMapping",            "Common Parallax Mapping" },
    { ParallaxMapping_SteepParallaxMapping,         "SteepParallaxMapping",             "Steep Parallax Mapping" },
    { ParallaxMapping_OcclusionParallaxMapping,     "OcclusionParallaxMapping",         "Occlusion Parallax Mapping" },
};



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 4;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/plane.fbx", //plane
    "plane_nt",         "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Mesh/Common/plane.fbx", //plane_nt
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/sphere.fbx", //sphere

};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //plane
    true, //plane_nt
    false, //cube
    false, //sphere

};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //plane  
    false, //plane_nt  
    false, //cube
    false, //sphere
    
};
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //plane_nt
    FMath::ms_mat4Unit, //cube
    FMath::ms_mat4Unit, //sphere

};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 20;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                  //Texture Type  //TextureIsRenderTarget  //TextureIsGraphicsComputeShared  //Texture Path
    "default_blackwhite_01",        "2D",           "false",                 "false",                          "Texture/Common/default_blackwhite.png", //default_blackwhite_01
    "terrain",                      "2D",           "false",                 "false",                          "Texture/Common/terrain.png", //terrain
    "default_white_01",             "2D",           "false",                 "false",                          "Texture/Common/default_white.bmp", //default_white_01
    
////Basic-Level Texture Operation
    "texturesampler_wrap",          "2D",           "false",                 "false",                          "Texture/Common/texture2d.jpg", //texturesampler_wrap
    "texturesampler_mirror",        "2D",           "false",                 "false",                          "Texture/Common/texture2d.jpg", //texturesampler_mirror
    "texturesampler_clamp",         "2D",           "false",                 "false",                          "Texture/Common/texture2d.jpg", //texturesampler_clamp
    "texturesampler_border",        "2D",           "false",                 "false",                          "Texture/Common/texture2d.jpg", //texturesampler_border
    "texture1d",                    "2D",           "false",                 "false",                          "Texture/Common/texture1d.tga", //texture1d
    "texture2d_01",                 "2D",           "false",                 "false",                          "Texture/Common/texture2d.jpg", //texture2d_01
    "texture2darray",               "2DArray",      "false",                 "false",                          "Texture/Terrain/shore_sand_albedo.png;Texture/Terrain/moss_albedo.png;Texture/Terrain/rock_cliff_albedo.png;Texture/Terrain/cliff_albedo.png", //texture2darray
    "texture3d",                    "3D",           "true",                  "false",                          "", //texture3d
    "texturecubemap",               "CubeMap",      "false",                 "false",                          "Texture/Sky/texturecubemap_x_right.png;Texture/Sky/texturecubemap_x_left.png;Texture/Sky/texturecubemap_y_up.png;Texture/Sky/texturecubemap_y_down.png;Texture/Sky/texturecubemap_z_front.png;Texture/Sky/texturecubemap_z_back.png", //texturecubemap
    "textureanimation_scroll",      "2DArray",      "false",                 "false",                          "Texture/Common/textureanimation1.png;Texture/Common/textureanimation2.png", //textureanimation_scroll
    "textureanimation_chunk",       "2DArray",      "false",                 "false",                          "Texture/Common/textureanimation3.png", //textureanimation_chunk

////High-Level Texture Operation
    "texturebumpmap_diffuse",       "2D",           "false",                 "false",                          "Texture/Common/bricks_diffuse.png", //texturebumpmap_diffuse
    "texturebumpmap_bumpmap",       "2D",           "false",                 "false",                          "Texture/Common/bricks_bumpmap.png", //texturebumpmap_bumpmap
    "texturenormalmap_normalmap",   "2D",           "false",                 "false",                          "Texture/Common/bricks_normalmap.png", //texturenormalmap_normalmap

    "rocks_color",                  "2D",           "false",                 "false",                          "Texture/Common/rocks_color.png", //rocks_color
    "rocks_normal_height",          "2D",           "false",                 "false",                          "Texture/Common/rocks_normal_height.png", //rocks_normal_height

    "stonefloor_color_height",      "2D",           "false",                 "false",                          "Texture/Common/stonefloor_color_height.png", //stonefloor_color_height

};
static int g_TextureChannels[g_TextureCount] = 
{
    4, //default_blackwhite_01
    4, //terrain
    4, //default_white_01

////Basic-Level Texture Operation
    4, //texturesampler_wrap
    4, //texturesampler_mirror
    4, //texturesampler_clamp
    4, //texturesampler_border
    4, //texture1d
    4, //texture2d_01
    4, //texture2darray
    1, //texture3d
    4, //texturecubemap
    4, //textureanimation_scroll
    4, //textureanimation_chunk

////High-Level Texture Operation
    4, //texturebumpmap_diffuse
    4, //texturebumpmap_bumpmap
    4, //texturenormalmap_normalmap

    4, //rocks_color
    4, //rocks_normal_height

    4, //stonefloor_color_height

};
static FTexturePixelFormatType g_TextureFormats[g_TextureCount] = 
{
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_blackwhite_01
    F_TexturePixelFormat_R8G8B8A8_SRGB, //terrain
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_white_01

////Basic-Level Texture Operation
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturesampler_wrap
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturesampler_mirror
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturesampler_clamp
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturesampler_border
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture1d
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d_01
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2darray
    F_TexturePixelFormat_R8_UNORM, //texture3d
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturecubemap
    F_TexturePixelFormat_R8G8B8A8_SRGB, //textureanimation_scroll
    F_TexturePixelFormat_R8G8B8A8_SRGB, //textureanimation_chunk

////High-Level Texture Operation
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturebumpmap_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturebumpmap_bumpmap
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texturenormalmap_normalmap

    F_TexturePixelFormat_R8G8B8A8_SRGB, //rocks_color
    F_TexturePixelFormat_R8G8B8A8_UNORM, //rocks_normal_height

    F_TexturePixelFormat_R8G8B8A8_UNORM, //stonefloor_color_height

};
static FTextureFilterType g_TextureFilters[2 * g_TextureCount] = 
{
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_blackwhite_01
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //terrain
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_white_01

////Basic-Level Texture Operation
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturesampler_wrap
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturesampler_mirror
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturesampler_clamp
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturesampler_border
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture1d
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture2d_01
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture2darray
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture3d
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturecubemap
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //textureanimation_scroll
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //textureanimation_chunk

////High-Level Texture Operation
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturebumpmap_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturebumpmap_bumpmap
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturenormalmap_normalmap

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //rocks_color
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //rocks_normal_height

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //stonefloor_color_height

};
static FTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    F_TextureAddressing_Clamp, //default_blackwhite_01
    F_TextureAddressing_Clamp, //terrain
    F_TextureAddressing_Clamp, //default_white_01

////Basic-Level Texture Operation
    F_TextureAddressing_Wrap, //texturesampler_wrap
    F_TextureAddressing_Mirror, //texturesampler_mirror
    F_TextureAddressing_Clamp, //texturesampler_clamp
    F_TextureAddressing_Border, //texturesampler_border
    F_TextureAddressing_Clamp, //texture1d
    F_TextureAddressing_Clamp, //texture2d_01
    F_TextureAddressing_Clamp, //texture2darray
    F_TextureAddressing_Clamp, //texture3d
    F_TextureAddressing_Wrap, //texturecubemap
    F_TextureAddressing_Wrap, //textureanimation_scroll
    F_TextureAddressing_Wrap, //textureanimation_chunk

////High-Level Texture Operation
    F_TextureAddressing_Clamp, //texturebumpmap_diffuse
    F_TextureAddressing_Clamp, //texturebumpmap_bumpmap
    F_TextureAddressing_Clamp, //texturenormalmap_normalmap

    F_TextureAddressing_Clamp, //rocks_color
    F_TextureAddressing_Clamp, //rocks_normal_height

    F_TextureAddressing_Clamp, //stonefloor_color_height

};
static FColor g_TextureBorderColors[g_TextureCount] = 
{
    FColor(0, 0, 0, 1), //default_blackwhite_01
    FColor(0, 0, 0, 1), //terrain
    FColor(0, 0, 0, 1), //default_white_01

////Basic-Level Texture Operation
    FColor(0, 0, 0, 1), //texturesampler_wrap
    FColor(0, 0, 0, 1), //texturesampler_mirror
    FColor(0, 0, 0, 1), //texturesampler_clamp
    FColor(0, 0, 0, 1), //texturesampler_border
    FColor(0, 0, 0, 1), //texture1d
    FColor(0, 0, 0, 1), //texture2d_01
    FColor(0, 0, 0, 1), //texture2darray
    FColor(0, 0, 0, 1), //texture3d
    FColor(0, 0, 0, 1), //texturecubemap
    FColor(0, 0, 0, 1), //textureanimation_scroll
    FColor(0, 0, 0, 1), //textureanimation_chunk

////High-Level Texture Operation
    FColor(0, 0, 0, 1), //texturebumpmap_diffuse
    FColor(0, 0, 0, 1), //texturebumpmap_bumpmap
    FColor(0, 0, 0, 1), //texturenormalmap_normalmap

    FColor(0, 0, 0, 1), //rocks_color
    FColor(0, 0, 0, 1), //rocks_normal_height

    FColor(0, 0, 0, 1), //stonefloor_color_height

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //default_blackwhite_01
    512,    512,    1, //terrain
     64,     64,    1, //default_white_01

////Basic-Level Texture Operation
    512,    512,    1, //texturesampler_wrap
    512,    512,    1, //texturesampler_mirror
    512,    512,    1, //texturesampler_clamp
    512,    512,    1, //texturesampler_border
     64,      1,    1, //texture1d
    512,    512,    1, //texture2d_01
   1024,   1024,    1, //texture2darray
    128,    128,    128, //texture3d
    512,    512,    1, //texturecubemap
     64,     64,    1, //textureanimation_scroll
    512,    512,    1, //textureanimation_chunk

////High-Level Texture Operation
    512,    512,    1, //texturebumpmap_diffuse
    512,    512,    1, //texturebumpmap_bumpmap
    512,    512,    1, //texturenormalmap_normalmap

    1024,  1024,    1, //rocks_color
    1024,  1024,    1, //rocks_normal_height

    1024,  1024,    1, //stonefloor_color_height

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //default_blackwhite_01
    0,    0, //terrain
    0,    0, //default_white_01

////Basic-Level Texture Operation
    0,    0, //texturesampler_wrap
    0,    0, //texturesampler_mirror
    0,    0, //texturesampler_clamp
    0,    0, //texturesampler_border
    0,    0, //texture1d
    0,    0, //texture2d_01
    0,    0, //texture2darray
    0,    0, //texture3d
    0,    0, //texturecubemap
    0,    0, //textureanimation_scroll
    4,    8, //textureanimation_chunk

////High-Level Texture Operation
    0,    0, //texturebumpmap_diffuse
    0,    0, //texturebumpmap_bumpmap
    0,    0, //texturenormalmap_normalmap

    0,    0, //rocks_color
    0,    0, //rocks_normal_height

    0,    0, //stonefloor_color_height

};


/////////////////////////// DescriptorSetLayout /////////////////
static const int g_DescriptorSetLayoutCount = 2;
static const char* g_nameDescriptorSetLayouts[g_DescriptorSetLayoutCount] =
{
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",
};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 22;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type              //path

    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_texsampler_lit",                "vert",             "standard_mesh_opaque_texsampler_lit.vert.spv", //standard_mesh_opaque_texsampler_lit vert
    "vert_standard_mesh_opaque_tex1d_lit",                     "vert",             "standard_mesh_opaque_tex1d_lit.vert.spv", //standard_mesh_opaque_tex1d_lit vert
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",             "standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_opaque_tex2darray_lit",                "vert",             "standard_mesh_opaque_tex2darray_lit.vert.spv", //standard_mesh_opaque_tex2darray_lit vert
    "vert_standard_mesh_opaque_tex3d_lit",                     "vert",             "standard_mesh_opaque_tex3d_lit.vert.spv", //standard_mesh_opaque_tex3d_lit vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",             "standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_texanim_scroll_lit",            "vert",             "standard_mesh_opaque_texanim_scroll_lit.vert.spv", //standard_mesh_opaque_texanim_scroll_lit vert
    "vert_standard_mesh_opaque_texanim_chunk_lit",             "vert",             "standard_mesh_opaque_texanim_chunk_lit.vert.spv", //standard_mesh_opaque_texanim_chunk_lit vert

    "vert_standard_mesh_opaque_texbumpmap_lit",                "vert",             "standard_mesh_opaque_texbumpmap_lit.vert.spv", //standard_mesh_opaque_texbumpmap_lit vert
    "vert_standard_mesh_opaque_texnormalmap_lit",              "vert",             "standard_mesh_opaque_texnormalmap_lit.vert.spv", //standard_mesh_opaque_texnormalmap_lit vert   
    "vert_standard_mesh_opaque_texparallaxmap_lit",            "vert",             "standard_mesh_opaque_texparallaxmap_lit.vert.spv", //standard_mesh_opaque_texparallaxmap_lit vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////

    ///////////////////////////////////////// tese /////////////////////////////////////////

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_texsampler_lit",                "frag",              "standard_mesh_opaque_texsampler_lit.frag.spv", //standard_mesh_opaque_texsampler_lit frag
    "frag_standard_mesh_opaque_tex1d_lit",                     "frag",              "standard_mesh_opaque_tex1d_lit.frag.spv", //standard_mesh_opaque_tex1d_lit frag
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_opaque_tex2darray_lit",                "frag",              "standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit frag
    "frag_standard_mesh_opaque_tex3d_lit",                     "frag",              "standard_mesh_opaque_tex3d_lit.frag.spv", //standard_mesh_opaque_tex3d_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_texanim_scroll_lit",            "frag",              "standard_mesh_opaque_texanim_scroll_lit.frag.spv", //standard_mesh_opaque_texanim_scroll_lit frag
    "frag_standard_mesh_opaque_texanim_chunk_lit",             "frag",              "standard_mesh_opaque_texanim_chunk_lit.frag.spv", //standard_mesh_opaque_texanim_chunk_lit frag
         
    "frag_standard_mesh_opaque_texbumpmap_lit",                "frag",              "standard_mesh_opaque_texbumpmap_lit.frag.spv", //standard_mesh_opaque_texbumpmap_lit frag
    "frag_standard_mesh_opaque_texnormalmap_lit",              "frag",              "standard_mesh_opaque_texnormalmap_lit.frag.spv", //standard_mesh_opaque_texnormalmap_lit frag
    "frag_standard_mesh_opaque_texparallaxmap_lit",            "frag",              "standard_mesh_opaque_texparallaxmap_lit.frag.spv", //standard_mesh_opaque_texparallaxmap_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////


};


/////////////////////////// Object //////////////////////////////
static const int g_ObjectCount = 18;
static const char* g_ObjectConfigs[8 * g_ObjectCount] = 
{
    //Object Name                       //Mesh Path                    //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS           //Texture FS                                                         //Texture CS
    "ground",                           "plane",                       "",                     "",                              "",                         "",                   "terrain",                                                           "", //ground

////Basic-Level Texture Operation
    "textureSampler_Wrap",              "plane",                       "",                     "",                              "",                         "",                   "texturesampler_wrap",                                                "", //textureSampler_Wrap
    "textureSampler_Mirror",            "plane",                       "",                     "",                              "",                         "",                   "texturesampler_mirror",                                              "", //textureSampler_Mirror
    "textureSampler_Clamp",             "plane",                       "",                     "",                              "",                         "",                   "texturesampler_clamp",                                               "", //textureSampler_Clamp
    "textureSampler_Border",            "plane",                       "",                     "",                              "",                         "",                   "texturesampler_border",                                              "", //textureSampler_Border
    "texture1D",                        "plane",                       "",                     "",                              "",                         "",                   "texture1d",                                                          "", //texture1D
    "texture2D",                        "plane",                       "",                     "",                              "",                         "",                   "texture2d_01",                                                       "", //texture2D
    "texture2Darray",                   "plane",                       "",                     "",                              "",                         "",                   "texture2darray",                                                     "", //texture2Darray
    "texture3D",                        "plane",                       "",                     "",                              "",                         "",                   "texture3d",                                                          "", //texture3D
    "textureCubeMap_SkyBox",            "cube",                        "",                     "",                              "",                         "",                   "texturecubemap",                                                     "", //textureCubeMap_SkyBox
    "textureCubeMap_Sphere",            "sphere",                      "",                     "",                              "",                         "",                   "texturecubemap",                                                     "", //textureCubeMap_Sphere
    "textureAnimation_Scroll",          "plane",                       "",                     "",                              "",                         "",                   "textureanimation_scroll",                                            "", //textureAnimation_Scroll
    "textureAnimation_Chunk",           "plane",                       "",                     "",                              "",                         "",                   "textureanimation_chunk",                                             "", //textureAnimation_Chunk

////High-Level Texture Operation
    "textureOriginal",                  "plane",                       "",                     "",                              "",                         "",                   "texturebumpmap_diffuse",                                             "", //textureOriginal    
    "textureBumpMap",                   "plane",                       "",                     "",                              "",                         "",                   "texturebumpmap_diffuse;texturebumpmap_bumpmap",                      "", //textureBumpMap
    "textureNormalMap",                 "plane_nt",                    "",                     "",                              "",                         "",                   "texturebumpmap_diffuse;texturenormalmap_normalmap",                  "", //textureNormalMap
    "textureParallaxMap",               "plane_nt",                    "",                     "",                              "",                         "",                   "rocks_color;rocks_normal_height",                                    "", //textureParallaxMap
    "textureOriginal2",           		"plane",                       "",                     "",                              "",  						"",                   "texturebumpmap_diffuse",                                    			"", //textureOriginal2

};

static const String g_Object_Texture3D = "texture3D";
static const String g_Object_TextureAnimation_Scroll = "textureAnimation_Scroll";
static const String g_Object_TextureAnimation_Chunk = "textureAnimation_Chunk";
static const String g_Object_TextureOriginal = "textureOriginal";
static const String g_Object_TextureBumpMap = "textureBumpMap";
static const String g_Object_TextureNormalMap = "textureNormalMap";
static const String g_Object_TextureParallaxMap = "textureParallaxMap";

static const char* g_ObjectNameShaderModules[6 * g_ObjectCount] = 
{
    //vert                                                  //tesc                                          //tese                                          //geom                      //frag                                                  //comp
    
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //ground

////Basic-Level Texture Operation
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Wrap 
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Mirror 
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Clamp 
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Border 
    "vert_standard_mesh_opaque_tex1d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex1d_lit",                  "", //texture1D 
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //texture2D 
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray 
    "vert_standard_mesh_opaque_tex3d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex3d_lit",                  "", //texture3D 
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //textureCubeMap_SkyBox 
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //textureCubeMap_Sphere 
    "vert_standard_mesh_opaque_texanim_scroll_lit",         "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texanim_scroll_lit",         "", //textureAnimation_Scroll 
    "vert_standard_mesh_opaque_texanim_chunk_lit",          "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texanim_chunk_lit",          "", //textureAnimation_Chunk 

////High-Level Texture Operation
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //textureOriginal 
    "vert_standard_mesh_opaque_texbumpmap_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texbumpmap_lit",             "", //textureBumpMap 
    "vert_standard_mesh_opaque_texnormalmap_lit",           "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texnormalmap_lit",           "", //textureNormalMap 
    "vert_standard_mesh_opaque_texparallaxmap_lit",         "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texparallaxmap_lit",         "", //textureParallaxMap 

    
    "vert_standard_mesh_opaque_tex2d_lit",     				"",      										"",      										"",                         "frag_standard_mesh_opaque_tex2d_lit",     				"", //textureOriginal2

};
static const char* g_ObjectNameDescriptorSetLayouts[2 * g_ObjectCount] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //ground 

////Basic-Level Texture Operation
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureSampler_Wrap 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureSampler_Mirror 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureSampler_Clamp 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureSampler_Border 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //texture1D 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //texture2D 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //texture2Darray
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //texture3D
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureCubeMap_SkyBox
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureCubeMap_Sphere
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureAnimation_Scroll
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureAnimation_Chunk

////High-Level Texture Operation
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                      "", //textureOriginal 
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",                            "", //textureBumpMap
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",                            "", //textureNormalMap
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",                            "", //textureParallaxMap
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",    									"", //textureOriginal2

};
static float g_instanceGap = 1.2f;
static int g_ObjectInstanceExtCount[g_ObjectCount] =
{
    0, //ground

////Basic-Level Texture Operation
    5, //textureSampler_Wrap 
    5, //textureSampler_Mirror 
    5, //textureSampler_Clamp 
    5, //textureSampler_Border 
    5, //texture1D 
    5, //texture2D 
    5, //texture2Darray 
    5, //texture3D 
    0, //textureCubeMap_SkyBox 
    5, //textureCubeMap_Sphere 
    5, //textureAnimation_Scroll
    5, //textureAnimation_Chunk

////High-Level Texture Operation
    5, //textureOriginal 
    5, //textureBumpMap 
    5, //textureNormalMap 
    5, //textureParallaxMap 
    0, //textureOriginal2 

};
static FVector3 g_ObjectTranforms[3 * g_ObjectCount] = 
{   
    FVector3(   0, -0.1,    0),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //ground

////Basic-Level Texture Operation
    FVector3(   0,  0.1,  0.4),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureSampler_Wrap
    FVector3(   0,  0.1,  1.5),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureSampler_Mirror
    FVector3(   0,  0.1,  2.6),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureSampler_Clamp
    FVector3(   0,  0.1,  3.7),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureSampler_Border
    FVector3(   0,  0.1,  4.8),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //texture1D
    FVector3(   0,  0.1,  5.9),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //texture2D
    FVector3(   0,  0.1,  7.0),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //texture2Darray
    FVector3(   0,  0.1,  8.1),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //texture3D
    FVector3(   0,    0,    0),     FVector3(     0,  0,  0),    FVector3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    FVector3(   0,  0.4,  9.2),     FVector3(     0,  0,  0),    FVector3( 0.005f,  0.005f,  0.005f), //textureCubeMap_Sphere
    FVector3(   0,  0.1, 10.3),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureAnimation_Scroll
    FVector3(   0,  0.1, 11.4),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureAnimation_Chunk

////High-Level Texture Operation
    FVector3(   0,  0.1, 12.5),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureOriginal
    FVector3(   0,  0.1, 13.6),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureBumpMap
    FVector3(   0,  0.1, 14.7),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureNormalMap
    FVector3(   0,  0.1, 15.8),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureParallaxMap
    FVector3(   0,  0.1, 16.9),     FVector3(     0,  0,  0),    FVector3( 0.01f,   0.01f,    0.01f), //textureOriginal2

};
static bool g_ObjectIsTransparents[g_ObjectCount] = 
{
    false, //ground

////Basic-Level Texture Operation
    false, //textureSampler_Wrap
    false, //textureSampler_Mirror
    false, //textureSampler_Clamp
    false, //textureSampler_Border
    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere
    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk

////High-Level Texture Operation
    false, //textureOriginal
    false, //textureBumpMap
    false, //textureNormalMap
    false, //textureParallaxMap
    false, //textureOriginal2

};
static bool g_ObjectIsShows[] = 
{
    true, //ground

////Basic-Level Texture Operation
    true, //textureSampler_Wrap
    true, //textureSampler_Mirror
    true, //textureSampler_Clamp
    true, //textureSampler_Border
    true, //texture1D
    true, //texture2D
    true, //texture2Darray
    true, //texture3D
    true, //textureCubeMap_SkyBox
    true, //textureCubeMap_Sphere
    true, //textureAnimation_Scroll
    true, //textureAnimation_Chunk

////High-Level Texture Operation
    true, //textureOriginal
    true, //textureBumpMap
    true, //textureNormalMap
    true, //textureParallaxMap
    true, //textureOriginal2

};
static GLenum g_ObjectTypeCulling[] = 
{
    GL_BACK, //ground

////Basic-Level Texture Operation
    GL_BACK, //textureSampler_Wrap
    GL_BACK, //textureSampler_Mirror
    GL_BACK, //textureSampler_Clamp
    GL_BACK, //textureSampler_Border
    GL_BACK, //texture1D
    GL_BACK, //texture2D
    GL_BACK, //texture2Darray
    GL_BACK, //texture3D
    GL_FRONT, //textureCubeMap_SkyBox
    GL_BACK, //textureCubeMap_Sphere
    GL_BACK, //textureAnimation_Scroll
    GL_BACK, //textureAnimation_Chunk

////High-Level Texture Operation
    GL_BACK, //textureOriginal
    GL_BACK, //textureBumpMap
    GL_BACK, //textureNormalMap
    GL_BACK, //textureParallaxMap
    GL_BACK, //textureOriginal2

};
static bool g_ObjectIsRotates[g_ObjectCount] =
{
    false, //ground

////Basic-Level Texture Operation
    false, //textureSampler_Wrap
    false, //textureSampler_Mirror
    false, //textureSampler_Clamp
    false, //textureSampler_Border
    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere
    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk

////High-Level Texture Operation
    false, //textureOriginal
    false, //textureBumpMap
    false, //textureNormalMap
    false, //textureParallaxMap
    false, //textureOriginal2

};
static bool g_ObjectIsLightings[g_ObjectCount] =
{
    true, //ground

////Basic-Level Texture Operation
    true, //textureSampler_Wrap
    true, //textureSampler_Mirror
    true, //textureSampler_Clamp
    true, //textureSampler_Border
    true, //texture1D
    true, //texture2D
    true, //texture2Darray
    true, //texture3D
    true, //textureCubeMap_SkyBox
    true, //textureCubeMap_Sphere
    true, //textureAnimation_Scroll
    true, //textureAnimation_Chunk

////High-Level Texture Operation
    true, //textureOriginal
    true, //textureBumpMap
    true, //textureNormalMap
    true, //textureParallaxMap
    true, //textureOriginal2

};


/////////////////////////// ModelMesh ///////////////////////////
bool OpenGLES_011_Texturing::ModelMesh::LoadMesh(bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(this->pathMesh, meshData, eMeshParserFlags))
    {
        F_LogError("*********************** OpenGLES_011_Texturing::ModelMesh::LoadMesh: load mesh failed: [%s] !", this->pathMesh.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tex2)
    {
        this->vertices_Pos3Color4Normal3Tex2.clear();
        this->vertices_Pos3Color4Normal3Tex2.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tex2 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.texCoord = vertex.texCoord;
            if (isTransformLocal)
            {
                v.pos = FMath::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4Normal3Tex2.push_back(v);
        }

        int count_index = (int)meshData.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshData.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        F_LogInfo("OpenGLES_011_Texturing::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                  this->nameMesh.c_str(),
                  (int)this->vertices_Pos3Color4Normal3Tex2.size(), 
                  (int)this->indices.size());
    }
    else if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
    {
        this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
        this->vertices_Pos3Color4Normal3Tangent3Tex2.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tangent3Tex2 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.tangent = vertex.tangent;
            v.texCoord = vertex.texCoord;
            if (isTransformLocal)
            {
                v.pos = FMath::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4Normal3Tangent3Tex2.push_back(v);
        }

        int count_index = (int)meshData.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshData.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        F_LogInfo("OpenGLES_011_Texturing::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                  this->nameMesh.c_str(),
                  (int)this->vertices_Pos3Color4Normal3Tangent3Tex2.size(), 
                  (int)this->indices.size());
    }

    //2> createBufferVertexIndex or createBufferVertex
    if (this->poIndexBuffer_Size > 0 &&
		this->poIndexBuffer_Data != nullptr)
	{
		this->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex("VertexIndex-" + this->nameMesh,
                                                                                 this->poTypeVertex,
                                                                                 this->poVertexBuffer_Size, 
                                                                                 (uint8*)this->poVertexBuffer_Data, 
                                                                                 false,
                                                                                 this->poIndexBuffer_Size, 
                                                                                 (uint8*)this->poIndexBuffer_Data, 
                                                                                 false);
		if (this->pBufferVertexIndex == nullptr)
		{
			F_LogError("*********************** DirectX11_010_Lighting::loadModel_VertexIndex: create buffer vertex index failed: [%s] !", this->nameMesh.c_str());
			return false;
		}
	}
	else
	{
		this->pBufferVertex = Base::GetWindowPtr()->createBufferVertex("Vertex-" + this->nameMesh,
                                                                       this->poTypeVertex,
                                                                       this->poVertexBuffer_Size, 
                                                                       (uint8*)this->poVertexBuffer_Data, 
                                                                       false);
		if (this->pBufferVertex == nullptr)
		{
			F_LogError("*********************** DirectX11_010_Lighting::loadModel_VertexIndex: create buffer vertex failed: [%s] !", this->nameMesh.c_str());
			return false;
		}
	}

    return true;
}



/////////////////////////// ModelObject /////////////////////////



OpenGLES_011_Texturing::OpenGLES_011_Texturing(String name)
    : OpenGLESWindow(name)
{
    this->cfg_isDepthStencil = true;
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
	this->cfg_isUseComputeShaderBeforeRender = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector4(0, -1, 0, 0); //y-
}

void OpenGLES_011_Texturing::createCamera()
{
    OpenGLESWindow::createCamera();
    
    cameraReset();
}
void OpenGLES_011_Texturing::cameraReset()
{
    OpenGLESWindow::cameraReset();

    this->pCamera->SetPos(FVector3(-4.0f, 24.0f, 3.6f));
    this->pCamera->SetEulerAngles(FVector3(80.0f, 0.0f, 0.0f));
	this->pCamera->UpdateViewMatrix();
}

void OpenGLES_011_Texturing::loadModel_Custom()
{
	createMeshes();
    createTextures();
    createShaderModules();
    createDescriptorSetLayouts();

    for (int i = 0; i < g_ObjectCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->indexModel = i;
        pModelObject->nameObject = g_ObjectConfigs[8 * i + 0];
        pModelObject->nameMesh = g_ObjectConfigs[8 * i + 1];

        //Mesh
        {
            ModelMesh* pMesh = this->findMesh(pModelObject->nameMesh);
            F_Assert(pMesh != nullptr && "OpenGLES_011_Texturing::loadModel_Custom")
            pModelObject->SetMesh(pMesh);
        }

        //Texture VS
        {
            String nameTextureVS = g_ObjectConfigs[8 * i + 2]; //Texture VS
            if (!nameTextureVS.empty())
            {
                StringVector aTextureVS = FUtilString::Split(nameTextureVS, ";");
                size_t count_tex = aTextureVS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    String nameTex = aTextureVS[j];
                    GLESTexture* pTextureVS = this->findTexture(nameTex);
                    pModelObject->AddTexture(F_GetShaderTypeName(F_Shader_Vertex), pTextureVS);
                }
            }
        }
        //Texture FS
        {
            String nameTextureFS = g_ObjectConfigs[8 * i + 6]; //Texture FS
            if (!nameTextureFS.empty())
            {
                StringVector aTextureFS = FUtilString::Split(nameTextureFS, ";");
                size_t count_tex = aTextureFS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    String nameTex = aTextureFS[j];
                    GLESTexture* pTextureFS = this->findTexture(nameTex);
                    pModelObject->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureFS);
                }
            }
        }
        //Texture CS
        {
            String nameTextureCS = g_ObjectConfigs[8 * i + 7]; //Texture CS
            if (!nameTextureCS.empty())
            {
                StringVector aTextureCS = FUtilString::Split(nameTextureCS, ";");
                size_t count_tex = aTextureCS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    String nameTex = aTextureCS[j];
                    GLESTexture* pTextureCS = this->findTexture(nameTex);
                    pModelObject->AddTexture(F_GetShaderTypeName(F_Shader_Compute), pTextureCS);
                }
            }
        }

        //Common
        pModelObject->isTransparent = g_ObjectIsTransparents[i];
        pModelObject->isShow = g_ObjectIsShows[i];
        pModelObject->isRotate = g_ObjectIsRotates[i];
        pModelObject->countInstanceExt = g_ObjectInstanceExtCount[i];
        pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;

        pModelObject->poTypeCulling = g_ObjectTypeCulling[i];

        m_aModelObjects.push_back(pModelObject);
        if (pModelObject->isTransparent)
            m_aModelObjects_Render.push_back(pModelObject);
        else 
            m_aModelObjects_Render.insert(m_aModelObjects_Render.begin(), pModelObject);
        m_mapModelObjects[pModelObject->nameObject] = pModelObject;
    }
}

void OpenGLES_011_Texturing::createCustomCB()
{
	rebuildInstanceCBs(true);
}
void OpenGLES_011_Texturing::rebuildInstanceCBs(bool isCreateBuffer)
{
	size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//1> Object
		pModelObject->instanceMatWorld.resize(MAX_OBJECT_COUNT);
        pModelObject->objectCBs.resize(MAX_OBJECT_COUNT);
        pModelObject->materialCBs.resize(MAX_MATERIAL_COUNT);
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectTranforms[i * 3 + 0] + FVector3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                        g_ObjectTranforms[i * 3 + 1],
                                                        g_ObjectTranforms[i * 3 + 2]);
            pModelObject->objectCBs[j] = objectConstants;
            pModelObject->instanceMatWorld[j] = objectConstants.g_MatWorld;

            //MaterialConstants
            MaterialConstants materialConstants;
            materialConstants.factorAmbient = FMath::RandomColor(false);
            materialConstants.factorDiffuse = FMath::RandomColor(false);
            materialConstants.factorSpecular = FMath::RandomColor(false);
            materialConstants.lighting.x = FMath::RandF(10.0f, 100.0f);
            materialConstants.lighting.y = FMath::RandF(0.2f, 0.9f);
            //Texture VS
            {
                GLESTexturePtrVector* pTextureVSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                if (pTextureVSs != nullptr)
                {

                }
            }
            //Texture FS
            {
                GLESTexturePtrVector* pTextureFSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                if (pTextureFSs != nullptr)
                {
                    int count_texture = (int)pTextureFSs->size();
                    for (int p = 0; p < count_texture; p++)
                    {
                        GLESTexture* pTexture = (*pTextureFSs)[p];
                        materialConstants.aTexLayers[p].texSize.x = (float)pTexture->width;
                        materialConstants.aTexLayers[p].texSize.y = (float)pTexture->height;
                        materialConstants.aTexLayers[p].texSize.z = (float)pTexture->depth;

                        if (pModelObject->nameObject == g_Object_Texture3D) //Texture3D
                        {
                            materialConstants.aTexLayers[p].texSize.w = FMath::RandF(0.0f, 1.0f);
                        }
                        else if (pModelObject->nameObject == g_Object_TextureAnimation_Scroll) //TextureAnimation_Scroll
                        {
                            if (pTexture->typeTexture == F_Texture_2DArray)
                            {
                                materialConstants.aTexLayers[p].texSize.w = (float)pTexture->RandomTextureIndex();

                                if (materialConstants.aTexLayers[p].texSize.w > 0)
                                {
                                    materialConstants.aTexLayers[p].texSpeed.x = FMath::RandF(1.0f, 10.0f);
                                }
                                else 
                                {
                                    materialConstants.aTexLayers[p].texSpeed.y = FMath::RandF(1.0f, 10.0f);
                                }
                            }
                        }
                        else if (pModelObject->nameObject == g_Object_TextureAnimation_Chunk) //TextureAnimation_Chunk
                        {
                            if (pTexture->texChunkMaxX > 0 &&
                                pTexture->texChunkMaxY > 0)
                            {
                                materialConstants.aTexLayers[p].texSize.w = 0;
                                materialConstants.aTexLayers[p].texChunk.x = (float)pTexture->texChunkMaxX;
                                materialConstants.aTexLayers[p].texChunk.y = (float)pTexture->texChunkMaxY;
                                int indexX = pTexture->texChunkIndex % pTexture->texChunkMaxX;
                                int indexZ = pTexture->texChunkIndex / pTexture->texChunkMaxX;
                                materialConstants.aTexLayers[p].texChunk.z = (float)indexX;
                                materialConstants.aTexLayers[p].texChunk.w = (float)indexZ;
                            }
                        }       
                        else if (pModelObject->nameObject == g_Object_TextureOriginal) //TextureOriginal
                        {
                            if (j == pModelObject->countInstance / 2)
                            {
                                materialConstants.factorAmbient = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
                                materialConstants.factorDiffuse = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                                materialConstants.factorSpecular = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                                materialConstants.lighting.z = 0.0f;
                            }
                        }
                        else if (pModelObject->nameObject == g_Object_TextureBumpMap) //TextureBumpMap
                        {
                            if (j == pModelObject->countInstance / 2)
                            {
                                materialConstants.factorAmbient = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
                                materialConstants.factorDiffuse = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                                materialConstants.factorSpecular = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                            }
                            materialConstants.aTexLayers[p].texSize.w = 1; 
                            materialConstants.aTexLayers[p].texSpeed.x = FMath::RandF(20.0f, 1000.0f);
                        } 
                        else if (pModelObject->nameObject == g_Object_TextureNormalMap) //TextureNormalMap
                        {
                            if (j == pModelObject->countInstance / 2)
                            {
                                materialConstants.factorAmbient = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
                                materialConstants.factorDiffuse = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                                materialConstants.factorSpecular = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                            }
                            materialConstants.aTexLayers[p].texSize.w = 2;
                        }
                        else if (pModelObject->nameObject == g_Object_TextureParallaxMap) //TextureParallaxMap
                        {
                            if (j == pModelObject->countInstance / 2)
                            {
                                materialConstants.factorAmbient = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
                                materialConstants.factorDiffuse = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                                materialConstants.factorSpecular = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                            }
                            materialConstants.aTexLayers[p].texSize.w = 3;
                            materialConstants.aTexLayers[p].texSpeed.x = 0.1f;
                            materialConstants.aTexLayers[p].texSpeed.y = -0.02f;
                            materialConstants.aTexLayers[p].texSpeed.z = 48.0f;
                        }
                    }
                }
            }
            //Texture CS
            {
                GLESTexturePtrVector* pTextureCSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                if (pTextureCSs != nullptr)
                {

                }
            }
            pModelObject->materialCBs[j] = materialConstants;
        }

		if (isCreateBuffer)
		{
            //ObjectConstants
			String nameBuffer = "ObjectConstants-" + FUtilString::SaveInt((int)i);
			pModelObject->poBufferUniform = createBufferUniform(nameBuffer,
																DescriptorSet_ObjectConstants,
																GL_DYNAMIC_DRAW,
																sizeof(ObjectConstants) * pModelObject->objectCBs.size(),
																(uint8*)(pModelObject->objectCBs.data()),
																false);
			if (!pModelObject->poBufferUniform)
			{
				String msg = "*********************** OpenGLES_011_Texturing::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError("%s", msg.c_str());
				throw std::runtime_error(msg);
			}

            //MaterialConstants
            nameBuffer = "MaterialConstants-" + FUtilString::SaveInt((int)i);
            pModelObject->poBufferUniform_Material = createBufferUniform(nameBuffer,
                                                                         DescriptorSet_MaterialConstants,
                                                                         GL_DYNAMIC_DRAW,
                                                                         sizeof(MaterialConstants) * pModelObject->materialCBs.size(),
                                                                         (uint8*)(pModelObject->materialCBs.data()),
                                                                         false);
            if (!pModelObject->poBufferUniform_Material)
            {
                String msg = "*********************** OpenGLES_011_Texturing::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }
		}
    }
}

void OpenGLES_011_Texturing::createCustomBeforePipeline()
{
    
}  
void OpenGLES_011_Texturing::createGraphicsPipeline_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//<1> Shaders
        String nameShaderVert = g_ObjectNameShaderModules[6 * i + 0];
        String nameShaderFrag = g_ObjectNameShaderModules[6 * i + 4];
        GLESShader* pShaderVertex = findShaderModule(nameShaderVert);
        GLESShader* pShaderFragment = findShaderModule(nameShaderFrag);
        DescriptorSetLayout* pDescriptorSetLayout = findDescriptorSetLayout(g_ObjectNameDescriptorSetLayouts[2 * i + 0]);

        bool poDepthEnabled = pModelObject->poDepthEnabled;
		GLenum poDepthFuncCompare = pModelObject->poDepthFuncCompare;
		bool poDepthTestEnabled = pModelObject->poDepthTestEnabled;
		bool poDepthWriteEnabled = pModelObject->poDepthWriteEnabled;
        bool poBlendEnabled = pModelObject->poBlendEnabled;
        GLenum poBlendColorFactorSrc = pModelObject->poBlendColorFactorSrc; 
		GLenum poBlendColorFactorDst = pModelObject->poBlendColorFactorDst; 
        if (pModelObject->isTransparent)
        {
            poDepthTestEnabled = false;
            poDepthWriteEnabled = false;

            poBlendEnabled = true;
            poBlendColorFactorSrc = GL_SRC_ALPHA;
            poBlendColorFactorDst = GL_ONE_MINUS_SRC_ALPHA;
        }

        //poStatePipelineGraphics
		String namePipelineGraphics = "PipelineGraphics-" + pModelObject->nameObject;
        pModelObject->poStatePipelineGraphics = createStatePipelineGraphics(namePipelineGraphics,
                                                                            pDescriptorSetLayout,
                                                                            pShaderVertex,
                                                                            nullptr,
                                                                            nullptr,
                                                                            nullptr,
                                                                            pShaderFragment,
                                                                            this->poTypeVertex,
                                                                            pModelObject->poTypePrimitive,
                                                                            pModelObject->poIsCull,
                                                                            pModelObject->poTypeFrontFace,
                                                                            pModelObject->poTypeCulling,
                                                                            pModelObject->poTypePolygonMode,
                                                                            poDepthEnabled,
                                                                            poDepthFuncCompare,
                                                                            poDepthTestEnabled,
                                                                            poDepthWriteEnabled,
                                                                            pModelObject->poStencilEnabled,
                                                                            pModelObject->poStencil_CompareFunction,
                                                                            pModelObject->poStencil_StencilFailureOp,
                                                                            pModelObject->poStencil_DepthFailureOp,
                                                                            pModelObject->poStencil_DepthStencilPassOp,
                                                                            pModelObject->poStencil_Ref,
                                                                            pModelObject->poStencil_Mask,
                                                                            poBlendEnabled,
                                                                            poBlendColorFactorSrc,
                                                                            poBlendColorFactorDst,
                                                                            pModelObject->poBlendColorOp,
                                                                            pModelObject->poBlendAlphaFactorSrc,
                                                                            pModelObject->poBlendAlphaFactorDst,
                                                                            pModelObject->poBlendAlphaOp,
                                                                            pModelObject->poColorWriteMask_Red,
                                                                            pModelObject->poColorWriteMask_Green,
                                                                            pModelObject->poColorWriteMask_Blue,
                                                                            pModelObject->poColorWriteMask_Alpha);
        if (pModelObject->poStatePipelineGraphics == nullptr)
        {
            String msg = "*********************** OpenGLES_011_Texturing::createGraphicsPipeline_Custom: Failed to create pipeline !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
}

void OpenGLES_011_Texturing::createComputePipeline_Custom()
{

}

void OpenGLES_011_Texturing::destroyMeshes()
{
    size_t count = this->m_aModelMesh.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelMesh* pMesh = this->m_aModelMesh[i];
        delete pMesh;
    }
    this->m_aModelMesh.clear();
    this->m_mapModelMesh.clear();
}
void OpenGLES_011_Texturing::createMeshes()
{
    for (int i = 0; i < g_MeshCount; i++)
    {
        String nameMesh = g_MeshPaths[5 * i + 0];
        String nameVertexType = g_MeshPaths[5 * i + 1];
        String nameMeshType = g_MeshPaths[5 * i + 2];
        String nameGeometryType = g_MeshPaths[5 * i + 3];
        String pathMesh = g_MeshPaths[5 * i + 4];
        
        FMeshVertexType typeVertex = F_ParseMeshVertexType(nameVertexType); 
        FMeshType typeMesh = F_ParseMeshType(nameMeshType);
        FMeshGeometryType typeGeometryType = F_MeshGeometry_EntityTriangle;
        if (!nameGeometryType.empty())
        {
            typeGeometryType = F_ParseMeshGeometryType(nameGeometryType);
        }

        ModelMesh* pMesh = new ModelMesh(this, 
                                         nameMesh,
                                         pathMesh,
                                         typeMesh,
                                         typeGeometryType,
                                         typeVertex);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTransformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals[i]))
        {
            String msg = "*********************** OpenGLES_011_Texturing::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("OpenGLES_011_Texturing::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), pathMesh.c_str());
    }
}
OpenGLES_011_Texturing::ModelMesh* OpenGLES_011_Texturing::findMesh(const String& nameMesh)
{
    ModelMeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_011_Texturing::destroyTextures()
{
    size_t count = this->m_aModelTexture.size();
    for (size_t i = 0; i < count; i++)
    {
        GLESTexture* pTexture = this->m_aModelTexture[i];
        delete pTexture;
    }
    this->m_aModelTexture.clear();
    this->m_mapModelTexture.clear();
}
void OpenGLES_011_Texturing::createTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        String nameTexture = g_TexturePaths[5 * i + 0];
        String nameType = g_TexturePaths[5 * i + 1];
        FTextureType typeTexture = F_ParseTextureType(nameType);
        String nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
        String nameIsGraphicsComputeShared = g_TexturePaths[5 * i + 3];
        bool isGraphicsComputeShared = FUtilString::ParserBool(nameIsGraphicsComputeShared);
        String pathTextures = g_TexturePaths[5 * i + 4];

        StringVector aPathTexture = FUtilString::Split(pathTextures, ";");
        GLESTexture* pTexture = new GLESTexture(nameTexture,
												aPathTexture,
												typeTexture,
												g_TextureFormats[i],
												g_TextureAddressings[i],
												g_TextureFilters[i * 2 + 0],
												g_TextureFilters[i * 2 + 1],
												F_MSAASampleCount_1_Bit,
												g_TextureBorderColors[i],
												true,
												true,
												false,
												isRenderTarget,
												false,
												FMath::ms_clBlack);
        pTexture->texChunkMaxX = (int)g_TextureAnimChunks[i * 2 + 0];
        pTexture->texChunkMaxY = (int)g_TextureAnimChunks[i * 2 + 1];
        if (pTexture->texChunkMaxX > 0 && 
            pTexture->texChunkMaxY > 0)
        {
            pTexture->texChunkIndex = FMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
        }
        pTexture->AddRef();

        int width = g_TextureSizes[3 * i + 0];
        int height = g_TextureSizes[3 * i + 1];
        int depth = g_TextureSizes[3 * i + 1];
        pTexture->LoadTexture(width, 
                              height,
                              depth,
                              g_TextureChannels[i],
                              nullptr);

        this->m_aModelTexture.push_back(pTexture);
        this->m_mapModelTexture[nameTexture] = pTexture;

        F_LogInfo("OpenGLES_011_Texturing::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
GLESTexture* OpenGLES_011_Texturing::findTexture(const String& nameTexture)
{
    GLESTexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_011_Texturing::destroyShaderModules()
{
    size_t count = this->m_aGLESShaderModules.size();
    for (size_t i = 0; i < count; i++)
    {
        GLESShader* pShader = this->m_aGLESShaderModules[i];
        F_DELETE(pShader)
    }
    this->m_aGLESShaderModules.clear();
    this->m_mapGLESShaderModules.clear();
}
void OpenGLES_011_Texturing::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = getShaderPathRelative(g_ShaderModulePaths[3 * i + 2]);

        GLESShader* pShader =  createShader(shaderName, shaderPath, shaderType);
        if (pShader == nullptr)
        {
            String msg = "*********************** OpenGLES_011_Texturing::createShaderModules: Failed to create shader: shaderPath: " + shaderPath;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShader);
        this->m_mapGLESShaderModules[shaderName] = pShader;
        F_LogInfo("OpenGLES_011_Texturing::createShaderModules: create shader [%s] success !", shaderPath.c_str());
    }
}
GLESShader* OpenGLES_011_Texturing::findShaderModule(const String& pathShaderModule)
{
    GLESShaderPtrMap::iterator itFind = this->m_mapGLESShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapGLESShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_011_Texturing::destroyDescriptorSetLayouts()
{
    size_t count = this->m_aDescriptorSetLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        DescriptorSetLayout* pDescriptorSetLayout = this->m_aDescriptorSetLayouts[i];
        F_DELETE(pDescriptorSetLayout)
    }
    this->m_aDescriptorSetLayouts.clear();
    this->m_mapDescriptorSetLayouts.clear();
}
void OpenGLES_011_Texturing::createDescriptorSetLayouts()
{
    for (size_t i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameDescriptorSetLayouts_Graphics(g_nameDescriptorSetLayouts[i]);
        DescriptorSetLayout* p = new DescriptorSetLayout();
        p->Init(nameDescriptorSetLayouts_Graphics);
        this->m_aDescriptorSetLayouts.push_back(p);
        this->m_mapDescriptorSetLayouts[nameDescriptorSetLayouts_Graphics] = p;
    }
    
}
DescriptorSetLayout* OpenGLES_011_Texturing::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    DescriptorSetLayoutPtrMap::iterator itFind = this->m_mapDescriptorSetLayouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapDescriptorSetLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void OpenGLES_011_Texturing::createDescriptorSets_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//Pipeline Graphics
		{
			int nIndexTextureVS = 0;
            int nIndexTextureFS = 0;
            int count_ds = (int)pModelObject->poStatePipelineGraphics->poDescriptorSetLayout->aLayouts.size();
            for (int j = 0; j < count_ds; j++)
            {
                String& nameDescriptorSet = pModelObject->poStatePipelineGraphics->poDescriptorSetLayout->aLayouts[j];
				
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants)) //PassConstants
                {
					uint32 nBindingIndex = (uint32)DescriptorSet_PassConstants;
					uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
					pModelObject->poStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants)) //ObjectConstants
                {
					uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
					uint32 nBindingIndex = (uint32)DescriptorSet_ObjectConstants;
					pModelObject->poStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
					pModelObject->poStatePipelineGraphics->BindBufferUniform(pModelObject->poBufferUniform, nBindingIndex);
                } 
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants)) //MaterialConstants
                {
					uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
					uint32 nBindingIndex = (uint32)DescriptorSet_MaterialConstants;
					pModelObject->poStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
					pModelObject->poStatePipelineGraphics->BindBufferUniform(pModelObject->poBufferUniform_Material, nBindingIndex);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_InstanceConstants)) //InstanceConstants
                {

                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureVS)) //TextureVS
                {
                    GLESTexture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Vertex), nIndexTextureVS);
					pModelObject->poStatePipelineGraphics->BindTextureVS(pTexture, nIndexTextureVS);
					nIndexTextureVS ++;
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureFS)) //TextureFS
                {
                    GLESTexture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Fragment), nIndexTextureFS);
                    pModelObject->poStatePipelineGraphics->BindTextureFS(pTexture, nIndexTextureFS);
					nIndexTextureFS ++;
                }
                else
                {
                    String msg = "*********************** OpenGLES_011_Texturing::createDescriptorSets_Custom: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
		}
    }
}

void OpenGLES_011_Texturing::updateCBs_Custom()
{
	GLESBufferUniform* pBufferUniform_Pass = GetUniform_PassCB();
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//0: PassConstants
		pModelObject->poStatePipelineGraphics->BindBufferUniform(pBufferUniform_Pass, (uint32)DescriptorSet_PassConstants);

		//1: ObjectConstants
        for (size_t j = 0; j < pModelObject->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
            if (pModelObject->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pModelObject->instanceMatWorld[j],
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->instanceMatWorld[j];
            }

			//MaterialConstants
			MaterialConstants& materialCB = pModelObject->materialCBs[j];
            if (pModelObject->nameObject == g_Object_TextureAnimation_Chunk)
            {
                GLESTexturePtrVector* pTextureFSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                if (pTextureFSs != nullptr)
                {
                    size_t count_texture = pTextureFSs->size();
                    for (size_t p = 0; p < count_texture; p++)
                    {
                        GLESTexture* pTexture = (*pTextureFSs)[p];

                        if (pTexture->texChunkMaxX > 0 &&
                            pTexture->texChunkMaxY > 0)
                        {
                            if (++ pTexture->frameCurrent >= 30)
                            {
                                pTexture->frameCurrent = 0;
                                pTexture->texChunkIndex ++;
                                if (pTexture->texChunkIndex >= pTexture->texChunkMaxX * pTexture->texChunkMaxY)
                                {
                                    pTexture->texChunkIndex = 0;
                                }
                                int indexX = pTexture->texChunkIndex % pTexture->texChunkMaxX;
                                int indexZ = pTexture->texChunkIndex / pTexture->texChunkMaxX;
                                materialCB.aTexLayers[0].texChunk.z = (float)indexX;
                                materialCB.aTexLayers[0].texChunk.w = (float)indexZ;
                            }   
                        }
                    }
                }
            }   
        }

        //ObjectConstants
		pModelObject->poBufferUniform->UpdateBuffer(sizeof(ObjectConstants) * pModelObject->objectCBs.size(),
                                                 	(uint8*)pModelObject->objectCBs.data(),
													GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		//MaterialConstants
        pModelObject->poBufferUniform_Material->UpdateBuffer(sizeof(MaterialConstants) * pModelObject->materialCBs.size(),
                                                             (uint8*)pModelObject->materialCBs.data(),
                                                             GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
}



bool OpenGLES_011_Texturing::beginRenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("OpenGLES_011_Texturing", &windowOpened, 0);
    {
        //0> Common
        commonConfig();
        
        //1> Camera
        cameraConfig();

        //2> Light
        lightConfig();

        //3> PassConstants
        passConstantsConfig();
        
        //4> Model
        modelConfig();

    }
    ImGui::End();

    return true;
}
void OpenGLES_011_Texturing::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
		float fGap = g_instanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 5.0f))
        {
            g_instanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        size_t count = this->m_aModelObjects.size();
        for (size_t i = 0; i < count; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];

            String nameModel = FUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                String nameIsShow = "Is Show - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                String nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
				String nameIsTransparent = "Is Transparent - " + pModelObject->nameObject;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
				String nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < pModelObject->countInstance; j++)
                    {
                        MaterialConstants& mat = pModelObject->materialCBs[j];
                        mat.lighting.z = pModelObject->isLighting ? 1.0f : 0.0f;
                    }
                }
                
                String nameInstances = "Instance - " + pModelObject->nameObject;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 10);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->pMesh->poVertexCount, (int)pModelObject->pMesh->poIndexCount);

                String nameWorld = "Model World - " + pModelObject->nameObject;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    int count_instance = pModelObject->countInstance;
                    for (int j = 0; j < count_instance; j++)
                    {
                        ObjectConstants& obj = pModelObject->objectCBs[j];
                        MaterialConstants& mat = pModelObject->materialCBs[j];

                        String nameModelInstance = nameModel + " - " + FUtilString::SaveInt(j);
                        if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                        {
                            //ObjectConstants
                            String nameObject = FUtilString::SaveInt(j) + " - Object - " + pModelObject->nameObject;
                            if (ImGui::CollapsingHeader(nameObject.c_str()))
                            {
                                const FMatrix4& mat4World = obj.g_MatWorld;
                                String nameTable = FUtilString::SaveInt(j) + " - matWorld - " + pModelObject->nameObject;
                                if (ImGui::BeginTable(nameTable.c_str(), 4))
                                {
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][3]);

                                    ImGui::EndTable();
                                }
                            }
                            
                            //MaterialConstants
                            String nameMaterial = FUtilString::SaveInt(j) + " - Material - " + pModelObject->nameObject;
                            if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                            {
                                //factorAmbient
                                String nameFactorAmbient = "FactorAmbient - " + FUtilString::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                {

                                }
                                ImGui::Spacing();

                                //factorDiffuse
                                String nameFactorDiffuse = "FactorDiffuse - " + FUtilString::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                {

                                }
                                ImGui::Spacing();

                                //factorSpecular
                                String nameFactorSpecular = "FactorSpecular - " + FUtilString::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                {

                                }
                                ImGui::Spacing();

                                //shininess
                                String nameShininess = "Shininess - " + FUtilString::SaveInt(j);
                                if (ImGui::DragFloat(nameShininess.c_str(), &mat.lighting.x, 0.01f, 0.01f, 100.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //alpha
                                String nameAlpha = "Alpha - " + FUtilString::SaveInt(j);
                                if (ImGui::DragFloat(nameAlpha.c_str(), &mat.lighting.y, 0.001f, 0.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //lighting
                                String nameLighting = "Lighting - " + FUtilString::SaveInt(j);
                                bool isLighting = mat.lighting.z == 1.0f ? true : false;
                                if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                {
                                    mat.lighting.z = isLighting ? 1.0f : 0.0f;
                                }

								//Texture VS
                                {
                                    GLESTexturePtrVector* pTextureVSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                                    if (pTextureVSs != nullptr)
                                    {

                                    }
                                }
                                //Texture FS
                                {
                                    GLESTexturePtrVector* pTextureFSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                                    if (pTextureFSs != nullptr)
                                    {
                                        int count_texture = (int)pTextureFSs->size();
                                        for (int p = 0; p < count_texture; p++)
                                        {
                                            GLESTexture* pTexture = (*pTextureFSs)[p];

                                            String nameMaterial_Texture = FUtilString::SaveInt(j) + " - Material - " + pModelObject->nameObject + " - TextureFS - " + FUtilString::SaveInt(p);
                                            if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                            {
                                                //texWidth
                                                String nameWidth = "Width - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int width = pTexture->width;
                                                ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                //texHeight
                                                String nameHeight = "Height - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int height = pTexture->height;
                                                ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                //texDepth
                                                String nameDepth = "Depth - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int depth = pTexture->depth;
                                                ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                //indexTextureArray
                                                String nameIndexTextureArray = "IndexTextureArray - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (pTexture->typeTexture == F_Texture_2DArray)
                                                {
                                                    int count_tex = (int)pTexture->aPathTexture.size();
                                                    int indexTextureArray = (int)mat.aTexLayers[p].texSize.w;
                                                    if (ImGui::DragInt(nameIndexTextureArray.c_str(), &indexTextureArray, 1, 0, count_tex-1))
                                                    {
                                                        mat.aTexLayers[p].texSize.w = (float)indexTextureArray;
                                                    }
                                                }
                                                else 
                                                {
                                                    if (pModelObject->nameObject == g_Object_TextureBumpMap) //TextureBumpMap
                                                    {
                                                        //BumpMapping Type
                                                        int nIndex = 0;
                                                        for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aBumpMappingDescs); nIndex++)
                                                        {
                                                            if (s_aBumpMappingDescs[nIndex].Value == (int)mat.aTexLayers[p].texSize.w)
                                                                break;
                                                        }
                                                        const char* preview_text = s_aBumpMappingDescs[nIndex].Name;
                                                        String nameBumpMappingType = "BumpMappingType - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::BeginCombo(nameBumpMappingType.c_str(), preview_text))
                                                        {
                                                            for (int q = 0; q < IM_ARRAYSIZE(s_aBumpMappingDescs); q++)
                                                            {
                                                                if (ImGui::Selectable(s_aBumpMappingDescs[q].Name, nIndex == q))
                                                                {
                                                                    mat.aTexLayers[p].texSize.w = (float)s_aBumpMappingDescs[q].Value;
                                                                    break;
                                                                }
                                                            }
                                                            ImGui::EndCombo();
                                                        }

                                                        //Bump Scale
                                                        String nameBumpScale = "BumpScale - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::DragFloat(nameBumpScale.c_str(), &mat.aTexLayers[p].texSpeed.x, 0.5f, 0.0f, 5000.0f))
                                                        {
                                                            
                                                        }
                                                    }
                                                    else if (pModelObject->nameObject == g_Object_TextureNormalMap) //TextureNormalMap
                                                    {
                                                        //NormalMapping Type
                                                        int nIndex = 0;
                                                        for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aNormalMappingDescs); nIndex++)
                                                        {
                                                            if (s_aNormalMappingDescs[nIndex].Value == mat.aTexLayers[p].texSize.w)
                                                                break;
                                                        }
                                                        const char* preview_text = s_aNormalMappingDescs[nIndex].Name;
                                                        String nameNormalMappingType = "NormalMappingType - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::BeginCombo(nameNormalMappingType.c_str(), preview_text))
                                                        {
                                                            for (int q = 0; q < IM_ARRAYSIZE(s_aNormalMappingDescs); q++)
                                                            {
                                                                if (ImGui::Selectable(s_aNormalMappingDescs[q].Name, nIndex == q))
                                                                {
                                                                    mat.aTexLayers[p].texSize.w = (float)s_aNormalMappingDescs[q].Value;
                                                                    break;
                                                                }
                                                            }
                                                            ImGui::EndCombo();
                                                        }
                                                    }
                                                    else if (pModelObject->nameObject == g_Object_TextureParallaxMap) //TextureParallaxMap
                                                    {
                                                        //ParallaxMapping Type
                                                        int nIndex = 0;
                                                        for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aParallaxMappingDescs); nIndex++)
                                                        {
                                                            if (s_aParallaxMappingDescs[nIndex].Value == mat.aTexLayers[p].texSize.w)
                                                                break;
                                                        }
                                                        const char* preview_text = s_aParallaxMappingDescs[nIndex].Name;
                                                        String nameParallaxMappingType = "ParallaxMappingType - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::BeginCombo(nameParallaxMappingType.c_str(), preview_text))
                                                        {
                                                            for (int q = 0; q < IM_ARRAYSIZE(s_aParallaxMappingDescs); q++)
                                                            {
                                                                if (ImGui::Selectable(s_aParallaxMappingDescs[q].Name, nIndex == q))
                                                                {
                                                                    mat.aTexLayers[p].texSize.w = (float)s_aParallaxMappingDescs[q].Value;
                                                                    break;
                                                                }
                                                            }
                                                            ImGui::EndCombo();
                                                        }

                                                        //heightScale
                                                        String nameHeightScale = "HeightScale - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::DragFloat(nameHeightScale.c_str(), &mat.aTexLayers[p].texSpeed.x, 0.01f, 0.0f, 10.0f))
                                                        {
                                                            
                                                        }
                                                        //parallaxBias
                                                        String nameParallaxBias = "ParallaxBias - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::DragFloat(nameParallaxBias.c_str(), &mat.aTexLayers[p].texSpeed.y, 0.01f, -5.0f, 5.0f))
                                                        {
                                                            
                                                        }
                                                        //numLayers
                                                        String nameNumLayers = "NumLayers - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        int numLayers = (int)mat.aTexLayers[p].texSpeed.z;
                                                        if (ImGui::DragInt(nameNumLayers.c_str(), &numLayers, 1, 1, 100))
                                                        {
                                                            mat.aTexLayers[p].texSpeed.z = (float)numLayers;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].texSize.w, 0.001f, 0.0f, 1.0f))
                                                        {

                                                        }
                                                    }
                                                }

                                                if (pModelObject->nameObject != g_Object_TextureBumpMap &&
                                                    pModelObject->nameObject != g_Object_TextureParallaxMap)
                                                {
                                                    //texSpeedU
                                                    String nameTexSpeedU = "TexSpeedU - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                    if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeed.x, 0.01f, 0.0f, 100.0f))
                                                    {
                                                        
                                                    }
                                                }
                                                
                                                if (pModelObject->nameObject != g_Object_TextureParallaxMap)
                                                {
                                                    //texSpeedV
                                                    String nameTexSpeedV = "TexSpeedV - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                    if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeed.y, 0.01f, 0.0f, 100.0f))
                                                    {
                                                        
                                                    }
                                                    //texSpeedW
                                                    String nameTexSpeedW = "TexSpeedW - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                    if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeed.z, 0.01f, 0.0f, 100.0f))
                                                    {
                                                        
                                                    }
                                                }
                                                
                                                //texChunkMaxX
                                                String nameTexChunkMaxX = "TexChunkMaxX - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkMaxX = mat.aTexLayers[p].texChunk.x;
                                                ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                //texChunkMaxY
                                                String nameTexChunkMaxY = "TexChunkMaxY - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkMaxY = mat.aTexLayers[p].texChunk.y;
                                                ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                //texChunkIndexX
                                                String nameTexChunkIndexX = "TexChunkIndexX - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkIndexX = mat.aTexLayers[p].texChunk.z;
                                                ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                //texChunkIndexY
                                                String nameTexChunkIndexY = "TexChunkIndexY - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkIndexY = mat.aTexLayers[p].texChunk.w;
                                                ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                            }
                                        }
                                    }
                                }
                                //Texture CS
                                {
                                    GLESTexturePtrVector* pTextureCSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                                    if (pTextureCSs != nullptr)
                                    {

                                    }
                                }

                                ImGui::Spacing();
                            }
                        }
                    }
                }
            }
        }
    }
}

void OpenGLES_011_Texturing::endRenderImgui()
{
    OpenGLESWindow::endRenderImgui();

}

void OpenGLES_011_Texturing::drawMeshDefault_Custom()
{   
	size_t count = this->m_aModelObjects_Render.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects_Render[i];
        if (!pModelObject->isShow)
            continue;
		
		//State/Shader/BufferUniform/Texture
		pModelObject->poStatePipelineGraphics->BindState();
		pModelObject->poStatePipelineGraphics->BindShader();
		pModelObject->poStatePipelineGraphics->BindBufferUniforms();
		pModelObject->poStatePipelineGraphics->BindTextures();
		
		//Draw
		if (pModelObject->pMesh->pBufferVertex != nullptr)
		{
			pModelObject->pMesh->pBufferVertex->BindVertexArray();
			drawInstance(pModelObject->poStatePipelineGraphics->poTypePrimitive, 0, pModelObject->pMesh->poVertexCount, pModelObject->countInstance);
		}
		else if (pModelObject->pMesh->pBufferVertexIndex != nullptr)
		{
			pModelObject->pMesh->pBufferVertexIndex->BindVertexArray();
			drawIndexedInstance(pModelObject->poStatePipelineGraphics->poTypePrimitive, pModelObject->pMesh->poIndexCount, GL_UNSIGNED_INT, 0, pModelObject->countInstance);
		}
		else
		{	
			F_Assert(false && "OpenGLES_011_Texturing::drawMeshDefault_Custom")
		}
        pModelObject->poStatePipelineGraphics->UnBindState();
    }
}

void OpenGLES_011_Texturing::cleanupCustom()
{
    destroyDescriptorSetLayouts();
    destroyShaderModules();
	destroyTextures();
    destroyMeshes();

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        F_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
	this->m_aModelObjects_Render.clear();
    this->m_mapModelObjects.clear();
}

void OpenGLES_011_Texturing::cleanupSwapChain_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }
}

void OpenGLES_011_Texturing::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}