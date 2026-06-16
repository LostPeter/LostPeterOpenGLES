/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-06-15
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_012_shadering.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 5;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "geo_triangle",     "Pos3Color4Normal3Tex2",                "geometry",         "EntityTriangle",           "", //geo_triangle

    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/plane.fbx", //plane
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/sphere.fbx", //sphere
    "bunny",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Model/bunny/bunny.obj", //bunny

};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //geo_triangle

    true, //plane
    false, //cube
    false, //sphere
    false, //bunny

};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //geo_triangle

    false, //plane  
    false, //cube
    false, //sphere
    false, //bunny
    
};
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //geo_triangle

    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //cube
    FMath::ms_mat4Unit, //sphere
    FMath::ms_mat4Unit, //bunny

};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 11;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type      //TextureIsRenderTarget     //TextureIsUnOrderedAccess          //Texture Path
    "default_white",                    "2D",               "false",                    "false",                            "Texture/Common/default_white.bmp", //default_white  
    "default_blackwhite_01",            "2D",               "false",                    "false",                            "Texture/Common/default_blackwhite.png", //default_blackwhite_01
    "bricks_diffuse",                   "2D",               "false",                    "false",                            "Texture/Common/bricks_diffuse.png", //bricks_diffuse
    "terrain",                          "2D",               "false",                    "false",                            "Texture/Common/terrain.png", //terrain
    "texture2d",                        "2D",               "false",                    "false",                            "Texture/Common/texture2d.jpg", //texture2d
    
    "texturecubemap",                   "CubeMap",          "false",                    "false",                            "Texture/Sky/texturecubemap_x_right.png;Texture/Sky/texturecubemap_x_left.png;Texture/Sky/texturecubemap_y_up.png;Texture/Sky/texturecubemap_y_down.png;Texture/Sky/texturecubemap_z_front.png;Texture/Sky/texturecubemap_z_back.png", //texturecubemap

    "texture_terrain_diffuse",          "2DArray",          "false",                    "false",                            "Texture/Terrain/shore_sand_albedo.png;Texture/Terrain/moss_albedo.png;Texture/Terrain/rock_cliff_albedo.png;Texture/Terrain/cliff_albedo.png", //texture_terrain_diffuse
    "texture_terrain_normal",           "2DArray",          "false",                    "false",                            "Texture/Terrain/shore_sand_norm.png;Texture/Terrain/moss_norm.tga;Texture/Terrain/rock_cliff_norm.tga;Texture/Terrain/cliff_norm.png", //texture_terrain_normal
    "texture_terrain_control",          "2DArray",          "false",                    "false",                            "Texture/Terrain/terrain_control.png", //texture_terrain_control

    "texture_rt_compute_copy_tex",      "2D",               "true",                     "true",                             "", //texture_rt_compute_copy_tex
    "texture_rt_compute_copy_texarray", "2D",                "true",                    "true",                             "", //texture_rt_compute_copy_texarray

};
static int g_TextureChannels[g_TextureCount] = 
{
    4, //default_white
    4, //default_blackwhite_01
    4, //bricks_diffuse
    4, //terrain
    4, //texture2d

    4, //texturecubemap

    4, //texture_terrain_diffuse
    4, //texture_terrain_normal
    4, //texture_terrain_control

    4, //texture_rt_compute_copy_tex
    4, //texture_rt_compute_copy_texarray

};
static FTexturePixelFormatType g_TextureFormats[g_TextureCount] = 
{
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_white
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_blackwhite_01
    F_TexturePixelFormat_R8G8B8A8_SRGB, //bricks_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //terrain
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d

    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturecubemap

    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture_terrain_diffuse
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_terrain_normal
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_terrain_control

    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_rt_compute_copy_tex
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_rt_compute_copy_texarray

};
static FTextureFilterType g_TextureFilters[2 * g_TextureCount] = 
{
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_white
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_blackwhite_01
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //bricks_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //terrain
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture2d

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturecubemap

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_terrain_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_terrain_normal
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_terrain_control

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_rt_compute_copy_tex
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_rt_compute_copy_texarray

};
static FTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    F_TextureAddressing_Clamp, //default_white
    F_TextureAddressing_Clamp, //default_blackwhite_01
    F_TextureAddressing_Clamp, //bricks_diffuse
    F_TextureAddressing_Clamp, //terrain
    F_TextureAddressing_Clamp, //texture2d

    F_TextureAddressing_Wrap, //texturecubemap

    F_TextureAddressing_Clamp, //texture_terrain_diffuse
    F_TextureAddressing_Clamp, //texture_terrain_normal
    F_TextureAddressing_Clamp, //texture_terrain_control

    F_TextureAddressing_Clamp, //texture_rt_compute_copy_tex
    F_TextureAddressing_Clamp, //texture_rt_compute_copy_texarray

};
static FColor g_TextureBorderColors[g_TextureCount] = 
{
    FColor(0, 0, 0, 1), //default_white
    FColor(0, 0, 0, 1), //default_blackwhite_01
    FColor(0, 0, 0, 1), //bricks_diffuse
    FColor(0, 0, 0, 1), //terrain
    FColor(0, 0, 0, 1), //texture2d

    FColor(0, 0, 0, 1), //texturecubemap

    FColor(0, 0, 0, 1), //texture_terrain_diffuse
    FColor(0, 0, 0, 1), //texture_terrain_normal
    FColor(0, 0, 0, 1), //texture_terrain_control

    FColor(0, 0, 0, 1), //texture_rt_compute_copy_tex
    FColor(0, 0, 0, 1), //texture_rt_compute_copy_texarray

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
     64,     64,    1, //default_white
    512,    512,    1, //default_blackwhite_01
    512,    512,    1, //bricks_diffuse
    512,    512,    1, //terrain
    512,    512,    1, //texture2d

    512,    512,    1, //texturecubemap

   1024,   1024,    1, //texture_terrain_diffuse
   1024,   1024,    1, //texture_terrain_normal
    512,    512,    1, //texture_terrain_control

   1024,   1024,    1, //texture_rt_compute_copy_tex
   2048,   2048,    1, //texture_rt_compute_copy_texarray

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //default_white
    0,    0, //default_blackwhite_01
    0,    0, //bricks_diffuse
    0,    0, //terrain
    0,    0, //texture2d

    0,    0, //texturecubemap

    0,    0, //texture_terrain_diffuse
    0,    0, //texture_terrain_normal
    0,    0, //texture_terrain_control

    0,    0, //texture_rt_compute_copy_tex
    0,    0, //texture_rt_compute_copy_texarray

};


