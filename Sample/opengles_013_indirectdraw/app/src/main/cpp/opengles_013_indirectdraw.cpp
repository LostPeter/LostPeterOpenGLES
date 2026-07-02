/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-02
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_013_indirectdraw.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 10;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/plane.fbx", //plane
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Common/sphere.fbx", //sphere

    "mountain",         "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Mesh/Model/mountain/mountain.obj", //mountain

    "rock",             "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Mesh/Model/rock/rock.fbx", //rock
    "cliff",            "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Mesh/Model/cliff/cliff.obj", //cliff

    "tree",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Model/tree/tree.fbx", //tree
    "tree_spruce",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Model/tree_spruce/tree_spruce.fbx", //tree_spruce

    "grass",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Model/grass/grass.fbx", //grass
    "flower",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Mesh/Model/flower/flower.fbx", //flower

};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //plane
    false, //cube
    false, //sphere

    false, //mountain

    false, //rock
    false, //cliff

    false, //tree
    false, //tree_spruce

    false, //grass
    false, //flower

};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //plane  
    false, //cube
    false, //sphere

    false, //mountain

    false, //rock
    false, //cliff

    false, //tree
    false, //tree_spruce

    false, //grass
    false, //flower
    
};
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //cube
    FMath::ms_mat4Unit, //sphere

    FMath::ms_mat4Unit, //mountain

    FMath::ms_mat4Unit, //rock
    FMath::ms_mat4Unit, //cliff

    FMath::ms_mat4Unit, //tree
    FMath::ms_mat4Unit, //tree_spruce

    FMath::ms_mat4Unit, //grass
    FMath::ms_mat4Unit, //flower

};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 24;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type      //TextureIsRenderTarget     //TextureIsUnOrderedAccess          //Texture Path
    "default_blackwhite",               "2D",            	"false",                  	"false",                           	"Texture/Common/default_blackwhite.png", //default_blackwhite
    "bricks_diffuse",                   "2D",            	"false",                  	"false",                           	"Texture/Common/bricks_diffuse.png", //bricks_diffuse
    "terrain",                          "2D",            	"false",                  	"false",                           	"Texture/Common/terrain.png", //terrain
    "texture2d",                        "2D",            	"false",                  	"false",                           	"Texture/Common/texture2d.jpg", //texture2d
    
    "texturecubemap",                   "CubeMap",      	"false",                  	"false",                           	"Texture/Sky/texturecubemap_x_right.png;Texture/Sky/texturecubemap_x_left.png;Texture/Sky/texturecubemap_y_up.png;Texture/Sky/texturecubemap_y_down.png;Texture/Sky/texturecubemap_z_front.png;Texture/Sky/texturecubemap_z_back.png", //texturecubemap

    "texture_terrain_diffuse",          "2DArray",       	"false",                  	"false",                           	"Texture/Terrain/shore_sand_albedo.png;Texture/Terrain/moss_albedo.png;Texture/Terrain/rock_cliff_albedo.png;Texture/Terrain/cliff_albedo.png", //texture_terrain_diffuse
    "texture_terrain_normal",           "2DArray",       	"false",                  	"false",                           	"Texture/Terrain/shore_sand_norm.png;Texture/Terrain/moss_norm.tga;Texture/Terrain/rock_cliff_norm.tga;Texture/Terrain/cliff_norm.png", //texture_terrain_normal
    "texture_terrain_control",          "2DArray",       	"false",                  	"false",                           	"Texture/Terrain/terrain_control.png", //texture_terrain_control

    "mountain_diffuse",                 "2D",           	"false",                  	"false",                           	"Texture/Model/mountain/mountain_diffuse.png", //mountain_diffuse
    "mountain_normal",                  "2D",            	"false",                  	"false",                           	"Texture/Model/mountain/mountain_normal.png", //mountain_normal

    "rock_diffuse",                     "2D",            	"false",                  	"false",                           	"Texture/Model/rock/rock_diffuse.png", //rock_diffuse
    "rock_normal",                      "2D",            	"false",                  	"false",                           	"Texture/Model/rock/rock_normal.png", //rock_normal
    "cliff_diffuse",                    "2D",            	"false",                  	"false",                           	"Texture/Model/cliff/cliff_diffuse.png", //cliff_diffuse
    "cliff_normal",                     "2D",            	"false",                  	"false",                           	"Texture/Model/cliff/cliff_normal.png", //cliff_normal

    "tree_diffuse",                     "2D",            	"false",                  	"false",                           	"Texture/Model/tree/tree_diffuse.png", //tree_diffuse
    "tree_spruce_diffuse",              "2D",            	"false",                  	"false",                           	"Texture/Model/tree_spruce/tree_spruce_diffuse.png", //tree_spruce_diffuse

    "grass_alien",                      "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_alien.png", //grass_alien
    "grass_field",                      "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_field.png", //grass_field
    "grass_pixelated",                  "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_pixelated.png", //grass_pixelated
    "grass_tall",                       "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_tall.png", //grass_tall
    "grass_thick",                      "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_thick.png", //grass_thick
    "grass_thin",                       "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_thin.png", //grass_thin
    "grass_wheat",                      "2D",            	"false",                  	"false",                           	"Texture/Model/grass/grass_wheat.png", //grass_wheat
    
    "flower_atlas",                     "2D",            	"false",                  	"false",                           	"Texture/Model/flower/flower_atlas.png", //flower_atlas

};
static int g_TextureChannels[g_TextureCount] = 
{
    4, //default_blackwhite
    4, //bricks_diffuse
    4, //terrain
    4, //texture2d

    4, //texturecubemap

    4, //texture_terrain_diffuse
    4, //texture_terrain_normal
    4, //texture_terrain_control

    4, //mountain_diffuse
    4, //mountain_normal

    4, //rock_diffuse
    4, //rock_normal
    4, //cliff_diffuse
    4, //cliff_normal

    4, //tree_diffuse
    4, //tree_spruce_diffuse

    4, //grass_alien
    4, //grass_field
    4, //grass_pixelated
    4, //grass_tall
    4, //grass_thick
    4, //grass_thin
    4, //grass_wheat

    4, //flower_atlas

};
static FTexturePixelFormatType g_TextureFormats[g_TextureCount] = 
{
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_blackwhite
    F_TexturePixelFormat_R8G8B8A8_SRGB, //bricks_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //terrain
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d

    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturecubemap

    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture_terrain_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture_terrain_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture_terrain_control

    F_TexturePixelFormat_R8G8B8A8_SRGB, //mountain_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //mountain_normal

    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //cliff_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //cliff_normal

    F_TexturePixelFormat_R8G8B8A8_SRGB, //tree_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //tree_spruce_diffuse

    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_alien
    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_field
    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_pixelated
    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_tall
    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_thick
    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_thin
    F_TexturePixelFormat_R8G8B8A8_SRGB, //grass_wheat

    F_TexturePixelFormat_R8G8B8A8_SRGB, //flower_atlas

};
static FTextureFilterType g_TextureFilters[2 * g_TextureCount] = 
{
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_blackwhite
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //bricks_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //terrain
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture2d

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texturecubemap

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_terrain_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_terrain_normal
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture_terrain_control

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //mountain_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //mountain_normal

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //rock_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //rock_normal
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //cliff_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //cliff_normal

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //tree_diffuse
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //tree_spruce_diffuse

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_alien
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_field
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_pixelated
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_tall
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_thick
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_thin
    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //grass_wheat

    F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //flower_atlas

};
static FTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    F_TextureAddressing_Clamp, //default_blackwhite
    F_TextureAddressing_Clamp, //bricks_diffuse
    F_TextureAddressing_Clamp, //terrain
    F_TextureAddressing_Clamp, //texture2d

    F_TextureAddressing_Wrap, //texturecubemap

    F_TextureAddressing_Clamp, //texture_terrain_diffuse
    F_TextureAddressing_Clamp, //texture_terrain_normal
    F_TextureAddressing_Clamp, //texture_terrain_control

    F_TextureAddressing_Clamp, //mountain_diffuse
    F_TextureAddressing_Clamp, //mountain_normal

    F_TextureAddressing_Clamp, //rock_diffuse
    F_TextureAddressing_Clamp, //rock_normal
    F_TextureAddressing_Clamp, //cliff_diffuse
    F_TextureAddressing_Clamp, //cliff_normal

    F_TextureAddressing_Clamp, //tree_diffuse
    F_TextureAddressing_Clamp, //tree_spruce_diffuse

    F_TextureAddressing_Clamp, //grass_alien
    F_TextureAddressing_Clamp, //grass_field
    F_TextureAddressing_Clamp, //grass_pixelated
    F_TextureAddressing_Clamp, //grass_tall
    F_TextureAddressing_Clamp, //grass_thick
    F_TextureAddressing_Clamp, //grass_thin
    F_TextureAddressing_Clamp, //grass_wheat

    F_TextureAddressing_Clamp, //flower_atlas

};
static FColor g_TextureBorderColors[g_TextureCount] = 
{
    FColor(0, 0, 0, 1), //default_blackwhite_01
    FColor(0, 0, 0, 1), //bricks_diffuse
    FColor(0, 0, 0, 1), //terrain
    FColor(0, 0, 0, 1), //texture2d

    FColor(0, 0, 0, 1), //texturecubemap

    FColor(0, 0, 0, 1), //texture_terrain_diffuse
    FColor(0, 0, 0, 1), //texture_terrain_normal
    FColor(0, 0, 0, 1), //texture_terrain_control

    FColor(0, 0, 0, 1), //mountain_diffuse
    FColor(0, 0, 0, 1), //mountain_normal

    FColor(0, 0, 0, 1), //rock_diffuse
    FColor(0, 0, 0, 1), //rock_normal
    FColor(0, 0, 0, 1), //cliff_diffuse
    FColor(0, 0, 0, 1), //cliff_normal

    FColor(0, 0, 0, 1), //tree_diffuse
    FColor(0, 0, 0, 1), //tree_spruce_diffuse

    FColor(0, 0, 0, 1), //grass_alien
    FColor(0, 0, 0, 1), //grass_field
    FColor(0, 0, 0, 1), //grass_pixelated
    FColor(0, 0, 0, 1), //grass_tall
    FColor(0, 0, 0, 1), //grass_thick
    FColor(0, 0, 0, 1), //grass_thin
    FColor(0, 0, 0, 1), //grass_wheat

    FColor(0, 0, 0, 1), //flower_atlas

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //default_blackwhite
    512,    512,    1, //bricks_diffuse
    512,    512,    1, //terrain
    512,    512,    1, //texture2d

    512,    512,    1, //texturecubemap

   1024,   1024,    1, //texture_terrain_diffuse
   1024,   1024,    1, //texture_terrain_normal
    512,    512,    1, //texture_terrain_control

   1024,   1024,    1, //mountain_diffuse
   1024,   1024,    1, //mountain_normal

    512,    512,    1, //rock_diffuse
    512,    512,    1, //rock_normal
    512,    512,    1, //cliff_diffuse
   1024,   1024,    1, //cliff_normal

   1024,   1024,    1, //tree_diffuse
   1024,   1024,    1, //tree_spruce_diffuse

   1024,   1024,    1, //grass_alien
   1024,   1024,    1, //grass_field
    128,    128,    1, //grass_pixelated
   1024,   1024,    1, //grass_tall
   1024,   1024,    1, //grass_thick
   1024,   1024,    1, //grass_thin
    128,    512,    1, //grass_wheat

   1024,   1024,    1, //flower_atlas

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //default_blackwhite
    0,    0, //bricks_diffuse
    0,    0, //terrain
    0,    0, //texture2d

    0,    0, //texturecubemap

    0,    0, //texture_terrain_diffuse
    0,    0, //texture_terrain_normal
    0,    0, //texture_terrain_control

    0,    0, //mountain_diffuse
    0,    0, //mountain_normal

    0,    0, //rock_diffuse
    0,    0, //rock_normal
    0,    0, //cliff_diffuse
    0,    0, //cliff_normal

    0,    0, //tree_diffuse
    0,    0, //tree_spruce_diffuse

    0,    0, //grass_alien
    0,    0, //grass_field
    0,    0, //grass_pixelated
    0,    0, //grass_tall
    0,    0, //grass_thick
    0,    0, //grass_thin
    0,    0, //grass_wheat

    0,    0, //flower_atlas

};