/////////////////////////// DescriptorSetLayout /////////////////
static const int g_DescriptorSetLayoutCount = 7;
static const char* g_nameDescriptorSetLayouts[g_DescriptorSetLayoutCount] =
{
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS-TextureFS",
    
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TessellationConstants",

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-GeometryConstants",

    "TextureCopyConstants-TextureCSR-TextureCSRW",

};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 10;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path

    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",              "standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_tex2darray_lit",                "vert",              "standard_mesh_opaque_tex2darray_lit.vert.spv", //standard_mesh_opaque_tex2darray_lit vert

    "vert_standard_terrain_opaque_lit",                        "vert",              "standard_terrain_opaque_lit.vert.spv", //standard_terrain_opaque_lit vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////

    ///////////////////////////////////////// tese /////////////////////////////////////////

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_tex2darray_lit",                "frag",              "standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit frag

    "frag_standard_terrain_opaque_lit",                        "frag",              "standard_terrain_opaque_lit.frag.spv", //standard_terrain_opaque_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////
    "comp_standard_compute_texcopy_tex2d",                     "comp",              "standard_compute_texcopy_tex2d.comp.spv", //standard_compute_texcopy_tex2d comp
    "comp_standard_compute_texcopy_tex2darray",                "comp",              "standard_compute_texcopy_tex2darray.comp.spv", //standard_compute_texcopy_tex2darray comp

};