/////////////////////////// DescriptorSetLayout /////////////////
static const int g_DescriptorSetLayoutCount = 4;
static const char* g_nameDescriptorSetLayouts[g_DescriptorSetLayoutCount] =
{
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",
	"PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS-TextureFS",

};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 20;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
	//name                                                     //type               //path

    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_transparent_lit",                      "vert",              "standard_mesh_transparent_lit.vert.spv", //standard_mesh_transparent_lit vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",              "standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_tex2darray_lit",                "vert",              "standard_mesh_opaque_tex2darray_lit.vert.spv", //standard_mesh_opaque_tex2darray_lit vert
    
    "vert_standard_terrain_opaque_lit",                        "vert",              "standard_terrain_opaque_lit.vert.spv", //standard_terrain_opaque_lit vert

    "vert_standard_mesh_opaque_normalmap_lit",                 "vert",              "standard_mesh_opaque_normalmap_lit.vert.spv", //standard_mesh_opaque_normalmap_lit vert
    "vert_standard_mesh_transparent_tree_lit",                 "vert",              "standard_mesh_transparent_tree_lit.vert.spv", //standard_mesh_transparent_tree_lit vert  
    "vert_standard_mesh_opaque_tree_alphatest_lit",            "vert",              "standard_mesh_opaque_tree_alphatest_lit.vert.spv", //standard_mesh_opaque_tree_alphatest_lit vert
    "vert_standard_mesh_opaque_grass_alphatest_lit",           "vert",              "standard_mesh_opaque_grass_alphatest_lit.vert.spv", //standard_mesh_opaque_grass_alphatest_lit vert  
    "vert_standard_mesh_opaque_flower_alphatest_lit",          "vert",              "standard_mesh_opaque_flower_alphatest_lit.vert.spv", //standard_mesh_opaque_flower_alphatest_lit vert  

    ///////////////////////////////////////// tesc /////////////////////////////////////////
    
    ///////////////////////////////////////// tese /////////////////////////////////////////
    
    ///////////////////////////////////////// geom /////////////////////////////////////////


    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_transparent_lit",                      "frag",              "standard_mesh_transparent_lit.frag.spv", //standard_mesh_transparent_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_tex2darray_lit",                "frag",              "standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit frag

    "frag_standard_terrain_opaque_lit",                        "frag",              "standard_terrain_opaque_lit.frag.spv", //standard_terrain_opaque_lit frag

    "frag_standard_mesh_opaque_normalmap_lit",                 "frag",              "standard_mesh_opaque_normalmap_lit.frag.spv", //standard_mesh_opaque_normalmap_lit frag
    "frag_standard_mesh_transparent_tree_lit",                 "frag",              "standard_mesh_transparent_tree_lit.frag.spv", //standard_mesh_transparent_tree_lit frag
    "frag_standard_mesh_opaque_tree_alphatest_lit",            "frag",              "standard_mesh_opaque_tree_alphatest_lit.frag.spv", //standard_mesh_opaque_tree_alphatest_lit frag
    "frag_standard_mesh_opaque_grass_alphatest_lit",           "frag",              "standard_mesh_opaque_grass_alphatest_lit.frag.spv", //standard_mesh_opaque_grass_alphatest_lit frag
    "frag_standard_mesh_opaque_flower_alphatest_lit",          "frag",              "standard_mesh_opaque_flower_alphatest_lit.frag.spv", //standard_mesh_opaque_flower_alphatest_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////
    

};



/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 8;
static const char* g_ObjectConfigs[2 * g_Object_Count] = 
{
    //Object Name                          //Mesh Name                                                                    
    "object_skybox",                       "cube", //object_skybox
    "object_mountain",                     "mountain", //object_mountain   

    "object_rock",                         "rock", //object_rock   
    "object_cliff",                        "cliff", //object_cliff   

    "object_tree",                         "tree", //object_tree        
    "object_tree_spruce",                  "tree_spruce", //object_tree_spruce
    
    "object_grass",                        "grass", //object_grass        
    "object_flower",                       "flower", //object_flower
};
static const char* g_Object_MeshSubsUsed[g_Object_Count] =
{
    "0", //object_skybox
    "0", //object_mountain

    "0", //object_rock
    "0", //object_cliff

    "0;1", //object_tree        
    "0;1", //object_tree_spruce

    "1;4;6;9", //object_grass
    "0;2;4;6;8;9;10;11", //object_flower

};  

static float g_Object_InstanceGap = 3.0f;
static int g_Object_InstanceExtCount[g_Object_Count] =
{
    0, //object_skybox
    0, //object_mountain 

    4, //object_rock 
    4, //object_cliff 

    4, //object_tree 
    4, //object_tree_spruce 

    4, //object_grass 
    4, //object_flower 

};
static bool g_Object_IsShows[] = 
{
    true, //object_skybox
    true, //object_mountain

    true, //object_rock
    true, //object_cliff

    true, //object_tree
    true, //object_tree_spruce

    true, //object_grass
    true, //object_flower

};
static bool g_Object_IsRotates[g_Object_Count] =
{
    false, //object_skybox
    false, //object_mountain

    false, //object_rock
    false, //object_cliff

    false, //object_tree
    false, //object_tree_spruce

    false, //object_grass
    false, //object_flower

};
static bool g_Object_IsLightings[g_Object_Count] =
{
    true, //object_skybox
    true, //object_mountain

    true, //object_rock
    true, //object_cliff

    true, //object_tree
    true, //object_tree_spruce

    true, //object_grass
    true, //object_flower

};
static bool g_Object_IsIndirectDraw[g_Object_Count] =
{
    false, //object_skybox
    false, //object_mountain

    false, //object_rock
    false, //object_cliff

    false, //object_tree
    false, //object_tree_spruce

    false, //object_grass
    true, //object_flower

};


/////////////////////////// ObjectRend //////////////////////////
static const int g_ObjectRend_Count = 20;
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                     //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                    //Texture CS
    "object_skybox-1",                     "",                     "",                              "",                         "",                    "texturecubemap",                                                               "", //object_skybox-1
    "object_mountain-1",                   "",                     "",                              "",                         "",                    "mountain_diffuse;mountain_normal",                                             "", //object_mountain-1

    "object_rock-1",                       "",                     "",                              "",                         "",                    "rock_diffuse;rock_normal",                                                     "", //object_rock-1
    "object_cliff-1",                      "",                     "",                              "",                         "",                    "cliff_diffuse;cliff_normal",                                                   "", //object_cliff-1

    "object_tree-1",                       "",                     "",                              "",                         "",                    "tree_diffuse",                                                                 "", //object_tree-1
    "object_tree-2",                       "",                     "",                              "",                         "",                    "tree_diffuse",                                                                 "", //object_tree-2
    "object_tree_spruce-1",                "",                     "",                              "",                         "",                    "tree_spruce_diffuse",                                                          "", //object_tree_spruce-1
    "object_tree_spruce-2",                "",                     "",                              "",                         "",                    "tree_spruce_diffuse",                                                          "", //object_tree_spruce-2

    "object_grass-1",                      "",                     "",                              "",                         "",                    "grass_field",                                                                  "", //object_grass-1
    "object_grass-2",                      "",                     "",                              "",                         "",                    "grass_wheat",                                                                  "", //object_grass-2
    "object_grass-3",                      "",                     "",                              "",                         "",                    "grass_tall",                                                                   "", //object_grass-3
    "object_grass-4",                      "",                     "",                              "",                         "",                    "grass_field",                                                                  "", //object_grass-4

    "object_flower-1",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-1
    "object_flower-2",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-2
    "object_flower-3",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-3
    "object_flower-4",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-4
    "object_flower-5",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-5
    "object_flower-6",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-6
    "object_flower-7",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-7
    "object_flower-8",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-8

};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                                  //tesc                                          //tese                                      //geom                      //frag                                                  //comp
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                         "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //object_skybox-1
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_mountain-1
    
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_rock-1
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_cliff-1

    "vert_standard_mesh_opaque_tree_alphatest_lit",         "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tree_alphatest_lit",         "", //object_tree-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_tree-2
    "vert_standard_mesh_opaque_tree_alphatest_lit",         "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tree_alphatest_lit",         "", //object_tree_spruce-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_tree_spruce-2

    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-1
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-2
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-3
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-4

    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-1
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-2
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-3
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-4
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-5
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-6
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-7
    "vert_standard_mesh_opaque_flower_alphatest_lit",       "",                                             "",                                         "",                         "frag_standard_mesh_opaque_flower_alphatest_lit",       "", //object_flower-8

};
static const char* g_ObjectRend_NameDescriptorSetLayouts[2 * g_ObjectRend_Count] = 
{
    //Pipeline Graphics                                                                                                     //Pipeline Compute
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_skybox-1
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",                                "", //object_mountain-1

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",                                "", //object_rock-1
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS-TextureFS",                                "", //object_cliff-1

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_tree-1
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_tree-2
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_tree_spruce-1
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_tree_spruce-2

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_grass-1
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_grass-2
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_grass-3
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-TextureFS",                                          "", //object_grass-4

    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-1
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-2
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-3
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-4
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-5
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-6
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-7
    "PassConstants-ObjectConstants-MaterialConstants-InstanceConstants-ValueUIntConstants-TextureFS",                       "", //object_flower-8

};
static FVector3 g_ObjectRend_Tranforms[3 * g_ObjectRend_Count] = 
{   
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(  500.0f,    500.0f,    500.0f), //object_skybox-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_mountain-1
 
    FVector3(   0,  0.0,   1.5),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_rock-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    0.1f,      0.1f,      0.1f), //object_cliff-1

    FVector3(   0,  0.0, -10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree-1
    FVector3(   0,  0.0, -10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree-2
    FVector3(   0,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree_spruce-1
    FVector3(   0,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree_spruce-2

    FVector3(   0,  0.0,   2.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-1
    FVector3(   0,  0.0,   2.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-2
    FVector3(   0,  0.0,   5.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-3
    FVector3(   0,  0.0,   5.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-4

    FVector3(   0,  0.0,  -1.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-1
    FVector3(   0,  0.0,  -1.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-2
    FVector3(   0,  0.0,  -2.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-3
    FVector3(   0,  0.0,  -2.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-4
    FVector3(   0,  0.0,  -3.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-5
    FVector3(   0,  0.0,  -3.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-6
    FVector3(   0,  0.0,  -4.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-7
    FVector3(   0,  0.0,  -4.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-8

};
static bool g_ObjectRend_IsTransparents[g_ObjectRend_Count] = 
{
    false, //object_skybox-1
    false, //object_mountain-1

    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4

    false, //object_flower-1
    false, //object_flower-2
    false, //object_flower-3
    false, //object_flower-4
    false, //object_flower-5
    false, //object_flower-6
    false, //object_flower-7
    false, //object_flower-8

};
static GLenum g_ObjectRend_TypeCulling[] = 
{
    GL_FRONT, //object_skybox-1
    GL_BACK, //object_mountain-1

    GL_BACK, //object_rock-1
    GL_BACK, //object_cliff-1

    GL_BACK, //object_tree-1
    GL_BACK, //object_tree-2
    GL_BACK, //object_tree_spruce-1
    GL_BACK, //object_tree_spruce-2

    GL_BACK, //object_grass-1
    GL_BACK, //object_grass-2
    GL_BACK, //object_grass-3
    GL_BACK, //object_grass-4

    GL_BACK, //object_flower-1
    GL_BACK, //object_flower-2
    GL_BACK, //object_flower-3
    GL_BACK, //object_flower-4
    GL_BACK, //object_flower-5
    GL_BACK, //object_flower-6
    GL_BACK, //object_flower-7
    GL_BACK, //object_flower-8

};
static bool g_ObjectRend_IsTopologyPatchLists[g_ObjectRend_Count] =
{
    false, //object_skybox-1
    false, //object_mountain-1
    
    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4

    false, //object_flower-1
    false, //object_flower-2
    false, //object_flower-3
    false, //object_flower-4
    false, //object_flower-5
    false, //object_flower-6
    false, //object_flower-7
    false, //object_flower-8

};
static bool g_ObjectRend_IsIndirectDraw[g_ObjectRend_Count] =
{
    false, //object_skybox-1
    false, //object_mountain-1
    
    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4

    true, //object_flower-1
    true, //object_flower-2
    true, //object_flower-3
    true, //object_flower-4
    true, //object_flower-5
    true, //object_flower-6
    true, //object_flower-7
    true, //object_flower-8

};



/////////////////////////// ModelObjectRend /////////////////////


/////////////////////////// ModelObjectRendIndirect /////////////
void OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::CreatePipelineGraphics()
{
    F_Assert(this->pRend && "OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::CreatePipelineGraphics")

    String nameStatePipelineGraphics = "RendIndirect-" + this->pRend->poStatePipelineGraphics->GetName();
    this->poStatePipelineGraphics = this->pRend->poStatePipelineGraphics->NewStatePipelineGraphics(nameStatePipelineGraphics);

}
void OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends)
{
    F_Assert(_aRends.size() > 0 && "OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::SetupVertexIndexBuffer")
    this->aRends.clear();
    this->aRends = _aRends;
    this->pRend = _aRends[0];

    //1> Vertex/Index
    this->aMeshSubs.clear();
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        this->aMeshSubs.push_back(pMeshSub);
    }

    this->vertices_Pos3Color4Normal3Tex2.clear();
    this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
    this->indices.clear();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        pMeshSub->WriteVertexData(this->vertices_Pos3Color4Normal3Tex2, this->vertices_Pos3Color4Normal3Tangent3Tex2);
        pMeshSub->WriteIndexData(this->indices);
    }
    
    //Vertex
    if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
    {
        this->poTypeVertex = F_MeshVertex_Pos3Color4Normal3Tex2;
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tex2.size();
        this->poVertexBuffer_PerVertexSize = sizeof(FVertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
    }
    else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
    {
        this->poTypeVertex = F_MeshVertex_Pos3Color4Normal3Tangent3Tex2;
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        this->poVertexBuffer_PerVertexSize = sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
    }
    else
    {
        F_Assert(false && "OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::SetupVertexIndexBuffer: No vertex data !")
    }
    this->poIndexCount = this->indices.size();
    this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
    this->poIndexBuffer_Data =  &this->indices[0];

    //2> createBufferVertexIndex or createBufferVertex
    if (this->poIndexBuffer_Size > 0 &&
		this->poIndexBuffer_Data != nullptr)
	{
		this->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex("VertexIndex-" + this->nameObjectRendIndirect,
                                                                                 this->poTypeVertex,
                                                                                 this->poVertexBuffer_Size, 
                                                                                 (uint8*)this->poVertexBuffer_Data, 
                                                                                 false,
                                                                                 this->poIndexBuffer_Size, 
                                                                                 (uint8*)this->poIndexBuffer_Data, 
                                                                                 false);
		if (this->pBufferVertexIndex == nullptr)
		{
			F_LogError("*********************** OpenGLES_013_IndirectDraw::SetupVertexIndexBuffer: create buffer vertex index failed: [%s] !", this->nameObjectRendIndirect.c_str());
		}
	}
	else
	{
		this->pBufferVertex = Base::GetWindowPtr()->createBufferVertex("Vertex-" + this->nameObjectRendIndirect,
                                                                       this->poTypeVertex,
                                                                       this->poVertexBuffer_Size, 
                                                                       (uint8*)this->poVertexBuffer_Data, 
                                                                       false);
		if (this->pBufferVertex == nullptr)
		{
			F_LogError("*********************** OpenGLES_013_IndirectDraw::SetupVertexIndexBuffer: create buffer vertex failed: [%s] !", this->nameObjectRendIndirect.c_str());
		}
	}
}

void OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::SetupUniformIndirectCommandBuffer()
{
    DescriptorSetLayout* pDSL = this->pRend->pModelObject->pWindow->findDescriptorSetLayout(g_ObjectRend_NameDescriptorSetLayouts[2 * this->pRend->indexRend + 0]);
    String nameBuffer;
    //1> Uniform Buffer
    {
        //ObjectConstants
        F_DELETE(this->poBufferUniform_Object)
        nameBuffer = "ObjectConstants-RendIndirect-" + pRend->pModelObject->nameObject;
        this->poBufferUniform_Object = this->pRend->pModelObject->pWindow->createBufferUniform(nameBuffer,
                                                                                               pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants)),
																							   GL_DYNAMIC_DRAW,
                                                                                               sizeof(ObjectConstants) * this->objectCBs.size(),
                                                                                               (uint8*)(this->objectCBs.data()),
                                                                                               false);
        if (!this->poBufferUniform_Object)
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::SetupUniformIndirectCommandBuffer: create buffer uniform: [" + nameBuffer + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }

        //MaterialConstants
        F_DELETE(this->poBufferUniform_Material)
        nameBuffer = "MaterialConstants-RendIndirect-" + pRend->pModelObject->nameObject;
        this->poBufferUniform_Material = this->pRend->pModelObject->pWindow->createBufferUniform(nameBuffer,
                                                                                                 pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants)),
																								 GL_DYNAMIC_DRAW,
                                                                                                 sizeof(MaterialConstants) * this->materialCBs.size(),
                                                                                                 (uint8*)(this->materialCBs.data()),
                                                                                                 false);
        if (!this->poBufferUniform_Material)
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::SetupUniformIndirectCommandBuffer: create buffer uniform: [" + nameBuffer + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }

        //ValueMatrix4Constants
        F_DELETE(this->poBufferUniform_Offset)
        nameBuffer = "ValueUIntConstants-RendIndirect-" + pRend->pModelObject->nameObject;
        this->poBufferUniform_Offset = this->pRend->pModelObject->pWindow->createBufferUniform(nameBuffer,
                                                                                               pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_ValueUIntConstants)),
																							   GL_DYNAMIC_DRAW,
                                                                                               sizeof(ValueUIntConstants),
                                                                                               (uint8*)(&this->offsetCBs[0]),
                                                                                               false);
        if (!this->poBufferUniform_Offset)
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::SetupUniformIndirectCommandBuffer: create buffer uniform: [" + nameBuffer + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            F_DELETE(this->poBufferUniform_Tessellation)
            nameBuffer = "TessellationConstants-RendIndirect-" + pRend->pModelObject->nameObject;
            this->poBufferUniform_Tessellation = this->pRend->pModelObject->pWindow->createBufferUniform(nameBuffer,
                                                                                                         pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_TessellationConstants)),
																				 						 GL_DYNAMIC_DRAW,
                                                                                                         sizeof(TessellationConstants) * this->tessellationCBs.size(),
                                                                                                         (uint8*)(this->tessellationCBs.data()),
                                                                                                         false);
            if (!this->poBufferUniform_Tessellation)
            {
                String msg = "*********************** OpenGLES_013_IndirectDraw::SetupUniformIndirectCommandBuffer: create buffer uniform: [" + nameBuffer + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }
        }  
    }

    //2> IndirectCommand Buffer
    {
        nameBuffer = "IndirectCommandBuffer-" + pRend->pModelObject->nameObject;
        this->poBuffer_IndirectCommand = this->pRend->pModelObject->pWindow->createBufferIndirectCommand_DrawIndexedInstance(nameBuffer, 
																															 GL_STATIC_DRAW,
																															 (int)this->countIndirectDraw);
        if (!this->poBuffer_IndirectCommand)
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::SetupUniformIndirectCommandBuffer: create buffer indirect command: [" + nameBuffer + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
    }
}

void OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::UpdateUniformBuffer()
{
    this->objectCBs.resize(MAX_OBJECT_COUNT);
    this->materialCBs.resize(MAX_MATERIAL_COUNT);
    this->offsetCBs.clear();
    this->tessellationCBs.resize(MAX_OBJECT_COUNT);

    int index = 0;
    int indexRend = 0;
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        ValueUIntConstants offset;
        offset.value = (uint32)indexRend;
        this->offsetCBs.push_back(offset);

        for (int j = 0; j < pR->countInstance; j++)
        {
            this->objectCBs[index] = pR->objectCBs[j];
            if (index < MAX_MATERIAL_COUNT)
                this->materialCBs[index] = pR->materialCBs[j];

            InstanceConstants ins = {};
            ins.offsetObject = indexRend;
            ins.indexObject = j;
            if (pRend->isUsedTessellation)
            {
                this->tessellationCBs[index] = pR->tessellationCBs[j];
            }

            ++index;
        }
        indexRend += pR->countInstance;

        F_Assert(index < MAX_OBJECT_COUNT && "OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::UpdateUniformBuffer")
    }
}

void OpenGLES_013_IndirectDraw::ModelObjectRendIndirect::UpdateIndirectCommandBuffer()
{
    this->indirectDrawIndexedInstanceCommandCBs.clear();

    uint32_t vertexOffset = 0;
    uint32_t indexOffset = 0;
    uint32_t instanceOffset = 0;
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        DrawElementsIndirectCommand indirectCommand = {};
        indirectCommand.indexCountPerInstance = pMeshSub->poIndexCount;
        indirectCommand.instanceCount = (uint32_t)pRend->countInstance;
        indirectCommand.startIndexLocation = indexOffset;
        indirectCommand.baseVertexLocation = vertexOffset;
        indirectCommand.startInstanceLocation = instanceOffset;
        this->indirectDrawIndexedInstanceCommandCBs.push_back(indirectCommand);

        indexOffset += pMeshSub->poIndexCount;
        vertexOffset += pMeshSub->poVertexCount;
        instanceOffset += (uint32_t)pRend->countInstance;
    }
    this->countIndirectDraw = (uint32_t)this->indirectDrawIndexedInstanceCommandCBs.size();
}


/////////////////////////// ModelObject /////////////////////////



OpenGLES_013_IndirectDraw::OpenGLES_013_IndirectDraw(String name)
    : OpenGLESWindow(name)
    , m_isDrawIndirect(false)
    , m_isDrawIndirectMulti(false)
{
    this->cfg_isDepthStencil = true;
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector4(0, -1, 0, 0); //y-
}

void OpenGLES_013_IndirectDraw::setUpEnabledFeatures()
{
    OpenGLESWindow::setUpEnabledFeatures();

    this->m_isDrawIndirectMulti = false;
}

void OpenGLES_013_IndirectDraw::createCamera()
{
    OpenGLESWindow::createCamera();
    
    cameraReset();
}
void OpenGLES_013_IndirectDraw::cameraReset()
{
    OpenGLESWindow::cameraReset();

    this->pCamera->SetPos(FVector3(-25.0f, 13.0f, 4.0f));
    this->pCamera->SetEulerAngles(FVector3(35.0f, 90.0f, 0.0f));
    this->pCamera->SetFarZ(100000.0f);
    this->pCamera->UpdateViewMatrix();
    this->pCamera->UpdateProjectionMatrix();
}

void OpenGLES_013_IndirectDraw::loadModel_Custom()
{
	createMeshes();
    createTextures();
    createShaderModules();
    createDescriptorSetLayouts();

    int nIndexObjectRend = 0;
    for (int i = 0; i < g_Object_Count; i++)
    {
        ModelObject* pModelObject = new ModelObject(this, i);

		//1> Object
        {
            pModelObject->indexModel = i;
			pModelObject->nameObject = g_ObjectConfigs[2 * i + 0];
			pModelObject->nameMesh = g_ObjectConfigs[2 * i + 1];

			//Mesh
			{
				Mesh* pMesh = this->findMesh(pModelObject->nameMesh);
				F_Assert(pMesh != nullptr && "OpenGLES_013_IndirectDraw::loadModel_Custom")
				pModelObject->SetMesh(pMesh);
			}
			//MeshSub Used
            {
                String nameMeshSubUsed = g_Object_MeshSubsUsed[i];
                StringVector aMeshSubUsed = FUtilString::Split(nameMeshSubUsed, ";");
                pModelObject->aMeshSubUsed.clear();
                size_t count_mesh_sub_used = aMeshSubUsed.size();
                for (size_t j = 0; j < count_mesh_sub_used; j++)
                {
                    String& name = aMeshSubUsed[j];
                    int indexMeshSub = FUtilString::ParserInt(name);
                    pModelObject->aMeshSubUsed.push_back(indexMeshSub);
                }
            }
            pModelObject->isShow = g_Object_IsShows[i];
            pModelObject->isRotate = g_Object_IsRotates[i];
            pModelObject->isIndirectDraw = g_Object_IsIndirectDraw[i];
        }

        //2> ObjectRend
        {
            size_t count_mesh_sub = pModelObject->pMesh->aMeshSubs.size();
            size_t count_mesh_sub_used = pModelObject->aMeshSubUsed.size();
            for (size_t j = 0; j < count_mesh_sub_used; j++)
            {
                int indexMeshSub = pModelObject->aMeshSubUsed[j];
                F_Assert(indexMeshSub >= 0 && indexMeshSub < count_mesh_sub && "OpenGLES_013_IndirectDraw::loadModel_Custom")

                MeshSub* pMeshSub = pModelObject->pMesh->aMeshSubs[indexMeshSub];
                String nameObjectRend = g_ObjectRend_Configs[7 * nIndexObjectRend + 0];
                ModelObjectRend* pRend = new ModelObjectRend(nameObjectRend, pModelObject, pMeshSub, nIndexObjectRend);

                pRend->countInstanceExt = g_Object_InstanceExtCount[i];
                pRend->countInstance = pRend->countInstanceExt * 2 + 1;

                //Texture VS
				{
					String nameTextureVS = g_ObjectRend_Configs[7 * nIndexObjectRend + 1]; //Texture VS
					if (!nameTextureVS.empty())
					{
						StringVector aTextureVS = FUtilString::Split(nameTextureVS, ";");
						size_t count_tex = aTextureVS.size();
						for (size_t j = 0; j < count_tex; j++)
						{
							String nameTex = aTextureVS[j];
							GLESTexture* pTextureVS = this->findTexture(nameTex);
							pRend->AddTexture(F_GetShaderTypeName(F_Shader_Vertex), pTextureVS);
						}
					}
				}
				//Texture FS
				{
					String nameTextureFS = g_ObjectRend_Configs[7 * nIndexObjectRend + 5]; //Texture FS
					if (!nameTextureFS.empty())
					{
						StringVector aTextureFS = FUtilString::Split(nameTextureFS, ";");
						size_t count_tex = aTextureFS.size();
						for (size_t j = 0; j < count_tex; j++)
						{
							String nameTex = aTextureFS[j];
							GLESTexture* pTextureFS = this->findTexture(nameTex);
							pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureFS);
						}
					}
				}
				//Texture CS
				{
					String nameTextureCS = g_ObjectRend_Configs[7 * nIndexObjectRend + 6]; //Texture CS
					if (!nameTextureCS.empty())
					{
						StringVector aTextureCS = FUtilString::Split(nameTextureCS, ";");
						size_t count_tex = aTextureCS.size();
						for (size_t j = 0; j < count_tex; j++)
						{
							String nameTex = aTextureCS[j];
							GLESTexture* pTextureCS = this->findTexture(nameTex);
							pRend->AddTexture(F_GetShaderTypeName(F_Shader_Compute), pTextureCS);
						}
					}
				}

                //Tessellation
				String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * i + 1];
				String nameShaderTese = g_ObjectRend_NameShaderModules[6 * i + 2];
				if (!nameShaderTesc.empty() || !nameShaderTese.empty())
				{
					pRend->isUsedTessellation = true;
					if (g_ObjectRend_IsTopologyPatchLists[i])
					{
						pRend->poTypePrimitive = GL_PATCHES;
					}
				}
				
				//Common
				pRend->isTransparent = g_ObjectRend_IsTransparents[i];
				pRend->poTypeCulling = g_ObjectRend_TypeCulling[i];
				pRend->isUsedIndirectDraw = g_ObjectRend_IsIndirectDraw[nIndexObjectRend];
                pRend->offsetCBs.value = 0;

				pModelObject->AddObjectRend(pRend);
                m_aModelObjectRends_All.push_back(pRend);
                if (pRend->isTransparent)
                    m_aModelObjectRends_Transparent.push_back(pRend);
                else 
                    m_aModelObjectRends_Opaque.push_back(pRend);

                nIndexObjectRend ++;
            }

            m_aModelObjects.push_back(pModelObject);
            m_mapModelObjects[pModelObject->nameObject] = pModelObject;
        }

        //3> ObjectRendIndirectDraw
        if (pModelObject->isIndirectDraw)
        {
            size_t count_object_rend = pModelObject->aRends.size();
            if (count_object_rend > 0)
            {
                ModelObjectRend* pRend = pModelObject->aRends[i];
                String nameObjectRendIndirect = pModelObject->nameObject + " - RendIndirect";
                pModelObject->pRendIndirect = new ModelObjectRendIndirect(nameObjectRendIndirect);
                pModelObject->pRendIndirect->SetupVertexIndexBuffer(pModelObject->aRends);
            }
        }
    }
}

void OpenGLES_013_IndirectDraw::createCustomCB()
{
	rebuildInstanceCBs(true);
}
void OpenGLES_013_IndirectDraw::rebuildInstanceCBs(bool isCreateBuffer)
{
	//1> ObjectRends
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
       ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        int indexObject = pRend->pModelObject->index;
        int count_instance = pRend->countInstance;
        bool isObjectLighting = g_Object_IsLightings[indexObject];
        

        pRend->instanceMatWorld.resize(MAX_OBJECT_COUNT);
        pRend->objectCBs.resize(MAX_OBJECT_COUNT);
        pRend->materialCBs.resize(MAX_MATERIAL_COUNT);
        pRend->tessellationCBs.resize(MAX_OBJECT_COUNT);
        for (int j = 0; j < count_instance; j++)
        {
            //ObjectConstants
            {
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[i * 3 + 0] + FVector3((j - pRend->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                            g_ObjectRend_Tranforms[i * 3 + 1],
                                                            g_ObjectRend_Tranforms[i * 3 + 2]);
                pRend->objectCBs[j] = objectConstants;
                pRend->instanceMatWorld[j] = objectConstants.g_MatWorld;
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = FMath::RandomColor(false);
                materialConstants.factorDiffuse = FMath::RandomColor(false);
                materialConstants.factorSpecular = FMath::RandomColor(false);
                materialConstants.lighting.x = FMath::RandF(10.0f, 100.0f);
                materialConstants.lighting.y = FMath::RandF(0.2f, 0.9f);
                materialConstants.lighting.z = isObjectLighting ? 1.0f : 0.0f;
                //Texture VS
                {
                    GLESTexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    GLESTexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {
                        
                    }
                }
                //Texture CS
                {
                    GLESTexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                    if (pTextureCSs != nullptr)
                    {

                    }
                }
                pRend->materialCBs[j] = materialConstants;
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevelOuter = 1.0f;
                tessellationConstants.tessLevelInner = 1.0f;
                tessellationConstants.tessAlpha = 1.0f;
                pRend->tessellationCBs[j] = tessellationConstants;
            }
        }

		if (isCreateBuffer)
		{
            DescriptorSetLayout* pDSL = findDescriptorSetLayout(g_ObjectRend_NameDescriptorSetLayouts[2 * i + 0]);

            //ObjectConstants
            F_DELETE(pRend->poBufferUniform_Object)
			String nameBuffer = "ObjectConstants-" + pRend->pModelObject->nameObject + "-" + FUtilString::SaveInt((int)i);
			pRend->poBufferUniform_Object = createBufferUniform(nameBuffer,
																pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants)),
																GL_DYNAMIC_DRAW,
																sizeof(ObjectConstants) * pRend->objectCBs.size(),
																(uint8*)(pRend->objectCBs.data()),
																false);
			if (!pRend->poBufferUniform_Object)
			{
				String msg = "*********************** OpenGLES_013_IndirectDraw::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError("%s", msg.c_str());
				throw std::runtime_error(msg);
			}

            //MaterialConstants
            F_DELETE(pRend->poBufferUniform_Material)
            nameBuffer = "MaterialConstants-" + pRend->pModelObject->nameObject + "-" + FUtilString::SaveInt((int)i);
            pRend->poBufferUniform_Material = createBufferUniform(nameBuffer,
                                                                  pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants)),
                                                                  GL_DYNAMIC_DRAW,
                                                                  sizeof(MaterialConstants) * pRend->materialCBs.size(),
                                                                  (uint8*)(pRend->materialCBs.data()),
                                                                  false);
            if (!pRend->poBufferUniform_Material)
            {
                String msg = "*********************** OpenGLES_013_IndirectDraw::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }

            //ValueUIntConstants
            if (pRend->isUsedIndirectDraw)
            {
                F_DELETE(pRend->poBufferUniform_Offset)
                nameBuffer = "ValueUIntConstants-" + pRend->pModelObject->nameObject + "-" + FUtilString::SaveInt((int)i);
                pRend->poBufferUniform_Offset = createBufferUniform(nameBuffer,
                                                                    pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_ValueUIntConstants)),
																	GL_DYNAMIC_DRAW,
                                                                    sizeof(ValueUIntConstants),
                                                                    (uint8*)(&pRend->offsetCBs),
                                                                    false);
                if (!pRend->poBufferUniform_Offset)
                {
                    String msg = "*********************** OpenGLES_013_IndirectDraw::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

			//TessellationConstants
            if (pRend->isUsedTessellation)
            {
                F_DELETE(pRend->poBufferUniform_Tessellation)
                nameBuffer = "TessellationConstants-" + pRend->pModelObject->nameObject + "-" + FUtilString::SaveInt((int)i);
                pRend->poBufferUniform_Tessellation = createBufferUniform(nameBuffer,
                                                                          pDSL->FindIndex(Util_GetDescriptorSetTypeName(DescriptorSet_TessellationConstants)),
																		  GL_DYNAMIC_DRAW,
																		  sizeof(TessellationConstants) * pRend->tessellationCBs.size(),
																		  (uint8*)(pRend->tessellationCBs.data()),
																		  false);
                if (!pRend->poBufferUniform_Tessellation)
                {
                    String msg = "*********************** OpenGLES_013_IndirectDraw::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg);
                }
            } 
		}
    }

    //2> ObjectRendIndriect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        if (pModelObject->isIndirectDraw &&
            pModelObject->pRendIndirect != nullptr)
        {
            pModelObject->pRendIndirect->UpdateUniformBuffer();
            pModelObject->pRendIndirect->UpdateIndirectCommandBuffer();

            if (isCreateBuffer)
            {
                pModelObject->pRendIndirect->SetupUniformIndirectCommandBuffer();
            }
        }
    }
}