/////////////////////////// Object //////////////////////////////
static const int g_ObjectCount = 15;
static const char* g_ObjectConfigs[5 * g_ObjectCount] = 
{
    //Object Name                               //Mesh Name         //Texture VS            //Texture FS                                                                    //Texture CS
    "textureCubeMap_SkyBox",                    "cube",             "",                     "texturecubemap",                                                               "", //textureCubeMap_SkyBox
    "texture2Darray_TerrainDiffuse",            "plane",            "",                     "texture_terrain_diffuse",                                                      "", //texture2Darray_TerrainDiffuse
    "texture2Darray_TerrainNormal",             "plane",            "",                     "texture_terrain_normal",                                                       "", //texture2Darray_TerrainNormal
    "texture2Darray_TerrainControl",            "plane",            "",                     "texture_terrain_control",                                                      "", //texture2Darray_TerrainControl

    "compute_CopyTexture",                      "plane",            "",                     "texture_rt_compute_copy_tex",                                                  "default_blackwhite_01;texture_rt_compute_copy_tex", //compute_CopyTexture
    "compute_CopyTextureArray",                 "plane",            "",                     "texture_rt_compute_copy_texarray",                                             "texture_terrain_diffuse;texture_rt_compute_copy_texarray", //compute_CopyTextureArray

    "tessellation_passthrough",                 "plane",            "",                     "bricks_diffuse",                                                               "", //tessellation_passthrough
    "tessellation_triangle_integer",            "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_integer
    "tessellation_triangle_fractional_even",    "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_fractional_even
    "tessellation_triangle_fractional_odd",     "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_fractional_odd
    "tessellation_triangle_pow2",               "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_pow2
    "tessellation_pntriangles",                 "plane",            "",                     "bricks_diffuse",                                                               "", //tessellation_pntriangles

    "geometry_show",                            "bunny",            "",                     "default_white",                                                                "", //geometry_show 
    "geometry_normal",                          "bunny",            "",                     "",                                                                             "", //geometry_normal         

    "terrain",                                  "plane",            "",                     "texture_terrain_diffuse;texture_terrain_normal;texture_terrain_control",       "", //terrain

};
static const char* g_ObjectNameShaderModules[6 * g_ObjectCount] = 
{
    //vert                                                  //tesc                                                  //tese                                          			//geom                                  //frag                                                  //comp
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_texcubemap_lit",             "", //textureCubeMap_SkyBox
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray_TerrainDiffuse
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray_TerrainNormal
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray_TerrainControl

	"vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "comp_standard_compute_texcopy_tex2d", //compute_CopyTexture
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "comp_standard_compute_texcopy_tex2darray", //compute_CopyTextureArray

    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",       			                                        "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //tessellation_passthrough
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",   			                                            "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //tessellation_triangle_integer
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                                         "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //tessellation_triangle_fractional_even
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                                         "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //tessellation_triangle_fractional_odd
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",          		                                        "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //tessellation_triangle_pow2
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",       			                                        "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //tessellation_pntriangles

    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             			"",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //geometry_show
    
	"vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             			"",        								"frag_standard_mesh_opaque_tex2d_lit",                  "", //geometry_normal

    "vert_standard_terrain_opaque_lit",                     "",                                                     "",                                             			"",                                     "frag_standard_terrain_opaque_lit",                     "", //terrain
    
};
static const char* g_ObjectNameDescriptorSetLayouts[2 * g_ObjectCount] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //textureCubeMap_SkyBox
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //texture2Darray_TerrainDiffuse
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //texture2Darray_TerrainNormal
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //texture2Darray_TerrainControl

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "TextureCopyConstants-TextureCSR-TextureCSRW", //compute_CopyTexture
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "TextureCopyConstants-TextureCSR-TextureCSRW", //compute_CopyTextureArray

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //tessellation_passthrough
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //tessellation_triangle_integer
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //tessellation_triangle_fractional_even
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //tessellation_triangle_fractional_odd
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //tessellation_triangle_pow2
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //tessellation_pntriangles

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                          "", //geometry_show
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-GeometryConstants",                  "", //geometry_normal

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS-TextureFS",      "", //terrain

};
static float g_instanceGap = 1.2f;
static int g_ObjectInstanceExtCount[g_ObjectCount] =
{
    0, //textureCubeMap_SkyBox
    0, //texture2Darray_TerrainDiffuse 
    0, //texture2Darray_TerrainNormal 
    0, //texture2Darray_TerrainControl 

    0, //compute_CopyTexture 
    0, //compute_CopyTextureArray 

    0, //tessellation_passthrough 
    0, //tessellation_triangle_integer 
    0, //tessellation_triangle_fractional_even 
    0, //tessellation_triangle_fractional_odd 
    0, //tessellation_triangle_pow2 
    0, //tessellation_pntriangles 

    0, //geometry_show 
    0, //geometry_normal 

    0, //terrain 

};
static FVector3 g_ObjectTranforms[3 * g_ObjectCount] = 
{   
    FVector3(   0,    0,   0),     FVector3(     0,  0,  0),    FVector3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    FVector3(-2.0,  1.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainDiffuse
    FVector3(   0,  1.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainNormal
    FVector3( 2.0,  1.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainControl

    FVector3(   0,  2.2,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //compute_CopyTexture
    FVector3(   0,  3.4,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //compute_CopyTextureArray

    FVector3(   0,  4.6,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //tessellation_passthrough
    FVector3(-2.0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_integer
    FVector3(-1.0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_fractional_even
    FVector3(   0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_fractional_odd
    FVector3( 1.0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_pow2
    FVector3(   0,  7.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //tessellation_pntriangles

    FVector3(   0,   0,  -10),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //geometry_show
    FVector3(   0,   0,  -10),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //geometry_normal

    FVector3(   0, -0.1,   0),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //terrain

};
static bool g_ObjectIsTransparents[g_ObjectCount] = 
{
    false, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    false, //tessellation_passthrough
    false, //tessellation_triangle_integer
    false, //tessellation_triangle_fractional_even
    false, //tessellation_triangle_fractional_odd
    false, //tessellation_triangle_pow2
    false, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    false, //terrain

};
static bool g_ObjectIsShows[] = 
{
    true, //textureCubeMap_SkyBox
    true, //texture2Darray_TerrainDiffuse
    true, //texture2Darray_TerrainNormal
    true, //texture2Darray_TerrainControl

    true, //compute_CopyTexture
    true, //compute_CopyTextureArray

    true, //tessellation_passthrough
    true, //tessellation_triangle_integer
    true, //tessellation_triangle_fractional_even
    true, //tessellation_triangle_fractional_odd
    true, //tessellation_triangle_pow2
    true, //tessellation_pntriangles

    true, //geometry_show
    true, //geometry_normal

    true, //terrain

};
static GLenum g_ObjectTypeCulling[] = 
{
    GL_FRONT, //textureCubeMap_SkyBox
	GL_BACK, //texture2Darray_TerrainDiffuse
    GL_BACK, //texture2Darray_TerrainNormal
    GL_BACK, //texture2Darray_TerrainControl

	GL_BACK, //compute_CopyTexture
    GL_BACK, //compute_CopyTextureArray

	GL_BACK, //tessellation_passthrough
    GL_BACK, //tessellation_triangle_integer
    GL_BACK, //tessellation_triangle_fractional_even
    GL_BACK, //tessellation_triangle_fractional_odd
    GL_BACK, //tessellation_triangle_pow2
    GL_BACK, //tessellation_pntriangles

	GL_BACK, //geometry_show
    GL_BACK, //geometry_normal

    GL_BACK, //terrain

};
static bool g_ObjectIsRotates[g_ObjectCount] =
{
    false, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    false, //tessellation_passthrough
    false, //tessellation_triangle_integer
    false, //tessellation_triangle_fractional_even
    false, //tessellation_triangle_fractional_odd
    false, //tessellation_triangle_pow2
    false, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    false, //terrain

};
static bool g_ObjectIsLightings[g_ObjectCount] =
{
    false, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    true, //tessellation_passthrough
    true, //tessellation_triangle_integer
    true, //tessellation_triangle_fractional_even
    true, //tessellation_triangle_fractional_odd
    true, //tessellation_triangle_pow2
    true, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    false, //terrain

};


/////////////////////////// ModelMesh ///////////////////////////
bool OpenGLES_012_Shadering::ModelMesh::LoadMesh(bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load
    FMeshDataPC meshDataPC;
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    if (this->typeMesh == F_Mesh_File)
    {
        unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
        if (!FMeshDataLoader::LoadMeshData(this->pathMesh, meshData, eMeshParserFlags))
        {
            F_LogError("*********************** OpenGLES_012_Shadering::ModelMesh::LoadMesh: load mesh failed: [%s] !", this->pathMesh.c_str());
            return false; 
        }
    }
    else if (this->typeMesh == F_Mesh_Geometry)
    {
        if (!FMeshGeometry::CreateGeometry(&meshDataPC, &meshData, this->typeGeometryType))
        {
            F_LogError("*********************** OpenGLES_012_Shadering::ModelMesh::LoadMesh: create geometry mesh failed: typeGeometry: [%s] !", F_GetMeshGeometryTypeName(this->typeGeometryType).c_str());
            return false; 
        }
    }
    else
    {
        F_Assert(false && "OpenGLES_012_Shadering::ModelMesh::LoadMesh: Wrong typeMesh !")
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

        F_LogInfo("OpenGLES_012_Shadering::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
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

        F_LogInfo("OpenGLES_012_Shadering::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
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
			F_LogError("*********************** OpenGLES_012_Shadering::loadModel_VertexIndex: create buffer vertex index failed: [%s] !", this->nameMesh.c_str());
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
			F_LogError("*********************** OpenGLES_012_Shadering::loadModel_VertexIndex: create buffer vertex failed: [%s] !", this->nameMesh.c_str());
			return false;
		}
	}

    return true;
}



/////////////////////////// ModelObject /////////////////////////



OpenGLES_012_Shadering::OpenGLES_012_Shadering(String name)
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

    this->cfg_cameraPos = FVector3(-2.5f, 2.0f, -20.0f);
    this->cfg_cameraLookTarget = FVector3(-2.5f, 5.0f, 0.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector4(0, -1, 0, 0); //y-
}

void OpenGLES_012_Shadering::createCamera()
{
    OpenGLESWindow::createCamera();
    
    cameraReset();
}
void OpenGLES_012_Shadering::cameraReset()
{
    OpenGLESWindow::cameraReset();
}

void OpenGLES_012_Shadering::loadModel_Custom()
{
	createMeshes();
    createTextures();
    createShaderModules();
    createDescriptorSetLayouts();

    for (int i = 0; i < g_ObjectCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->indexModel = i;
        pModelObject->nameObject = g_ObjectConfigs[5 * i + 0];
        pModelObject->nameMesh = g_ObjectConfigs[5 * i + 1];

        //Mesh
        {
            ModelMesh* pMesh = this->findMesh(pModelObject->nameMesh);
            if (pMesh == nullptr)
            {
                F_LogError("*********************** OpenGLES_012_Shadering::loadModel_Custom: can not find mesh: [%s] !", pModelObject->nameMesh.c_str());
            }
            F_Assert(pMesh != nullptr && "OpenGLES_012_Shadering::loadModel_Custom")
            pModelObject->SetMesh(pMesh);
        }

        //Texture VS
        {
            String nameTextureVS = g_ObjectConfigs[5 * i + 2]; //Texture VS
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
            String nameTextureFS = g_ObjectConfigs[5 * i + 3]; //Texture FS
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
            String nameTextureCS = g_ObjectConfigs[5 * i + 4]; //Texture CS
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

        //Compute
        String nameShaderComp = g_ObjectNameShaderModules[6 * i + 5];
        if (!nameShaderComp.empty())
        {
			pModelObject->isUsedCompute = true;
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

void OpenGLES_012_Shadering::createCustomCB()
{
	rebuildInstanceCBs(true);
}
void OpenGLES_012_Shadering::rebuildInstanceCBs(bool isCreateBuffer)
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
            {
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectTranforms[i * 3 + 0] + FVector3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                            g_ObjectTranforms[i * 3 + 1],
                                                            g_ObjectTranforms[i * 3 + 2]);
                pModelObject->objectCBs[j] = objectConstants;
                pModelObject->instanceMatWorld[j] = objectConstants.g_MatWorld;
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = FMath::RandomColor(false);
                materialConstants.factorDiffuse = FMath::RandomColor(false);
                materialConstants.factorSpecular = FMath::RandomColor(false);
                materialConstants.lighting.x = FMath::RandF(10.0f, 100.0f);
                materialConstants.lighting.y = FMath::RandF(0.2f, 0.9f);
                materialConstants.lighting.z = g_ObjectIsLightings[i] ? 1.0f : 0.0f;
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
				String msg = "*********************** OpenGLES_012_Shadering::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
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
                String msg = "*********************** OpenGLES_012_Shadering::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }

            //TextureCopyConstants
            if (pModelObject->isUsedCompute)
            {
                pModelObject->textureCopyCBs.clear();
                TextureCopyConstants textureCopyCB;
                pModelObject->textureCopyCBs.push_back(textureCopyCB);

                F_DELETE(pModelObject->poBufferUniform_TextureCopy)
                nameBuffer = "TextureCopyConstants-" + pModelObject->nameObject;
                pModelObject->poBufferUniform_TextureCopy = createBufferUniform(nameBuffer,
																				DescriptorSet_TextureCopyConstants,
																				GL_DYNAMIC_DRAW,
                                                                                sizeof(TextureCopyConstants) * pModelObject->textureCopyCBs.size(),
                                                                                (uint8*)pModelObject->textureCopyCBs.data(),
                                                                                false);
                if (!pModelObject->poBufferUniform_TextureCopy)
                {
                    String msg = "*********************** OpenGLES_012_Shadering::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
		}
    }
}

void OpenGLES_012_Shadering::createCustomBeforePipeline()
{
    
}  
void OpenGLES_012_Shadering::createGraphicsPipeline_Custom()
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
                                                                            pModelObject->pMesh->poTypeVertex,
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
            String msg = "*********************** OpenGLES_012_Shadering::createGraphicsPipeline_Custom: Failed to create pipeline !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
}

void OpenGLES_012_Shadering::createComputePipeline_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        
        String nameShaderComp = g_ObjectNameShaderModules[6 * i + 5];
        if (nameShaderComp.empty())
            continue;

        StringVector aShaderComps = FUtilString::Split(nameShaderComp, ";");
        StringVector aDSLs = FUtilString::Split(g_ObjectNameDescriptorSetLayouts[2 * i + 1], ";");
        F_Assert(aShaderComps.size() == aDSLs.size() && "OpenGLES_012_Shadering::createComputePipeline_Custom")

        size_t count_cs = aShaderComps.size();
        for (size_t j = 0; j < count_cs; j++)
        {
            const String& nameCS = aShaderComps[j];
            GLESShader* pShaderCS = findShaderModule(nameCS);
            const String& nameDSL = aDSLs[j];
            DescriptorSetLayout* pDSL = findDescriptorSetLayout(nameDSL);

            String nameStateCompute = "PipelineCompute-" + pModelObject->nameObject + "-" + FUtilString::SaveSizeT(j);
            GLESStatePipelineCompute* pStatePipelineCompute = createStatePipelineCompute(nameStateCompute,
                                                                                         pDSL,
                                                                                         pShaderCS);
            if (pStatePipelineCompute == nullptr)
            {
                String msg = "*********************** OpenGLES_012_Shadering::createComputePipeline_Custom: Failed to create pipeline compute !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->AddPipelineCompute(pStatePipelineCompute);
            
        }
    }
}

void OpenGLES_012_Shadering::destroyMeshes()
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
void OpenGLES_012_Shadering::createMeshes()
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
            String msg = "*********************** OpenGLES_012_Shadering::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("OpenGLES_012_Shadering::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), pathMesh.c_str());
    }
}
OpenGLES_012_Shadering::ModelMesh* OpenGLES_012_Shadering::findMesh(const String& nameMesh)
{
    ModelMeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_012_Shadering::destroyTextures()
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
void OpenGLES_012_Shadering::createTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        String nameTexture = g_TexturePaths[5 * i + 0];
        String nameType = g_TexturePaths[5 * i + 1];
        FTextureType typeTexture = F_ParseTextureType(nameType);
        String nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
        String nameIsUnOrderedAccess = g_TexturePaths[5 * i + 3];
        bool isUnOrderedAccess = FUtilString::ParserBool(nameIsUnOrderedAccess);
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
												isUnOrderedAccess,
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

        F_LogInfo("OpenGLES_012_Shadering::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
GLESTexture* OpenGLES_012_Shadering::findTexture(const String& nameTexture)
{
    GLESTexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_012_Shadering::destroyShaderModules()
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
void OpenGLES_012_Shadering::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = getShaderPathRelative(g_ShaderModulePaths[3 * i + 2]);

        GLESShader* pShader =  createShader(shaderName, shaderPath, shaderType);
        if (pShader == nullptr)
        {
            String msg = "*********************** OpenGLES_012_Shadering::createShaderModules: Failed to create shader: shaderPath: " + shaderPath;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShader);
        this->m_mapGLESShaderModules[shaderName] = pShader;
        F_LogInfo("OpenGLES_012_Shadering::createShaderModules: create shader [%s] success !", shaderPath.c_str());
    }
}
GLESShader* OpenGLES_012_Shadering::findShaderModule(const String& pathShaderModule)
{
    GLESShaderPtrMap::iterator itFind = this->m_mapGLESShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapGLESShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_012_Shadering::destroyDescriptorSetLayouts()
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
void OpenGLES_012_Shadering::createDescriptorSetLayouts()
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
DescriptorSetLayout* OpenGLES_012_Shadering::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    DescriptorSetLayoutPtrMap::iterator itFind = this->m_mapDescriptorSetLayouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapDescriptorSetLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void OpenGLES_012_Shadering::createDescriptorSets_Custom()
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
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TessellationConstants)) //TessellationConstants
                {

                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_GeometryConstants)) //GeometryConstants
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
                    String msg = "*********************** OpenGLES_012_Shadering::createDescriptorSets_Custom: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
		}

        //Pipeline Computes
        if (pModelObject->isUsedCompute)
		{
			size_t count_comp = pModelObject->aPipelineComputes.size();
            for (int j = 0; j < count_comp; j++)
            {
                GLESStatePipelineCompute* pStatePipelineCompute = pModelObject->aPipelineComputes[j];

                int nIndexTextureCS = 0;
                size_t count_names = pStatePipelineCompute->poDescriptorSetLayout->aLayouts.size();
                for (size_t p = 0; p < count_names; p++)
                {
                    String& nameDescriptorSet = pStatePipelineCompute->poDescriptorSetLayout->aLayouts[p];
                    int nBindingIndex = (int)p;

                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureCopyConstants)) //TextureCopyConstants
                    {
						uint32 nBindingIndex = (uint32)DescriptorSet_TextureCopyConstants;
						uint32 nUniformBlockIndex = pStatePipelineCompute->GetUniformBlockIndex(nameDescriptorSet);
						pStatePipelineCompute->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
						pStatePipelineCompute->BindBufferUniform(pModelObject->poBufferUniform_TextureCopy, nBindingIndex);
                    }   
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureCSR)) //TextureCSR
                    {
						GLESTexture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
						pStatePipelineCompute->BindTextureCS(pTexture, nIndexTextureCS);
						nIndexTextureCS ++;
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureCSRW)) //TextureCSRW
                    {
						GLESTexture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
						pStatePipelineCompute->BindTextureImageCS(pTexture, nIndexTextureCS);
						nIndexTextureCS ++;
                    }
                    else
                    {
                        String msg = "*********************** OpenGLES_012_Shadering::createDescriptorSets_Custom: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                }
            }
		}
    }
}

void OpenGLES_012_Shadering::updateCompute_BeforeRender_Custom()
{
	size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
	{
		ModelObject* pModelObject = this->m_aModelObjects[i];
        if (!pModelObject->isUsedCompute)
            continue;

		size_t count_comp = pModelObject->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
		{
			GLESStatePipelineCompute* pStatePipelineCompute = pModelObject->aPipelineComputes[j];

            bool isRand = false;
            if (++pModelObject->frameRand > 15)
            {
                isRand = true;
                pModelObject->frameRand = 0;
            }
            GLESTexture* pTextureSrc = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Compute), 0);
            GLESTexture* pTextureTarget = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Compute), 1);
            TextureCopyConstants& textureCopyCB = pModelObject->textureCopyCBs[0];
            textureCopyCB.texInfo.x = (float)pTextureSrc->width;
            textureCopyCB.texInfo.y = (float)pTextureSrc->height;
            textureCopyCB.texInfo.z = 0;
            textureCopyCB.texInfo.w = 0;
            if (isRand)
            {
                textureCopyCB.texOffset.x = (FMath::RandF(0, 1) >= 0.5f ? 1.0f : 0.0f) * (float)pTextureSrc->width;
                textureCopyCB.texOffset.y = (FMath::RandF(0, 1) >= 0.5f ? 1.0f : 0.0f) * (float)pTextureSrc->height;
                textureCopyCB.texOffset.z = 0;
                textureCopyCB.texOffset.w = 0;

                int seed = FMath::Rand(0, 10000);
                int start = seed % 4;
                textureCopyCB.texIndexArray.x = (float)start;
                textureCopyCB.texIndexArray.y = (float)(++start % 4);
                textureCopyCB.texIndexArray.z = (float)(++start % 4);
                textureCopyCB.texIndexArray.w = (float)(++start % 4);
            }
            textureCopyCB.texClearColor.x = 0;
            textureCopyCB.texClearColor.y = 0;
            textureCopyCB.texClearColor.z = 0;
            textureCopyCB.texClearColor.w = 1;

            pModelObject->poBufferUniform_TextureCopy->UpdateBuffer(sizeof(TextureCopyConstants) * pModelObject->textureCopyCBs.size(),
                                                                    (uint8*)pModelObject->textureCopyCBs.data(),
                                                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

            pStatePipelineCompute->BindState();
            pStatePipelineCompute->BindShader();
            pStatePipelineCompute->BindBufferUniforms();
            pStatePipelineCompute->BindTextures();
            
            uint32_t groupX = (uint32_t)(pTextureTarget->width / 8);
            uint32_t groupY = (uint32_t)(pTextureTarget->height / 8);
            dispatch(groupX, groupY, 1);

            pStatePipelineCompute->UnBindState();
		}
	}
}

void OpenGLES_012_Shadering::updateCBs_Custom()
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



bool OpenGLES_012_Shadering::beginRenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("OpenGLES_012_Shadering", &windowOpened, 0);
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
void OpenGLES_012_Shadering::modelConfig()
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
                                                    if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].texSize.w, 0.001f, 0.0f, 1.0f))
                                                    {

                                                    }
                                                }

                                                //texSpeedU
                                                String nameTexSpeedU = "TexSpeedU - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeed.x, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }
                                                
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

void OpenGLES_012_Shadering::endRenderImgui()
{
    OpenGLESWindow::endRenderImgui();

}

void OpenGLES_012_Shadering::drawMeshDefault_Custom()
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
			F_Assert(false && "OpenGLES_012_Shadering::drawMeshDefault_Custom")
		}
        pModelObject->poStatePipelineGraphics->UnBindState();
    }
}

void OpenGLES_012_Shadering::cleanupCustom()
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

void OpenGLES_012_Shadering::cleanupSwapChain_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }
}

void OpenGLES_012_Shadering::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}