void OpenGLES_013_IndirectDraw::createCustomBeforePipeline()
{
    
}  
void OpenGLES_013_IndirectDraw::createGraphicsPipeline_Custom()
{
    //1> Object Rend
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

		//<1> Shaders
        String nameShaderVert = g_ObjectRend_NameShaderModules[6 * i + 0];
        String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * i + 1];
        String nameShaderTese = g_ObjectRend_NameShaderModules[6 * i + 2];
        String nameShaderGeom = g_ObjectRend_NameShaderModules[6 * i + 3];
        String nameShaderFrag = g_ObjectRend_NameShaderModules[6 * i + 4];
        GLESShader* pShaderVertex = findShaderModule(nameShaderVert);
        GLESShader* pShaderTesc = findShaderModule(nameShaderTesc);
        GLESShader* pShaderTese = findShaderModule(nameShaderTese);
		GLESShader* pShaderGeom = findShaderModule(nameShaderGeom);
        GLESShader* pShaderFragment = findShaderModule(nameShaderFrag);
        DescriptorSetLayout* pDSL = findDescriptorSetLayout(g_ObjectRend_NameDescriptorSetLayouts[2 * i + 0]);

        bool poDepthEnabled = pRend->poDepthEnabled;
		GLenum poDepthFuncCompare = pRend->poDepthFuncCompare;
		bool poDepthTestEnabled = pRend->poDepthTestEnabled;
		bool poDepthWriteEnabled = pRend->poDepthWriteEnabled;
        bool poBlendEnabled = pRend->poBlendEnabled;
        GLenum poBlendColorFactorSrc = pRend->poBlendColorFactorSrc; 
		GLenum poBlendColorFactorDst = pRend->poBlendColorFactorDst; 
        if (pRend->isTransparent)
        {
            poDepthTestEnabled = false;
            poDepthWriteEnabled = false;

            poBlendEnabled = true;
            poBlendColorFactorSrc = GL_SRC_ALPHA;
            poBlendColorFactorDst = GL_ONE_MINUS_SRC_ALPHA;
        }

        //poStatePipelineGraphics
		String namePipelineGraphics = "PipelineGraphics-" + pRend->pModelObject->nameObject + "-" + FUtilString::SaveInt((int)i);
        pRend->poStatePipelineGraphics = createStatePipelineGraphics(namePipelineGraphics,
																	 pDSL,
																	 pShaderVertex,
																	 pShaderTesc,
																	 pShaderTese,
																	 pShaderGeom,
																	 pShaderFragment,
																	 pRend->pMeshSub->poTypeVertex,
																	 pRend->poTypePrimitive,
																	 pRend->poIsCull,
																	 pRend->poTypeFrontFace,
																	 pRend->poTypeCulling,
																	 pRend->poTypePolygonMode,
																	 poDepthEnabled,
																	 poDepthFuncCompare,
																	 poDepthTestEnabled,
																	 poDepthWriteEnabled,
																	 pRend->poStencilEnabled,
																	 pRend->poStencil_CompareFunction,
																	 pRend->poStencil_StencilFailureOp,
																	 pRend->poStencil_DepthFailureOp,
																	 pRend->poStencil_DepthStencilPassOp,
																	 pRend->poStencil_Ref,
																	 pRend->poStencil_Mask,
																	 poBlendEnabled,
																	 poBlendColorFactorSrc,
																	 poBlendColorFactorDst,
																	 pRend->poBlendColorOp,
																	 pRend->poBlendAlphaFactorSrc,
																	 pRend->poBlendAlphaFactorDst,
																	 pRend->poBlendAlphaOp,
																	 pRend->poColorWriteMask_Red,
																	 pRend->poColorWriteMask_Green,
																	 pRend->poColorWriteMask_Blue,
																	 pRend->poColorWriteMask_Alpha);
        if (pRend->poStatePipelineGraphics == nullptr)
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::createGraphicsPipeline_Custom: Failed to create pipeline graphics !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        if (pModelObject->pRendIndirect != nullptr)
        {
            pModelObject->pRendIndirect->CreatePipelineGraphics();
        }
    }
}

void OpenGLES_013_IndirectDraw::createComputePipeline_Custom()
{
    
}

void OpenGLES_013_IndirectDraw::destroyMeshes()
{
    size_t count = this->m_aModelMesh.size();
    for (size_t i = 0; i < count; i++)
    {
        Mesh* pMesh = this->m_aModelMesh[i];
        delete pMesh;
    }
    this->m_aModelMesh.clear();
    this->m_mapModelMesh.clear();
}
void OpenGLES_013_IndirectDraw::createMeshes()
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

        Mesh* pMesh = new Mesh(0, 
                               nameMesh,
                               pathMesh,
                               typeMesh,
                               typeVertex,
                               typeGeometryType,
                               nullptr);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTransformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals[i]))
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("OpenGLES_013_IndirectDraw::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), pathMesh.c_str());
    }
}
Mesh* OpenGLES_013_IndirectDraw::findMesh(const String& nameMesh)
{
    MeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_013_IndirectDraw::destroyTextures()
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
void OpenGLES_013_IndirectDraw::createTextures()
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

        F_LogInfo("OpenGLES_013_IndirectDraw::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
GLESTexture* OpenGLES_013_IndirectDraw::findTexture(const String& nameTexture)
{
    GLESTexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_013_IndirectDraw::destroyShaderModules()
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
void OpenGLES_013_IndirectDraw::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = getShaderPathRelative(g_ShaderModulePaths[3 * i + 2]);

        GLESShader* pShader =  createShader(shaderName, shaderPath, shaderType);
        if (pShader == nullptr)
        {
            String msg = "*********************** OpenGLES_013_IndirectDraw::createShaderModules: Failed to create shader: shaderPath: " + shaderPath;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShader);
        this->m_mapGLESShaderModules[shaderName] = pShader;
        F_LogInfo("OpenGLES_013_IndirectDraw::createShaderModules: create shader [%s] success !", shaderPath.c_str());
    }
}
GLESShader* OpenGLES_013_IndirectDraw::findShaderModule(const String& pathShaderModule)
{
    GLESShaderPtrMap::iterator itFind = this->m_mapGLESShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapGLESShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void OpenGLES_013_IndirectDraw::destroyDescriptorSetLayouts()
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
void OpenGLES_013_IndirectDraw::createDescriptorSetLayouts()
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
DescriptorSetLayout* OpenGLES_013_IndirectDraw::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    DescriptorSetLayoutPtrMap::iterator itFind = this->m_mapDescriptorSetLayouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapDescriptorSetLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void OpenGLES_013_IndirectDraw::createDescriptorSets_Custom()
{
    //1> Object Rend
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

		//Pipeline Graphics
		{
			createDescriptorSets_Graphics(pRend, 
                                          pRend->poStatePipelineGraphics, 
                                          pRend->poBufferUniform_Object,
                                          pRend->poBufferUniform_Material,
                                          pRend->poBufferUniform_Offset,
                                          pRend->poBufferUniform_Tessellation);
		}

        //Pipeline Computes
		{
			
		}
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        if (pModelObject->pRendIndirect != nullptr)
        {
            createDescriptorSets_Graphics(pModelObject->pRendIndirect->pRend, 
                                          pModelObject->pRendIndirect->poStatePipelineGraphics, 
                                          pModelObject->pRendIndirect->poBufferUniform_Object,
                                          pModelObject->pRendIndirect->poBufferUniform_Material,
                                          nullptr,
                                          pModelObject->pRendIndirect->poBufferUniform_Tessellation);
        }
    }
}
void OpenGLES_013_IndirectDraw::createDescriptorSets_Graphics(ModelObjectRend* pRend,
															  GLESStatePipelineGraphics* pStatePipelineGraphics,
															  GLESBufferUniform* pBufferUniform_Object,
															  GLESBufferUniform* pBufferUniform_Material,
															  GLESBufferUniform* pBufferUniform_Offset,
															  GLESBufferUniform* pBufferUniform_Tessellation)
{
	int nIndexTextureVS = 0;
	int nIndexTextureTESC = 0;
	int nIndexTextureTESE = 0;
	int nIndexTextureFS = 0;
	int count_ds = (int)pStatePipelineGraphics->poDescriptorSetLayout->aLayouts.size();
	for (int j = 0; j < count_ds; j++)
	{
		String& nameDescriptorSet = pStatePipelineGraphics->poDescriptorSetLayout->aLayouts[j];
        uint32 nBindingIndex = (uint32)j;
		
		if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants)) //PassConstants
		{
			uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
			pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
		}
		else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants)) //ObjectConstants
		{
			uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
			pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pStatePipelineGraphics->BindBufferUniform(pBufferUniform_Object, nBindingIndex);
		} 
		else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants)) //MaterialConstants
		{
			uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
			pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pStatePipelineGraphics->BindBufferUniform(pBufferUniform_Material, nBindingIndex);
		}
		else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_InstanceConstants)) //InstanceConstants
		{

		}
		else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_ValueUIntConstants)) //ValueUIntConstants
        {
            if (pBufferUniform_Offset != nullptr)
            {
				uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
				pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
				pStatePipelineGraphics->BindBufferUniform(pBufferUniform_Offset, nBindingIndex);
            }
        }
		else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TessellationConstants)) //TessellationConstants
		{
			if (pBufferUniform_Tessellation != nullptr)
			{
				uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
				pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
				pStatePipelineGraphics->BindBufferUniform(pBufferUniform_Tessellation, nBindingIndex);
			}
		}
		else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureFS)) //TextureFS
		{
			GLESTexture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Fragment), nIndexTextureFS);
			pStatePipelineGraphics->BindTextureFS(pTexture, nIndexTextureFS);
			nIndexTextureFS ++;
		}
		else
		{
			String msg = "*********************** OpenGLES_013_IndirectDraw::createDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg.c_str());
		}
	}
}


void OpenGLES_013_IndirectDraw::updateCompute_BeforeRender_Custom()
{
	
}

void OpenGLES_013_IndirectDraw::updateCBs_Custom()
{
    //1> Object Rend
	GLESBufferUniform* pBufferUniform_Pass = GetUniform_PassCB();
    float time = this->pTimer->GetTimeSinceStart();
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        
		//0: PassConstants
		pRend->poStatePipelineGraphics->BindBufferUniform(pBufferUniform_Pass, (uint32)DescriptorSet_PassConstants);

		//1: ObjectConstants
        for (size_t j = 0; j < pRend->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pRend->objectCBs[j];
            if (pRend->isRotate || pRend->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pRend->instanceMatWorld[j],
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pRend->instanceMatWorld[j];
            }

			//MaterialConstants
			MaterialConstants& materialCB = pRend->materialCBs[j];
            
            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants& tessellationCB = pRend->tessellationCBs[j];
            }
        }

        //ObjectConstants
		pRend->poBufferUniform_Object->UpdateBuffer(sizeof(ObjectConstants) * pRend->objectCBs.size(),
                                                    (uint8*)pRend->objectCBs.data(),
                                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		//MaterialConstants
        pRend->poBufferUniform_Material->UpdateBuffer(sizeof(MaterialConstants) * pRend->materialCBs.size(),
                                                      (uint8*)pRend->materialCBs.data(),
                                                      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

        //ValueUIntConstants
        if (pRend->isUsedIndirectDraw)
        {
            pRend->poBufferUniform_Offset->UpdateBuffer(sizeof(ValueUIntConstants),
                                                        (uint8*)&pRend->offsetCBs,
                                                        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        }

		//TessellationConstants
        if (pRend->isUsedTessellation)
        {
            pRend->poBufferUniform_Tessellation->UpdateBuffer(sizeof(TessellationConstants) * pRend->tessellationCBs.size(),
															  (uint8*)pRend->tessellationCBs.data(),
															  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        ModelObjectRendIndirect* pRendIndirect = pModelObject->pRendIndirect;
        if (pRendIndirect != nullptr)
        {
            pRendIndirect->UpdateUniformBuffer();

            //PassConstants
		    pRendIndirect->poStatePipelineGraphics->BindBufferUniform(pBufferUniform_Pass, (uint32)DescriptorSet_PassConstants);
            
            //ObjectConstants
            {
                pRendIndirect->poBufferUniform_Object->UpdateBuffer(sizeof(ObjectConstants) * pRendIndirect->objectCBs.size(),
                                                                    (uint8*)pRendIndirect->objectCBs.data(),
                                                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            }

            //MaterialConstants
            {
                pRendIndirect->poBufferUniform_Material->UpdateBuffer(sizeof(MaterialConstants) * pRendIndirect->materialCBs.size(),
                                                                      (uint8*)pRendIndirect->materialCBs.data(),
                                                                      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            }

            //TessellationConstants
            if (pRendIndirect->pRend->isUsedTessellation)
            {
                pRendIndirect->poBufferUniform_Tessellation->UpdateBuffer(sizeof(TessellationConstants) * pRendIndirect->tessellationCBs.size(),
                                                                          (uint8*)pRendIndirect->tessellationCBs.data(),
                                                                          GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            }

            //IndirectCommand
            {
                pRendIndirect->poBuffer_IndirectCommand->UpdateBuffer(pRendIndirect->indirectDrawIndexedInstanceCommandCBs);
            }
        }
    }
}



bool OpenGLES_013_IndirectDraw::beginRenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("OpenGLES_013_IndirectDraw", &windowOpened, 0);
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
void OpenGLES_013_IndirectDraw::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
		//m_isDrawIndirect
        if (ImGui::Checkbox("Is DrawIndirect", &this->m_isDrawIndirect))
        {
            
        }
        //m_isDrawIndirectMulti
        if (ImGui::Checkbox("Is DrawIndirectMulti", &this->m_isDrawIndirectMulti))
        {
            
        }
        this->m_isDrawIndirectMulti = false;

        float fGap = g_Object_InstanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 5.0f))
        {
            g_Object_InstanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        size_t count = this->m_aModelObjects.size();
        for (size_t i = 0; i < count; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];
            ModelObjectRendIndirect* pRendIndirect = pModelObject->pRendIndirect;
            size_t count_object_rend = pModelObject->aRends.size();

            //1> ModelObject
            String nameModel = FUtilString::SaveInt((int32)i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                //isShow
                String nameIsShow = "Is Show - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isShow = pModelObject->isShow;
                    }
                }
                //isWireFrame
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isWireFrame = pModelObject->isWireFrame;
                    }
                }
                //isRotate
                String nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isRotate = pModelObject->isRotate;
                    }
                }
                //isLighting
                String nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        for (int p = 0; p < pRend->countInstance; p++)
                        {
                            MaterialConstants& mat = pRend->materialCBs[p];
                            mat.lighting.z = pModelObject->isLighting ? 1.0f : 0.0f;
                        }

                        String nameInstances = "Instance - " + pModelObject->nameObject;
                        int countInstanceExt = pRend->countInstanceExt;
                        ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 3);
                        if (countInstanceExt != pRend->countInstanceExt)
                        {
                            pRend->countInstanceExt = countInstanceExt;
                            pRend->countInstance = countInstanceExt * 2 + 1;
                            rebuildInstanceCBs(false);
                        }

                    }
                }

                //2> ModelObjectRend
                if (pRendIndirect != nullptr && this->m_isDrawIndirect)
                {
                    String& nameObjectRendIndirect = pRendIndirect->nameObjectRendIndirect;
                    if (ImGui::CollapsingHeader(nameObjectRendIndirect.c_str()))
                    {
                        ImGui::Text("Rend Count: [%d], Vertex: [%d], Index: [%d]", 
                            (int)pRendIndirect->aRends.size(), 
                            (int)pRendIndirect->poVertexCount,
                            (int)pRendIndirect->poIndexCount);
                        //isShow
                        String nameIsShowRend = "Is Show - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRendIndirect->isShow))
                        {
                            if (pRendIndirect->isShow)
                            {
                                pModelObject->isShow = true;
                            }
                        }
                        //isWireFrame
                        String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRendIndirect->isWireFrame))
                        {
                            if (!pRendIndirect->isWireFrame)
                            {
                                pModelObject->isWireFrame = false;
                            }
                        }
                        //isRotate
                        String nameIsRotateRend = "Is Rotate - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRendIndirect->isRotate))
                        {
                            
                        }
                        //isLighting
                        String nameIsLightingRend = "Is Lighting - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRendIndirect->isLighting))
                        {
                            if (pRendIndirect->isLighting)
                            {
                                pModelObject->isLighting = true;
                            }
                            for (size_t p = 0; p < pRendIndirect->materialCBs.size(); p++)
                            {
                                MaterialConstants& mat = pRendIndirect->materialCBs[p];
                                mat.lighting.z = pRendIndirect->isLighting ? 1.0f : 0.0f;
                            }
                        }
                        //isTransparent
                        String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRendIndirect;
                        bool isTransparent = pRendIndirect->isTransparent;
                        ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                        //countIndirectDraw
                        String nameCountIndirectDraw = "Count IndirectDraw - " + nameObjectRendIndirect;
                        int countIndirectDraw = (int)pRendIndirect->countIndirectDraw;
                        if (ImGui::DragInt(nameCountIndirectDraw.c_str(), &countIndirectDraw, 1, 0, (int)pRendIndirect->indirectDrawIndexedInstanceCommandCBs.size()))
                        {
                            pRendIndirect->countIndirectDraw = (uint32_t)countIndirectDraw;
                        }

                        //ObjectRend
                        size_t count_rend = pRendIndirect->aRends.size();
                        for (int j = 0; j < count_rend; j++)
                        {
                            ModelObjectRend* pRend = pRendIndirect->aRends[j];

                            ImGui::Text("[%d], Vertex: [%d], Index: [%d], - [%s]", 
                            j,
                            (int)pRend->pMeshSub->poVertexCount,
                            (int)pRend->pMeshSub->poIndexCount,
                            pRend->nameObjectRend.c_str());
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        String& nameObjectRend = pRend->nameObjectRend;
                        if (ImGui::CollapsingHeader(nameObjectRend.c_str()))
                        {
                            ImGui::Text("Vertex: [%d], Index: [%d]", (int)pRend->pMeshSub->poVertexCount, (int)pRend->pMeshSub->poIndexCount);
                            //isShow
                            String nameIsShowRend = "Is Show - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRend->isShow))
                            {
                                if (pRend->isShow)
                                {
                                    pModelObject->isShow = true;
                                }
                            }
                            //isWireFrame
                            String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRend->isWireFrame))
                            {
                                if (!pRend->isWireFrame)
                                {
                                    pModelObject->isWireFrame = false;
                                }
                            }
                            //isRotate
                            String nameIsRotateRend = "Is Rotate - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRend->isRotate))
                            {
                                
                            }
                            //isLighting
                            String nameIsLightingRend = "Is Lighting - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRend->isLighting))
                            {
                                if (pRend->isLighting)
                                {
                                    pModelObject->isLighting = true;
                                }
                                for (int p = 0; p < pRend->countInstance; p++)
                                {
                                    MaterialConstants& mat = pRend->materialCBs[p];
                                    mat.lighting.z = pModelObject->isLighting ? 1.0f : 0.0f;
                                }
                            }
                            //isTransparent
                            String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRend;
                            bool isTransparent = pRend->isTransparent;
                            ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                            String nameWorld = "Model Object - " + nameObjectRend;
                            if (ImGui::CollapsingHeader(nameWorld.c_str()))
                            {
                                int count_instance = pRend->countInstance;
                                for (int p = 0; p < count_instance; p++)
                                {
                                    ObjectConstants& obj = pRend->objectCBs[p];
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = FUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            const FMatrix4& mat4World = obj.g_MatWorld;
                                            String nameTable = FUtilString::SaveInt(p) + " - matWorld - " + nameObjectRend;
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
                                        String nameMaterial = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend;
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
                                                GLESTexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                                                if (pTextureVSs != nullptr)
                                                {

                                                }
                                            }
                                            //Texture FS
                                            {
                                                GLESTexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                                                if (pTextureFSs != nullptr)
                                                {
                                                    int count_texture = (int)pTextureFSs->size();
                                                    for (int q = 0; q < count_texture; q++)
                                                    {
                                                        GLESTexture* pTexture = (*pTextureFSs)[q];

                                                        String nameMaterial_Texture = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend + " - TextureFS - " + FUtilString::SaveInt(q);
                                                        if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                                        {
                                                            //texWidth
                                                            String nameWidth = "Width - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int width = pTexture->width;
                                                            ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                            //texHeight
                                                            String nameHeight = "Height - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int height = pTexture->height;
                                                            ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                            //texDepth
                                                            String nameDepth = "Depth - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
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
                                                GLESTexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                                                if (pTextureCSs != nullptr)
                                                {

                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                        //TessellationConstants
                                        String nameTessellation = FUtilString::SaveInt(p) + " - Tessellation - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                        {
                                            if (pRend->isUsedTessellation)
                                            {
                                                TessellationConstants& tess = pRend->tessellationCBs[j];
                                                //tessLevelOuter
                                                String nameTessLevelOuter = "tessLevelOuter - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessLevelInner
                                                String nameTessLevelInner = "tessLevelInner - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessAlpha
                                                String nameTessAlpha = "tessAlpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                                {
                                                    
                                                }
                                                //tessStrength
                                                String nameTessStrength = "tessStrength - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessStrength.c_str(), &tess.tessStrength, 0.025f, 0.1f, 100.0f))
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
        }
    }
}

void OpenGLES_013_IndirectDraw::endRenderImgui()
{
    OpenGLESWindow::endRenderImgui();

}

void OpenGLES_013_IndirectDraw::drawMeshDefault_Custom()
{   
    if (this->m_isDrawIndirect)
    {
        //1> Opaque
        {
            drawModelObjectRendIndirects(this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(this->m_aModelObjectRends_Transparent);
        }
    }
    else
    {
        //1> Opaque
        {
            drawModelObjectRends(this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(this->m_aModelObjectRends_Transparent);
        }
    }
}

void OpenGLES_013_IndirectDraw::drawModelObjectRends(ModelObjectRendPtrVector& aRends)
{
	size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        if (!pRend->isShow ||
            !pRend->pModelObject->isShow)
            continue;
        drawModelObjectRend(pRend);
    }
}
void OpenGLES_013_IndirectDraw::drawModelObjectRend(ModelObjectRend* pRend)
{
	ModelObject* pModelObject = pRend->pModelObject;
    MeshSub* pMeshSub = pRend->pMeshSub;

	//State/Shader/BufferUniform/Texture
	pRend->poStatePipelineGraphics->BindState();
	pRend->poStatePipelineGraphics->BindShader();
	pRend->poStatePipelineGraphics->BindBufferUniforms();
	pRend->poStatePipelineGraphics->BindTextures();
	
	//Draw
	if (pRend->pMeshSub->pBufferVertex != nullptr)
	{
		pRend->pMeshSub->pBufferVertex->BindVertexArray();
		drawInstance(pRend->poStatePipelineGraphics->poTypePrimitive, 0, pRend->pMeshSub->poVertexCount, pRend->countInstance);
	}
	else if (pRend->pMeshSub->pBufferVertexIndex != nullptr)
	{
		pRend->pMeshSub->pBufferVertexIndex->BindVertexArray();
		drawIndexedInstance(pRend->poStatePipelineGraphics->poTypePrimitive, pRend->pMeshSub->poIndexCount, GL_UNSIGNED_INT, 0, pRend->countInstance);
	}
	else
	{	
		F_Assert(false && "OpenGLES_013_IndirectDraw::drawModelObjectRend")
	}
	pRend->poStatePipelineGraphics->UnBindState();
}

void OpenGLES_013_IndirectDraw::drawModelObjectRendIndirects(ModelObjectRendPtrVector& aRends)
{
	ModelObjectRendIndirect* pRendIndirect_Last = nullptr;
    size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        ModelObjectRendIndirect* pRendIndirect = pRend->pModelObject->pRendIndirect;
        if (pRendIndirect != nullptr)
        {
            if (pRendIndirect_Last != nullptr && pRendIndirect_Last == pRendIndirect)
                continue;
            if (!pRendIndirect->isShow)
            {
                pRendIndirect_Last = nullptr;
                continue;
            }
            drawModelObjectRendIndirect(pRendIndirect);
            pRendIndirect_Last = pRendIndirect;
        }
        else
        {
            if (!pRend->isShow)
                continue;
            drawModelObjectRend(pRend);
        }
    }
}
void OpenGLES_013_IndirectDraw::drawModelObjectRendIndirect(ModelObjectRendIndirect* pRendIndirect)
{
	ModelObjectRend* pRend = pRendIndirect->pRend;
    ModelObject* pModelObject = pRend->pModelObject;

    //State/Shader/BufferUniform/Texture
    pRendIndirect->poStatePipelineGraphics->BindState();
    pRendIndirect->poStatePipelineGraphics->BindShader();
    pRendIndirect->poStatePipelineGraphics->BindBufferUniforms();
    pRendIndirect->poStatePipelineGraphics->BindTextures();

    //Draw
    uint32_t drawCount = pRendIndirect->countIndirectDraw;
    if (pRendIndirect->pBufferVertexIndex != nullptr)
	{
		pRendIndirect->pBufferVertexIndex->BindVertexArray();
        pRendIndirect->poBufferUniform_Offset->BindBufferUniformBlockIndex();
        pRendIndirect->poBuffer_IndirectCommand->BindBufferIndirectCommand();

        if (m_isDrawIndirectMulti)
        {
            size_t offset = 0;
            drawMultiIndexedInstanceIndirect(pRend->poTypePrimitive, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset), drawCount, sizeof(DrawElementsIndirectCommand));
        }
        else
        {
            for (uint32_t i = 0; i < drawCount; i++)
            {
                pRendIndirect->poBufferUniform_Offset->UpdateBuffer(sizeof(ValueUIntConstants),
                                                                    (uint8*)&pRendIndirect->offsetCBs[i],
                                                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

                size_t offset = sizeof(DrawElementsIndirectCommand) * i;
                drawIndexedInstanceIndirect(pRend->poTypePrimitive, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset));
            }
        }
	}
	else if (pRendIndirect->pBufferVertex != nullptr)
	{
		pRendIndirect->pBufferVertex->BindVertexArray();
        pRendIndirect->poBufferUniform_Offset->BindBufferUniformBlockIndex();
        pRendIndirect->poBuffer_IndirectCommand->BindBufferIndirectCommand();

        if (m_isDrawIndirectMulti)
        {
            size_t offset = 0;
            drawMultiInstanceIndirect(pRend->poTypePrimitive, reinterpret_cast<void*>(offset), drawCount, sizeof(DrawArraysIndirectCommand));
        }
        else
        {
            for (uint32_t i = 0; i < drawCount; i++)
            {
                pRendIndirect->poBufferUniform_Offset->UpdateBuffer(sizeof(ValueUIntConstants),
                                                                    (uint8*)&pRendIndirect->offsetCBs[i],
                                                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            
                size_t offset = sizeof(DrawArraysIndirectCommand) * i;
                drawInstanceIndirect(pRend->poTypePrimitive, reinterpret_cast<void*>(offset));
            }
        }
	}
	else
    {	
        F_Assert(false && "OpenGLES_013_IndirectDraw::drawModelObjectRendIndirect")
    }
    pRendIndirect->poStatePipelineGraphics->UnBindState();
}


void OpenGLES_013_IndirectDraw::cleanupCustom()
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
    this->m_mapModelObjects.clear();
    this->m_aModelObjectRends_All.clear();
    this->m_aModelObjectRends_Opaque.clear();
    this->m_aModelObjectRends_Transparent.clear();
}

void OpenGLES_013_IndirectDraw::cleanupSwapChain_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }
}

void OpenGLES_013_IndirectDraw::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->RecreateSwapChain();
    }
}