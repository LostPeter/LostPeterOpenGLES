/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/OpenGLESWindow.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

namespace LostPeterOpenGLES
{
#if F_DEBUG == 1
    bool OpenGLESWindow::s_isEnableValidationLayers = true;
#else
    bool OpenGLESWindow::s_isEnableValidationLayers = false;
#endif
    int OpenGLESWindow::s_maxFramesInFight = 2;
    
    
    /////////////////////////// OpenGLESWindow Internal /////////////
    void OpenGLESWindow::createInternal()
    {
        //Mesh
        createMeshes_Internal();
		//Shader
        createShaders_Internal();
		//DescriptorSetLayout
        createDescriptorSetLayouts_Internal();
        //Texture
        createTextures_Internal();
    }
    void OpenGLESWindow::cleanupInternal()
    {
        //Texture
        destroyTextures_Internal();
		//DescriptorSetLayout
        destroyDescriptorSetLayouts_Internal();
		//Shader
        destroyShaders_Internal();
        //Mesh
        destroyMeshes_Internal();
    }

    void OpenGLESWindow::createResourceInternal()
    {   
        //Uniform ConstantBuffer
        createUniformCB_Internal();
        //PipelineCompute
        createPipelineCompute_Internal();
        //PipelineGraphics
        createPipelineGraphics_Internal();
    }   
    void OpenGLESWindow::destroyResourceInternal()
    {
        //Uniform ConstantBuffer
        destroyUniformCB_Internal();
        //PipelineCompute
        destroyPipelineCompute_Internal();
        //PipelineGraphics
        destroyPipelineGraphics_Internal();
    }

    //Mesh
    static const int g_MeshCount_Internal = 51;
    static const char* g_MeshPaths_Internal[7 * g_MeshCount_Internal] =
    {
        //Mesh Name                     //Vertex Type                 //Mesh Type         //Mesh Path                           //Mesh Geometry Type        //Mesh Geometry Param                                           //Mesh Geometry Vertex Is Update
        "geo_line_line_2d",             "Pos3Color4",                 "geometry",         "",                                   "LineLine2D",               "0.5;0.9;0.7;0.9;1;1;1;1",                                      "false", //geo_line_line_2d
        "geo_line_triangle_2d",         "Pos3Color4",                 "geometry",         "",                                   "LineTriangle2D",           "0.6;0.8;0.5;0.6;0.7;0.6;1;1;1;1",                              "false", //geo_line_triangle_2d
        "geo_line_quad_2d",             "Pos3Color4",                 "geometry",         "",                                   "LineQuad2D",               "0.5;0.5;0.5;0.3;0.7;0.3;0.7;0.5;1;1;1;1",                      "false", //geo_line_quad_2d
        "geo_line_grid_2d",             "Pos3Color4",                 "geometry",         "",                                   "LineGrid2D",               "0.5;0.2;0.5;0.0;0.7;0.0;0.7;0.2;10;10;1;1;1;1",                "false", //geo_line_grid_2d
        "geo_line_quad_convex_2d",      "Pos3Color4",                 "geometry",         "",                                   "LineQuad2D",               "0.55;-0.1;0.5;-0.3;0.65;-0.3;0.7;-0.1;1;1;1;1",                "false", //geo_line_quad_convex_2d
        "geo_line_quad_concave_2d",     "Pos3Color4",                 "geometry",         "",                                   "LineQuad2D",               "0.5;-0.4;0.5;-0.6;0.7;-0.6;0.55;-0.5;1;1;1;1",                 "false", //geo_line_quad_concave_2d
        "geo_line_circle_2d",           "Pos3Color4",                 "geometry",         "",                                   "LineCircle2D",             "0.6;-0.8;1.0;0.0;0.1;1280;720;50;true;1;1;1;1",                "true", //geo_line_circle_2d

        "geo_flat_triangle_2d",         "Pos3Color4",                 "geometry",         "",                                   "FlatTriangle2D",           "0.85;0.8;0.75;0.6;0.95;0.6;1;1;1;1",                           "false", //geo_flat_triangle_2d
        "geo_flat_quad_2d",             "Pos3Color4",                 "geometry",         "",                                   "FlatQuad2D",               "0.75;0.5;0.75;0.3;0.95;0.3;0.95;0.5;1;1;1;1",                  "false", //geo_flat_quad_2d
        "geo_flat_quad_convex_2d",      "Pos3Color4",                 "geometry",         "",                                   "FlatQuad2D",               "0.8;0.2;0.75;0.0;0.9;0.0;0.95;0.2;1;1;1;1",                    "false", //geo_flat_quad_convex_2d
        "geo_flat_quad_concave_2d",     "Pos3Color4",                 "geometry",         "",                                   "FlatQuad2D",               "0.75;-0.1;0.75;-0.3;0.95;-0.3;0.8;-0.2;1;1;1;1",               "false", //geo_flat_quad_concave_2d
        "geo_flat_circle_2d",           "Pos3Color4",                 "geometry",         "",                                   "FlatCircle2D",             "0.85;-0.5;1.0;0.0;0.1;1280;720;50;1;1;1;1",                    "true", //geo_flat_circle_2d

        "geo_line_line_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineLine3D",               "0;0;0;1;0;0;1;1;1;1",                                          "false", //geo_line_line_3d
        "geo_line_triangle_3d",         "Pos3Color4",                 "geometry",         "",                                   "LineTriangle3D",           "0;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;1;1;1;1",                       "false", //geo_line_triangle_3d
        "geo_line_quad_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;1;1;1;1",          "false", //geo_line_quad_3d
        "geo_line_grid_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineGrid3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;10;10;1;1;1;1",    "false", //geo_line_grid_3d
        "geo_line_quad_convex_3d",      "Pos3Color4",                 "geometry",         "",                                   "LineQuad3D",               "-0.2;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.8;0.5;0;1;1;1;1",          "false", //geo_line_quad_convex_3d
        "geo_line_quad_concave_3d",     "Pos3Color4",                 "geometry",         "",                                   "LineQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;-0.25;-0.25;0;1;1;1;1",      "false", //geo_line_quad_concave_3d
        "geo_line_circle_3d",           "Pos3Color4",                 "geometry",         "",                                   "LineCircle3D",             "0;0;0;1;0;0;0;0;-1;0.5;100;true;1;1;1;1",                      "false", //geo_line_circle_3d
        "geo_line_aabb_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineAABB3D",               "0;0;0;0.5;0.5;0.5;1;1;1;1",                                    "false", //geo_line_aabb_3d
        "geo_line_sphere_3d",           "Pos3Color4",                 "geometry",         "",                                   "LineSphere3D",             "0;0;0;0;1;0;0.5;30;30;1;1;1;1",                                "false", //geo_line_sphere_3d
        "geo_line_cylinder_3d",         "Pos3Color4",                 "geometry",         "",                                   "LineCylinder3D",           "0;0;0;0;1;0;0.5;0.5;1;50;true;1;1;1;1",                        "false", //geo_line_cylinder_3d
        "geo_line_capsule_3d",          "Pos3Color4",                 "geometry",         "",                                   "LineCapsule3D",            "0;0;0;0;1;0;0.5;1;10;50;1;1;1;1",                              "false", //geo_line_capsule_3d
        "geo_line_cone_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineCone3D",               "0;0;0;0;1;0;0.5;1;50;1;1;1;1",                                 "false", //geo_line_cone_3d
        "geo_line_torus_3d",            "Pos3Color4",                 "geometry",         "",                                   "LineTorus3D",              "0;0;0;0;1;0;0.5;0.2;50;20;1;1;1;1",                            "false", //geo_line_torus_3d
        
        "geo_flat_triangle_3d",         "Pos3Color4",                 "geometry",         "",                                   "FlatTriangle3D",           "0;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;1;1;1;1",                       "false", //geo_flat_triangle_3d
        "geo_flat_quad_3d",             "Pos3Color4",                 "geometry",         "",                                   "FlatQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;1;1;1;1",          "false", //geo_flat_quad_3d
        "geo_flat_quad_convex_3d",      "Pos3Color4",                 "geometry",         "",                                   "FlatQuad3D",               "-0.2;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.8;0.5;0;1;1;1;1",          "false", //geo_flat_quad_convex_3d
        "geo_flat_quad_concave_3d",     "Pos3Color4",                 "geometry",         "",                                   "FlatQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;-0.25;-0.25;0;1;1;1;1",      "false", //geo_flat_quad_concave_3d
        "geo_flat_circle_3d",           "Pos3Color4",                 "geometry",         "",                                   "FlatCircle3D",             "0;0;0;1;0;0;0;0;-1;0.5;100;1;1;1;1",                           "false", //geo_flat_circle_3d
        "geo_flat_aabb_3d",             "Pos3Color4",                 "geometry",         "",                                   "FlatAABB3D",               "0;0;0;0.5;0.5;0.5;1;1;1;1",                                    "false", //geo_flat_aabb_3d
        "geo_flat_sphere_3d",           "Pos3Color4",                 "geometry",         "",                                   "FlatSphere3D",             "0;0;0;0;1;0;0.5;30;30;1;1;1;1",                                "false", //geo_flat_sphere_3d
        "geo_flat_cylinder_3d",         "Pos3Color4",                 "geometry",         "",                                   "FlatCylinder3D",           "0;0;0;0;1;0;0.5;0.5;1;50;30;1;1;1;1",                          "false", //geo_flat_cylinder_3d
        "geo_flat_capsule_3d",          "Pos3Color4",                 "geometry",         "",                                   "FlatCapsule3D",            "0;0;0;0;1;0;0.5;1;10;50;30;1;1;1;1",                           "false", //geo_flat_capsule_3d
        "geo_flat_cone_3d",             "Pos3Color4",                 "geometry",         "",                                   "FlatCone3D",               "0;0;0;0;1;0;0.5;1;50;30;1;1;1;1",                              "false", //geo_flat_cone_3d
        "geo_flat_torus_3d",            "Pos3Color4",                 "geometry",         "",                                   "FlatTorus3D",              "0;0;0;0;1;0;0.5;0.2;50;20;1;1;1;1",                            "false", //geo_flat_torus_3d

        "geo_entity_triangle",          "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityTriangle",           "false;false",                                                  "false", //geo_entity_triangle
        "geo_entity_quad",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityQuad",               "false;false;0;0;1;1;0",                                        "false", //geo_entity_quad
        "geo_entity_grid",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityGrid",               "false;false;1;1;10;10",                                        "false", //geo_entity_grid
        "geo_entity_circle",            "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCircle",             "false;false;0.5;100",                                          "false", //geo_entity_circle
        "geo_entity_aabb",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityAABB",               "false;false;1;1;1;0",                                          "false", //geo_entity_aabb
        "geo_entity_sphere",            "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntitySphere",             "false;false;0.5;30;30",                                        "false", //geo_entity_sphere
        "geo_entity_geosphere",         "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityGeoSphere",          "false;false;0.5;5",                                            "false", //geo_entity_geosphere
        "geo_entity_cylinder",          "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCylinder",           "false;false;0.5;0.5;1;0;50;30",                                "false", //geo_entity_cylinder
        "geo_entity_capsule",           "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCapsule",            "false;false;0.5;1;0;10;50;30",                                 "false", //geo_entity_capsule
        "geo_entity_cone",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCone",               "false;false;0.5;1;0;50;30",                                    "false", //geo_entity_cone
        "geo_entity_torus",             "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityTorus",              "false;false;0.5;0.2;50;20",                                    "false", //geo_entity_torus

        "quad",                         "Pos3Color4Tex2",             "geometry",         "",                                   "EntityQuad",               "true;false;0;0;1;1;0",                                         "false", //quad
        "plane",                        "Pos3Color4Normal3Tex2",      "file",             "Mesh/Common/plane.fbx",              "",                         "",                                                             "false", //plane
        "cube",                         "Pos3Color4Normal3Tex2",      "file",             "Mesh/Common/cube.obj",               "",                         "",                                                             "false", //cube
        "sphere",                       "Pos3Color4Normal3Tex2",      "file",             "Mesh/Common/sphere.fbx",             "",                         "",                                                             "false", //sphere

    };
    static bool g_MeshIsFlipYs_Internal[g_MeshCount_Internal] = 
    {
        false, //geo_line_line_2d
        false, //geo_line_triangle_2d
        false, //geo_line_quad_2d
        false, //geo_line_grid_2d
        false, //geo_line_quad_convex_2d
        false, //geo_line_quad_concave_2d
        false, //geo_line_circle_2d

        false, //geo_flat_triangle_2d
        false, //geo_flat_quad_2d
        false, //geo_flat_quad_convex_2d
        false, //geo_flat_quad_concave_2d
        false, //geo_flat_circle_2d

        false, //geo_line_line_3d
        false, //geo_line_triangle_3d
        false, //geo_line_quad_3d
        false, //geo_line_grid_3d
        false, //geo_line_quad_convex_3d
        false, //geo_line_quad_concave_3d
        false, //geo_line_circle_3d
        false, //geo_line_aabb_3d
        false, //geo_line_sphere_3d
        false, //geo_line_cylinder_3d
        false, //geo_line_capsule_3d
        false, //geo_line_cone_3d
        false, //geo_line_torus_3d

        false, //geo_flat_triangle_3d
        false, //geo_flat_quad_3d
        false, //geo_flat_quad_convex_3d
        false, //geo_flat_quad_concave_3d
        false, //geo_flat_circle_3d
        false, //geo_flat_aabb_3d
        false, //geo_flat_sphere_3d
        false, //geo_flat_cylinder_3d
        false, //geo_flat_capsule_3d
        false, //geo_flat_cone_3d
        false, //geo_flat_torus_3d

        false, //geo_entity_triangle
        false, //geo_entity_quad
        false, //geo_entity_grid
        false, //geo_entity_circle
        false, //geo_entity_aabb
        false, //geo_entity_sphere
        false, //geo_entity_geosphere
        false, //geo_entity_cylinder
        false, //geo_entity_capsule
        false, //geo_entity_cone
        false, //geo_entity_torus

        true, //quad
        true, //plane
        false, //cube
        false, //sphere

    };
    static bool g_MeshIsTranformLocals_Internal[g_MeshCount_Internal] = 
    {
        false, //geo_line_line_2d
        false, //geo_line_triangle_2d
        false, //geo_line_quad_2d
        false, //geo_line_grid_2d
        false, //geo_line_quad_convex_2d
        false, //geo_line_quad_concave_2d
        false, //geo_line_circle_2d

        false, //geo_flat_triangle_2d
        false, //geo_flat_quad_2d
        false, //geo_flat_quad_convex_2d
        false, //geo_flat_quad_concave_2d
        false, //geo_flat_circle_2d

        false, //geo_line_line_3d
        true, //geo_line_triangle_3d
        true, //geo_line_quad_3d
        true, //geo_line_grid_3d
        true, //geo_line_quad_convex_3d
        true, //geo_line_quad_concave_3d
        true, //geo_line_circle_3d
        false, //geo_line_aabb_3d
        false, //geo_line_sphere_3d
        false, //geo_line_cylinder_3d
        false, //geo_line_capsule_3d
        false, //geo_line_cone_3d
        false, //geo_line_torus_3d

        true, //geo_flat_triangle_3d
        true, //geo_flat_quad_3d
        true, //geo_flat_quad_convex_3d
        true, //geo_flat_quad_concave_3d
        true, //geo_flat_circle_3d
        false, //geo_flat_aabb_3d
        false, //geo_flat_sphere_3d
        false, //geo_flat_cylinder_3d
        false, //geo_flat_capsule_3d
        false, //geo_flat_cone_3d
        false, //geo_flat_torus_3d

        true, //geo_entity_triangle
        true, //geo_entity_quad
        true, //geo_entity_grid
        true, //geo_entity_circle
        false, //geo_entity_aabb
        false, //geo_entity_sphere
        false, //geo_entity_geosphere
        false, //geo_entity_cylinder
        false, //geo_entity_capsule
        false, //geo_entity_cone
        false, //geo_entity_torus

        false, //quad
        false, //plane  
        false, //cube
        false, //sphere

    };
    static FMatrix4 g_MeshTranformLocals_Internal[g_MeshCount_Internal] = 
    {
        FMath::ms_mat4Unit, //geo_line_line_2d
        FMath::ms_mat4Unit, //geo_line_triangle_2d
        FMath::ms_mat4Unit, //geo_line_quad_2d
        FMath::ms_mat4Unit, //geo_line_grid_2d
        FMath::ms_mat4Unit, //geo_line_quad_convex_2d
        FMath::ms_mat4Unit, //geo_line_quad_concave_2d
        FMath::ms_mat4Unit, //geo_line_circle_2d

        FMath::ms_mat4Unit, //geo_flat_triangle_2d
        FMath::ms_mat4Unit, //geo_flat_quad_2d
        FMath::ms_mat4Unit, //geo_flat_quad_convex_2d
        FMath::ms_mat4Unit, //geo_flat_quad_concave_2d
        FMath::ms_mat4Unit, //geo_flat_circle_2d

        FMath::ms_mat4Unit, //geo_line_line_3d
        FMath::RotateX(90.0f), //geo_line_triangle_3d
        FMath::RotateX(90.0f), //geo_line_quad_3d
        FMath::RotateX(90.0f), //geo_line_grid_3d
        FMath::RotateX(90.0f), //geo_line_quad_convex_3d
        FMath::RotateX(90.0f), //geo_line_quad_concave_3d
        FMath::RotateX(90.0f), //geo_line_circle_3d
        FMath::ms_mat4Unit, //geo_line_aabb_3d
        FMath::ms_mat4Unit, //geo_line_sphere_3d
        FMath::ms_mat4Unit, //geo_line_cylinder_3d
        FMath::ms_mat4Unit, //geo_line_capsule_3d
        FMath::ms_mat4Unit, //geo_line_cone_3d
        FMath::ms_mat4Unit, //geo_line_torus_3d

        FMath::RotateX(90.0f), //geo_flat_triangle_3d
        FMath::RotateX(90.0f), //geo_flat_quad_3d
        FMath::RotateX(90.0f), //geo_flat_quad_convex_3d
        FMath::RotateX(90.0f), //geo_flat_quad_concave_3d
        FMath::RotateX(90.0f), //geo_flat_circle_3d
        FMath::ms_mat4Unit, //geo_flat_aabb_3d
        FMath::ms_mat4Unit, //geo_flat_sphere_3d
        FMath::ms_mat4Unit, //geo_flat_cylinder_3d
        FMath::ms_mat4Unit, //geo_flat_capsule_3d
        FMath::ms_mat4Unit, //geo_flat_cone_3d
        FMath::ms_mat4Unit, //geo_flat_torus_3d

        FMath::RotateX(90.0f), //geo_entity_triangle  
        FMath::RotateX(90.0f), //geo_entity_quad 
        FMath::RotateX(90.0f), //geo_entity_grid
        FMath::RotateX(90.0f), //geo_entity_circle
        FMath::ms_mat4Unit, //geo_entity_aabb
        FMath::ms_mat4Unit, //geo_entity_sphere
        FMath::ms_mat4Unit, //geo_entity_geosphere
        FMath::ms_mat4Unit, //geo_entity_cylinder
        FMath::ms_mat4Unit, //geo_entity_capsule
        FMath::ms_mat4Unit, //geo_entity_cone
        FMath::ms_mat4Unit, //geo_entity_torus

        FMath::ms_mat4Unit, //quad
        FMath::ms_mat4Unit, //plane
        FMath::ms_mat4Unit, //cube
        FMath::ms_mat4Unit, //sphere

    };
    void OpenGLESWindow::destroyMeshes_Internal()
    {
        size_t count = this->m_aMeshes_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            Mesh* pMesh = this->m_aMeshes_Internal[i];
            delete pMesh;
        }
        this->m_aMeshes_Internal.clear();
        this->m_mapMeshes_Internal.clear();
    }
    void OpenGLESWindow::createMeshes_Internal()
    {
        for (int i = 0; i < g_MeshCount_Internal; i++)
        {
            String nameMesh = g_MeshPaths_Internal[7 * i + 0];
            String nameVertexType = g_MeshPaths_Internal[7 * i + 1];
            String nameMeshType = g_MeshPaths_Internal[7 * i + 2];
            String pathMesh = g_MeshPaths_Internal[7 * i + 3];
            String nameGeometryType = g_MeshPaths_Internal[7 * i + 4];
            String nameGeometryParam = g_MeshPaths_Internal[7 * i + 5];
            String nameGeometryIsVertexUpdate = g_MeshPaths_Internal[7 * i + 6];
            bool isVertexUpdate = FUtilString::ParserBool(nameGeometryIsVertexUpdate);
            
            FMeshVertexType typeVertex = F_ParseMeshVertexType(nameVertexType); 
            FMeshType typeMesh = F_ParseMeshType(nameMeshType);
            FMeshGeometryType typeGeometryType = F_MeshGeometry_EntityTriangle;
            if (!nameGeometryType.empty())
            {
                typeGeometryType = F_ParseMeshGeometryType(nameGeometryType);
            }
            FMeshCreateParam* pParam = nullptr;
            if (!nameGeometryParam.empty())
            {
                pParam = FMeshGeometry::CreateParam(typeGeometryType, nameGeometryParam);
                if (typeGeometryType == F_MeshGeometry_LineCircle2D)
                {
                    FMeshCreateParam_LineCircle2D* pParam_LineCircle2D = (FMeshCreateParam_LineCircle2D*)pParam;
                    pParam_LineCircle2D->viewWidth = (int32)this->poViewport.right;
                    pParam_LineCircle2D->viewHeight = (int32)this->poViewport.bottom;
                }
                else if (typeGeometryType == F_MeshGeometry_FlatCircle2D)
                {
                    FMeshCreateParam_FlatCircle2D* pParam_FlatCircle2D = (FMeshCreateParam_FlatCircle2D*)pParam;
                    pParam_FlatCircle2D->viewWidth = (int32)this->poViewport.right;
                    pParam_FlatCircle2D->viewHeight = (int32)this->poViewport.bottom;
                }
            }

            Mesh* pMesh = new Mesh(0,
                                   nameMesh,
                                   pathMesh,
                                   typeMesh,
                                   typeVertex,
                                   typeGeometryType,
                                   pParam);
            bool isFlipY = g_MeshIsFlipYs_Internal[i];
            bool isTransformLocal = g_MeshIsTranformLocals_Internal[i];
            if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals_Internal[i], isVertexUpdate))
            {
                String msg = "*********************** OpenGLESWindow::createMeshes_Internal: create mesh: [" + nameMesh + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }

            this->m_aMeshes_Internal.push_back(pMesh);
            this->m_mapMeshes_Internal[nameMesh] = pMesh;

            F_LogInfo("OpenGLESWindow::createMeshes_Internal: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], mesh sub count: [%d], path: [%s] success !", 
                      nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), (int)pMesh->aMeshSubs.size(), pathMesh.c_str());
        }
    }
    Mesh* OpenGLESWindow::FindMesh_Internal(const String& nameMesh)
    {
        MeshPtrMap::iterator itFind = this->m_mapMeshes_Internal.find(nameMesh);
        if (itFind == this->m_mapMeshes_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //Texture
    static const int g_TextureCount_Internal = 4;
    static const char* g_TexturePaths_Internal[5 * g_TextureCount_Internal] = 
    {
        //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
        "default_black",                    "2D",            "false",                  "false",                           "Texture/Common/default_black.bmp", //default_black
        "default_white",                    "2D",            "false",                  "false",                           "Texture/Common/default_white.bmp", //default_white
        "default_blackwhite",               "2D",            "false",                  "false",                           "Texture/Common/default_blackwhite.png", //default_blackwhite
        "texture2d",                        "2D",            "false",                  "false",                           "Texture/Common/texture2d.jpg", //texture2d
        
    };
    static FTexturePixelFormatType g_TextureFormats_Internal[g_TextureCount_Internal] = 
    {
        F_TexturePixelFormat_R8G8B8A8_SRGB, //default_black
        F_TexturePixelFormat_R8G8B8A8_SRGB, //default_white
        F_TexturePixelFormat_R8G8B8A8_SRGB, //default_blackwhite
        F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d

    };
    static FTextureAddressingType g_TextureAddressings_Internal[g_TextureCount_Internal] = 
    {
        F_TextureAddressing_Wrap, //default_black
        F_TextureAddressing_Wrap, //default_white
        F_TextureAddressing_Wrap, //default_blackwhite
        F_TextureAddressing_Clamp, //texture2d

    };
    static FTextureFilterType g_TextureFilters_Internal[g_TextureCount_Internal * 2] = 
    {
        F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_black
        F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_white
        F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //default_blackwhite
        F_TextureFilter_Bilinear, F_TextureFilter_Bilinear, //texture2d

    };
    static FColor g_TextureBorderColors_Internal[g_TextureCount_Internal] = 
    {
        FColor(0, 0, 0, 1), //default_black
        FColor(0, 0, 0, 1), //default_white
        FColor(0, 0, 0, 1), //default_blackwhite
        FColor(0, 0, 0, 1), //texture2d

    };
    static int g_TextureSizes_Internal[3 * g_TextureCount_Internal] = 
    {
         64,     64,    1, //default_black
         64,     64,    1, //default_white
        512,    512,    1, //default_blackwhite
        512,    512,    1, //texture2d

    };
    void OpenGLESWindow::destroyTextures_Internal()
    {
        size_t count = this->m_aTextures_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            GLESTexture* pTexture = this->m_aTextures_Internal[i];
            delete pTexture;
        }
        this->m_aTextures_Internal.clear();
        this->m_mapTextures_Internal.clear();
    }
    void OpenGLESWindow::createTextures_Internal()
    {
        for (int i = 0; i < g_TextureCount_Internal; i++)
        {
            String nameTexture = g_TexturePaths_Internal[5 * i + 0];
            String nameType = g_TexturePaths_Internal[5 * i + 1];
            FTextureType typeTexture = F_ParseTextureType(nameType);
            String nameIsRenderTarget = g_TexturePaths_Internal[5 * i + 2];
            bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
            String nameIsGraphicsComputeShared = g_TexturePaths_Internal[5 * i + 3];
            bool isGraphicsComputeShared = FUtilString::ParserBool(nameIsGraphicsComputeShared);
            String pathTextures = g_TexturePaths_Internal[5 * i + 4];

            StringVector aPathTexture = FUtilString::Split(pathTextures, ";");
            GLESTexture* pTexture = new GLESTexture(nameTexture,
                                                    aPathTexture,
                                                    typeTexture,
                                                    g_TextureFormats_Internal[i],
                                                    g_TextureAddressings_Internal[i],
                                                    g_TextureFilters_Internal[2 * i + 0],
                                                    g_TextureFilters_Internal[2 * i + 1],
                                                    F_MSAASampleCount_1_Bit,
                                                    g_TextureBorderColors_Internal[i],
                                                    true,
                                                    true,
                                                    isGraphicsComputeShared,
													isRenderTarget,
                                                    false,
                                                    FMath::ms_clBlack);
            pTexture->texChunkMaxX = 0;
            pTexture->texChunkMaxY = 0; 
            if (pTexture->texChunkMaxX > 0 && 
                pTexture->texChunkMaxY > 0)
            {
                pTexture->texChunkIndex = FMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
            }
            pTexture->AddRef();

            int width = g_TextureSizes_Internal[3 * i + 0];
            int height = g_TextureSizes_Internal[3 * i + 1];
            int depth = g_TextureSizes_Internal[3 * i + 1];
            if (!pTexture->LoadTexture(width, 
                                       height,
                                       depth,
                                       4,
                                       nullptr))
            {
                String msg = "*********************** OpenGLESWindow::createTextures_Internal: create texture: [" + nameTexture + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }

            this->m_aTextures_Internal.push_back(pTexture);
            this->m_mapTextures_Internal[nameTexture] = pTexture;

            F_LogInfo("OpenGLESWindow::createTextures_Internal: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                      nameTexture.c_str(), 
                      nameType.c_str(), 
                      isRenderTarget ? "true" : "false",
                      pathTextures.c_str());
        }
    }
    GLESTexture* OpenGLESWindow::FindTexture_Internal(const String& nameTexture)
    {
        GLESTexturePtrMap::iterator itFind = this->m_mapTextures_Internal.find(nameTexture);
        if (itFind == this->m_mapTextures_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //DescriptorSetLayouts
    static const int g_DescriptorSetLayoutCount_Internal = 13;
    static const char* g_DescriptorSetLayoutNames_Internal[g_DescriptorSetLayoutCount_Internal] =
    {
        "PassConstants",
        "PassConstants-ObjectConstants",
        "PassConstants-CullInstance-BufferRWObjectCullInstance-BufferRWResultCB",
        "CopyBlitObjectConstants-TextureFrameColor",
        "CopyBlitObjectConstants-TextureFrameDepth",
        "Cull-BufferRWArgsCB",
        "Cull-CullObjectConstants-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB",
        "Cull-CullObjectConstants-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB-TextureCSR",
        "Cull-CullObjectConstants-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB-BufferRWClipCB-TextureCSR",
        "HizDepth-TextureFS",
        "HizDepth-TextureCSRWSrc-TextureCSRWDst",
        "TextureCopy-TextureCSR-TextureCSRW",
        "PassConstants-TerrainObjectConstants-Material-Instance-Terrain-TextureVS-TextureVS-TextureFS-TextureFS-TextureFS",
    };
    void OpenGLESWindow::destroyDescriptorSetLayouts_Internal()
    {
        size_t count = this->m_aDescriptorSetLayouts_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            DescriptorSetLayout* pDes = this->m_aDescriptorSetLayouts_Internal[i];
            delete pDes;
        }
        this->m_aDescriptorSetLayouts_Internal.clear();
        this->m_mapDescriptorSetLayouts_Internal.clear();
        this->m_mapName2Layouts_Internal.clear();
    }   
    void OpenGLESWindow::createDescriptorSetLayouts_Internal()
    {
        for (int i = 0; i < g_DescriptorSetLayoutCount_Internal; i++)
        {
            String nameLayout(g_DescriptorSetLayoutNames_Internal[i]);
            DescriptorSetLayout* pDes = new DescriptorSetLayout();
            pDes->Init(nameLayout);

            this->m_aDescriptorSetLayouts_Internal.push_back(pDes);
            this->m_mapDescriptorSetLayouts_Internal[nameLayout] = pDes;
            this->m_mapName2Layouts_Internal[nameLayout] = pDes->aLayouts;

            F_LogInfo("OpenGLESWindow::createDescriptorSetLayouts_Internal: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
        }
    }
    DescriptorSetLayout* OpenGLESWindow::FindDescriptorSetLayout_Internal(const String& nameDescriptorSetLayout)
    {
        DescriptorSetLayoutPtrMap::iterator itFind = this->m_mapDescriptorSetLayouts_Internal.find(nameDescriptorSetLayout);
        if (itFind == this->m_mapDescriptorSetLayouts_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //ShaderModule
    static const int g_ShaderCount_Internal = 4;
    static const char* g_ShaderModulePaths_Internal[3 * g_ShaderCount_Internal] = 
    {
        //name                                                     //type               //path
        ///////////////////////////////////////// vert /////////////////////////////////////////
        "vert_standard_copy_blit_from_frame",                     "vert",              "standard_copy_blit_from_frame.vert.spv", //standard_copy_blit_from_frame vert
        "vert_standard_copy_blit_to_frame",                       "vert",              "standard_copy_blit_to_frame.vert.spv", //standard_copy_blit_to_frame vert
        
        ///////////////////////////////////////// tesc /////////////////////////////////////////
    

        ///////////////////////////////////////// tese /////////////////////////////////////////
    

        ///////////////////////////////////////// geom /////////////////////////////////////////


        ///////////////////////////////////////// frag /////////////////////////////////////////
        "frag_standard_copy_blit_from_frame",                     "frag",              "standard_copy_blit_from_frame.frag.spv", //standard_copy_blit_from_frame frag
        "frag_standard_copy_blit_to_frame",                       "frag",              "standard_copy_blit_to_frame.frag.spv", //standard_copy_blit_to_frame frag
       
        ///////////////////////////////////////// comp /////////////////////////////////////////
        
    };
    void OpenGLESWindow::destroyShaders_Internal()
    {
        size_t count = this->m_aShaders_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            GLESShader* pShader = this->m_aShaders_Internal[i];
            F_DELETE(pShader)
        }
        this->m_aShaders_Internal.clear();
        this->m_mapShaders_Internal.clear();
    }
    void OpenGLESWindow::createShaders_Internal()
    {
        for (int i = 0; i < g_ShaderCount_Internal; i++)
        {
            String shaderName = g_ShaderModulePaths_Internal[3 * i + 0];
            String shaderType = g_ShaderModulePaths_Internal[3 * i + 1];
            String shaderPath = getShaderPathRelative(g_ShaderModulePaths_Internal[3 * i + 2]);
            FShaderType typeShader = F_ParseShaderType(shaderType);

            GLESShader* pShader = createShader(shaderName, shaderPath, typeShader);
            if (pShader == nullptr)
            {
                String msg = "*********************** OpenGLESWindow::createShaders_Internal: create shader: [" + shaderName + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }
            this->m_aShaders_Internal.push_back(pShader);
            this->m_mapShaders_Internal[shaderName] = pShader;
            F_LogInfo("OpenGLESWindow::createShaders_Internal: create shader, name: [%s], type: [%s], path: [%s] success !", 
                      shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
        }
    }
    GLESShader* OpenGLESWindow::FindShader_Internal(const String& nameShaderModule)
    {
        GLESShaderPtrMap::iterator itFind = this->m_mapShaders_Internal.find(nameShaderModule);
        if (itFind == this->m_mapShaders_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //UniformConstantBuffer
    void OpenGLESWindow::destroyUniformCB_Internal()
    {
        //1> PassCB
        destroyUniform_PassCB();
    }
        void OpenGLESWindow::destroyUniform_PassCB()
        {
            size_t count = this->poBuffers_PassCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                F_DELETE(this->poBuffers_PassCB[i])
            }
            this->poBuffers_PassCB.clear();
        }
        
    void OpenGLESWindow::createUniformCB_Internal()
    {
        //1> PassCB
        createUniform_PassCB();
    }
        void OpenGLESWindow::createUniform_PassCB()
        {
            size_t bufferSize = sizeof(PassConstants);
            size_t count = s_maxFramesInFight;
            this->poBuffers_PassCB.resize(count);
            for (size_t i = 0; i < count; i++) 
            {
                String nameBuffer = "PassConstants-" + FUtilString::SaveSizeT(i);
                GLESBufferUniform* pBufferUniform = createBufferUniform(nameBuffer,
                                                                        DescriptorSet_PassConstants,
                                                                        GL_DYNAMIC_DRAW,
                                                                        bufferSize,
                                                                        (uint8*)(&this->passCB),
                                                                        false);
                if (!pBufferUniform)
                {
                    String msg = "*********************** OpenGLESWindow::createUniform_PassCB: create buffer uniform: [" + nameBuffer + "] failed !";
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg);
                }
                this->poBuffers_PassCB[i] = pBufferUniform;
            }

            F_LogInfo("OpenGLESWindow::createUniform_PassCB: Create Uniform Pass constant buffer success !");
        }
	GLESBufferUniform* OpenGLESWindow::GetUniform_PassCB()
	{
		return this->poBuffers_PassCB[this->poCurrentFrame];
	}

    //PipelineCompute
    void OpenGLESWindow::destroyPipelineCompute_Internal()
    {
        destroyPipelineCompute_Cull();
        destroyPipelineCompute_Terrain();
    }
        void OpenGLESWindow::destroyPipelineCompute_Cull()
        {

        }
        void OpenGLESWindow::destroyPipelineCompute_Terrain()
        {

        }

    void OpenGLESWindow::createPipelineCompute_Internal()
    {
        createPipelineCompute_Cull();
        createPipelineCompute_Terrain();
    }
        void OpenGLESWindow::createPipelineCompute_Cull()
        {

        }
        void OpenGLESWindow::createPipelineCompute_Terrain()
        {

        }

    //PipelineGraphics
    void OpenGLESWindow::destroyPipelineGraphics_Internal()
    {
        destroyPipelineGraphics_Terrain();
        destroyPipelineGraphics_DepthHiz();
        destroyPipelineGraphics_DepthShadowMap();
        destroyPipelineGraphics_CopyBlitToFrame();
        destroyPipelineGraphics_CopyBlitFromFrame();
    }
        void OpenGLESWindow::destroyPipelineGraphics_CopyBlitFromFrame()
        {

        }
        void OpenGLESWindow::destroyPipelineGraphics_CopyBlitToFrame()
        {
            F_DELETE(this->m_pPipelineGraphics_CopyBlitToFrame)
        }
        void OpenGLESWindow::destroyPipelineGraphics_DepthShadowMap()
        {

        }
        void OpenGLESWindow::destroyPipelineGraphics_DepthHiz()
        {

        }
        void OpenGLESWindow::destroyPipelineGraphics_Terrain()
        {

        }

    void OpenGLESWindow::createPipelineGraphics_Internal()
    {
        createPipelineGraphics_CopyBlitFromFrame();
        createPipelineGraphics_CopyBlitToFrame();
        createPipelineGraphics_DepthShadowMap();
        createPipelineGraphics_DepthHiz();
        createPipelineGraphics_Terrain();

    }
        void OpenGLESWindow::createPipelineGraphics_CopyBlitFromFrame()
        {

        }

        void OpenGLESWindow::createPipelineGraphics_CopyBlitToFrame()
        {
            this->m_pPipelineGraphics_CopyBlitToFrame = new GLESPipelineGraphicsCopyBlitToFrame("PipelineGraphics-CopyBlitToFrame");
            String descriptorSetLayout = "CopyBlitObjectConstants-TextureFrameColor";
            DescriptorSetLayout* pDescriptorSetLayout_Blit = FindDescriptorSetLayout_Internal(descriptorSetLayout);

            F_Assert(pDescriptorSetLayout_Blit != nullptr && "OpenGLESWindow::createPipelineGraphics_CopyBlitToFrame")

            String nameShaderVert = "vert_standard_copy_blit_to_frame";
            String nameShaderFrag = "frag_standard_copy_blit_to_frame";
            GLESShader* pShaderVertex = FindShader_Internal(nameShaderVert);
            GLESShader* pShaderFrag = FindShader_Internal(nameShaderFrag);
            F_Assert(pShaderVertex && pShaderFrag && "OpenGLESWindow::createPipelineGraphics_CopyBlitToFrame");

            Mesh* pMeshBlit = FindMesh_Internal("quad");
            F_Assert(pMeshBlit && "OpenGLESWindow::createPipelineGraphics_CopyBlitToFrame");
            if (!this->m_pPipelineGraphics_CopyBlitToFrame->Init(pDescriptorSetLayout_Blit,
                                                                 pShaderVertex,
                                                                 pShaderFrag,
                                                                 pMeshBlit))
            {
                F_LogError("*********************** OpenGLESWindow::createPipelineGraphics_CopyBlitToFrame: PipelineGraphics_CopyBlitToFrame->Init failed !");
                return;
            }
            F_LogInfo("OpenGLESWindow::createPipelineGraphics_CopyBlitToFrame: [PipelineGraphics_CopyBlitToFrame] create success !");
        }
    void OpenGLESWindow::UpdateDescriptorSets_Graphics_CopyBlitToFrame()
    {
        this->m_pPipelineGraphics_CopyBlitToFrame->UpdateDescriptorSets();
    }
    void OpenGLESWindow::UpdateBuffer_Graphics_CopyBlitToFrame()
    {
        this->m_pPipelineGraphics_CopyBlitToFrame->UpdateBuffer();
    }   
    void OpenGLESWindow::UpdateBuffer_Graphics_CopyBlitToFrame(const CopyBlitObjectConstants& object)
    {
        this->m_pPipelineGraphics_CopyBlitToFrame->UpdateBuffer(object);
    }   
    void OpenGLESWindow::Draw_Graphics_CopyBlitToFrame(GLESFrameBuffer* pFrameBuffer)
    {
        if (pFrameBuffer == nullptr)
            return;

		this->m_pPipelineGraphics_CopyBlitToFrame->pStatePipelineGraphics->BindState();
		this->m_pPipelineGraphics_CopyBlitToFrame->pStatePipelineGraphics->BindShader();
		this->m_pPipelineGraphics_CopyBlitToFrame->pStatePipelineGraphics->BindBufferUniforms();
		GLESTexture* pTexture = pFrameBuffer->GetColorTexture(0);
		pTexture->BindTexture(0, true);
		
		Mesh* pMesh = this->m_pPipelineGraphics_CopyBlitToFrame->pMeshBlit;
		MeshSub* pMeshSub = pMesh->aMeshSubs[0];
		pMeshSub->pBufferVertexIndex->BindVertexArray();
		drawIndexed(GL_TRIANGLES, (int)pMeshSub->poIndexCount, GL_UNSIGNED_INT, 0);
		this->m_pPipelineGraphics_CopyBlitToFrame->pStatePipelineGraphics->UnBindState();
        pTexture->BindTexture(0, false);
    }

        void OpenGLESWindow::createPipelineGraphics_DepthShadowMap()
        {

        }

        void OpenGLESWindow::createPipelineGraphics_DepthHiz()
        {

        }

        void OpenGLESWindow::createPipelineGraphics_Terrain()
        {
            
        }


	Mesh* OpenGLESWindow::CreateMesh(const MeshInfo* pMI)
	{
		Mesh* pMesh = new Mesh(0,
							   pMI->nameMesh,
							   pMI->pathMesh,
							   pMI->typeMesh,
							   pMI->typeVertex,
							   pMI->typeGeometryType,
							   pMI->pMeshCreateParam);
		if (!pMesh->LoadMesh(pMI->isFlipY, pMI->isTransformLocal, pMI->matTransformLocal))
		{
			String msg = "*********************** OpenGLESWindow::CreateMesh: create mesh: name: [" + pMI->nameMesh + "], path: [" + pMI->pathMesh + "] failed !";	
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg);
		}

		F_LogInfo("OpenGLESWindow::CreateMesh: create mesh, name: [%s], path: [%s] success !", 
				  pMI->nameMesh.c_str(), pMI->pathMesh.c_str());
		return pMesh;
	}
	void OpenGLESWindow::CreateMeshes(const MeshInfoPtrVector& aMIs, MeshPtrVector& aMeshes, MeshPtrMap& mapMeshes)
	{
		size_t count = aMIs.size();
		for (size_t i = 0; i < count; i++)
		{
			Mesh* pMesh = CreateMesh(aMIs[i]);
			if (pMesh != nullptr)
			{
				aMeshes.push_back(pMesh);
				mapMeshes[pMesh->GetName()] = pMesh;
			}
		}
	}

	GLESShader* OpenGLESWindow::CreateShader(const ShaderModuleInfo& si)
	{ 
		GLESShader* pShader = createShader(si.nameShader, si.pathShader, si.nameShaderType);
		if (pShader == nullptr)
		{
			String msg = "*********************** OpenGLESWindow::CreateShader: create shader: name: [" + si.nameShader + "], path: [" + si.pathShader + "], type: [" + si.nameShaderType + "] failed !";
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg);
		}
		
		F_LogInfo("OpenGLESWindow::CreateShader: create shader: name: [%s], path: [%s], type: [%s] success !", 
				  si.nameShader.c_str(), si.pathShader.c_str(), si.nameShaderType.c_str());
		return pShader;
	}
	void OpenGLESWindow::CreateShaders(const ShaderModuleInfoVector& aSIs, GLESShaderPtrVector& aShaders, GLESShaderPtrMap& mapShaders)
	{
		size_t count = aSIs.size();
		for (size_t i = 0; i < count; i++)
		{
			const ShaderModuleInfo& si = aSIs[i];
			GLESShader* pShader = CreateShader(si);
			if (pShader != nullptr)
			{
				aShaders.push_back(pShader);
				mapShaders[si.nameShader] = pShader;
			}
		}
	}

    /////////////////////////// OpenGLESWindow //////////////////////
    const String OpenGLESWindow::c_strShaderProgram = "ShaderProgram";
    FDynamicLib* OpenGLESWindow::s_poDynLoader_EGL = nullptr;
    OpenGLESWindow::OpenGLESWindow(String name)
        : OpenGLESBase(name)

        , poDebug(nullptr)
        , poShaderInclude(nullptr)
        , poMSAASamples(1)

        , poSwapChainImageFormat(GL_RGBA)
        , poDepthImageFormat(GL_DEPTH24_STENCIL8)

		, poWindowContentScale(1.0f, 1.0f)

        , poTextureColor(nullptr)
        , poRenderBufferDepthStencil(nullptr)
        , poRenderPass(nullptr)
        , poCurrentFrame(0)
        , poSwapChainImageIndex(0)

        , poVertexCount(0)
        , poVertexBuffer_Size(0)
        , poVertexBuffer_Data(nullptr)
        , poIndexCount(0)
        , poIndexBuffer_Size(0)
        , poIndexBuffer_Data(nullptr)
        , pBufferVertex(nullptr)
        , pBufferVertexIndex(nullptr)

		, poTypePrimitive(GL_TRIANGLES)
		, poIsCull(true)
        , poTypeFrontFace(GL_CW)
        , poTypeCulling(GL_BACK)
        , poTypePolygonMode(0)

		, poDepthEnabled(false)
		, poDepthFuncCompare(GL_LEQUAL)
		, poDepthTestEnabled(false)
		, poDepthWriteEnabled(false)

		, poStencilEnabled(false)
		, poStencil_CompareFunction(GL_LEQUAL)
		, poStencil_StencilFailureOp(GL_KEEP)
		, poStencil_DepthFailureOp(GL_KEEP)
		, poStencil_DepthStencilPassOp(GL_KEEP)
		, poStencil_Ref(0)
		, poStencil_Mask(0)
		
		, poBlendEnabled(false)
		, poBlendColorFactorSrc(GL_ONE)
		, poBlendColorFactorDst(GL_ZERO)
		, poBlendColorOp(GL_FUNC_ADD)
		, poBlendAlphaFactorSrc(GL_ONE)
		, poBlendAlphaFactorDst(GL_ZERO)
		, poBlendAlphaOp(GL_FUNC_ADD)

		, poColorWriteMask_Red(true)
		, poColorWriteMask_Green(true)
		, poColorWriteMask_Blue(true)
		, poColorWriteMask_Alpha(true)

		, poStatePipelineGraphics(nullptr)
        , poTypeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
        , poShaderVertex(nullptr)
        , poShaderFragment(nullptr)

        , poDescriptorSetLayoutName("")
        , pDescriptorSetLayout(nullptr)

        , poTexture(nullptr)
        

        , cfg_colorBackground(FMath::ToLinear(FColor(0.0f, 0.2f, 0.4f, 1.0f)))

        , cfg_isRenderPassShadowMap(false)
        , cfg_isRenderPassDefaultCustom(false)
		, cfg_isDepthStencil(false)

        , cfg_isMSAA(false)
        , cfg_isImgui(false)
        , cfg_isWireFrame(false)
        , cfg_isRotate(false)


		, cfg_isUseComputeShaderBeforeRender(false)
        , cfg_isUseComputeShaderAfterRender(false)


        , cfg_cameraPos(0.0f, 0.0f, -5.0f)
        , cfg_cameraLookTarget(0.0f, 0.0f, 0.0f)
        , cfg_cameraUp(0.0f, 1.0f, 0.0f)
        , cfg_cameraFov(45.0f)
        , cfg_cameraNear(0.05f)
        , cfg_cameraFar(1000.0f)
        , cfg_cameraSpeedMove(1000.0f)
        , cfg_cameraSpeedZoom(0.1f)
        , cfg_cameraSpeedRotate(0.1f)


        , cfg_model_Path("")
        , cfg_shaderVertex_Path("")
        , cfg_shaderFragment_Path("")
        , cfg_texture_Path("")
        , cfg_terrain_Path("")
        , cfg_terrainTextureDiffuse_Path("Texture/Terrain/shore_sand_albedo.png;Texture/Terrain/moss_albedo.png;Texture/Terrain/rock_cliff_albedo.png;Texture/Terrain/cliff_albedo.png")
        , cfg_terrainTextureNormal_Path("Texture/Terrain/shore_sand_norm.png;Texture/Terrain/moss_norm.tga;Texture/Terrain/rock_cliff_norm.tga;Texture/Terrain/cliff_norm.png")
        , cfg_terrainTextureControl_Path("Texture/Terrain/terrain_control.png")
        , cfg_terrainHeightStart(0.0f)
        , cfg_terrainHeightMax(200.0f)


        , imgui_IsEnable(false)
        , imgui_MinimalSwapchainImages(0)
        , imgui_PathIni("")
        , imgui_PathLog("")

        //Internal

        , m_pPipelineGraphics_CopyBlitToFrame(nullptr)


        , pCamera(nullptr)
        , pCameraRight(nullptr)
        , pCameraMainLight(new FCamera)

        , mouseButtonDownLeft(false)
        , mouseButtonDownRight(false)
        , mouseButtonDownMiddle(false)

        , cfg_isEditorCreate(false)
        , cfg_isEditorGridShow(false)
        , cfg_isEditorCameraAxisShow(false)
        , cfg_isEditorCoordinateAxisShow(false)
        , cfg_editorGrid_Color(0.5f, 0.5f, 0.5f, 0.5f)
        , cfg_editorCoordinateAxis_MoveSpeed(0.01f)
        , cfg_editorCoordinateAxis_RotateSpeed(0.01f)
        , cfg_editorCoordinateAxis_ScaleSpeed(0.01f)
        , pEditorGrid(nullptr)
        , pEditorCameraAxis(nullptr)
        , pEditorCoordinateAxis(nullptr)
        , pEditorLineFlat2DCollector(nullptr)
        , pEditorLineFlat3DCollector(nullptr)
    {

        Base::ms_pWindow = this;
    }

    OpenGLESWindow::~OpenGLESWindow()
    {
        F_DELETE(pCamera)
        F_DELETE(pCameraRight)
        F_DELETE(pCameraMainLight)
        if (s_poDynLoader_EGL != nullptr)
        {
            s_poDynLoader_EGL->Unload();
            delete s_poDynLoader_EGL;
        }
        s_poDynLoader_EGL = nullptr;
    }

    void OpenGLESWindow::OnInit(int w, int h)
    {
        resizeWindow(w, h, true);
        createPipeline();
    }

    void OpenGLESWindow::OnLoad()
    {
        loadAssets();
    }

    bool OpenGLESWindow::OnIsInit()
    {
        return this->isCreateDevice &&
            this->isLoadAsset;
    }

    void OpenGLESWindow::OnResize(int w, int h, bool force)
    {
		if (this->width != w || this->height != h)
		{
			recreateSwapChain();
		}
        resizeWindow(w, h, force);
		createViewport();

        if (this->pCamera != nullptr)
        {
            this->pCamera->PerspectiveLH(this->cfg_cameraFov, this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
        }
    }

    bool OpenGLESWindow::OnBeginCompute_BeforeRender()
    {
        return beginCompute_BeforeRender();
    }
        void OpenGLESWindow::OnUpdateCompute_BeforeRender()
        {
            updateCompute_BeforeRender();
        }
        void OpenGLESWindow::OnCompute_BeforeRender()
        {
            compute_BeforeRender();
        }
    void OpenGLESWindow::OnEndCompute_BeforeRender()
    {
        endCompute_BeforeRender();
    }


    bool OpenGLESWindow::OnBeginRender()
    {
        return beginRender();
    }
        void OpenGLESWindow::OnUpdateRender()
        {
            updateRender();
        }
        void OpenGLESWindow::OnRender()
        {
            render();
        }
    void OpenGLESWindow::OnEndRender()
    {
        endRender();
    }


    bool OpenGLESWindow::OnBeginCompute_AfterRender()
    {
        return beginCompute_AfterRender();
    }
        void OpenGLESWindow::OnUpdateCompute_AfterRender()
        {
            updateCompute_AfterRender();
        }
        void OpenGLESWindow::OnCompute_AfterRender()
        {
            compute_AfterRender();
        }
    void OpenGLESWindow::OnEndCompute_AfterRender()
    {
        endCompute_AfterRender();
    }


    void OpenGLESWindow::OnPresent()
    {

        this->poCurrentFrame = (this->poCurrentFrame + 1) % s_maxFramesInFight;
    }
    void OpenGLESWindow::OnDestroy()
    {
        //1> Wait
        if (this->isCreateDevice)
        {

            //2> Cleanup
            cleanup();
        }
    }

    void OpenGLESWindow::OnMouseInput()
    {   
        // double cursorX; double cursorY;
        // glfwGetCursorPos(this->pWindow, &cursorX, &cursorY);
        // cursorX *= this->poWindowContentScale.x;
        // cursorY *= this->poWindowContentScale.y;

        // //Mouse Left
        // int actionLeft = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_LEFT);
        // if (actionLeft == GLFW_PRESS)
        // {
        //     if (!this->mouseButtonDownLeft)
        //     {
        //         OnMouseLeftDown(cursorX, cursorY);
        //     }
        //     else
        //     {
        //         OnMouseMove(GLFW_MOUSE_BUTTON_LEFT, cursorX, cursorY);
        //     }
        // }
        // else
        // {
        //     if (this->mouseButtonDownLeft)
        //     {
        //         OnMouseLeftUp(cursorX, cursorY);
        //     }
        // }
        
        // //Mouse Right
        // int actionRight = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_RIGHT);
        // if (actionRight == GLFW_PRESS)
        // {
        //     if (!this->mouseButtonDownRight)
        //     {
        //         OnMouseRightDown(cursorX, cursorY);
        //     }
        //     else
        //     {
        //         OnMouseMove(GLFW_MOUSE_BUTTON_RIGHT, cursorX, cursorY);
        //     }
        // }
        // else
        // {
        //     if (this->mouseButtonDownRight)
        //     {
        //         OnMouseRightUp(cursorX, cursorY);
        //     }
        // }

        // //Mouse Middle
        // int actionMiddle = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_MIDDLE);
        // if (actionMiddle == GLFW_PRESS)
        // {
        //     if (!this->mouseButtonDownMiddle)
        //     {
        //         OnMouseMiddleDown(cursorX, cursorY);
        //     }
        //     else
        //     {
        //         OnMouseMove(GLFW_MOUSE_BUTTON_MIDDLE, cursorX, cursorY);
        //     }
        // }
        // else
        // {
        //     if (this->mouseButtonDownMiddle)
        //     {
        //         OnMouseMiddleUp(cursorX, cursorY);
        //     }
        // }

        // //Mouse Hover
        // if (!this->mouseButtonDownLeft &&
        //     !this->mouseButtonDownRight &&
        //     !this->mouseButtonDownMiddle)
        // {
        //     OnMouseHover(cursorX, cursorY);
        // }

        // this->mousePosLast.x = (float)cursorX;
        // this->mousePosLast.y = (float)cursorY;
    }
    void OpenGLESWindow::OnMouseLeftDown(double x, double y)
    {
        this->mouseButtonDownLeft = true;
        {
            OnEditorCoordinateMouseLeftDown(x, y);
        }
    }
    void OpenGLESWindow::OnMouseLeftUp(double x, double y)
    {   
        this->mouseButtonDownLeft = false;

        OnEditorCoordinateMouseLeftUp(x, y);
    }
    void OpenGLESWindow::OnMouseRightDown(double x, double y)
    {
        this->mouseButtonDownRight = true;
        {

        }
    }
    void OpenGLESWindow::OnMouseRightUp(double x, double y)
    {
        this->mouseButtonDownRight = false;

    }
    void OpenGLESWindow::OnMouseMiddleDown(double x, double y)
    {
        this->mouseButtonDownMiddle = true;
        {

        }
    }
    void OpenGLESWindow::OnMouseMiddleUp(double x, double y)
    {
        this->mouseButtonDownMiddle = false;

    }   
    void OpenGLESWindow::OnMouseMove(int button, double x, double y)
    {
        // if (button == GLFW_MOUSE_BUTTON_LEFT)
        // {
        //     OnEditorCoordinateMouseMove(x, y);

        // }
        // else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        // {
        //     OnCameraMouseMoveProcess(x, y, this->mousePosLast.x, this->mousePosLast.y);
            
        // }
        // else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        // {
            
        // }
    }
    void OpenGLESWindow::OnMouseHover(double x, double y)
    {
        OnEditorCoordinateMouseHover(x, y);

    }
    void OpenGLESWindow::OnMouseWheel(double x, double y)
    {
        OnCameraMouseZoomProcess(y);

    }

    void OpenGLESWindow::OnKeyboardInput()
    {
        OnCameraMouseKeyboardProcess();

    }
    void OpenGLESWindow::OnKeyDown(int key)
    {
        // if (key == GLFW_KEY_ESCAPE)
        // {
        //     glfwSetWindowShouldClose(this->pWindow, true);
        // }
        // if (key == GLFW_KEY_R)
        // {
        //     cameraReset();
        // }
        // if (key == GLFW_KEY_T)
        // {
        //     this->cfg_isRotate = !this->cfg_isRotate;
        // }
        // if (key == GLFW_KEY_F)
        // {
        //     this->cfg_isWireFrame = !this->cfg_isWireFrame;
        //     SetIsWireFrame(this->cfg_isWireFrame);
        // }
    }
    void OpenGLESWindow::OnKeyUp(int key)
    {

    }

    void OpenGLESWindow::OnCameraMouseMoveProcess(double newX, double newY, double oldX, double oldY)
    {
        if (this->pCamera != nullptr)
        {
            float fX = static_cast<float>(newX - oldX);
            float fY = static_cast<float>(newY - oldY);
            if (fX != 0 || fY != 0)
            {
                float fRotYAngle = fX * this->cfg_cameraSpeedRotate;
                float fRotXAngle = fY * this->cfg_cameraSpeedRotate;
                FVector3 vEulerAngles = pCamera->GetEulerAngles();
                vEulerAngles.x += fRotXAngle;
                vEulerAngles.y += fRotYAngle;
                vEulerAngles.z = 0;
                pCamera->SetEulerAngles(vEulerAngles);
            }
        }
    }
    void OpenGLESWindow::OnCameraMouseZoomProcess(double zoom) 
    {
        if (this->pCamera != nullptr)
        {
            float fDis = (float)(this->cfg_cameraSpeedZoom * zoom);
            this->pCamera->Walk(fDis);
        }
    }
    void OpenGLESWindow::OnCameraMouseKeyboardProcess()
    {
        if (this->pCamera != nullptr)
        {
            // float speedMove = this->cfg_cameraSpeedMove;
            // if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            //     glfwGetKey(this->pWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) 
            // {
            //     speedMove *= 10.0f;
            // }

            // float timeDelta = this->pTimer->GetTimeDelta();
            // if (glfwGetKey(this->pWindow, GLFW_KEY_W) == GLFW_PRESS)
            // {
            //     this->pCamera->Walk(speedMove * timeDelta);
            // }
            // if (glfwGetKey(this->pWindow, GLFW_KEY_S) == GLFW_PRESS)
            // {
            //     this->pCamera->Walk(- speedMove * timeDelta);
            // }
            // if (glfwGetKey(this->pWindow, GLFW_KEY_A) == GLFW_PRESS)
            // {
            //     this->pCamera->Strafe(- speedMove * timeDelta);
            // }
            // if (glfwGetKey(this->pWindow, GLFW_KEY_D) == GLFW_PRESS)
            // {
            //     this->pCamera->Strafe(speedMove * timeDelta);
            // }
            this->pCamera->UpdateViewMatrix();
        }
    }

    void OpenGLESWindow::OnEditorCoordinateMouseLeftDown(double x, double y)
    {

    }
    void OpenGLESWindow::OnEditorCoordinateMouseMove(double x, double y)
    {

    }
    void OpenGLESWindow::OnEditorCoordinateMouseLeftUp(double x, double y)
    {

    }
    void OpenGLESWindow::OnEditorCoordinateMouseHover(double x, double y)
    {

    }   

    bool OpenGLESWindow::HasConfig_RenderPassDefaultCustom()
    {
        return this->cfg_isRenderPassDefaultCustom;
    }
	bool OpenGLESWindow::HasConfig_DepthStencil()
	{
		return this->cfg_isDepthStencil;
	}
    bool OpenGLESWindow::HasConfig_MASS()
    {
        return this->cfg_isMSAA;
    }
    bool OpenGLESWindow::HasConfig_Imgui()
    {
        return this->cfg_isImgui;
    }

    bool OpenGLESWindow::IsEnable_MASS()
    {
        return this->cfg_isMSAA;
    }
    bool OpenGLESWindow::IsEnable_Imgui()
    {
        return this->cfg_isImgui &&
               this->imgui_IsEnable;
    }


    void OpenGLESWindow::SetIsWireFrame(bool isWireFrame)
    {
        this->cfg_isWireFrame = isWireFrame;
        // if (isWireFrame)
        //     this->cfg_glPolygonMode = GL_LINE;
        // else
        //     this->cfg_glPolygonMode = GL_FILL;
    }


    void OpenGLESWindow::createPipeline()
    {
        F_LogInfo("**********<1> OpenGLESWindow::createPipeline start **********");
        {
            //1> Create Resize callback
            createWindowCallback();
        
            //2> Create Device
            createDevice();

            //3> Create Feature Support
            createFeatureSupport();

            //4> Create Command Objects
            createCommandObjects();

            //5> createInternal/createResourceInternal
            createInternal();
            createResourceInternal();

            //6> Create Swap Chain Objects
            createSwapChainObjects();

            //7> Camera/Light/Shadow/Terrain
            createCamera();
            createLightMain();
            createShadowLightMain();
            createTerrain();

            //8> Create Pipeline Objects
            createPipelineObjects();

            //9> Create Sync Objects
			createSyncObjects();

            //10> createDescriptorSetLayouts
            createDescriptorSetLayouts();

            //11> isCreateDevice
            this->isCreateDevice = true;
        }
        F_LogInfo("**********<1> OpenGLESWindow::createPipeline finish **********");
    }

    void OpenGLESWindow::createWindowCallback()
    {   
        

        F_LogInfo("*****<1-1> OpenGLESWindow::createWindowCallback finish *****");
    }
        void* OpenGLESWindow::GetEGLSymbol(const char* name)
        {
            void* result = s_poDynLoader_EGL->GetSymbol(name);
            if (result == nullptr) 
            {
                result = (void*)eglGetProcAddress(name);
            }
            return result;
        }
    void OpenGLESWindow::createDevice()
    {
        F_LogInfo("*****<1-2> OpenGLESWindow::createDevice start *****");
        {
            //1> libEGL
            String strlibEGL = "libEGL.so";
            s_poDynLoader_EGL = new FDynamicLib(strlibEGL, strlibEGL);
            if (!s_poDynLoader_EGL->Load())
            {
                F_LogError("*********************** OpenGLESWindow::createDevice: load [libEGL.so] failed !");
                exit(0);
            }
            F_LogInfo("OpenGLESWindow::createDevice: load [libEGL.so] success !");

            //2> glad load all OpenGLES function pointers 
            if (!gladLoadGLES2Loader((GLADloadproc)GetEGLSymbol)) 
            {
                F_LogError("*********************** OpenGLESWindow::createDevice: gladLoadGLES2Loader load failed !");
                s_poDynLoader_EGL->Unload();
                exit(0);
            }
            F_LogInfo("OpenGLESWindow::createDevice: gladLoadGLES2Loader load success !");
            this->poDebug = new GLESDebug();
            this->poDebug->Init();
            this->poShaderInclude = new GLESShaderInclude();
            this->poShaderInclude->Init();

            //3> setUpDebugMessenger
            setUpDebugMessenger();

            //3> createSurface
            createSurface();

            //4> pickPhysicalDevice
            pickPhysicalDevice();

            //5> createLogicalDevice
            createLogicalDevice();
        }
        F_LogInfo("*****<1-2> OpenGLESWindow::createDevice finish *****");
    }
        void APIENTRY glDebugOutput(GLenum source, 
                                    GLenum type, 
                                    unsigned int id, 
                                    GLenum severity, 
                                    GLsizei length, 
                                    const char *message, 
                                    const void *userParam)
        {
            if (id <= 0)
                return;
            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) 
                return; // ignore these non-significant error codes

            String prefix("");
            String msg = "Debug message (" + FUtilString::SaveUInt(id) + "): " + message;

            switch (source)
            {
                case GL_DEBUG_SOURCE_API:             prefix = "Source: API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   prefix = "Source: Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: prefix = "Source: Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     prefix = "Source: Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION:     prefix = "Source: Application"; break;
                case GL_DEBUG_SOURCE_OTHER:           prefix = "Source: Other"; break;
            }

            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               prefix = "Type: Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: prefix = "Type: Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  prefix = "Type: Undefined Behaviour"; break; 
                case GL_DEBUG_TYPE_PORTABILITY:         prefix = "Type: Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         prefix = "Type: Performance"; break;
                case GL_DEBUG_TYPE_MARKER:              prefix = "Type: Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          prefix = "Type: Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP:           prefix = "Type: Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER:               prefix = "Type: Other"; break;
            }
            
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:         prefix = "Severity: high"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       prefix = "Severity: medium"; break;
                case GL_DEBUG_SEVERITY_LOW:          prefix = "Severity: low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: prefix = "Severity: notification"; break;
            } 

            F_LogInfo("[%s]: [%s]", prefix.c_str(), msg.c_str());
        }
        void OpenGLESWindow::setUpDebugMessenger()
        {
            if (s_isEnableValidationLayers)
            {
                int flags; 
                glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
                if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
                {
                    setEnable(GL_DEBUG_OUTPUT, true);
                    setEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS, true); //makes sure errors are displayed synchronously
                    glDebugMessageCallback(glDebugOutput, nullptr);
                    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
                }
            }
            
            F_LogInfo("<1-1-2> OpenGLESWindow::setUpDebugMessenger finish !");
        }
            bool OpenGLESWindow::isExtensionSupported(const char* extension)
            {
                GLint numExtensions = 0;
                glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
                for (int i = 0; i < numExtensions; i++) 
                {
                    const GLubyte* extenName = glGetStringi(GL_EXTENSIONS, i);
                    if (strcmp((const char*)extenName, extension) == 0) 
                    {
                        return true;
                    }
                }
                return false;
            }
        void OpenGLESWindow::createSurface()
        {

            F_LogInfo("<1-2-3> OpenGLESWindow::createSurface finish !");
        }
        void OpenGLESWindow::pickPhysicalDevice()
        {
            F_LogInfo("**************** OpenGLESWindow::pickPhysicalDevice: GL PhysicalDeviceProperties ****************");
            {
                F_LogInfo("  VendorName: [%s]", glGetString(GL_VENDOR));
                F_LogInfo("  RendererName: [%s]", glGetString(GL_RENDERER));
                F_LogInfo("  OpenGLVersion: [%s]", glGetString(GL_VERSION));
                F_LogInfo("  GLSLVersion: [%s]", glGetString(GL_SHADING_LANGUAGE_VERSION));

                GLint size2D = 0;
                GLint size3D = 0;
                GLint sizeCubeMap = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size2D);
                glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &size3D);
                glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &sizeCubeMap);
                F_LogInfo("  maxImageDimension1D: [%d]", size2D);
                F_LogInfo("  maxImageDimension2D: [%d]", size2D);
                F_LogInfo("  maxImageDimension3D: [%d]", size3D);
                F_LogInfo("  maxImageDimensionCube: [%d]", sizeCubeMap);
                
                
            }
            F_LogInfo("**************** OpenGLESWindow::pickPhysicalDevice: GL PhysicalDeviceProperties ****************");


            F_LogInfo("**************** OpenGLESWindow::pickPhysicalDevice: GL Extensions ****************");
            GLint numExtensions = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
            for (int i = 0; i < numExtensions; i++) 
            {
                const GLubyte* extenName = glGetStringi(GL_EXTENSIONS, i);
                F_LogInfo("  Extension: Index: [%d], Name: [%s] !", i, extenName);
            }
            F_LogInfo("**************** OpenGLESWindow::pickPhysicalDevice: GL Extensions ****************");


            F_LogInfo("**************** OpenGLESWindow::pickPhysicalDevice: GL Features ****************");
            {

            }
            F_LogInfo("**************** OpenGLESWindow::pickPhysicalDevice: GL Features ****************");


            F_LogInfo("<1-2-4> OpenGLESWindow::pickPhysicalDevice finish !");
        }
        void OpenGLESWindow::setUpEnabledFeatures()
        {

        }
        void OpenGLESWindow::createLogicalDevice()
        {

            F_LogInfo("<1-2-5> OpenGLESWindow::createLogicalDevice finish !");
        }


    void OpenGLESWindow::createFeatureSupport()
    {

    }

    void OpenGLESWindow::createCamera()
    {
        if (this->pCamera == nullptr)
        {
            this->pCamera = new FCamera();
        }
    }
    void OpenGLESWindow::createLightMain()
    {

    }
    void OpenGLESWindow::createShadowLightMain()
    {

    }
    void OpenGLESWindow::createTerrain()
    {

    }


    void OpenGLESWindow::createCommandObjects()
    {
        F_LogInfo("*****<1-4> OpenGLESWindow::createCommandObjects start *****");
        {
            


        }
        F_LogInfo("*****<1-4> OpenGLESWindow::createCommandObjects finish *****");
    }


    void OpenGLESWindow::createSwapChainObjects()
    {
        F_LogInfo("*****<1-6> OpenGLESWindow::createSwapChainObjects start *****");
        {
            //1> createSwapChain
            createSwapChain();

            //2> createSwapChainImageViews
            createSwapChainImageViews();

            //3> createColorResources
            if (HasConfig_MASS())
            {
                createColorResources();
            }

            //4> createDepthStencilResources
			if (HasConfig_DepthStencil())
			{
				createDepthStencilResources();
			}

            //5> createColorResourceLists
            createColorResourceLists();
        }
        F_LogInfo("*****<1-6> OpenGLESWindow::createSwapChainObjects finish *****");
    }
        void OpenGLESWindow::createSwapChain()
        {
            //1> Default Framebuffer Color/Depth format

            //2> Framebuffer
			refreshFramebufferSize(this->width, this->height);

			//3> createViewport
			createViewport();

            
            F_LogInfo("<1-5-1> OpenGLESWindow::createSwapChain finish, Swapchain size: [%f, %f], window size: [%d, %d], scale: [%f, %f], format color: [%d], format depth stencil: [%d] !", 
					  this->poFramebufferSize.x, this->poFramebufferSize.y, 
					  this->width, this->height, 
					  this->poWindowContentScale.x, this->poWindowContentScale.y, 
					  this->poSwapChainImageFormat, this->poDepthImageFormat);
        }
            void OpenGLESWindow::createViewport()
            {
                int w = (int)this->poFramebufferSize.x;
                int h = (int)this->poFramebufferSize.y;

                this->poOffset.x = 0;
                this->poOffset.y = 0;
                this->poExtent.x = w;
                this->poExtent.y = h;

                this->poViewport.left = 0;
                this->poViewport.top = 0;
                this->poViewport.right = w;
                this->poViewport.bottom = h;

                this->poScissor.left = 0;
                this->poScissor.top = 0;
                this->poScissor.right = w;
                this->poScissor.bottom = h;

				F_LogInfo("OpenGLESWindow::createViewport: Viewport change to: WidthHeight: [%d - %d], FrameBuffer: [%f - %f] !", 
						  this->width, this->height,
						  this->poFramebufferSize.x, this->poFramebufferSize.y);
            }
			void OpenGLESWindow::createViewport(uint32_t width,
												uint32_t height,
												FRectI& poViewport, 
												FRectI& poScissor)
			{
				poViewport.left = 0;
				poViewport.top = 0;
				poViewport.right = (int)width;
				poViewport.bottom = (int)height;

				poScissor.left = 0;
				poScissor.top = 0;
				poScissor.right = (int)width;
				poScissor.bottom = (int)height;
			}
        void OpenGLESWindow::createSwapChainImageViews()
        {
            int count_swapchain = s_maxFramesInFight;

            StringVector aPathTexture;
            int w = (int)this->poFramebufferSize.x;
            int h = (int)this->poFramebufferSize.y;
            this->poSwapChains.resize(count_swapchain);
            for (int i = 0; i < count_swapchain; i++)
            {
                String nameSwapChain = "Texture-SwapChain-" + FUtilString::SaveInt(i);
                GLESTexture* pTexture = createTexture(nameSwapChain,
                                                      aPathTexture,
                                                      nullptr,
                                                      4,
                                                      w,
                                                      h,
                                                      0,
                                                      F_Texture_2D,
                                                      F_TexturePixelFormat_R8G8B8A8_SRGB,
                                                      F_TextureAddressing_Wrap,
                                                      F_TextureFilter_Bilinear,
                                                      F_TextureFilter_Bilinear,
                                                      F_MSAASampleCount_1_Bit,
                                                      FColor(0, 0, 0, 1),
                                                      true,
                                                      true,
                                                      false,
                                                      true,
                                                      false,
                                                      FMath::ms_clBlack);
                if (pTexture == nullptr)
                {
                    F_LogError("*********************** OpenGLESWindow::createSwapChainImageViews: Failed to create texture, name: [%s] !", nameSwapChain.c_str());
                    F_DELETE(pTexture)
                    return;
                }
                this->poSwapChains[i] = pTexture;
            }
        }
            void OpenGLESWindow::createColorResources()
            {
                StringVector aPathTexture;
                int w = (int)this->poFramebufferSize.x;
                int h = (int)this->poFramebufferSize.y;
                String nameColor = "Texture-Color";
                this->poTextureColor = createTexture(nameColor,
													 aPathTexture,
													 nullptr,
													 4,
													 w,
													 h,
													 0,
													 F_Texture_2D,
													 F_TexturePixelFormat_R8G8B8A8_SRGB,
													 F_TextureAddressing_Wrap,
													 F_TextureFilter_Bilinear,
													 F_TextureFilter_Bilinear,
													 F_MSAASampleCount_1_Bit,
													 FColor(0, 0, 0, 1),
													 true,
													 true,
													 false,
													 true,
													 false,
                                                     FMath::ms_clBlack);
                if (this->poTextureColor == nullptr)
                {
                    F_LogError("*********************** OpenGLESWindow::createColorResources: Failed to create texture, name: [%s] !", nameColor.c_str());
                    return;
                }
            }
            void OpenGLESWindow::createDepthStencilResources()
            {
                StringVector aPathTexture;
                int w = (int)this->poFramebufferSize.x;
                int h = (int)this->poFramebufferSize.y;
                String nameDepthStencil = "Texture-DepthStencil";
                this->poRenderBufferDepthStencil = createRenderBuffer(nameDepthStencil,
																	  w,
																	  h,
																	  GL_DEPTH24_STENCIL8,
																	  GL_DEPTH_STENCIL_ATTACHMENT,
																	  0);
                if (this->poRenderBufferDepthStencil == nullptr)
                {
                    F_LogError("*********************** OpenGLESWindow::createDepthStencilResources: Failed to create texture, name: [%s] !", nameDepthStencil.c_str());
                    return;
                }
            }
        void OpenGLESWindow::createColorResourceLists()
        {

        }

    void OpenGLESWindow::createDescriptorSetLayouts()
    {
        F_LogInfo("*****<1-10> OpenGLESWindow::createDescriptorSetLayouts start *****");
        {
            //1> createDescriptorSetLayout_Default
            createDescriptorSetLayout_Default();
            F_LogInfo("<1-10-1> OpenGLESWindow::createDescriptorSetLayouts: createDescriptorSetLayout_Default finish !");

            //3> createDescriptorSetLayout_Custom
            createDescriptorSetLayout_Custom();
            F_LogInfo("<1-10-2> OpenGLESWindow::createDescriptorSetLayouts: createDescriptorSetLayout_Custom finish !");
        }
        F_LogInfo("*****<1-10> OpenGLESWindow::createDescriptorSetLayouts finish *****");
    }
    void OpenGLESWindow::createDescriptorSetLayout_Default()
    {
        if (this->poDescriptorSetLayoutName.empty())
            return;

        this->pDescriptorSetLayout = new DescriptorSetLayout();
        this->pDescriptorSetLayout->Init(this->poDescriptorSetLayoutName);
    }
    void OpenGLESWindow::createDescriptorSetLayout_Custom()
    {
        
    }

    void OpenGLESWindow::createPipelineObjects()
    {
        F_LogInfo("*****<1-8> OpenGLESWindow::createPipelineObjects start *****");
        {
            //1> createRenderPasses
            createRenderPasses();
            F_LogInfo("<1-8-1> OpenGLESWindow::createPipelineObjects: Success to create RenderPasses !");

            //2> createFramebuffers
            createFramebuffers();
            F_LogInfo("<1-8-2> OpenGLESWindow::createPipelineObjects: Success to create Framebuffers !");
        }
        F_LogInfo("*****<1-8> OpenGLESWindow::createPipelineObjects finish *****");
    }
        void OpenGLESWindow::createRenderPasses()
        {
            createRenderPass_ShadowMap();
            createRenderPass_Default();
            createRenderPass_Cull();
            createRenderPass_Terrain();
            createRenderPass_Custom();
        }
            void OpenGLESWindow::createRenderPass_ShadowMap()
            {

            }
            void OpenGLESWindow::createRenderPass_Default()
            {
                if (HasConfig_RenderPassDefaultCustom())
                {
                    this->poRenderPass = createRenderPass_DefaultCustom();
                }
                else
                {
                    if (HasConfig_MASS())
                    {
                        this->poRenderPass = createRenderPass_ColorDepthMSAA(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poMSAASamples);
                    }
                    else
                    {
                        this->poRenderPass = createRenderPass_KhrDepth(this->poSwapChainImageFormat, this->poDepthImageFormat);
                    }
                }
            }
            void OpenGLESWindow::createRenderPass_Cull()
            {

            }
            void OpenGLESWindow::createRenderPass_Terrain()
            {

            }
            void OpenGLESWindow::createRenderPass_Custom()
            {

            }

                GLESRenderPass* OpenGLESWindow::createRenderPass_DefaultCustom()
                {
                    return nullptr;
                }
                GLESRenderPass* OpenGLESWindow::createRenderPass_KhrDepth(int formatSwapChain, int formatDepth)
                {
                    String nameRenderPass = "RenderPass-Default-KhrDepth";
                    GLESRenderPass* pRenderPass = new GLESRenderPass(nameRenderPass);
                    if (!pRenderPass->Init())
                    {
                        F_LogError("*********************** OpenGLESWindow::createRenderPass_KhrDepth: Failed to create RenderPass, name: [%s] !", nameRenderPass.c_str());
                        return nullptr;
                    }

                    return pRenderPass;
                }
                GLESRenderPass* OpenGLESWindow::createRenderPass_ColorDepthMSAA(int formatColor, int formatDepth, int formatSwapChain, int samples)
                {
                    String nameRenderPass = "RenderPass-Default-ColorDepthMSAA";
                    GLESRenderPass* pRenderPass = new GLESRenderPass(nameRenderPass);
                    if (!pRenderPass->Init())
                    {
                        F_LogError("*********************** OpenGLESWindow::createRenderPass_ColorDepthMSAA: Failed to create RenderPass, name: [%s] !", nameRenderPass.c_str());
                        return nullptr;
                    }

                    return pRenderPass;
                }

				GLESRenderPass* OpenGLESWindow::createRenderPass(String nameRenderPass,
															     GLESFrameBuffer* pFrameBuffer)
				{
					GLESRenderPass* pRenderPass = new GLESRenderPass(nameRenderPass);
                    if (!pRenderPass->Init())
                    {
                        F_LogError("*********************** OpenGLESWindow::createRenderPass: Failed to create RenderPass, name: [%s] !", nameRenderPass.c_str());
                        return nullptr;
                    }
					pRenderPass->SetFrameBuffer(pFrameBuffer);

                    return pRenderPass;
				}

        void OpenGLESWindow::createFramebuffers()
        {
            //1> createFramebuffer_Default
            createFramebuffer_Default();

            //2> createFramebuffer_Custom
            createFramebuffer_Custom();
        }
            void OpenGLESWindow::createFramebuffer_Default()
            {
                if (HasConfig_RenderPassDefaultCustom())
                {
                    createFramebuffer_DefaultCustom();
                }
                else
                {
                    int w = (int)this->poFramebufferSize.x;
                    int h = (int)this->poFramebufferSize.y;
                    size_t count = this->poSwapChains.size();
                    this->poFrameBuffers.resize(count);
                    for (size_t i = 0; i < count; i++)
                    {
                        GLESTexturePtrVector aColorTexture;
                        aColorTexture.push_back(this->poSwapChains[i]);
                        if (this->poTextureColor != nullptr)
                            aColorTexture.push_back(this->poTextureColor); 
                        String nameFrameBuffer = "FrameBuffer-" + FUtilString::SaveSizeT(i);
                        GLESFrameBuffer* pFrameBuffer = createFrameBuffer(nameFrameBuffer,
                                                                          w,
                                                                          h,
                                                                          aColorTexture,
                                                                          this->poRenderBufferDepthStencil,
                                                                          false,
                                                                          false);
                        if (pFrameBuffer == nullptr)
                        {
                            String msg = "*********************** OpenGLESWindow::createFramebuffer_Default: Failed to create framebuffer: " + nameFrameBuffer;
                            F_LogError("%s", msg.c_str());
                            throw std::runtime_error(msg);
                        }
                        this->poFrameBuffers[i] = pFrameBuffer;
                    }
                }

                F_LogInfo("OpenGLESWindow::createFramebuffer_Default: Success to create Framebuffer_Default !");
            }
            void OpenGLESWindow::createFramebuffer_Custom()
            {

            }
                void OpenGLESWindow::createFramebuffer_DefaultCustom()
                {

                }

            GLESRenderBuffer* OpenGLESWindow::createRenderBuffer(const String& nameRenderBuffer,
                                                                 int width,
                                                                 int height,
                                                                 GLenum format,
                                                                 GLenum attachment, 
                                                                 GLenum renderbuffertarget)
            {
                GLESRenderBuffer* pGLRenderBuffer = new GLESRenderBuffer(nameRenderBuffer);
                if (!pGLRenderBuffer->Init(width, 
                                           height,
                                           format,
                                           attachment,
                                           renderbuffertarget))
                {
                    F_LogError("*********************** OpenGLESWindow::createRenderBuffer: Create RenderBuffer failed, Name: [%s] !", nameRenderBuffer.c_str());
                    F_DELETE(pGLRenderBuffer)
                    return nullptr;
                }   

                return pGLRenderBuffer;
            }

            bool OpenGLESWindow::createGLRenderBuffer(const String& nameRenderBuffer,
                                                      int width,
                                                      int height,
                                                      GLenum format,
                                                      uint32& nRenderBufferID)
            {
                nRenderBufferID = 0;
                glGenRenderbuffers(1, &nRenderBufferID);
                if (nRenderBufferID <= 0)
                {
                    F_LogError("*********************** OpenGLESWindow::createGLRenderBuffer: Create RenderBuffer failed, id: [%d], name: [%s] !", nRenderBufferID, nameRenderBuffer.c_str());
                    return false;
                }
                glBindRenderbuffer(GL_RENDERBUFFER, nRenderBufferID);
                glRenderbufferStorage(GL_RENDERBUFFER, format, width, height); 
                
                this->poDebug->SetGLRenderBuffer(nRenderBufferID, nameRenderBuffer);
                return true;
            }
            void OpenGLESWindow::bindGLRenderBuffer(uint32 nRenderBufferID)
            {
                if (nRenderBufferID <= 0)
                {
                    F_LogError("*********************** OpenGLESWindow::bindGLRenderBuffer: RenderBuffer id: [%d] is not valid !", nRenderBufferID);
                    return;
                }
                glBindRenderbuffer(GL_RENDERBUFFER, nRenderBufferID);
            }
            void OpenGLESWindow::destroyGLRenderBuffer(uint32 nRenderBufferID)
            {
                if (nRenderBufferID <= 0)
                    return;
                glDeleteRenderbuffers(1, &nRenderBufferID);
            }


            GLESFrameBuffer* OpenGLESWindow::createFrameBuffer(const String& nameFrameBuffer,
                                                               int width,
                                                               int height,
                                                               const GLESTexturePtrVector& aColorTexture,
                                                               GLESRenderBuffer* pDepthStencil,
                                                               bool isDeleteColors /*= false*/,
                                                               bool isDeleteDepthStencil /*= false*/)
            {
                GLESFrameBuffer* pGLFrameBuffer = new GLESFrameBuffer(nameFrameBuffer);
                if (!pGLFrameBuffer->Init(width, 
                                          height,
                                          aColorTexture,
                                          pDepthStencil,
                                          isDeleteColors,
                                          isDeleteDepthStencil))
                {
                    F_LogError("*********************** OpenGLESWindow::createFrameBuffer: Create FrameBuffer failed, Name: [%s] !", nameFrameBuffer.c_str());
                    F_DELETE(pGLFrameBuffer)
                    return nullptr;
                }   

                return pGLFrameBuffer;
            }
            bool OpenGLESWindow::createGLFrameBuffer(const String& nameFrameBuffer, 
                                                     const UintType2UintIDMap& mapType2IDs,
                                                     uint32 nDepthStencilID,
                                                     uint32& nFrameBufferID)
            {
                nFrameBufferID = 0;
                glGenFramebuffers(1, &nFrameBufferID);
                if (nFrameBufferID <= 0)
                {
                    F_LogError("*********************** OpenGLESWindow::createGLFrameBuffer: Create FrameBuffer failed, id: [%d], name: [%s] !", nFrameBufferID, nameFrameBuffer.c_str());
                    return false;
                }
                bindGLFrameBuffer(nFrameBufferID);
                {
                    //Color Attachment
                    for (UintType2UintIDMap::const_iterator it = mapType2IDs.begin();
                         it != mapType2IDs.end(); ++it)
                    {
                        glFramebufferTexture2D(GL_FRAMEBUFFER, it->first, GL_TEXTURE_2D, it->second, 0);
                    }

                    //Depth Stencil Attachment
					if (nDepthStencilID > 0)
					{
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, nDepthStencilID); 
					}

					//Check
                    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    {
                        F_LogError("*********************** OpenGLESWindow::createGLFrameBuffer: FrameBuffer is not complete, id: [%d], name: [%s] !", nFrameBufferID, nameFrameBuffer.c_str());
                    }
                }
                bindGLFrameBuffer(0);

                this->poDebug->SetGLFrameBuffer(nFrameBufferID, nameFrameBuffer);
                return true;
            }
            void OpenGLESWindow::bindGLFrameBuffer(uint32 nFrameBufferID)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, nFrameBufferID);
            }
            void OpenGLESWindow::destroyGLFrameBuffer(uint32 nFrameBufferID)
            {
                if (nFrameBufferID <= 0)
                    return;
                glDeleteFramebuffers(1, &nFrameBufferID);
            }

        
        void OpenGLESWindow::createSyncObjects()
        {
            createFence();

            F_LogInfo("*****<1-9> OpenGLESWindow::createSyncObjects finish *****");
        }
            void OpenGLESWindow::createFence()
            {
                
            }


    void OpenGLESWindow::loadAssets()
    {
        F_LogInfo("**********<2> OpenGLESWindow::loadAssets start **********");
        {
            //0> Camera/Light/Shadow
            cameraReset();
            lightMainReset();
            shadowReset();
            terrainReset();

            //1> loadGeometry
            loadGeometry();

            //2> Imgui
            if (HasConfig_Imgui())
            {
                createImgui();
            }

            //3> Editor
            if (this->cfg_isEditorCreate)
            {
                createEditor();
            }

            this->isLoadAsset = true;
        }
        F_LogInfo("**********<2> OpenGLESWindow::loadAssets finish **********");
    }
        void OpenGLESWindow::loadGeometry()
        {
            F_LogInfo("*****<2-1> OpenGLESWindow::loadGeometry start *****");
            {
                //1> loadVertexIndexBuffer
                loadVertexIndexBuffer();

                //2> loadTexture
                loadTexture();

                //3> createConstBuffers
                createConstBuffers();

                //4> createCustomBeforePipeline
                createCustomBeforePipeline();

                //5> createGraphicsPipeline
                createGraphicsPipeline();

                //6> createComputePipeline
                createComputePipeline();

                //7> createDescriptorSets
                createDescriptorSets();   

            }
            F_LogInfo("*****<2-1> OpenGLESWindow::loadGeometry finish *****");
        }
            void OpenGLESWindow::loadVertexIndexBuffer()
            {
                F_LogInfo("**<2-1-1> OpenGLESWindow::loadVertexIndexBuffer start **");
                {
                    //1> loadModel
                    loadModel();

                    //2> createBufferVertexIndex
                    if (this->poVertexBuffer_Size > 0 &&
                        this->poVertexBuffer_Data != nullptr &&
                        this->poIndexBuffer_Size > 0 &&
                        this->poIndexBuffer_Data != nullptr)
                    {
                        this->pBufferVertexIndex = createBufferVertexIndex("VertexIndex-" + this->nameTitle,
                                                                           this->poTypeVertex,
                                                                           this->poVertexBuffer_Size,
                                                                           this->poVertexBuffer_Data,
                                                                           false,
                                                                           this->poIndexBuffer_Size,
                                                                           this->poIndexBuffer_Data,
                                                                           false);
                    }
                    //3> createBufferVertex
                    else if (this->poVertexBuffer_Size > 0 &&
                             this->poVertexBuffer_Data != nullptr)
                    {
                        this->pBufferVertex = createBufferVertex("Vertex-" + this->nameTitle,
                                                                 this->poTypeVertex,
                                                                 this->poVertexBuffer_Size, 
                                                                 this->poVertexBuffer_Data, 
                                                                 false);
                    }
                }
                F_LogInfo("**<2-1-1> OpenGLESWindow::loadVertexIndexBuffer finish **");
            }
                void OpenGLESWindow::loadModel()
                {
                    F_LogInfo("**<2-1-1-1> OpenGLESWindow::loadModel start **");
                    {
                        //1> model 
                        if (!this->cfg_model_Path.empty())
                        {
                            loadModel_Default();
                        }
                        //2> model user
                        else
                        {
                            loadModel_Custom();
                        }
                    }
                    F_LogInfo("**<2-1-1-1> OpenGLESWindow::loadModel finish **");
                }
                    void OpenGLESWindow::loadModel_Default()
                    {

                    }
                    void OpenGLESWindow::loadModel_Custom()
                    {

                    }
                GLESBufferVertex* OpenGLESWindow::createBufferVertex(const String& nameBuffer,
                                                                     FMeshVertexType type,
                                                                     size_t bufSize, 
                                                                     uint8* pBuf,
                                                                     bool isDelete)
                {
                    GLESBufferVertex* pBufferVertex = new GLESBufferVertex(nameBuffer);
                    if (!pBufferVertex->Init(type, 
                                             bufSize, 
                                             pBuf, 
                                             isDelete))
                    {
                        F_LogError("*********************** OpenGLESWindow::createBufferVertex: Failed to create buffer vertex: [%s] !", nameBuffer.c_str());
                        F_DELETE(pBufferVertex)
                        return nullptr;
                    }
                    return pBufferVertex;
                }
                void OpenGLESWindow::updateBufferVertex(GLESBufferVertex* pBufferVertex,
                                                        FMeshVertexType type,
                                                        size_t bufSize, 
                                                        uint8* pBuf,
                                                        bool isDelete)
                {
                    if (pBufferVertex == nullptr)
                        return;

                    pBufferVertex->Update(type,
                                          bufSize,
                                          pBuf,
                                          isDelete);
                }

                GLESBufferVertexIndex* OpenGLESWindow::createBufferVertexIndex(const String& nameBuffer,
                                                                               FMeshVertexType type,
                                                                               size_t bufSize_Vertex, 
                                                                               uint8* pBuf_Vertex,
                                                                               bool isDelete_Vertex,
                                                                               size_t bufSize_Index, 
                                                                               uint8* pBuf_Index,
                                                                               bool isDelete_Index)
                {
                    GLESBufferVertexIndex* pBufferVertexIndex = new GLESBufferVertexIndex(nameBuffer);
                    if (!pBufferVertexIndex->Init(type,
                                                  bufSize_Vertex, 
                                                  pBuf_Vertex, 
                                                  isDelete_Vertex,
                                                  bufSize_Index,
                                                  pBuf_Index,
                                                  isDelete_Index))
                    {
                        F_LogError("*********************** OpenGLESWindow::createBufferVertexIndex: Failed to create buffer vertex index: [%s] !", nameBuffer.c_str());
                        F_DELETE(pBufferVertexIndex)
                        return nullptr;
                    }
                    return pBufferVertexIndex;
                }
                void OpenGLESWindow::updateBufferVertexIndex(GLESBufferVertexIndex* pBufferVertexIndex,
                                                             FMeshVertexType type,
                                                             size_t bufSize_Vertex, 
                                                             uint8* pBuf_Vertex,
                                                             bool isDelete_Vertex,
                                                             size_t bufSize_Index, 
                                                             uint8* pBuf_Index,
                                                             bool isDelete_Index)
                {
                    if (pBufferVertexIndex == nullptr)
                        return;

                    pBufferVertexIndex->Update(type,  
                                               bufSize_Vertex,
                                               pBuf_Vertex,
                                               isDelete_Vertex,
                                               bufSize_Index,
                                               pBuf_Index,
                                               isDelete_Index);
                }   

                GLESBufferUniform* OpenGLESWindow::createBufferUniform(const String& nameBuffer,
                                                                       uint32 bindingIndex,
                                                                       GLenum usage,
                                                                       size_t bufSize, 
                                                                       uint8* pBuf,
                                                                       bool isDelete)
                {
                    GLESBufferUniform* pBufferUniform = new GLESBufferUniform(nameBuffer);
                    if (!pBufferUniform->Init(bindingIndex,
                                              usage,
                                              bufSize, 
                                              pBuf, 
                                              isDelete))
                    {
                        F_LogError("*********************** OpenGLESWindow::createBufferUniform: Failed to create buffer uniform: [%s] !", nameBuffer.c_str());
                        F_DELETE(pBufferUniform)
                        return nullptr;
                    }
                    return pBufferUniform;
                }   
                void OpenGLESWindow::updateBufferUniform(GLESBufferUniform* pBufferUniform,
                                                         size_t offset,
                                                         size_t bufSize, 
                                                         uint8* pBuf)
                {
                    if (pBufferUniform == nullptr)
                        return;

                    pBufferUniform->Update(offset,
                                           bufSize,  
                                           pBuf);
                }

                bool OpenGLESWindow::createGLBufferVertex(const String& nameBuffer,
                                                          FMeshVertexType type,
                                                          size_t bufSize,
                                                          uint8* pBuf,
                                                          uint32& nVAO,
                                                          uint32& nVBO)
                {
                    glGenVertexArrays(1, &nVAO);
                    glGenBuffers(1, &nVBO);

                    updateGLBufferVertex(type, 
                                         bufSize,
                                         pBuf,
                                         nVAO,
                                         nVBO);

                    this->poDebug->SetGLBufferName(nVBO, "BufferVertex-" + nameBuffer);
                    this->poDebug->SetGLVertexArrayName(nVAO, "VertexArray-" + nameBuffer);
                    return true;
                }
                void OpenGLESWindow::updateGLBufferVertex(FMeshVertexType type,
                                                          size_t bufSize,
                                                          uint8* pBuf,
                                                          uint32 nVAO,
                                                          uint32 nVBO)
                {
                    glBindVertexArray(nVAO);

                    glBindBuffer(GL_ARRAY_BUFFER, nVBO);
                    glBufferData(GL_ARRAY_BUFFER, bufSize, pBuf, GL_STATIC_DRAW);

                    Util_BindAttributeDescriptions(type);

                    glBindBuffer(GL_ARRAY_BUFFER, 0); 
                    glBindVertexArray(0); 
                }
                void OpenGLESWindow::destroyGLBufferVertex(uint32 nVAO, uint32 nVBO)
                {
                    if (nVAO > 0)
                    {
                        glDeleteVertexArrays(1, &nVAO);
                    }
                    if (nVBO > 0)
                    {
                        glDeleteBuffers(1, &nVBO);
                    }
                }

                bool OpenGLESWindow::createGLBufferVertexIndex(const String& nameBuffer,
                                                               FMeshVertexType type,
                                                               size_t bufSize_Vertex,
                                                               uint8* pBuf_Vertex,
                                                               size_t bufSize_Index,
                                                               uint8* pBuf_Index,
                                                               uint32& nVAO,
                                                               uint32& nVBO,
                                                               uint32& nVEO)
                {
                    glGenVertexArrays(1, &nVAO);
                    glGenBuffers(1, &nVBO);
                    glGenBuffers(1, &nVEO);

                    updateGLBufferVertexIndex(type, 
                                              bufSize_Vertex,
                                              pBuf_Vertex,
                                              bufSize_Index,
                                              pBuf_Index,
                                              nVAO,
                                              nVBO,
                                              nVEO);

                    this->poDebug->SetGLBufferName(nVBO, "BufferVertex-" + nameBuffer);
                    this->poDebug->SetGLBufferName(nVEO, "BufferIndex-" + nameBuffer);
                    this->poDebug->SetGLVertexArrayName(nVAO, "VertexArray-" + nameBuffer);
                    return true;
                }
                void OpenGLESWindow::updateGLBufferVertexIndex(FMeshVertexType type,
                                                               size_t bufSize_Vertex,
                                                               uint8* pBuf_Vertex,
                                                               size_t bufSize_Index,
                                                               uint8* pBuf_Index,
                                                               uint32 nVAO,
                                                               uint32 nVBO,
                                                               uint32 nVEO)
                {
                    glBindVertexArray(nVAO);

                    glBindBuffer(GL_ARRAY_BUFFER, nVBO);
                    glBufferData(GL_ARRAY_BUFFER, bufSize_Vertex, pBuf_Vertex, GL_STATIC_DRAW);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nVEO);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize_Index, pBuf_Index, GL_STATIC_DRAW);

                    Util_BindAttributeDescriptions(type);

                    glBindBuffer(GL_ARRAY_BUFFER, 0); 
                    glBindVertexArray(0); 
                }       
                void OpenGLESWindow::bindGLVertexArray(uint32 nVAO)
                {
                    if (nVAO > 0)
                    {
                        glBindVertexArray(nVAO);
                    }
                }
                void OpenGLESWindow::destroyGLBufferVertexIndex(uint32 nVAO, uint32 nVBO, uint32 nVEO)
                {
                    if (nVAO > 0)
                    {
                        glDeleteVertexArrays(1, &nVAO);
                    }
                    if (nVBO > 0)
                    {
                        glDeleteBuffers(1, &nVBO);
                    }
                    if (nVEO > 0)
                    {
                        glDeleteBuffers(1, &nVEO);
                    }
                }

                bool OpenGLESWindow::createGLBufferUniform(const String& nameBuffer,
                                                           uint32 bindingIndex,
                                                           GLenum usage,
                                                           size_t bufSize, 
                                                           uint8* pBuf,
                                                           uint32& nBufferUniformID)
                {
                    glGenBuffers(1, &nBufferUniformID);
                    glBindBuffer(GL_UNIFORM_BUFFER, nBufferUniformID);
                    glBufferData(GL_UNIFORM_BUFFER, bufSize, pBuf, usage);
                    glBindBuffer(GL_UNIFORM_BUFFER, 0);
					glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, nBufferUniformID, 0, bufSize);

                    if (GL_NO_ERROR != glGetError())
                    {
                        F_LogError("*********************** OpenGLESWindow::createGLBufferUniform: create uniform buffer error, GL error: [%u] !", glGetError());
                    }

                    this->poDebug->SetGLBufferUniformName(nBufferUniformID, "BufferUniform-" + nameBuffer);
                    return true;
                }
                void OpenGLESWindow::updateGLBufferUniform(size_t offset,
                                                           size_t bufSize,
                                                           uint8* pBuf,
                                                           uint32 nBufferUniformID)
                {
                    glBindBuffer(GL_UNIFORM_BUFFER, nBufferUniformID);
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, bufSize, pBuf);
                    glBindBuffer(GL_UNIFORM_BUFFER, 0);
                }
                void OpenGLESWindow::bindGLBufferUniform(uint32 nBufferUniformID)
                {
                    glBindBuffer(GL_UNIFORM_BUFFER, nBufferUniformID); 
                }
                void OpenGLESWindow::bindGLBufferUniformBlockIndex(uint32 nBufferUniformID, uint32 bindingIndex, size_t offset, size_t bufSize)
                {
                    glBindBuffer(GL_UNIFORM_BUFFER, nBufferUniformID);
                    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, nBufferUniformID);
                    glBindBuffer(GL_UNIFORM_BUFFER, 0);

					glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, nBufferUniformID, offset, bufSize);
                }
                void OpenGLESWindow::destroyGLBufferUniform(uint32 nBufferUniformID)
                {
                    if (nBufferUniformID > 0)
                    {
                        glDeleteBuffers(1, &nBufferUniformID);
                    }
                }

                void* OpenGLESWindow::mapGLBufferRange(uint32 nBufferID, uint32 nBlockIndex, GLenum target, size_t offset, size_t bufSize, GLbitfield access)
                {
                    if (nBufferID <= 0)
                        return nullptr;

                    UTIL_GLES_CHECK(glBindBufferRange(target, nBlockIndex, nBufferID, offset, bufSize));
                    UTIL_GLES_CHECK(void* pData = glMapBufferRange(target, offset, bufSize, access));
                    if (pData == nullptr)
                    {
                        F_LogError("*********************** OpenGLESWindow::mapGLBufferRange: GL error: [%u] !", glGetError());
                    }
                    return pData;
                }
                    void OpenGLESWindow::flushGLMappedBufferRange(uint32 nBufferID, size_t offset, size_t bufSize)
                    {
                        glFlushMappedBufferRange(nBufferID, offset, bufSize);
                    }
                void OpenGLESWindow::unMapGLBufferRange(GLenum target)
                {
                    UTIL_GLES_CHECK(GLboolean result = glUnmapBuffer(target));
                    if (!result)
                    {
                        F_LogError("*********************** OpenGLESWindow::unMapGLBufferRange: UnMap buffer data failed, GL error: [%u] !", glGetError());
                    }
                    glBindBuffer(target, 0);
                }


            void OpenGLESWindow::loadTexture()
            {
                F_LogInfo("**<2-1-2> OpenGLESWindow::loadTexture start **");
                {
                    //1> Texture Default 
                    loadTexture_Default();

                    //2> Texture Custom
                    loadTexture_Custom();
                }
                F_LogInfo("**<2-1-2> OpenGLESWindow::loadTexture finish **");
            }
                void OpenGLESWindow::loadTexture_Default()
                {
                    if (!this->cfg_texture_Path.empty())
                    {
                        String nameTexture;
                        String pathBase;
                        FUtilString::SplitFileName(this->cfg_texture_Path, nameTexture, pathBase);
                        StringVector aPathTexture;
                        aPathTexture.push_back(this->cfg_texture_Path);
                        this->poTexture = createTexture(nameTexture,
                                                        aPathTexture,
                                                        nullptr,
                                                        4,
                                                        0,
                                                        0,
                                                        0,
                                                        F_Texture_2D,
                                                        F_TexturePixelFormat_R8G8B8A8_SRGB,
                                                        F_TextureAddressing_Wrap,
                                                        F_TextureFilter_Bilinear,
                                                        F_TextureFilter_Bilinear,
                                                        F_MSAASampleCount_1_Bit,
                                                        FColor(0, 0, 0, 1),
                                                        true,
                                                        true,
                                                        false,
                                                        false,
                                                        false,
                                                        FMath::ms_clBlack);
                        if (this->poTexture == nullptr)
                        {
                            F_LogError("*********************** OpenGLESWindow::loadTexture_Default: Failed to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), this->cfg_texture_Path.c_str());
                            F_DELETE(this->poTexture)
                            return;
                        }
                        F_LogInfo("<2-1-2-1> OpenGLESWindow::loadTexture_Default finish !");
                    }
                }
                void OpenGLESWindow::loadTexture_Custom()
                {

                }

                GLESTexture* OpenGLESWindow::createTexture(const String& nameTexture,
                                                           const StringVector& aPathTexture,
                                                           uint8* pData,
                                                           int channel,
                                                           int width, 
                                                           int height,
                                                           int depth,
                                                           FTextureType typeTexture,
                                                           FTexturePixelFormatType typePixelFormat,
                                                           FTextureAddressingType typeAddressing,
                                                           FTextureFilterType typeFilterSizeMin,
                                                           FTextureFilterType typeFilterSizeMag,
                                                           FMSAASampleCountType numSamples,
                                                           const FColor& borderColor,
                                                           bool isUseBorderColor,
                                                           bool isAutoMipmap,
                                                           bool isGraphicsComputeShared,
                                                           bool isRenderTarget,
                                                           bool isUnOrderedAccess,
                                                           const FColor& rtColor)
                {
                    GLESTexture* pTexture = new GLESTexture(nameTexture,
                                                            aPathTexture,
                                                            typeTexture,
                                                            typePixelFormat,
                                                            typeAddressing,
                                                            typeFilterSizeMin,
                                                            typeFilterSizeMag,
                                                            numSamples,
                                                            borderColor,
                                                            isUseBorderColor,
                                                            isAutoMipmap,
                                                            isGraphicsComputeShared,
                                                            isRenderTarget,
                                                            isUnOrderedAccess,
														    rtColor);
                    if (!pTexture->LoadTexture(width, 
                                               height,
                                               depth,
                                               channel,
                                               pData))
                    {
                        F_DELETE(pTexture)
                        F_LogError("*********************** OpenGLESWindow::createTexture: Failed to create texture, name: [%s] !", nameTexture.c_str());
                        return nullptr;
                    }

                    return pTexture;
                }   

                bool OpenGLESWindow::createTexture1D(const String& nameTexture,
                                                     const String& pathAsset_Tex,
                                                     int& mipMapCount, 
                                                     bool isAutoMipmap,
                                                     FTextureType typeTexture, 
                                                     FTexturePixelFormatType typePixelFormat,
                                                     FTextureAddressingType typeAddressing,
                                                     FTextureFilterType typeFilterSizeMin,
                                                     FTextureFilterType typeFilterSizeMag,
                                                     FMSAASampleCountType numSamples, 
                                                     const FColor& borderColor,
                                                     bool isUseBorderColor,
                                                     bool isGraphicsComputeShared,
												     bool isUnOrderedAccess,
                                                     uint32& nTextureID)
                {
                    //1> Load Texture From File
                    String pathTexture = GetAssetFullPath(pathAsset_Tex);
                    CharVector content;
                    if (!FUtil::LoadAssetFileContent(pathTexture.c_str(), content))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture1D: Failed to load texture from file, path: [%s] !", pathTexture.c_str());
                        return false;
                    }
                    int width, height, texChannels;
                    stbi_uc* pixels = stbi_load_from_memory((stbi_uc const *)content.data(), (int)content.size(), &width, &height, &texChannels, 0);
                    mipMapCount = static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1;
                    if (!pixels) 
                    {
                        String msg = "*********************** OpenGLESWindow::createTexture1D: Failed to load texture image: " + pathAsset_Tex;
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg);
                    }

                    //2> Create
                    if (!createGLTexture(nameTexture,
                                         pixels,
                                         texChannels,
                                         width,
                                         height,
                                         0,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture1D: Failed to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), pathAsset_Tex.c_str());
                        stbi_image_free(pixels);
                        return false;
                    }
                    stbi_image_free(pixels);

                    F_LogInfo("OpenGLESWindow::createTexture1D: Success to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), pathAsset_Tex.c_str());
                    return true;
                }

                bool OpenGLESWindow::createTexture2D(const String& nameTexture,
                                                     const String& pathAsset_Tex,
                                                     int& mipMapCount, 
                                                     bool isAutoMipmap,
                                                     FTextureType typeTexture, 
                                                     FTexturePixelFormatType typePixelFormat,
                                                     FTextureAddressingType typeAddressing,
                                                     FTextureFilterType typeFilterSizeMin,
                                                     FTextureFilterType typeFilterSizeMag,
                                                     FMSAASampleCountType numSamples, 
                                                     const FColor& borderColor,
                                                     bool isUseBorderColor,
                                                     bool isGraphicsComputeShared,
                                                     bool isUnOrderedAccess,
                                                     uint32& nTextureID)
                {
                    //1> Load Texture From File
                    String pathTexture = GetAssetFullPath(pathAsset_Tex);
                    CharVector content;
                    if (!FUtil::LoadAssetFileContent(pathTexture.c_str(), content))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture2D: Failed to load texture from file, path: [%s] !", pathTexture.c_str());
                        return false;
                    }
                    int width, height, texChannels;
                    stbi_uc* pixels = stbi_load_from_memory((stbi_uc const *)content.data(), (int)content.size(), &width, &height, &texChannels, 0);
                    mipMapCount = static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1;
                    if (!pixels) 
                    {
                        String msg = "*********************** OpenGLESWindow::createTexture2D: Failed to load texture image: " + pathAsset_Tex;
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg);
                    }

                    //2> Create
                    if (!createGLTexture(nameTexture,
                                         pixels,
                                         texChannels,
                                         width,
                                         height,
                                         0,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
                                         isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture2D: Failed to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), pathAsset_Tex.c_str());
                        stbi_image_free(pixels);
                        return false;
                    }
                    stbi_image_free(pixels);

                    F_LogInfo("OpenGLESWindow::createTexture2D: Success to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), pathAsset_Tex.c_str());
                    return true;
                }

                static void s_DeletePixels(const std::vector<stbi_uc*>& aPixels)
				{
					size_t count_tex = aPixels.size();
					for (size_t i = 0; i < count_tex; i++)
					{
						stbi_uc* pixels = aPixels[i];
						stbi_image_free(pixels);
					}
				}
				bool OpenGLESWindow::createTexture2DArray(const String& nameTexture,
													      const StringVector& aPathAsset_Tex, 
													      int& mipMapCount, 
													      bool isAutoMipmap,
													      FTextureType typeTexture, 
													      FTexturePixelFormatType typePixelFormat,
													      FTextureAddressingType typeAddressing,
													      FTextureFilterType typeFilterSizeMin,
													      FTextureFilterType typeFilterSizeMag,
													      FMSAASampleCountType numSamples, 
													      const FColor& borderColor,
													      bool isUseBorderColor,
													      bool isGraphicsComputeShared,
													      bool isUnOrderedAccess,
													      uint32& nTextureID)
				{
					//1> Load Texture From File
					std::vector<int> aWidth;
					std::vector<int> aHeight;
					std::vector<stbi_uc*> aPixels;
					
					size_t count_tex = aPathAsset_Tex.size();
					if (count_tex <= 0)
					{
						F_LogError("*********************** OpenGLESWindow::createTexture2DArray: Texture path count <= 0 !");
						return false;
					}
					for (size_t i = 0; i < count_tex; i++)
					{
						const String& pathAsset_Tex = aPathAsset_Tex[i];
                        String pathTexture = GetAssetFullPath(pathAsset_Tex);
                        CharVector content;
                        if (!FUtil::LoadAssetFileContent(pathTexture.c_str(), content))
                        {
                            F_LogError("*********************** OpenGLESWindow::createTexture2DArray: Failed to load texture from file, path: [%s] !", pathTexture.c_str());
                            return false;
                        }
                        int width, height, texChannels;
                        stbi_uc* pixels = stbi_load_from_memory((stbi_uc const *)content.data(), (int)content.size(), &width, &height, &texChannels, STBI_rgb_alpha);
                        if (!pixels) 
                        {
                            s_DeletePixels(aPixels);
                            String msg = "*********************** OpenGLESWindow::createTexture2DArray: Failed to load texture image: " + pathTexture;
                            F_LogError("%s", msg.c_str());
                            throw std::runtime_error(msg);
                        }

						aWidth.push_back(width);
						aHeight.push_back(height);
						aPixels.push_back(pixels);
					}

					int width = aWidth[0];
					int height = aHeight[0];
					for (size_t i = 1; i < count_tex; i++)
					{
						if (aWidth[i] != width)
						{
							s_DeletePixels(aPixels);
							String msg = "*********************** OpenGLESWindow::createTexture2DArray: Texture image's all width must the same !";
							F_LogError("%s", msg.c_str());
							throw std::runtime_error(msg);
						}
						if (aHeight[i] != height)
						{
							s_DeletePixels(aPixels);
							String msg = "*********************** OpenGLESWindow::createTexture2DArray: Texture image's all height must the same !";
							F_LogError("%s", msg.c_str());
							throw std::runtime_error(msg);
						}
					}

					int channel = 4;
					uint8* pData = new uint8[count_tex * width * height * channel];
					for (size_t i = 0; i < count_tex; i++)
					{
						stbi_uc* pixels = aPixels[i];
						uint8* pSrc = pData + i * width * height * channel;
						memcpy(pSrc, pixels, width * height * channel);
					}
					s_DeletePixels(aPixels);
					
					//2> Create
                    if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         0,
                                         (int)count_tex,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture2DArray: Failed to create texture, name: [%s] !", nameTexture.c_str());
						F_DELETE_T(pData)
                        return false;
                    }
                    F_DELETE_T(pData)

                    F_LogInfo("OpenGLESWindow::createTexture2DArray: Success to create texture, name: [%s] !", nameTexture.c_str());
                    return true;
				}

				bool OpenGLESWindow::createTexture3D(const String& nameTexture,
                                                     const String& pathAsset_Tex,
                                                     int& mipMapCount, 
                                                     bool isAutoMipmap,
                                                     FTextureType typeTexture, 
                                                     FTexturePixelFormatType typePixelFormat,
                                                     FTextureAddressingType typeAddressing,
                                                     FTextureFilterType typeFilterSizeMin,
                                                     FTextureFilterType typeFilterSizeMag,
                                                     FMSAASampleCountType numSamples, 
                                                     const FColor& borderColor,
                                                     bool isUseBorderColor,
                                                     bool isGraphicsComputeShared,
                                                     bool isUnOrderedAccess,
                                                     uint32& nTextureID)
				{
					//1> Load Texture From File
                    String pathTexture = GetAssetFullPath(pathAsset_Tex);
                    CharVector content;
                    if (!FUtil::LoadAssetFileContent(pathTexture.c_str(), content))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture3D: Failed to load texture from file, path: [%s] !", pathTexture.c_str());
                        return false;
                    }
                    int width, height, texChannels;
                    stbi_uc* pixels = stbi_load_from_memory((stbi_uc const *)content.data(), (int)content.size(), &width, &height, &texChannels, 0);
                    mipMapCount = static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1;
                    if (!pixels) 
                    {
                        String msg = "*********************** OpenGLESWindow::createTexture3D: Failed to load texture image: " + pathAsset_Tex;
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg);
                    }

                    //2> Create
                    if (!createGLTexture(nameTexture,
                                         pixels,
                                         texChannels,
                                         width,
                                         height,
                                         1,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture3D: Failed to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), pathAsset_Tex.c_str());
                        stbi_image_free(pixels);
                        return false;
                    }
                    stbi_image_free(pixels);

                    F_LogInfo("OpenGLESWindow::createTexture3D: Success to create texture, name: [%s], path: [%s] !", nameTexture.c_str(), pathAsset_Tex.c_str());
                    return true;
				}
                bool OpenGLESWindow::createTexture3D(const String& nameTexture,
                                                     uint8* pDataRGBA,
                                                     int channel,
                                                     int width,
                                                     int height,
                                                     int depth,
                                                     int& mipMapCount, 
                                                     bool isAutoMipmap,
                                                     FTextureType typeTexture, 
                                                     FTexturePixelFormatType typePixelFormat,
                                                     FTextureAddressingType typeAddressing,
                                                     FTextureFilterType typeFilterSizeMin,
                                                     FTextureFilterType typeFilterSizeMag,
                                                     FMSAASampleCountType numSamples, 
                                                     const FColor& borderColor,
                                                     bool isUseBorderColor,
                                                     bool isGraphicsComputeShared,
                                                     bool isUnOrderedAccess,
                                                     uint32& nTextureID)
				{
					//2> Create
                    if (!createGLTexture(nameTexture,
                                         pDataRGBA,
                                         channel,
                                         width,
                                         height,
                                         depth,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTexture3D: Failed to create texture, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_LogInfo("OpenGLESWindow::createTexture3D: Success to create texture, name: [%s] !", nameTexture.c_str());
                    return true;
				}

				bool OpenGLESWindow::createTextureCubeMap(const String& nameTexture,
													      const StringVector& aPathAsset_Tex, 
													      int& mipMapCount, 
													      bool isAutoMipmap,
													      FTextureType typeTexture, 
													      FTexturePixelFormatType typePixelFormat,
													      FTextureAddressingType typeAddressing,
													      FTextureFilterType typeFilterSizeMin,
													      FTextureFilterType typeFilterSizeMag,
													      FMSAASampleCountType numSamples, 
													      const FColor& borderColor,
													      bool isUseBorderColor,
													      bool isGraphicsComputeShared,
													      bool isUnOrderedAccess,
													      uint32& nTextureID)
				{
					//1> Load Texture From File
					std::vector<int> aWidth;
					std::vector<int> aHeight;
					std::vector<stbi_uc*> aPixels;
					
					size_t count_tex = aPathAsset_Tex.size();
					if (count_tex <= 0)
					{
						F_LogError("*********************** OpenGLESWindow::createTextureCubeMap: Texture path count <= 0 !");
						return false;
					}
					for (size_t i = 0; i < count_tex; i++)
					{
						const String& pathAsset_Tex = aPathAsset_Tex[i];
                        String pathTexture = GetAssetFullPath(pathAsset_Tex);
                        CharVector content;
                        if (!FUtil::LoadAssetFileContent(pathTexture.c_str(), content))
                        {
                            F_LogError("*********************** OpenGLESWindow::createTextureCubeMap: Failed to load texture from file, path: [%s] !", pathTexture.c_str());
                            return false;
                        }
                        int width, height, texChannels;
                        stbi_uc* pixels = stbi_load_from_memory((stbi_uc const *)content.data(), (int)content.size(), &width, &height, &texChannels, STBI_rgb_alpha);
                        if (!pixels) 
                        {
                            s_DeletePixels(aPixels);
                            String msg = "*********************** OpenGLESWindow::createTextureCubeMap: Failed to load texture image: " + pathAsset_Tex;
                            F_LogError("%s", msg.c_str());
                            throw std::runtime_error(msg);
                        }

						aWidth.push_back(width);
						aHeight.push_back(height);
						aPixels.push_back(pixels);
					}

					int width = aWidth[0];
					int height = aHeight[0];
					for (size_t i = 1; i < count_tex; i++)
					{
						if (aWidth[i] != width)
						{
							s_DeletePixels(aPixels);
							String msg = "*********************** OpenGLESWindow::createTextureCubeMap: Texture image's all width must the same !";
							F_LogError("%s", msg.c_str());
							throw std::runtime_error(msg);
						}
						if (aHeight[i] != height)
						{
							s_DeletePixels(aPixels);
							String msg = "*********************** OpenGLESWindow::createTextureCubeMap: Texture image's all height must the same !";
							F_LogError("%s", msg.c_str());
							throw std::runtime_error(msg);
						}
					}

					int channel = 4;
					uint8* pData = new uint8[count_tex * width * height * channel];
					for (size_t i = 0; i < count_tex; i++)
					{
						stbi_uc* pixels = aPixels[i];
						uint8* pSrc = pData + i * width * height * channel;
						memcpy(pSrc, pixels, width * height * channel);
					}
					s_DeletePixels(aPixels);
					
					//2> Create
                    if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         0,
                                         (int)count_tex,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTextureCubeMap: Failed to create texture, name: [%s] !", nameTexture.c_str());
						F_DELETE_T(pData)
                        return false;
                    }
                    F_DELETE_T(pData)

                    F_LogInfo("OpenGLESWindow::createTextureCubeMap: Success to create texture, name: [%s] !", nameTexture.c_str());
                    return true;
				}

				bool OpenGLESWindow::createTextureRenderTarget1D(const String& nameTexture,
                                                                 const FVector4& clDefault,
                                                                 bool isSetColor,
                                                                 int channel,
                                                                 int width, 
                                                                 int height,
                                                                 int& mipMapCount, 
                                                                 bool isAutoMipmap,
                                                                 FTextureType typeTexture, 
                                                                 FTexturePixelFormatType typePixelFormat,
                                                                 FTextureAddressingType typeAddressing,
                                                                 FTextureFilterType typeFilterSizeMin,
                                                                 FTextureFilterType typeFilterSizeMag,
                                                                 FMSAASampleCountType numSamples, 
                                                                 const FColor& borderColor,
                                                                 bool isUseBorderColor,
                                                                 bool isGraphicsComputeShared,
															     bool isUnOrderedAccess,
                                                                 uint32& nTextureID)
                {
                    int imageSize = width * height * channel;
                    uint8* pData = nullptr;
                    if (isSetColor)
                    {
                        pData = new uint8[imageSize];
                        uint8 r = (uint8)(clDefault.x * 255);
                        uint8 g = (uint8)(clDefault.y * 255);
                        uint8 b = (uint8)(clDefault.z * 255);
                        uint8 a = (uint8)(clDefault.w * 255);

                        uint8* pColor = (uint8*)pData;
                        for (int i = 0; i < width * height; i++)
                        {
                            pColor[channel * i + 0] = r;
                            if (channel > 1)
                                pColor[channel * i + 1] = g;
                            if (channel > 2)
                                pColor[channel * i + 2] = b;
                            if (channel > 3)
                                pColor[channel * i + 3] = a;
                        }
                    }

                    if (!createTextureRenderTarget1D(nameTexture,
                                                     pData,
                                                     channel,
                                                     width,
                                                     height,
                                                     mipMapCount,
                                                     isAutoMipmap,
                                                     typeTexture,
                                                     typePixelFormat,
                                                     typeAddressing,
                                                     typeFilterSizeMin,
                                                     typeFilterSizeMag,
                                                     numSamples,
                                                     borderColor,
                                                     isUseBorderColor,
                                                     isGraphicsComputeShared,
													 isUnOrderedAccess,
                                                     nTextureID))
                    {
                        F_DELETE_T(pData)
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget1D: Failed to create texture RenderTarget1D, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_DELETE_T(pData)
                    return true;
                }
                bool OpenGLESWindow::createTextureRenderTarget1D(const String& nameTexture,
                                                                 uint8* pData,
                                                                 int channel,
                                                                 int width, 
                                                                 int height,
                                                                 int& mipMapCount, 
                                                                 bool isAutoMipmap,
                                                                 FTextureType typeTexture, 
                                                                 FTexturePixelFormatType typePixelFormat,
                                                                 FTextureAddressingType typeAddressing,
                                                                 FTextureFilterType typeFilterSizeMin,
                                                                 FTextureFilterType typeFilterSizeMag,
                                                                 FMSAASampleCountType numSamples, 
                                                                 const FColor& borderColor,
                                                                 bool isUseBorderColor,
                                                                 bool isGraphicsComputeShared,
															     bool isUnOrderedAccess,
                                                                 uint32& nTextureID)
                {
                    if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         0,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget1D: Failed to create texture RenderTarget1D, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_LogInfo("OpenGLESWindow::createTextureRenderTarget1D: Success to create texture RenderTarget1D, name: [%s] !", nameTexture.c_str());
                    return true;
                }

                bool OpenGLESWindow::createTextureRenderTarget2D(const String& nameTexture,
                                                                 const FVector4& clDefault,
                                                                 bool isSetColor,
                                                                 int channel,
                                                                 int width, 
                                                                 int height,
                                                                 int& mipMapCount, 
                                                                 bool isAutoMipmap,
                                                                 FTextureType typeTexture, 
                                                                 FTexturePixelFormatType typePixelFormat,
                                                                 FTextureAddressingType typeAddressing,
                                                                 FTextureFilterType typeFilterSizeMin,
                                                                 FTextureFilterType typeFilterSizeMag,
                                                                 FMSAASampleCountType numSamples, 
                                                                 const FColor& borderColor,
                                                                 bool isUseBorderColor,
                                                                 bool isGraphicsComputeShared,
                                                                 bool isUnOrderedAccess,
                                                                 uint32& nTextureID)
                {
                    int imageSize = width * height * channel;
                    uint8* pData = nullptr;
                    if (isSetColor)
                    {
                        pData = new uint8[imageSize];
                        uint8 r = (uint8)(clDefault.x * 255);
                        uint8 g = (uint8)(clDefault.y * 255);
                        uint8 b = (uint8)(clDefault.z * 255);
                        uint8 a = (uint8)(clDefault.w * 255);

                        uint8* pColor = (uint8*)pData;
                        for (int i = 0; i < width * height; i++)
                        {
                            pColor[channel * i + 0] = r;
                            if (channel > 1)
                                pColor[channel * i + 1] = g;
                            if (channel > 2)
                                pColor[channel * i + 2] = b;
                            if (channel > 3)
                                pColor[channel * i + 3] = a;
                        }
                    }

                    if (!createTextureRenderTarget2D(nameTexture,
                                                     pData,
                                                     channel,
                                                     width,
                                                     height,
                                                     mipMapCount,
                                                     isAutoMipmap,
                                                     typeTexture,
                                                     typePixelFormat,
                                                     typeAddressing,
                                                     typeFilterSizeMin,
                                                     typeFilterSizeMag,
                                                     numSamples,
                                                     borderColor,
                                                     isUseBorderColor,
                                                     isGraphicsComputeShared,
                                                     isUnOrderedAccess,
                                                     nTextureID))
                    {
                        F_DELETE_T(pData)
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget2D: Failed to create texture RenderTarget2D, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_DELETE_T(pData)
                    return true;
                }
                bool OpenGLESWindow::createTextureRenderTarget2D(const String& nameTexture,
                                                                 uint8* pData,
                                                                 int channel,
                                                                 int width, 
                                                                 int height,
                                                                 int& mipMapCount, 
                                                                 bool isAutoMipmap,
                                                                 FTextureType typeTexture, 
                                                                 FTexturePixelFormatType typePixelFormat,
                                                                 FTextureAddressingType typeAddressing,
                                                                 FTextureFilterType typeFilterSizeMin,
                                                                 FTextureFilterType typeFilterSizeMag,
                                                                 FMSAASampleCountType numSamples, 
                                                                 const FColor& borderColor,
                                                                 bool isUseBorderColor,
                                                                 bool isGraphicsComputeShared,
                                                                 bool isUnOrderedAccess,
                                                                 uint32& nTextureID)
                {
                    if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         0,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
                                         isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget2D: Failed to create texture RenderTarget2D, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_LogInfo("OpenGLESWindow::createTextureRenderTarget2D: Success to create texture RenderTarget2D, name: [%s] !", nameTexture.c_str());
                    return true;
                }

                bool OpenGLESWindow::createTextureRenderTarget2DArray(const String& nameTexture,
                                                                      const FVector4& clDefault,
                                                                      bool isSetColor,
                                                                      int channel,
                                                                      int width, 
                                                                      int height,
                                                                      int numArray,
                                                                      int& mipMapCount, 
                                                                      bool isAutoMipmap,
                                                                      FTextureType typeTexture, 
                                                                      FTexturePixelFormatType typePixelFormat,
                                                                      FTextureAddressingType typeAddressing,
                                                                      FTextureFilterType typeFilterSizeMin,
                                                                      FTextureFilterType typeFilterSizeMag,
                                                                      FMSAASampleCountType numSamples, 
                                                                      const FColor& borderColor,
                                                                      bool isUseBorderColor,
                                                                      bool isGraphicsComputeShared,
                                                                      bool isUnOrderedAccess,
                                                                      const FColor& rtColor,
                                                                      uint32& nTextureID)
				{
					int imageSize = width * height * channel;
					uint8* pDataOne = nullptr;
					if (isSetColor)
						pDataOne = new uint8[imageSize];
					uint8* pData = new uint8[numArray * imageSize];
					memset(pData, 0, numArray * imageSize);
                    for (int i = 0; i < numArray; i++)
                    {
                        if (isSetColor)
                        {
                            uint8 r = (uint8)(clDefault.x * 255);
                            uint8 g = (uint8)(clDefault.y * 255);
                            uint8 b = (uint8)(clDefault.z * 255);
                            uint8 a = (uint8)(clDefault.w * 255);

                            uint8* pColor = (uint8*)pDataOne;
                            for (int i = 0; i < width * height; i++)
                            {
                                pColor[channel * i + 0] = r;
                                if (channel > 1)
                                    pColor[channel * i + 1] = g;
                                if (channel > 2)
                                    pColor[channel * i + 2] = b;
                                if (channel > 3)
                                    pColor[channel * i + 3] = a;
                            }
							memcpy(pData + i * imageSize, pDataOne, imageSize);
                        }
                    }
					F_DELETE_T(pDataOne)

                    if (!createTextureRenderTarget2DArray(nameTexture,
                                                          pData,
                                                          channel,
                                                          width,
                                                          height,
                                                          numArray,
                                                          mipMapCount,
                                                          isAutoMipmap,
                                                          typeTexture,
                                                          typePixelFormat,
                                                          typeAddressing,
                                                          typeFilterSizeMin,
                                                          typeFilterSizeMag,
                                                          numSamples,
                                                          borderColor,
                                                          isUseBorderColor,
                                                          isGraphicsComputeShared,
                                                          isUnOrderedAccess,
                                                          rtColor,
                                                          nTextureID))
                    {
                        F_DELETE_T(pData)
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget2DArray: Failed to create texture RenderTarget2DArray, name: [%s] !", nameTexture.c_str());
                        return false;
                    }
					F_DELETE_T(pData)
                    
                    return true;
				}
                bool OpenGLESWindow::createTextureRenderTarget2DArray(const String& nameTexture,
                                                                      uint8* pData, 
                                                                      int channel,
                                                                      int width, 
                                                                      int height,
                                                                      int numArray,
                                                                      int& mipMapCount, 
                                                                      bool isAutoMipmap,
                                                                      FTextureType typeTexture, 
                                                                      FTexturePixelFormatType typePixelFormat,
                                                                      FTextureAddressingType typeAddressing,
                                                                      FTextureFilterType typeFilterSizeMin,
                                                                      FTextureFilterType typeFilterSizeMag,
                                                                      FMSAASampleCountType numSamples, 
                                                                      const FColor& borderColor,
                                                                      bool isUseBorderColor,
                                                                      bool isGraphicsComputeShared,
                                                                      bool isUnOrderedAccess,
                                                                      const FColor& rtColor,
                                                                      uint32& nTextureID)
				{
					if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         0,
                                         numArray,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget2DArray: Failed to create texture RenderTarget2DArray, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_LogInfo("OpenGLESWindow::createTextureRenderTarget2DArray: Success to create texture RenderTarget2DArray, name: [%s] !", nameTexture.c_str());
                    return true;
				}

				bool OpenGLESWindow::createTextureRenderTarget3D(const String& nameTexture,
															     const FVector4& clDefault,
															     bool isSetColor,
															     int channel,
															     int width, 
															     int height,
															     int depth,
															     int& mipMapCount, 
															     bool isAutoMipmap,
															     FTextureType typeTexture, 
															     FTexturePixelFormatType typePixelFormat,
															     FTextureAddressingType typeAddressing,
															     FTextureFilterType typeFilterSizeMin,
															     FTextureFilterType typeFilterSizeMag,
															     FMSAASampleCountType numSamples, 
															     const FColor& borderColor,
															     bool isUseBorderColor,
															     bool isGraphicsComputeShared,
															     bool isUnOrderedAccess,
															     const FColor& rtColor,
															     uint32& nTextureID)
				{
					int imageSize = width * height * depth * channel;
					uint8* pData = new uint8[imageSize];
					memset(pData, 0, imageSize);
					if (isSetColor)
					{
						uint8 r = (uint8)(clDefault.x * 255);
						uint8 g = (uint8)(clDefault.y * 255);
						uint8 b = (uint8)(clDefault.z * 255);
						uint8 a = (uint8)(clDefault.w * 255);
						uint8* pColor = (uint8*)pData;
						for (uint32_t i = 0; i < width * height * depth; i++)
						{
							pColor[channel * i + 0] = r;
							if (channel > 1)
								pColor[channel * i + 1] = g;
							if (channel > 2)
								pColor[channel * i + 2] = b;
							if (channel > 3)
								pColor[channel * i + 3] = a;
						}
					}

                    if (!createTextureRenderTarget3D(nameTexture,
													 pData,
													 channel,
													 width,
													 height,
													 depth,
													 mipMapCount,
													 isAutoMipmap,
													 typeTexture,
													 typePixelFormat,
													 typeAddressing,
													 typeFilterSizeMin,
													 typeFilterSizeMag,
													 numSamples,
													 borderColor,
													 isUseBorderColor,
													 isGraphicsComputeShared,
													 isUnOrderedAccess,
													 rtColor,
													 nTextureID))
                    {
                        F_DELETE_T(pData)
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget3D: Failed to create texture RenderTarget3D, name: [%s] !", nameTexture.c_str());
                        return false;
                    }
					F_DELETE_T(pData)
                    
                    return true;
				}
                bool OpenGLESWindow::createTextureRenderTarget3D(const String& nameTexture,
															     uint8* pData, 
															     int channel,
															     int width, 
															     int height,
															     int depth,
															     int& mipMapCount, 
															     bool isAutoMipmap,
															     FTextureType typeTexture, 
															     FTexturePixelFormatType typePixelFormat,
															     FTextureAddressingType typeAddressing,
															     FTextureFilterType typeFilterSizeMin,
															     FTextureFilterType typeFilterSizeMag,
															     FMSAASampleCountType numSamples, 
															     const FColor& borderColor,
															     bool isUseBorderColor,
															     bool isGraphicsComputeShared,
															     bool isUnOrderedAccess,
															     const FColor& rtColor,
															     uint32& nTextureID)
				{
					if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         depth,
                                         1,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTarget3D: Failed to create texture RenderTarget3D, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_LogInfo("OpenGLESWindow::createTextureRenderTarget3D: Success to create texture RenderTarget3D, name: [%s] !", nameTexture.c_str());
                    return true;
				}

				bool OpenGLESWindow::createTextureRenderTargetCubeMap(const String& nameTexture,
																	  const FVector4& clDefault,
																	  bool isSetColor,
																	  int channel,
																	  int width, 
																	  int height,
																	  int depth,
																	  int& mipMapCount, 
																	  bool isAutoMipmap,
																	  FTextureType typeTexture, 
																	  FTexturePixelFormatType typePixelFormat,
																	  FTextureAddressingType typeAddressing,
																	  FTextureFilterType typeFilterSizeMin,
																	  FTextureFilterType typeFilterSizeMag,
																	  FMSAASampleCountType numSamples, 
																	  const FColor& borderColor,
																	  bool isUseBorderColor,
																	  bool isGraphicsComputeShared,
																	  bool isUnOrderedAccess,
																	  const FColor& rtColor,
																	  uint32& nTextureID)
				{
					int imageSize = width * height * depth * channel;
					uint8* pData = new uint8[imageSize];
					memset(pData, 0, imageSize);
					if (isSetColor)
					{
						uint8 r = (uint8)(clDefault.x * 255);
						uint8 g = (uint8)(clDefault.y * 255);
						uint8 b = (uint8)(clDefault.z * 255);
						uint8 a = (uint8)(clDefault.w * 255);
						uint8* pColor = (uint8*)pData;
						for (uint32_t i = 0; i < width * height * depth; i++)
						{
							pColor[channel * i + 0] = r;
							if (channel > 1)
								pColor[channel * i + 1] = g;
							if (channel > 2)
								pColor[channel * i + 2] = b;
							if (channel > 3)
								pColor[channel * i + 3] = a;
						}
					}

                    if (!createTextureRenderTargetCubeMap(nameTexture,
														  pData,
														  channel,
														  width,
														  height,
														  depth,
														  mipMapCount,
														  isAutoMipmap,
														  typeTexture,
														  typePixelFormat,
														  typeAddressing,
														  typeFilterSizeMin,
														  typeFilterSizeMag,
														  numSamples,
														  borderColor,
														  isUseBorderColor,
														  isGraphicsComputeShared,
														  isUnOrderedAccess,
														  rtColor,
														  nTextureID))
                    {
                        F_DELETE_T(pData)
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTargetCubeMap: Failed to create texture RenderTargetCubeMap, name: [%s] !", nameTexture.c_str());
                        return false;
                    }
					F_DELETE_T(pData)
                    
                    return true;
				}
                bool OpenGLESWindow::createTextureRenderTargetCubeMap(const String& nameTexture,
																	  uint8* pData, 
																	  int channel,
																	  int width, 
																	  int height,
																	  int depth,
																	  int& mipMapCount, 
																	  bool isAutoMipmap,
																	  FTextureType typeTexture, 
																	  FTexturePixelFormatType typePixelFormat,
																	  FTextureAddressingType typeAddressing,
																	  FTextureFilterType typeFilterSizeMin,
																	  FTextureFilterType typeFilterSizeMag,
																	  FMSAASampleCountType numSamples, 
																	  const FColor& borderColor,
																	  bool isUseBorderColor,
																	  bool isGraphicsComputeShared,
																	  bool isUnOrderedAccess,
																	  const FColor& rtColor,
																	  uint32& nTextureID)
				{
					if (!createGLTexture(nameTexture,
                                         pData,
                                         channel,
                                         width,
                                         height,
                                         0,
                                         depth,
                                         mipMapCount,
                                         isAutoMipmap,
                                         typeTexture,
                                         typePixelFormat,
                                         typeAddressing,
                                         typeFilterSizeMin,
                                         typeFilterSizeMag,
                                         numSamples,
                                         borderColor,
                                         isUseBorderColor,
                                         isGraphicsComputeShared,
										 isUnOrderedAccess,
                                         nTextureID))
                    {
                        F_LogError("*********************** OpenGLESWindow::createTextureRenderTargetCubeMap: Failed to create texture RenderTargetCubeMap, name: [%s] !", nameTexture.c_str());
                        return false;
                    }

                    F_LogInfo("OpenGLESWindow::createTextureRenderTargetCubeMap: Success to create texture RenderTargetCubeMap, name: [%s] !", nameTexture.c_str());
                    return true;
				}

                bool OpenGLESWindow::createGLTexture(const String& nameTexture,
                                                     uint8* pData,
                                                     int channel,
                                                     int width, 
                                                     int height, 
                                                     int depth, 
                                                     int numArray,
                                                     int mipMapCount, 
                                                     bool isAutoMipmap,
                                                     FTextureType typeTexture, 
                                                     FTexturePixelFormatType typePixelFormat,
                                                     FTextureAddressingType typeAddressing,
                                                     FTextureFilterType typeFilterSizeMin,
                                                     FTextureFilterType typeFilterSizeMag,
                                                     FMSAASampleCountType numSamples, 
                                                     const FColor& borderColor,
                                                     bool isUseBorderColor,
                                                     bool isGraphicsComputeShared,
                                                     bool isUnOrderedAccess,
                                                     uint32& nTextureID)
                {
                    glGenTextures(1, &nTextureID);
                    
                    //1> Texture Type
                    GLenum type = Util_Transform2GLTextureType(typeTexture);
                    glBindTexture(type, nTextureID);

                    //2> Texture Data
                    GLenum typeFormat = Util_Transform2GLFormat(typePixelFormat);
                    GLenum formatInternal;
                    GLenum format;
					if (channel == 1) {
                        formatInternal = GL_RED;
                        format = GL_RED;
                    }
					else if (channel == 3) {
                        formatInternal = GL_RGB;
                        format = GL_RGB;
                    }
					else if (channel == 4) {
                        formatInternal = GL_RGBA8;
                        format = GL_RGBA;
                    }

                    if (typeTexture == F_Texture_1D)
                    {
                        glTexImage2D(type, 
									 0, 
									 formatInternal, 
									 width, 
                                     1,
									 0, 
									 format, 
									 GL_UNSIGNED_BYTE, 
									 pData);
                    }
                    else if (typeTexture == F_Texture_2D)
                    {
                        glTexImage2D(type, 
									 0, 
									 formatInternal, 
									 width, 
									 height, 
									 0, 
									 format, 
									 GL_UNSIGNED_BYTE, 
									 pData);
                    }
                    else if (typeTexture == F_Texture_2DArray)
                    {
                        glTexImage3D(GL_TEXTURE_2D_ARRAY, 
									 0, 
									 formatInternal,               
									 width,               
									 height,               
									 numArray,           
									 0,                    
									 format,               
									 GL_UNSIGNED_BYTE,     
									 pData);
                    }
                    else if (typeTexture == F_Texture_3D)
                    {
                        glTexImage3D(GL_TEXTURE_3D, 
									 0, 
									 formatInternal,               
									 width,               
									 height,               
									 depth,           
									 0,                    
									 format,               
									 GL_UNSIGNED_BYTE,     
									 pData);
                    }
                    else if (typeTexture == F_Texture_CubeMap)
                    {
                        for (int i = 0; i < numArray; i++)
						{
							uint8* pDataCur = pData + i * width * height * channel;
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
										 0, 
										 formatInternal, 
										 width, 
										 height, 
										 0, 
										 format, 
										 GL_UNSIGNED_BYTE, 
										 pDataCur);
						}
                    }
                    else
                    {
                        String msg = "*********************** OpenGLESWindow::createGLTexture: Wrong texture type, Create texture failed, name: [" + nameTexture + "] !";
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg);
                    }
                    
                    //3> AutoMipMap
                    if (isAutoMipmap)
                    {
                        glGenerateMipmap(type);
                    }

                    //4> Wrapping parameters
                    GLenum typeAddress = Util_Transform2GLSamplerAddressMode(typeAddressing);
                    glTexParameteri(type, GL_TEXTURE_WRAP_S, typeAddress);
                    glTexParameteri(type, GL_TEXTURE_WRAP_T, typeAddress);
                    
                    //5> Filtering parameters
                    GLenum typeFilterMin = Util_Transform2GLFilter(typeFilterSizeMin, F_TextureFilterSize_Min);
                    GLenum typeFilterMag = Util_Transform2GLFilter(typeFilterSizeMag, F_TextureFilterSize_Mag);
                    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, typeFilterMin);
                    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, typeFilterMag);


                    return true;
                }
                void OpenGLESWindow::bindGLTexture(FTextureType typeTexture, uint slot, uint32 nTextureID)
                {
                    GLenum type = Util_Transform2GLTextureType(typeTexture);
                    glActiveTexture(GL_TEXTURE0 + slot);
                    glBindTexture(type, nTextureID);
                }
                void OpenGLESWindow::destroyGLTexture(uint32 nTextureID)
                {
                    if (nTextureID > 0)
                    {
                        glDeleteTextures(1, &nTextureID);
                    }
                }

                void OpenGLESWindow::bindGLTextureImage(FTextureType typeTexture, uint slot, uint32 nTextureID, uint32 access, uint32 format)
                {
                    glBindImageTexture(slot,
                                       nTextureID,
                                       0,
                                       GL_FALSE,
                                       0,
                                       access,
                                       format);
                }


            void OpenGLESWindow::createConstBuffers()
            {
                F_LogInfo("**<2-1-3> OpenGLESWindow::createConstBuffers start **");
                {
                    //1> createObjectCB
                    createObjectCB();

                    //2> createMaterialCB
                    createMaterialCB();

                    //3> createInstanceCB
                    createInstanceCB();

                    //4> createCustomCB
                    createCustomCB();
                }
                F_LogInfo("**<2-1-3> OpenGLESWindow::createConstBuffers finish **");
            }
                void OpenGLESWindow::createObjectCB()
                {
                    buildObjectCB();

                    size_t count_object = this->objectCBs.size();
                    if (count_object <= 0)
                        return;
                    size_t bufferSize = sizeof(ObjectConstants) * count_object;
                    size_t count = this->poSwapChains.size();
                    this->poBuffers_ObjectCB.resize(count);
                    for (size_t i = 0; i < count; i++) 
                    {
                        String nameBuffer = "Object-" + FUtilString::SaveSizeT(i);
                        GLESBufferUniform* pBufferUniform = createBufferUniform(nameBuffer,
                                                                                DescriptorSet_ObjectConstants,
                                                                                GL_DYNAMIC_DRAW,
                                                                                bufferSize,
                                                                                (uint8*)(this->objectCBs.data()),
                                                                                false);
                        if (!pBufferUniform)
                        {
                            String msg = "*********************** OpenGLESWindow::createObjectCB: create buffer uniform: [" + nameBuffer + "] failed !";
                            F_LogError("%s", msg.c_str());
                            throw std::runtime_error(msg);
                        }
                        this->poBuffers_ObjectCB[i] = pBufferUniform;
                    }
                    F_LogInfo("<2-1-3-1> OpenGLESWindow::createObjectCB finish !");
                }
                    void OpenGLESWindow::buildObjectCB()
                    {
                        
                    }
                void OpenGLESWindow::createMaterialCB()
                {

                }
                    void OpenGLESWindow::buildMaterialCB()
                    {

                    }
                void OpenGLESWindow::createInstanceCB()
                {

                }
                    void OpenGLESWindow::buildInstanceCB()
                    {

                    }
                void OpenGLESWindow::createCustomCB()
                {

                }

			GLESShader* OpenGLESWindow::createShader(const String& nameShader, const String& pathFile, const String& nameShaderType)
			{
				FShaderType typeShader = F_ParseShaderType(nameShaderType);
				return createShader(nameShader, pathFile, typeShader);
			}
            GLESShader* OpenGLESWindow::createShader(const String& nameShader, const String& pathFile, FShaderType typeShader)
            {
                GLESShader* pShader = new GLESShader(nameShader);
                if (!pShader->Init(pathFile, 
                                   typeShader))
                {
                    F_LogError("*********************** OpenGLESWindow::createShader failed, name: [%s], path: [%s] !", nameShader.c_str(), pathFile.c_str());
                    return nullptr;
                }
                return pShader;
            }
            String OpenGLESWindow::getShaderPathRelative(const String& nameShader)
            {
                String pathRelative = "Shader/Android/" + nameShader;
                return pathRelative;
            }
            String OpenGLESWindow::getShaderPath(const String& nameShader)
            {
                String pathRelative = getShaderPathRelative(nameShader);
                return GetAssetFullPath(pathRelative);
            }

            bool OpenGLESWindow::createGLShader(const String& nameShader, const String& pathFile, FShaderType typeShader, uint32& nShaderID)
            {
                const String& strTypeShader = F_GetShaderTypeName(typeShader);
                return createGLShader(nameShader, strTypeShader, pathFile, typeShader, nShaderID);
            }
            bool OpenGLESWindow::createGLShader(const String& nameShader, const String& strTypeShader, const String& pathFile, FShaderType typeShader, uint32& nShaderID)
            {
                if (pathFile.empty())
                    return false;

                String code;
                if (!FUtil::LoadAssetFileToString(pathFile.c_str(), code))
                {
                    F_LogError("*********************** OpenGLESWindow::createGLShader failed, path: [%s] !", pathFile.c_str());
                    return false;
                }
                if (code.size() <= 0)
                {
                    return false;
                }
                const char* pCode = code.c_str();

                GLenum shaderType = Util_Transform2GLShaderType(typeShader);
                nShaderID = glCreateShader(shaderType);

                glShaderSource(nShaderID, 1, &pCode, nullptr);
                this->poShaderInclude->ShaderInclude(nameShader, 
                                                     code);
                glCompileShader(nShaderID);
                if (checkGLShaderCompileErrors(nShaderID, strTypeShader))
                {
                    F_LogError("*********************** OpenGLESWindow::createGLShader: Failed to create shader, name: [%s], type: [%s], path: [%s] !", nameShader.c_str(), strTypeShader.c_str(), pathFile.c_str());
                    return false;
                }
                F_LogInfo("OpenGLESWindow::createGLShader success, id: [%u], type: [%s], name: [%s], path: [%s] !", nShaderID, strTypeShader.c_str(), nameShader.c_str(), pathFile.c_str());

                this->poDebug->SetGLShaderName(nShaderID, "Shader-" + nameShader);
                return true;
            }
            void OpenGLESWindow::destroyGLShader(uint32 nShaderID)
            {
                if (nShaderID > 0)
                {
                    glDeleteShader(nShaderID);
                }
            }

            GLESShaderProgram* OpenGLESWindow::createShaderProgram(const String& nameShaderProgram,
                                                                   GLESShader* pShaderVertex,
                                                                   GLESShader* pShaderTessellationControl,
                                                                   GLESShader* pShaderTessellationEvaluation,
                                                                   GLESShader* pShaderGeometry,
                                                                   GLESShader* pShaderFragment)
            {
                GLESShaderProgram* pShaderProgram = new GLESShaderProgram(nameShaderProgram);
                if (!pShaderProgram->Init(pShaderVertex, 
                                          pShaderTessellationControl,
                                          pShaderTessellationEvaluation,
                                          pShaderGeometry,
                                          pShaderFragment))
                {
                    F_LogError("*********************** OpenGLESWindow::createShaderProgram failed, name: [%s] !", nameShaderProgram.c_str());
                    return nullptr;
                }
                F_LogInfo("OpenGLESWindow::createShaderProgram graphic success, id: [%u], name: [%s] !", pShaderProgram->nShaderProgramID, nameShaderProgram.c_str());

                return pShaderProgram;
            }
            GLESShaderProgram* OpenGLESWindow::createShaderProgram(const String& nameShaderProgram,
                                                               GLESShader* pShaderCompute)
            {
                GLESShaderProgram* pShaderProgram = new GLESShaderProgram(nameShaderProgram);
                if (!pShaderProgram->Init(pShaderCompute))
                {
                    F_LogError("*********************** OpenGLESWindow::createShaderProgram failed, name: [%s] !", nameShaderProgram.c_str());
                    return nullptr;
                }
                F_LogInfo("OpenGLESWindow::createShaderProgram compute success, id: [%u], name: [%s] !", pShaderProgram->nShaderProgramID, nameShaderProgram.c_str());

                return pShaderProgram;
            }

            uint32 OpenGLESWindow::createGLShaderProgram()
            {
                uint32 nShaderComputeID = glCreateProgram();
                if (nShaderComputeID <= 0)
                {
                    F_LogError("*********************** OpenGLESWindow::createGLShaderProgram: Failed to create shader program !");
                    return 0;
                }
                return nShaderComputeID;
            }
            bool OpenGLESWindow::createGLShaderProgram(const String& nameShaderProgram,
                                                       uint32 nShaderVertexID,
                                                       uint32 nShaderTessellationControlID,
                                                       uint32 nShaderTessellationEvaluationID,
                                                       uint32 nShaderGeometryID,
                                                       uint32 nShaderFragmentID,
                                                       uint32& nShaderProgramID)
            {
                if (nShaderVertexID <= 0 && nShaderFragmentID <= 0)
                    return false;
                nShaderProgramID = createGLShaderProgram();
                if (nShaderProgramID <= 0)
                    return false;

                //Shader Vertex
                if (nShaderVertexID > 0)
                {
                    glAttachShader(nShaderProgramID, nShaderVertexID);
                }
                //Shader Tessellation Control
                if (nShaderTessellationControlID > 0)
                {
                    glAttachShader(nShaderProgramID, nShaderTessellationControlID);
                }
                //Shader Tessellation Evaluation
                if (nShaderTessellationEvaluationID > 0)
                {
                    glAttachShader(nShaderProgramID, nShaderTessellationEvaluationID);
                }
                //Shader Geometry
                if (nShaderGeometryID > 0)
                {
                    glAttachShader(nShaderProgramID, nShaderGeometryID);
                }
                //Shader Fragment
                if (nShaderFragmentID > 0)
                {
                    glAttachShader(nShaderProgramID, nShaderFragmentID);
                }

                glLinkProgram(nShaderProgramID);
                if (checkGLShaderCompileErrors(nShaderProgramID, c_strShaderProgram))
                {
                    return false;
                }

                this->poDebug->SetGLShaderProgramName(nShaderProgramID, "ShaderProgram-" + nameShaderProgram);
                return true;
            }
            bool OpenGLESWindow::createGLShaderProgram(const String& nameShaderProgram,
                                                       uint32 nShaderComputeID,
                                                       uint32& nShaderProgramID)
            {
                if (nShaderComputeID <= 0)
                    return false;
                nShaderProgramID = createGLShaderProgram();
                if (nShaderProgramID <= 0)
                    return false;

                //Shader Compute
                if (nShaderComputeID > 0)
                {
                    glAttachShader(nShaderProgramID, nShaderComputeID);
                }

                glLinkProgram(nShaderProgramID);
                if (checkGLShaderCompileErrors(nShaderProgramID, c_strShaderProgram))
                {
                    return false;
                }

                this->poDebug->SetGLShaderProgramName(nShaderProgramID, "ShaderProgram-" + nameShaderProgram);
                return true;
            }
                void OpenGLESWindow::bindGLShaderProgram(uint32 nShaderProgramID)
                {
                    if (nShaderProgramID > 0)
                    {
                        glUseProgram(nShaderProgramID); 
                    }
                }   

                uint32 OpenGLESWindow::getUniformBlockIndex(uint32 nShaderProgramID, const String& name)
                {
                    uint32 index = glGetUniformBlockIndex(nShaderProgramID, name.c_str()); 
                    if (GL_INVALID_INDEX == index)
                    {
                        F_LogError("*********************** OpenGLESWindow::getUniformBlockIndex: Failed to get uniform block index from shader program: [%u], name: [%s] !", nShaderProgramID, name.c_str());
                    }
                    return index;
                }
                void OpenGLESWindow::setUniformBlockBinding(uint32 nShaderProgramID, uint32 nUniformBlockIndex, uint32 nUniformBlockBinding)
                {
                    if (nShaderProgramID > 0)
                    {
                        glUniformBlockBinding(nShaderProgramID, nUniformBlockIndex, nUniformBlockBinding); 
                    }
                }

                void OpenGLESWindow::setUniform1i(uint32 nShaderProgramID, const String& name, int value)
                {
                    setUniform1i(getUniformLocation(nShaderProgramID, name), value);
                }
                void OpenGLESWindow::setUniform1f(uint32 nShaderProgramID, const String& name, float value)
                {
                    setUniform1f(getUniformLocation(nShaderProgramID, name), value);
                }
                void OpenGLESWindow::setUniform2f(uint32 nShaderProgramID, const String& name, float x, float y)
                {
                    setUniform2f(getUniformLocation(nShaderProgramID, name), x, y);
                }
                void OpenGLESWindow::setUniform3f(uint32 nShaderProgramID, const String& name, float x, float y, float z)
                {
                    setUniform3f(getUniformLocation(nShaderProgramID, name), x, y, z);
                }
                void OpenGLESWindow::setUniform4f(uint32 nShaderProgramID, const String& name, float x, float y, float z, float w)
                {
                    setUniform4f(getUniformLocation(nShaderProgramID, name), x, y, z, w);
                }
                void OpenGLESWindow::setUniform2fv(uint32 nShaderProgramID, const String& name, const FVector2& v2)
                {
                    setUniform2fv(getUniformLocation(nShaderProgramID, name), v2);
                }
                void OpenGLESWindow::setUniform3fv(uint32 nShaderProgramID, const String& name, const FVector3& v3)
                {
                    setUniform3fv(getUniformLocation(nShaderProgramID, name), v3);
                }
                void OpenGLESWindow::setUniform4fv(uint32 nShaderProgramID, const String& name, const FVector4& v4)
                {
                    setUniform4fv(getUniformLocation(nShaderProgramID, name), v4);
                }
                void OpenGLESWindow::setUniformMatrix2fv(uint32 nShaderProgramID, const String& name, const glm::mat2& m2)
                {
                    setUniformMatrix2fv(getUniformLocation(nShaderProgramID, name), m2);
                }
                void OpenGLESWindow::setUniformMatrix3fv(uint32 nShaderProgramID, const String& name, const FMatrix3& m3)
                {
                    setUniformMatrix3fv(getUniformLocation(nShaderProgramID, name), m3);
                }
                void OpenGLESWindow::setUniformMatrix4fv(uint32 nShaderProgramID, const String& name, const FMatrix4& m4)
                {
                    setUniformMatrix4fv(getUniformLocation(nShaderProgramID, name), m4);
                }

                int OpenGLESWindow::getUniformLocation(uint32 nShaderProgramID, const String& name)
                {
                    return glGetUniformLocation(nShaderProgramID, name.c_str());
                }
                int OpenGLESWindow::getUniformLocation(uint32 nShaderProgramID, const char* name)
                {
                    return glGetUniformLocation(nShaderProgramID, name);
                }

                void OpenGLESWindow::setUniform1i(int location, int value)
                {
                    glUniform1i(location, value); 
                }
                void OpenGLESWindow::setUniform1f(int location, float value)
                {
                    glUniform1f(location, value); 
                }   
                void OpenGLESWindow::setUniform2f(int location, float x, float y)
                {
                    glUniform2f(location, x, y); 
                }
                void OpenGLESWindow::setUniform3f(int location, float x, float y, float z)
                {
                    glUniform3f(location, x, y, z); 
                }
                void OpenGLESWindow::setUniform4f(int location, float x, float y, float z, float w)
                {
                    glUniform4f(location, x, y, z, w); 
                }
                void OpenGLESWindow::setUniform2fv(int location, const FVector2& v2)
                {
                    glUniform2fv(location, 1, &v2[0]); 
                }   
                void OpenGLESWindow::setUniform3fv(int location, const FVector3& v3)
                {
                    glUniform3fv(location, 1, &v3[0]); 
                }
                void OpenGLESWindow::setUniform4fv(int location, const FVector4& v4)
                {
                    glUniform4fv(location, 1, &v4[0]); 
                }
                void OpenGLESWindow::setUniformMatrix2fv(int location, const glm::mat2& m2)
                {
                    glUniformMatrix2fv(location, 1, GL_FALSE, &m2[0][0]); 
                }
                void OpenGLESWindow::setUniformMatrix3fv(int location, const FMatrix3& m3)
                {
                    glUniformMatrix3fv(location, 1, GL_FALSE, &m3[0][0]); 
                }
                void OpenGLESWindow::setUniformMatrix4fv(int location, const FMatrix4& m4)
                {
                    glUniformMatrix4fv(location, 1, GL_FALSE, &m4[0][0]);
                }

                bool OpenGLESWindow::hasGLShaderGLSLInclude(const String& key)
                {
                    return false;
                    //return glIsNamedStringARB((int)key.size(), key.c_str());
                }
                void OpenGLESWindow::addGLShaderGLSLInclude(const String& key, const String& content)
                {
                    if (!hasGLShaderGLSLInclude(key))
                    {
                       // glNamedStringARB(GL_SHADER_INCLUDE_ARB, (int)key.size(), key.c_str(), (int)content.size(), content.c_str());
                    }
                }
                void OpenGLESWindow::removeGLShaderGLSLInclude(const String& key)
                {
                    // glDeleteNamedStringARB((int)key.size(), key.c_str());
                }

            void OpenGLESWindow::destroyGLShaderProgram(uint32 nShaderProgramID)
            {
                if (nShaderProgramID > 0)
                {
                    glDeleteProgram(nShaderProgramID);
                }
            }
            bool OpenGLESWindow::checkGLShaderCompileErrors(uint32 nShader, const String& type)
            {
                int32 nSuccess;
                std::vector<GLchar> errorLog; 
                if (type != c_strShaderProgram)
                {
                    glGetShaderiv(nShader, GL_COMPILE_STATUS, &nSuccess);
                    if (!nSuccess)
                    {
                        getGLShaderCompileErrors(nShader, errorLog);
                        F_LogError("*********************** OpenGLESWindow::checkGLShaderCompileErrors: Failed to compile shader type: [%s], error: [%s] !", type.c_str(), errorLog.data());
                        return true;
                    }
                }
                else
                {
                    glGetProgramiv(nShader, GL_LINK_STATUS, &nSuccess);
                    if (!nSuccess)
                    {
                        getGLShaderProgramCompileErrors(nShader, errorLog);
                        F_LogError("*********************** OpenGLESWindow::checkGLShaderCompileErrors: Failed to compile shader type: [%s], error: [%s] !", type.c_str(), errorLog.data());
                        return true;
                    }
                }
                return false;
            }   
            void OpenGLESWindow::getGLShaderCompileErrors(uint32 nShader, std::vector<GLchar>& errorLog)
            {
                GLint maxLength = 0;
                glGetShaderiv(nShader, GL_INFO_LOG_LENGTH, &maxLength);
                errorLog.resize(maxLength);
                glGetShaderInfoLog(nShader, maxLength, &maxLength, &errorLog[0]);
            }
            void OpenGLESWindow::getGLShaderProgramCompileErrors(uint32 nShader, std::vector<GLchar>& errorLog)
            {
                GLint maxLength = 0;
                glGetProgramiv(nShader, GL_INFO_LOG_LENGTH, &maxLength);
                errorLog.resize(maxLength);
                glGetProgramInfoLog(nShader, maxLength, &maxLength, &errorLog[0]);
            }
            

            void OpenGLESWindow::createCustomBeforePipeline()
            {
                F_LogInfo("**<2-1-4> OpenGLESWindow::createCustomBeforePipeline finish **");
            }
            void OpenGLESWindow::createGraphicsPipeline()
            {
                F_LogInfo("**<2-1-5> OpenGLESWindow::createGraphicsPipeline start **");
                {
                    //1> createGraphicsPipeline_Default
                    createGraphicsPipeline_Default();
                    F_LogInfo("<2-1-5-1> OpenGLESWindow::createGraphicsPipeline: createGraphicsPipeline_Default finish !");

                    //2> createGraphicsPipeline_Custom
                    createGraphicsPipeline_Custom();
                    F_LogInfo("<2-1-5-2> OpenGLESWindow::createGraphicsPipeline: createGraphicsPipeline_Custom finish !");
                }
                F_LogInfo("**<2-1-5> OpenGLESWindow::createGraphicsPipeline finish **");
            }
                void OpenGLESWindow::createGraphicsPipeline_Default()
                {
                    if (this->cfg_shaderVertex_Path.empty() ||
                        this->cfg_shaderFragment_Path.empty())
                    {
                        return;
                    }

                    //1> Shader
                    String nameVertexShader;
                    String namePathBase;
                    FUtilString::SplitFileName(this->cfg_shaderVertex_Path, nameVertexShader, namePathBase);
                    this->poShaderVertex = createShader(nameVertexShader, this->cfg_shaderVertex_Path, F_Shader_Vertex);
                    if (this->poShaderVertex == nullptr)
                    {
                        String msg = "*********************** OpenGLESWindow::createGraphicsPipeline_Default: Failed to create shader vertex: " + this->cfg_shaderVertex_Path;
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg);
                    }
                    
                    String nameFragmentShader;
                    FUtilString::SplitFileName(this->cfg_shaderFragment_Path, nameFragmentShader, namePathBase);
                    this->poShaderFragment = createShader(nameFragmentShader, this->cfg_shaderFragment_Path, F_Shader_Fragment);
                    if (this->poShaderFragment == nullptr)
                    {
                        String msg = "*********************** OpenGLESWindow::createGraphicsPipeline_Default: Failed to create shader fragment: " + this->cfg_shaderFragment_Path;
                        F_LogError("%s", msg.c_str());
                        throw std::runtime_error(msg);
                    }

                    //2> Shader Pipeline Graphics
                    String nameStatePipelineGraphics = "StatePipelineGraphics-Default";
					this->poStatePipelineGraphics = createStatePipelineGraphics(nameStatePipelineGraphics,
                                                                                this->pDescriptorSetLayout,
																				this->poShaderVertex,
						                                            			nullptr,
																				nullptr,
																				nullptr,
																				this->poShaderFragment,
																				this->poTypeVertex,
																				this->poTypePrimitive,
																				this->poIsCull,
																				this->poTypeFrontFace,
																				this->poTypeCulling,
																				this->poTypePolygonMode,
																				this->poDepthEnabled,
																				this->poDepthFuncCompare,
																				this->poDepthTestEnabled,
																				this->poDepthWriteEnabled,
																				this->poStencilEnabled,
																				this->poStencil_CompareFunction,
																				this->poStencil_StencilFailureOp,
																				this->poStencil_DepthFailureOp,
																				this->poStencil_DepthStencilPassOp,
																				this->poStencil_Ref,
																				this->poStencil_Mask,
																				this->poBlendEnabled,
																				this->poBlendColorFactorSrc,
																				this->poBlendColorFactorDst,
																				this->poBlendColorOp,
																				this->poBlendAlphaFactorSrc,
																				this->poBlendAlphaFactorDst,
																				this->poBlendAlphaOp,
																				this->poColorWriteMask_Red,
																				this->poColorWriteMask_Green,
																				this->poColorWriteMask_Blue,
																				this->poColorWriteMask_Alpha);
					if (!this->poStatePipelineGraphics)
					{
						String msg = "*********************** OpenGLESWindow::createGraphicsPipeline_Default: Failed to create state pipeline graphics: " + nameStatePipelineGraphics;
						F_LogError("%s", msg.c_str());
						throw std::runtime_error(msg);
					}

                }
                void OpenGLESWindow::createGraphicsPipeline_Custom()
                {

                }

				 	GLESStatePipelineGraphics* OpenGLESWindow::createStatePipelineGraphics(const String& nameStatePipelineGraphics,
                                                                                           DescriptorSetLayout* pDSL,
																						   GLESShaderProgram* pShaderProgram,
																						   bool deleteShaderProgram,
																						   FMeshVertexType typeVertex,
																						   GLenum typePrimitive,
																						   bool isCull,
																						   GLenum typeFrontFace,
																						   GLenum typeCulling,
																						   GLenum typePolygonMode,
																						   bool depthEnabled,
																						   GLenum depthFuncCompare,
																						   bool depthTestEnabled,
																						   bool depthWriteEnabled,
																						   bool stencilEnabled,
																						   GLenum stencil_CompareFunction,
																						   GLenum stencil_StencilFailureOp,
																						   GLenum stencil_DepthFailureOp,
																						   GLenum stencil_DepthStencilPassOp,
																						   uint32_t stencil_Ref,
																						   uint32_t stencil_Mask,
																						   bool blendEnabled,
																						   GLenum blendColorFactorSrc, 
																						   GLenum blendColorFactorDst,
																						   GLenum blendColorOp,
																						   GLenum blendAlphaFactorSrc, 
																						   GLenum blendAlphaFactorDst,
																						   GLenum blendAlphaOp,
																						   GLboolean colorWriteMask_Red,
																						   GLboolean colorWriteMask_Green,
																						   GLboolean colorWriteMask_Blue,
																						   GLboolean colorWriteMask_Alpha)
					{
						GLESStatePipelineGraphics* pStatePipelineGraphics = new GLESStatePipelineGraphics(nameStatePipelineGraphics);
						if (!pStatePipelineGraphics->Init(pDSL,
                                                          pShaderProgram,
														  deleteShaderProgram,
														  typeVertex,
														  typePrimitive,
														  isCull,
														  typeFrontFace,
														  typeCulling,
														  typePolygonMode,
														  depthEnabled,
														  depthFuncCompare,
														  depthTestEnabled,
														  depthWriteEnabled,
														  stencilEnabled,
														  stencil_CompareFunction,
														  stencil_StencilFailureOp,
														  stencil_DepthFailureOp,
														  stencil_DepthStencilPassOp,
														  stencil_Ref,
														  stencil_Mask,
														  blendEnabled,
														  blendColorFactorSrc, 
														  blendColorFactorDst,
														  blendColorOp,
														  blendAlphaFactorSrc, 
														  blendAlphaFactorDst,
														  blendAlphaOp,
														  colorWriteMask_Red,
														  colorWriteMask_Green,
														  colorWriteMask_Blue,
														  colorWriteMask_Alpha))
						{
							F_DELETE(pStatePipelineGraphics)
							return nullptr;
						}
						return pStatePipelineGraphics;
					}
 					GLESStatePipelineGraphics* OpenGLESWindow::createStatePipelineGraphics(const String& nameStatePipelineGraphics,
                                                                                           DescriptorSetLayout* pDSL,
																						   GLESShader* pShaderVertex,
																						   GLESShader* pShaderTessellationControl,
																						   GLESShader* pShaderTessellationEvaluation,
																						   GLESShader* pShaderGeometry,
																						   GLESShader* pShaderFragment,
																						   FMeshVertexType typeVertex,
																						   GLenum typePrimitive,
																						   bool isCull,
																						   GLenum typeFrontFace,
																						   GLenum typeCulling,
																						   GLenum typePolygonMode,
																						   bool depthEnabled,
																						   GLenum depthFuncCompare,
																						   bool depthTestEnabled,
																						   bool depthWriteEnabled,
																						   bool stencilEnabled,
																						   GLenum stencil_CompareFunction,
																						   GLenum stencil_StencilFailureOp,
																						   GLenum stencil_DepthFailureOp,
																						   GLenum stencil_DepthStencilPassOp,
																						   uint32_t stencil_Ref,
																						   uint32_t stencil_Mask,
																						   bool blendEnabled,
																						   GLenum blendColorFactorSrc, 
																						   GLenum blendColorFactorDst,
																						   GLenum blendColorOp,
																						   GLenum blendAlphaFactorSrc, 
																						   GLenum blendAlphaFactorDst,
																						   GLenum blendAlphaOp,
																						   GLboolean colorWriteMask_Red,
																						   GLboolean colorWriteMask_Green,
																						   GLboolean colorWriteMask_Blue,
																						   GLboolean colorWriteMask_Alpha)
					{
						GLESStatePipelineGraphics* pStatePipelineGraphics = new GLESStatePipelineGraphics(nameStatePipelineGraphics);
						if (!pStatePipelineGraphics->Init(pDSL,
                                                          pShaderVertex,
														  pShaderTessellationControl,
													   	  pShaderTessellationEvaluation,
														  pShaderGeometry,
														  pShaderFragment,
														  typeVertex,
														  typePrimitive,
														  isCull,
														  typeFrontFace,
														  typeCulling,
														  typePolygonMode,
														  depthEnabled,
														  depthFuncCompare,
														  depthTestEnabled,
														  depthWriteEnabled,
														  stencilEnabled,
														  stencil_CompareFunction,
														  stencil_StencilFailureOp,
														  stencil_DepthFailureOp,
														  stencil_DepthStencilPassOp,
														  stencil_Ref,
														  stencil_Mask,
														  blendEnabled,
														  blendColorFactorSrc, 
														  blendColorFactorDst,
														  blendColorOp,
														  blendAlphaFactorSrc, 
														  blendAlphaFactorDst,
														  blendAlphaOp,
														  colorWriteMask_Red,
														  colorWriteMask_Green,
														  colorWriteMask_Blue,
														  colorWriteMask_Alpha))
						{
							F_DELETE(pStatePipelineGraphics)
							return nullptr;
						}
						return pStatePipelineGraphics;
					}
            void OpenGLESWindow::createComputePipeline()
            {
                F_LogInfo("**<2-1-6> OpenGLESWindow::createComputePipeline start **");
                {
                    //1> createComputePipeline_Default
                    createComputePipeline_Default();

                    //2> createComputePipeline_Custom
                    createComputePipeline_Custom();
                }
                F_LogInfo("**<2-1-6> OpenGLESWindow::createComputePipeline finish **");
            }
                void OpenGLESWindow::createComputePipeline_Default()
                {

                }
                void OpenGLESWindow::createComputePipeline_Custom()
                {

                }

                    GLESStatePipelineCompute* OpenGLESWindow::createStatePipelineCompute(const String& nameStatePipelineCompute,
                                                                                         DescriptorSetLayout* pDSL,
                                                                                         GLESShader* pShaderCompute)
					{
						GLESStatePipelineCompute* pStatePipelineCompute = new GLESStatePipelineCompute(nameStatePipelineCompute);
                        if (!pStatePipelineCompute->Init(pDSL,
                                                         pShaderCompute)) 
                        {
                            F_LogError("*********************** OpenGLESWindow::createStatePipelineCompute failed, name: [%s] !", nameStatePipelineCompute.c_str());
                            F_DELETE(pStatePipelineCompute)
                            return nullptr;
                        }

                        return pStatePipelineCompute;
					}


            void OpenGLESWindow::createDescriptorSets()
            {
                F_LogInfo("**<2-1-7> OpenGLESWindow::createDescriptorSets start **");
                {
                    //1> createDescriptorSets_Default
                    createDescriptorSets_Default();
                    F_LogInfo("<2-1-7-1> OpenGLESWindow::createDescriptorSets: createDescriptorSets_Default finish !");

                    //2> createDescriptorSets_Terrain
                    createDescriptorSets_Terrain();
                    F_LogInfo("<2-1-7-2> OpenGLESWindow::createDescriptorSets: createDescriptorSets_Terrain finish !");

                    //3> createDescriptorSets_Custom
                    createDescriptorSets_Custom();
                    F_LogInfo("<2-1-7-3> OpenGLESWindow::createDescriptorSets: createDescriptorSets_Custom finish !");
                }
                F_LogInfo("**<2-1-7> OpenGLESWindow::createDescriptorSets finish **");
            }
                void OpenGLESWindow::createDescriptorSets_Default()
                {
                    if (this->pDescriptorSetLayout == nullptr)
                    {
						if (this->poTexture != nullptr)
						{
							this->poStatePipelineGraphics->BindTextureFS(this->poTexture, 0);
						}
						return;
					}

                    updateDescriptorSets(this->pDescriptorSetLayout, this->poStatePipelineGraphics);
                }
                void OpenGLESWindow::createDescriptorSets_Terrain()
                {

                }
                void OpenGLESWindow::createDescriptorSets_Custom()
                {

                }
                    void OpenGLESWindow::updateDescriptorSets(DescriptorSetLayout* pDSL, GLESStatePipelineGraphics* pStatePipelineGraphics)
                    {
                        F_Assert(pDSL && pStatePipelineGraphics && "OpenGLESWindow::updateDescriptorSets")
                        
                        uint32 count_fb = (uint32)this->poSwapChains.size();
                        uint32 count_ds = (uint32)pDSL->aLayouts.size();
						uint32 nBindingTextureFragementIndex = 0;
                        for (uint32 i = 0; i < count_ds; i++)
                        {
                            const String& nameDS = pDSL->aLayouts[i];

                            if (nameDS == Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants)) //PassConstants
                            {
								uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDS);
								uint32 nBindingIndex = (uint32)DescriptorSet_PassConstants;
								pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
                            }
                            else if (nameDS == Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants)) //ObjectConstants
                            {
								uint32 nUniformBlockIndex = pStatePipelineGraphics->GetUniformBlockIndex(nameDS);
								uint32 nBindingIndex = (uint32)DescriptorSet_ObjectConstants;
								pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
                            }
							else if (nameDS == Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants)) //MaterialConstants
							{

							}
							else if (nameDS == Util_GetDescriptorSetTypeName(DescriptorSet_InstanceConstants)) //InstanceConstants
							{

							}
							else if (nameDS == Util_GetDescriptorSetTypeName(DescriptorSet_TextureFS)) //TextureFS
							{
								if (this->poTexture != nullptr)
								{
									pStatePipelineGraphics->BindTextureFS(this->poTexture, nBindingTextureFragementIndex);
									nBindingTextureFragementIndex ++;
								}
							}
                            else
                            {
                                String msg = "*********************** OpenGLESWindow::updateDescriptorSets: Wrong DescriptorSet name: " + nameDS;
                                F_LogError("%s", msg.c_str());
                                throw std::runtime_error(msg.c_str());
                            }
                        }
                    }



        void OpenGLESWindow::createImgui()
        {
            F_LogInfo("**********<2-2> OpenGLESWindow::createImgui start **********");
            {
                //1> createImgui_DescriptorPool
                //createImgui_DescriptorPool();
                
                //2> createImgui_Init
                createImgui_Init();
            }
            F_LogInfo("**********<2-2> OpenGLESWindow::createImgui finish **********");
        }
            void OpenGLESWindow::createImgui_Init()
            {
                //1> Config
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO();
                this->imgui_PathIni = this->pathBin + "Log/" + this->nameTitle + ".ini";
                this->imgui_PathLog = this->pathBin + "Log/" + this->nameTitle + ".log";
                io.IniFilename = this->imgui_PathIni.c_str();
                io.LogFilename = this->imgui_PathLog.c_str();

                //2> Setup Dear ImGui style
                ImGui::StyleColorsDark();
                //ImGui::StyleColorsClassic();

                //3> Setup Platform/Renderer backends
                ImGui_ImplAndroid_Init(App::GetAndroidApp()->window);
                ImGui_ImplOpenGL3_Init("#version 300 es");

                //4> Imgui Font
                ImFontConfig font_cfg;
                font_cfg.SizePixels = 22.0f;
                io.Fonts->AddFontDefault(&font_cfg);

                //5> Arbitrary scale-up
                ImGui::GetStyle().ScaleAllSizes(3.0f);

                F_LogInfo("<2-2-1> OpenGLESWindow::createImgui_Init finish !");
            }


        void OpenGLESWindow::createEditor()
        {
			F_LogInfo("**********<2-3> OpenGLESWindow::createEditor start **********");
			{
				//1> createEditor_Grid
				createEditor_Grid();
				
				//2> createEditor_CameraAxis
				createEditor_CameraAxis();

				//3> createEditor_CoordinateAxis
				createEditor_CoordinateAxis();

				//4> createEditor_LineFlat2DCollector
				createEditor_LineFlat2DCollector();

				//5> createEditor_LineFlat3DCollector
				createEditor_LineFlat3DCollector();
			}
			F_LogInfo("**********<2-3> OpenGLESWindow::createEditor finish **********");
        }
            void OpenGLESWindow::createEditor_Grid()
            {
				this->pEditorGrid = new EditorGrid();
				this->pEditorGrid->Init();

				F_LogInfo("<2-3-1> OpenGLESWindow::createEditor_Grid finish !");
            }
            void OpenGLESWindow::createEditor_CameraAxis()
            {
				this->pEditorCameraAxis = new EditorCameraAxis();
				this->pEditorCameraAxis->Init();

				F_LogInfo("<2-3-2> OpenGLESWindow::createEditor_CameraAxis finish !");
            }
            void OpenGLESWindow::createEditor_CoordinateAxis()    
            {

            }
            void OpenGLESWindow::createEditor_LineFlat2DCollector()
            {

            }
            void OpenGLESWindow::createEditor_LineFlat3DCollector()
            {

            }
        void OpenGLESWindow::destroyEditor()
        {
			F_DELETE(this->pEditorGrid)
			F_DELETE(this->pEditorCameraAxis)
			F_DELETE(this->pEditorCoordinateAxis)
			F_DELETE(this->pEditorLineFlat2DCollector)
			F_DELETE(this->pEditorLineFlat3DCollector)
        }


    void OpenGLESWindow::resizeWindow(int w, int h, bool force)
    {
        if (this->width == w &&
            this->height == h &&
            !force)
        {
            return;
        }
        this->width = w;
        this->height = h;

		refreshFramebufferSize(w, h);
        RefreshAspectRatio();
    }
	void OpenGLESWindow::refreshFramebufferSize(int w, int h)
	{
		float scaleX = 1.0f;
		float scaleY = 1.0f;
		//glfwGetWindowContentScale(this->pWindow, &scaleX, &scaleY);
		this->poWindowContentScale.x = scaleX;
		this->poWindowContentScale.y = scaleY;

		int frameW = w;
		int frameH = h;
		//glfwGetFramebufferSize(this->pWindow, &frameW, &frameH);
		this->poFramebufferSize.x = (float)frameW;
		this->poFramebufferSize.y = (float)frameH;
	}

    //Compute Before Render
    bool OpenGLESWindow::beginCompute_BeforeRender() 
    {
        if (!this->cfg_isUseComputeShaderBeforeRender)
        {
            return false;
        }
        return true;
    }
        void OpenGLESWindow::updateCompute_BeforeRender()
        {
            //CommandBuffer
            updateComputeCommandBuffer_BeforeRender();

        }
            void OpenGLESWindow::updateComputeCommandBuffer_BeforeRender()
            {
                updateCompute_BeforeRender_Default();
                updateCompute_BeforeRender_Terrain();
                updateCompute_BeforeRender_Custom();
                updateCompute_BeforeRender_Cull();
            }
                void OpenGLESWindow::updateCompute_BeforeRender_Default()
                {

                }
                void OpenGLESWindow::updateCompute_BeforeRender_Terrain()
                {

                }
                void OpenGLESWindow::updateCompute_BeforeRender_Custom()
                {

                }
                void OpenGLESWindow::updateCompute_BeforeRender_Cull()
                {

                }
                    
        void OpenGLESWindow::compute_BeforeRender()
        {

        }
    void OpenGLESWindow::endCompute_BeforeRender()
    {

    }

    //Compute After Render
    bool OpenGLESWindow::beginCompute_AfterRender()
    {
        return true;
    }
        void OpenGLESWindow::updateCompute_AfterRender()
        {

        }
            void OpenGLESWindow::updateComputeCommandBuffer_AfterRender()
            {

            }
                void OpenGLESWindow::updateCompute_AfterRender_Default()
                {

                }
                void OpenGLESWindow::updateCompute_AfterRender_Custom()
                {

                }
                void OpenGLESWindow::updateCompute_AfterRender_HizDepthGenerate()
                {

                }

        void OpenGLESWindow::compute_AfterRender()
        {

        }
    void OpenGLESWindow::endCompute_AfterRender()
    {

    }


    bool OpenGLESWindow::beginRender()
    {
        GLESFrameBuffer* pFrameBuffer = this->poFrameBuffers[this->poCurrentFrame];
        this->poRenderPass->SetFrameBuffer(pFrameBuffer);
        return true;
    }
        void OpenGLESWindow::updateRender()
        {
            //1> updateCBs_Default
            updateCBs_Default();

            //2> updateCBs_Terrain
            updateCBs_Terrain();

            //3> updateCBs_ImGUI
            updateCBs_ImGUI();

            //4> updateCBs_Editor
            updateCBs_Editor();

            //5> CB Custom
            updateCBs_Custom();

            //6> CommandBuffers
            updateRenderCommandBuffers_CustomBeforeDefault();
            {
                updateRenderCommandBuffers_Default();
            }
            updateRenderCommandBuffers_CustomAfterDefault();

			//7> BlitToFrame
			updateRenderPass_BlitFromFrame();
        }
            void OpenGLESWindow::updateCBs_Default()
            {
                updateCBs_Pass();
                updateCBs_Objects();
                updateCBs_Materials();
                updateCBs_Instances();
            }
                void OpenGLESWindow::updateCBs_Pass()
                {
                    //TransformConstants/CameraConstants
                    if (this->pCamera != nullptr)
                    {
                        updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
                        if (this->pCameraRight == nullptr)
                        {
                            this->passCB.g_Transforms[1] = this->passCB.g_Transforms[0];
                            this->passCB.g_Cameras[1] = this->passCB.g_Cameras[0];
                        }
                    }
                    else
                    {
                        TransformConstants& transformConstants = this->passCB.g_Transforms[0];
                        transformConstants.mat4View = glm::lookAtLH(this->cfg_cameraPos, 
                                                                    this->cfg_cameraLookTarget,
                                                                    this->cfg_cameraUp);
                        transformConstants.mat4View_Inv = FMath::InverseMatrix4(transformConstants.mat4View);
                        transformConstants.mat4Proj = glm::perspectiveLH(glm::radians(this->cfg_cameraFov), 
                                                                         this->poViewport.right / (float)this->poViewport.bottom,
                                                                         this->cfg_cameraNear, 
                                                                         this->cfg_cameraFar);
                        transformConstants.mat4Proj_Inv = FMath::InverseMatrix4(transformConstants.mat4Proj);
                        transformConstants.mat4ViewProj = transformConstants.mat4Proj * transformConstants.mat4View;
                        transformConstants.mat4ViewProj_Inv = FMath::InverseMatrix4(transformConstants.mat4ViewProj);
                        
                        //CameraConstants
                        CameraConstants& cameraConstants = this->passCB.g_Cameras[0];
                        cameraConstants.posEyeWorld = this->cfg_cameraPos;
                        cameraConstants.fNearZ = this->cfg_cameraNear;
                        cameraConstants.fFarZ = this->cfg_cameraFar;
                    }   
                    if (this->pCameraRight != nullptr)
                    {
                        updateCBs_PassTransformAndCamera(this->passCB, this->pCameraRight, 1); 
                    }

                    //TimeConstants
                    this->passCB.g_TotalTime = this->pTimer->GetTimeSinceStart();
                    this->passCB.g_DeltaTime = this->pTimer->GetTimeDelta();

                    //RenderTarget
                    this->passCB.g_RenderTargetSize = FVector2(this->poViewport.right, this->poViewport.bottom);
                    this->passCB.g_RenderTargetSize_Inv = FVector2(1.0f / this->poViewport.right, 1.0f / this->poViewport.bottom);

                    //Light Settings
                    if (this->cfg_isRenderPassShadowMap)
                    {
                        const FMatrix4& depthViewMatrix = this->pCameraMainLight->GetMatrix4View();
                        const FMatrix4& depthProjectionMatrix = this->pCameraMainLight->GetMatrix4Projection();
                        this->mainLight.depthMVP = depthProjectionMatrix * depthViewMatrix;
                    }
                    memcpy(&this->passCB.g_MainLight, &this->mainLight, sizeof(LightConstants));
                    for (int i = 0; i < MAX_LIGHT_COUNT; i++)
                    {
                        memcpy(&this->passCB.g_AdditionalLights[i], &this->aAdditionalLights[i], sizeof(LightConstants));
                    }

                    //Update Buffer
                    GLESBufferUniform* pBufferUniform = this->poBuffers_PassCB[this->poCurrentFrame];
                    pBufferUniform->UpdateBuffer(sizeof(PassConstants),
                                                 (uint8*)(&this->passCB),
                                                 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
					if (this->poStatePipelineGraphics != nullptr)
					{
						this->poStatePipelineGraphics->BindBufferUniform(pBufferUniform, pBufferUniform->GetBindingIndex());
					}
                }
                    void OpenGLESWindow::updateCBs_PassTransformAndCamera(PassConstants& pass, FCamera* pCam, int nIndex)
                    {
                        //TransformConstants
                        TransformConstants& transformConstants = pass.g_Transforms[nIndex];
                        transformConstants.mat4View = pCam->GetMatrix4View();
                        transformConstants.mat4View_Inv = FMath::InverseMatrix4(transformConstants.mat4View);
                        transformConstants.mat4Proj = pCam->GetMatrix4Projection();
                        transformConstants.mat4Proj_Inv = FMath::InverseMatrix4(transformConstants.mat4Proj);
                        transformConstants.mat4ViewProj = transformConstants.mat4Proj * transformConstants.mat4View;
                        transformConstants.mat4ViewProj_Inv = FMath::InverseMatrix4(transformConstants.mat4ViewProj);

                        //CameraConstants
                        CameraConstants& cameraConstants = pass.g_Cameras[nIndex];
                        cameraConstants.posEyeWorld = pCam->GetPos();
                        cameraConstants.fNearZ = pCam->GetNearZ();
                        cameraConstants.fFarZ = pCam->GetFarZ();
                    }
                void OpenGLESWindow::updateCBs_Objects()
                {
                    if (this->poBuffers_ObjectCB.size() <= 0)
                        return;

                    size_t count = this->objectCBs.size();
                    if (count >= MAX_OBJECT_COUNT)
                    {
                        F_LogError("*********************** OpenGLESWindow::updateCBs_Objects: Max object count can not > [%d]", MAX_OBJECT_COUNT);
                        return;
                    }

                    updateCBs_ObjectsContent();

                    //Update Buffer
                    GLESBufferUniform* pBufferUniform = this->poBuffers_ObjectCB[this->poCurrentFrame];
                    pBufferUniform->UpdateBuffer(sizeof(ObjectConstants) * count,
                                                 (uint8*)this->objectCBs.data(),
												 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
					if (this->poStatePipelineGraphics != nullptr)
					{
						this->poStatePipelineGraphics->BindBufferUniform(pBufferUniform, pBufferUniform->GetBindingIndex());
					}
                }
                    void OpenGLESWindow::updateCBs_ObjectsContent()
                    {
                        ObjectConstants& objectCB = this->objectCBs[0];
                        if (this->cfg_isRotate)
                        {
                            float time = this->pTimer->GetTimeSinceStart();
                            objectCB.g_MatWorld = glm::rotate(this->poMatWorld, 
                                                              time * glm::radians(90.0f), 
                                                              FVector3(0.0f, 1.0f, 0.0f));
                        }
                        else
                        {
                            objectCB.g_MatWorld = this->poMatWorld;
                        }
                    }
                void OpenGLESWindow::updateCBs_Materials()    
                {

                }
                    void OpenGLESWindow::updateCBs_MaterialsContent()
                    {

                    }
                void OpenGLESWindow::updateCBs_Instances()
                {

                }
                    void OpenGLESWindow::updateCBs_InstancesContent()
                    {

                    }
                void OpenGLESWindow::updateCBs_Terrain()
                {

                }
                void OpenGLESWindow::updateCBs_ImGUI()
                {
                    if (IsEnable_Imgui())
                    {
                        if (beginRenderImgui())
                        {
                            endRenderImgui();
                        }
                    }
                }
                    bool OpenGLESWindow::beginRenderImgui()
                    {   

                        return false;
                    }
                        void OpenGLESWindow::commonConfig()
                        {
                            ImGui::Text("Frametime: %f", this->fFPS);

                            ImGui::Separator();
                            commonShowConfig();

                            ImGui::Separator();
                            commonEditorConfig();
                        }
                            void OpenGLESWindow::commonShowConfig()
                            {
                                if (ImGui::CollapsingHeader("Common Show"))
                                {
                                    ImGui::Text("Op ResetCamera -- Key R");
                                    ImGui::Text("Op WireFrame -- Key F");
                                    ImGui::Text("Op Rotate -- Key T");

                                    ImGui::Separator();
                                    ImGui::Text("Viewport Offset: [%d, %d]",
                                                this->poViewport.left,
                                                this->poViewport.top);
                                    ImGui::Text("Viewport Size: [%d, %d]",
                                                this->poViewport.right,
                                                this->poViewport.bottom);
                                    ImGui::Text("Framebuffer Size: [%f, %f]",
                                                this->poFramebufferSize.x,
                                                this->poFramebufferSize.y);
                                    ImGui::Text("Glfw WindowContent Scale: [%f, %f]",
                                                this->poWindowContentScale.x,
                                                this->poWindowContentScale.y);

                                    ImGui::Separator();
                                    ImGui::Text("Mouse Screen XY: [%f, %f]", 
                                                this->mousePosLast.x,
                                                this->mousePosLast.y);
                                    if (this->poViewport.right > 0 && this->poViewport.bottom > 0)
                                    ImGui::Text("Mouse Viewport XY: [%f, %f]", 
                                                this->mousePosLast.x / this->poViewport.right,
                                                this->mousePosLast.y / this->poViewport.bottom);
                                    ImGui::Text("Mouse NDC XY: [%f, %f]", 
                                                (float)((this->mousePosLast.x - this->poViewport.left) * 2.0f / this->poViewport.right - 1.0f),
                                                (float)(1.0f - (this->mousePosLast.y - this->poViewport.top) * 2.0f / this->poViewport.bottom));
                                    ImGui::Text("Mouse LeftDown: [%s]", 
                                                this->mouseButtonDownLeft ? "true" : "false");
                                    ImGui::Text("Mouse RightDown: [%s]", 
                                                this->mouseButtonDownRight ? "true" : "false");
                                    ImGui::Text("Mouse MiddleDown: [%s]", 
                                                this->mouseButtonDownMiddle ? "true" : "false");

                                    ImGui::Separator();

                                }
                            }
                            void OpenGLESWindow::commonEditorConfig()
                            {
                                if (ImGui::Checkbox("Is WireFrame", &cfg_isWireFrame))
                                {
                                    SetIsWireFrame(cfg_isWireFrame);
                                }
                                ImGui::Checkbox("Is Rotate", &cfg_isRotate);
                                ImGui::Separator();
                                if (ImGui::CollapsingHeader("EditorGrid Settings"))
                                {
                                    ImGui::Checkbox("Is EditorGridShow", &cfg_isEditorGridShow);
                                    if (this->pEditorGrid != nullptr)
                                    {
                                        if (ImGui::ColorEdit4("EditorGrid Color", (float*)&this->cfg_editorGrid_Color))
                                        {
                                            this->pEditorGrid->SetColor(this->cfg_editorGrid_Color);
                                        }
                                    }
                                }
                                if (ImGui::CollapsingHeader("EditorCameraAxis Settings"))
                                {
                                    ImGui::Checkbox("Is EditorCameraAxisShow", &cfg_isEditorCameraAxisShow);

                                }
                            }
                        void OpenGLESWindow::cameraConfig()
                        {
                            if (this->pCamera == nullptr)
                                return;

                            if (ImGui::CollapsingHeader("Camera Settings"))
                            {
                                if (ImGui::Button("Camera Reset"))
                                {
                                    cameraReset();
                                }
                                if (ImGui::CollapsingHeader("Camera Transform"))
                                {
                                    //Position
                                    FVector3 vPos = this->pCamera->GetPos();
                                    if (ImGui::DragFloat3("Position", &vPos[0], 0.05f, -FLT_MAX, FLT_MAX))
                                    {
                                        this->pCamera->SetPos(vPos);
                                        this->pCamera->UpdateViewMatrix();
                                    }
                                    //Rotation
                                    FVector3 vEulerAngle = this->pCamera->GetEulerAngles();
                                    if (ImGui::DragFloat3("Rotation", &vEulerAngle[0], 0.1f, -180, 180))
                                    {
                                        this->pCamera->SetEulerAngles(vEulerAngle);
                                        this->pCamera->UpdateViewMatrix();
                                    }
                                    ImGui::Spacing();
                                    //Right
                                    FVector3 vRight = this->pCamera->GetRight();
                                    if (ImGui::DragFloat3("Right (X axis)", &vRight[0], 0.1f, -1.0f, 1.0f))
                                    {
                                        
                                    }
                                    //Up
                                    FVector3 vUp = this->pCamera->GetUp();
                                    if (ImGui::DragFloat3("Up (Y axis)", &vUp[0], 0.1f, -1.0f, 1.0f))
                                    {
                                        
                                    }
                                    //Direction
                                    FVector3 vDir = this->pCamera->GetDir();
                                    if (ImGui::DragFloat3("Direction (Z axis)", &vDir[0], 0.1f, -1.0f, 1.0f))
                                    {
                                        
                                    }
                                }
                                if (ImGui::CollapsingHeader("Camera Param"))
                                {
                                    //FovY
                                    float fAngle = this->pCamera->GetFovY();
                                    if (ImGui::DragFloat("FovY Angle", &fAngle, 0.1f, 0.1f, 180.0f))
                                    {
                                        this->pCamera->SetFovY(fAngle);
                                        this->pCamera->UpdateProjectionMatrix();
                                    }
                                    //Aspect
                                    float fAspect = this->pCamera->GetAspect();
                                    if (ImGui::DragFloat("Aspect", &fAspect, 0.1f, 0.1f, 10.0f))
                                    {
                                        this->pCamera->SetAspect(fAspect);
                                        this->pCamera->UpdateProjectionMatrix();
                                    }

                                    //NearZ/FarZ
                                    float fNearDist = this->pCamera->GetNearZ();
                                    float fFarDist = this->pCamera->GetFarZ();
                                    if (ImGui::DragFloat("Near Distance", &fNearDist, 0.1f, 0.1f, fFarDist - 1.0f))
                                    {
                                        this->pCamera->SetNearZ(fNearDist);
                                        this->pCamera->UpdateProjectionMatrix();
                                    }
                                    if (ImGui::DragFloat("Far Distance", &fFarDist, 0.1f, fNearDist + 1.0f, FLT_MAX))
                                    {
                                        this->pCamera->SetFarZ(fFarDist);
                                        this->pCamera->UpdateProjectionMatrix();
                                    }

                                    ImGui::Separator();
                                    ImGui::Spacing();
                                    
                                    //SpeedMove
                                    if (ImGui::DragFloat("Speed Move", &cfg_cameraSpeedMove, 0.1f, 1.0f, 10000.0f))
                                    {
                                        
                                    }
                                    //SpeedZoom
                                    if (ImGui::DragFloat("Speed Zoom", &cfg_cameraSpeedZoom, 0.001f, 0.01f, 5.0f))
                                    {

                                    }
                                    //SpeedRotate
                                    if (ImGui::DragFloat("Speed Rotate", &cfg_cameraSpeedRotate, 0.001f, 0.001f, 5.0f))
                                    {

                                    }
                                }
                                if (ImGui::CollapsingHeader("Camera Matrix4 World"))
                                {
                                    FMatrix4 mat4World = this->pCamera->GetMatrix4World();
                                    if (ImGui::BeginTable("split_camera_world", 4))
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
                                if (ImGui::CollapsingHeader("Camera Matrix4 View"))
                                {
                                    const FMatrix4& mat4View = this->pCamera->GetMatrix4View();
                                    if (ImGui::BeginTable("split_camera_view", 4))
                                    {
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][3]);

                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][3]);

                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][3]);

                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][3]);
                                    
                                        ImGui::EndTable();
                                    }
                                }
                                if (ImGui::CollapsingHeader("Camera Matrix4 Projection"))
                                {
                                    const FMatrix4& mat4Projection = pCamera->GetMatrix4Projection();
                                    if (ImGui::BeginTable("split_camera_projection", 4))
                                    {
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][3]);

                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][3]);

                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][3]);

                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][0]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][1]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][2]);
                                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][3]);

                                        ImGui::EndTable();
                                    }
                                }
                            }
                            ImGui::Separator();
                            ImGui::Spacing();
                        }
                            void OpenGLESWindow::cameraReset()
                            {
                                if (this->pCamera == nullptr)
                                    return;

                                this->pCamera->LookAtLH(this->cfg_cameraPos, this->cfg_cameraLookTarget, this->cfg_cameraUp);
                                this->pCamera->PerspectiveLH(this->cfg_cameraFov, this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
                                this->pCamera->UpdateViewMatrix();
                            }
                        void OpenGLESWindow::lightConfig()
                        {
                            if (ImGui::CollapsingHeader("Light Settings"))
                            {
                                //Main Light
                                lightConfigItem(this->mainLight, "Light - Main", 0, false, true);
                                
                                //Additional Light
                                int count_light = MAX_LIGHT_COUNT;
                                for (int i = 0; i < count_light; i++)
                                {
                                    LightConstants& lc = this->aAdditionalLights[i];
                                    String nameLight = "Light - " + FUtilString::SaveInt(i);
                                    lightConfigItem(lc, nameLight, i, true, false);
                                }
                            }
                            ImGui::Separator();
                            ImGui::Spacing();
                        }
                            void OpenGLESWindow::lightConfigItem(LightConstants& lc, const String& name, int index, bool canChangeType, bool bIsMainLight)
                            {

                            }
                            void OpenGLESWindow::lightMainReset()
                            {

                            }
                        void OpenGLESWindow::shadowConfig()
                        {

                        }
                            void OpenGLESWindow::shadowConfigItem(ShadowConstants& sc, const String& name, bool bIsMainLight)
                            {

                            }
                            void OpenGLESWindow::shadowReset()
                            {

                            }
                        void OpenGLESWindow::cullConfig()
                        {

                        }
                        void OpenGLESWindow::terrainConfig()
                        {

                        }
                            void OpenGLESWindow::terrainConfigItem(TerrainConstants& tc, const String& name)
                            {

                            }
                                bool OpenGLESWindow::terrainConfigSplatItem(TerrainSplatConstants& tsc, const String& name)
                                {
                                    return true;
                                }
                            void OpenGLESWindow::terrainReset()
                            {

                            }
                        void OpenGLESWindow::passConstantsConfig()
                        {
                            if (ImGui::CollapsingHeader("PassConstants Settings"))
                            {
                                //g_AmbientLight
                                if (ImGui::ColorEdit4("Global AmbientLight", (float*)&(this->passCB.g_AmbientLight)))
                                {
                                    
                                }
                            }
                            ImGui::Separator();
                            ImGui::Spacing();
                        }
                        void OpenGLESWindow::modelConfig()
                        {

                        }
                void OpenGLESWindow::endRenderImgui()
                {
                    ImGui::Render();
                }
            void OpenGLESWindow::updateCBs_Editor()
            {
				if (this->pEditorGrid != nullptr)
                {
                    this->pEditorGrid->UpdateCBs();
                }
                if (this->pEditorCameraAxis != nullptr)
                {
                    this->pEditorCameraAxis->UpdateCBs();
                }
                // if (this->pEditorCoordinateAxis != nullptr)
                // {
                //     this->pEditorCoordinateAxis->UpdateCBs();
                // }
            }
            void OpenGLESWindow::updateCBs_Custom()
            {

            }

            void OpenGLESWindow::updateRenderCommandBuffers_CustomBeforeDefault()
            {

            }
            void OpenGLESWindow::updateRenderCommandBuffers_Default()
            {

				updateRenderPass_EditorCameraAxis();

                {
                    updateRenderPass_CustomBeforeDefault();
                    {
                        updateRenderPass_Default();
                    }
                    updateRenderPass_CustomAfterDefault();
                }

            }
				void OpenGLESWindow::updateRenderPass_EditorCameraAxis()
				{
					if (this->pEditorCameraAxis == nullptr ||
						!this->cfg_isEditorCameraAxisShow)
					{
						return;
					}

					GLESRenderPass* pRenderPass = this->pEditorCameraAxis->poRenderPassCameraAxis;
					if (pRenderPass == nullptr)
						return;

					beginRenderPass("[RenderPass-EditorCameraAxis]",
									pRenderPass,
									this->pEditorCameraAxis->poOffset,
									this->pEditorCameraAxis->poExtent,
									this->pEditorCameraAxis->poColorBackground,
									1.0f,
									0);
					{
						//1> Viewport
						setViewportScissorRect(this->pEditorCameraAxis->poViewport, this->pEditorCameraAxis->poScissor);

						//2> Render CameraAxis
						this->pEditorCameraAxis->Draw();
					}
					endRenderPass(pRenderPass);
				}
                void OpenGLESWindow::updateRenderPass_CustomBeforeDefault()
                {

                }
                void OpenGLESWindow::updateRenderPass_Default()
                {
                    updateMeshDefault_Before();
                    {
                        beginRenderPass("[RenderPass-Default]",
                                        this->poRenderPass,
                                        this->poOffset,
                                        this->poExtent,
                                        this->cfg_colorBackground,
                                        1.0f,
                                        0);
                        {
                            //1> Viewport
                            setViewportScissorRect(this->poViewport, this->poScissor);

                            //2> Default
                            drawMeshDefault();
                            drawMeshTerrain();
                            drawMeshDefault_Custom();
                            drawMeshDefault_Editor();
                            drawMeshDefault_CustomBeforeImgui();

                            //3> ImGui 
                            drawMeshDefault_Imgui();
                        }
                        endRenderPass(this->poRenderPass);
                    }
                    updateMeshDefault_After();
                }
                    void OpenGLESWindow::updateMeshDefault_Before()
                    {

                    }
                        void OpenGLESWindow::drawMeshDefault()
                        {
                            if (this->poStatePipelineGraphics == nullptr)
                                return;

							//setPolygonMode(GL_FRONT_AND_BACK, this->poTypePolygonMode);

                            //State/Shader/BufferUniform/Texture
							this->poStatePipelineGraphics->poTypePolygonMode = this->poTypePolygonMode;
							this->poStatePipelineGraphics->BindState();
							this->poStatePipelineGraphics->BindShader();
							this->poStatePipelineGraphics->BindBufferUniforms();
							this->poStatePipelineGraphics->BindTextures();

                            //Draw
                            if (this->pBufferVertex != nullptr)
                            {
                                this->pBufferVertex->BindVertexArray();
                                drawInstance(this->poStatePipelineGraphics->poTypePrimitive, 0, this->poVertexCount, 1);
                            }
                            else if (this->pBufferVertexIndex != nullptr)
                            {   
                                this->pBufferVertexIndex->BindVertexArray();
                                drawIndexedInstance(this->poStatePipelineGraphics->poTypePrimitive, this->poIndexCount, GL_UNSIGNED_INT, 0, 1);
                            }
							else
							{	
								F_Assert(false && "OpenGLESWindow::drawMeshDefault")
							}
							this->poStatePipelineGraphics->UnBindState();
                        }
                        void OpenGLESWindow::drawMeshTerrain()
                        {

                        }
                        void OpenGLESWindow::drawMeshDefault_Custom()
                        {

                        }
                        void OpenGLESWindow::drawMeshDefault_Editor()
                        {
							if (this->pEditorGrid != nullptr)
                            {
                                if (this->cfg_isEditorGridShow)
                                {
                                    this->pEditorGrid->Draw();
                                }
                            }
                            if (this->pEditorCameraAxis != nullptr)
                            {
                                if (this->cfg_isEditorCameraAxisShow)
                                {
                                    this->pEditorCameraAxis->DrawQuad();
                                }
                            }
                            // if (this->pEditorCoordinateAxis != nullptr)
                            // {
                            //     if (this->cfg_isEditorCoordinateAxisShow)
                            //     {
                            //         this->pEditorCoordinateAxis->Draw();
                            //     }
                            // }
                            // if (this->pEditorLineFlat2DCollector != nullptr)
                            // {
                            //     this->pEditorLineFlat2DCollector->Draw();
                            // }
                            // if (this->pEditorLineFlat3DCollector != nullptr)
                            // {
                            //     this->pEditorLineFlat3DCollector->Draw();
                            // }
                        }
                        void OpenGLESWindow::drawMeshDefault_CustomBeforeImgui()
                        {

                        }
                        void OpenGLESWindow::drawMeshDefault_Imgui()
                        {
                            if (HasConfig_Imgui())
                            {
                                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                            }
                        }
                    void OpenGLESWindow::updateMeshDefault_After()
                    {

                    }
                void OpenGLESWindow::updateRenderPass_CustomAfterDefault()
                {

                }

				void OpenGLESWindow::updateRenderPass_BlitFromFrame()
				{
					Draw_Graphics_CopyBlitToFrame(this->poRenderPass->pFrameBuffer);
				}

                void OpenGLESWindow::beginRenderPass(const String& nameRenderPass,
                                                     GLESRenderPass* pRenderPass,
                                                     const FSizeI& offset,
                                                     const FSizeI& extent,
                                                     const FVector4& clBg,
                                                     float depth,
                                                     int stencil)
                { 
                    this->poDebug->BeginRegion(nameRenderPass.c_str(), GL_DEBUG_SOURCE_APPLICATION);

                    pRenderPass->pFrameBuffer->BindFrameBuffer();
					if (pRenderPass->pFrameBuffer->HasDepthStencil())
					{
						setClearColorDepthStencil(clBg, depth, stencil);
						clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
					}
					else
					{
						setClearColor(clBg);
						clear(GL_COLOR_BUFFER_BIT);
					}
                }

                    void OpenGLESWindow::setEnable(GLenum cap, bool enable)
                    {
                        if (enable)
                            glEnable(cap);
                        else
                            glDisable(cap);
                    }   
                    void OpenGLESWindow::setEnableDepthTest(bool enable)
                    {
                        if (enable)
                            glEnable(GL_DEPTH_TEST); 
                        else
                            glDisable(GL_DEPTH_TEST);
                    }
                    void OpenGLESWindow::setClearColor(float r, float g, float b, float a)
                    {
                        glClearColor(r, g, b, a);
                    }
                    void OpenGLESWindow::setClearColor(const FVector4& color)
                    {
                        setClearColor(color.x, color.y, color.z, color.w);
                    }
                    void OpenGLESWindow::setClearDepth(float depth)
                    {
                        glClearDepthf(depth);
                    }
                    void OpenGLESWindow::setClearStencil(int stencil)
                    {
                        glClearStencil(stencil);
                    }
                    void OpenGLESWindow::setClearColorDepthStencil(float r, float g, float b, float a, float depth, int stencil)
                    {
                        setClearColor(r, g, b, a);
                        setClearDepth(depth);
                        setClearStencil(stencil);
                    }
                    void OpenGLESWindow::setClearColorDepthStencil(const FVector4& color, float depth, int stencil)
                    {
                        setClearColorDepthStencil(color.x, color.y, color.z, color.w, depth, stencil);
                    }
                    void OpenGLESWindow::clear(GLbitfield mask)
                    {
                        glClear(mask);
                    }
                    void OpenGLESWindow::setFrontFace(GLenum mode)
                    {
                        glFrontFace(mode);
                    }
                    void OpenGLESWindow::setCullFace(GLenum mode)
                    {
                        glCullFace(mode);
                    }
                    void OpenGLESWindow::setPolygonMode(GLenum face, GLenum mode)
                    {
                        // glPolygonMode(face, mode);
                    }
					void OpenGLESWindow::setDepthWrite(GLboolean flag)
					{
						glDepthMask(flag);
					}
					void OpenGLESWindow::setDepthFunc(GLenum func)
					{
						glDepthFunc(func);
					}
					void OpenGLESWindow::setStencilFunc(GLenum func, GLint ref, GLuint mask)
					{
						glStencilFunc(func, ref, mask);
					}
					void OpenGLESWindow::setStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
					{
						glStencilOp(fail, zfail, zpass);
					}
					void OpenGLESWindow::setStencilMask(GLuint mask)
					{
						glStencilMask(mask);
					}
                    void OpenGLESWindow::setBlendFunc(GLenum sfactor, GLenum dfactor)
                    {
                        glBlendFunc(sfactor, dfactor);
                    }
					void OpenGLESWindow::setBlendFunci(GLuint buf, GLenum src, GLenum dst)
					{
						glBlendFunci(buf, src, dst);
					}
					void OpenGLESWindow::setBlendEquation(GLenum mode)
					{
						glBlendEquation(mode);
					}
					void OpenGLESWindow::setColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
					{
						glColorMask(red, green, blue, alpha);
					}

					void OpenGLESWindow::setViewport(GLint x, GLint y, GLsizei width, GLsizei height)
					{
						glViewport(x, y, width, height);
					}
					void OpenGLESWindow::setScissorRect(GLint x, GLint y, GLsizei width, GLsizei height)
					{
						glScissor(x, y, width, height);
					}
					void OpenGLESWindow::setViewportScissorRect(const FRectI& poViewport, const FRectI& poScissor)
					{
						setViewport(poViewport.left, poViewport.top, poViewport.Width(), poViewport.Height());
						setScissorRect(poScissor.left, poScissor.top, poScissor.Width(), poScissor.Height());
					}
                    void OpenGLESWindow::draw(GLenum mode, GLint first, GLsizei count)
                    {
                        glDrawArrays(mode, first, count);
                    }
					void OpenGLESWindow::drawInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
					{
						glDrawArraysInstanced(mode, first, count, instancecount);
					}
                    void OpenGLESWindow::drawIndexed(GLenum mode, GLsizei count, GLenum type, const void* indices)
                    {
                        glDrawElements(mode, count, type, indices);
                    }
					void OpenGLESWindow::drawIndexedInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount)
					{
						glDrawElementsInstanced(mode, count, type, indices, instancecount);
					}
					void OpenGLESWindow::drawIndexedInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance)
					{
						glDrawElementsInstancedBaseInstanceEXT(mode, count, type, indices, instancecount, baseinstance);
					}

                    void OpenGLESWindow::dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
					{
						glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
					}

                void OpenGLESWindow::endRenderPass(GLESRenderPass* pRenderPass)
                {
					bindGLFrameBuffer(0);
					
					setEnableDepthTest(false);
					//setPolygonMode(GL_FRONT_AND_BACK, 0);
					setClearColor(this->cfg_colorBackground);
					clear(GL_COLOR_BUFFER_BIT);

                    this->poDebug->EndRegion(); 
                }


            void OpenGLESWindow::updateRenderCommandBuffers_CustomAfterDefault()
            {

            }


        void OpenGLESWindow::render()
        {

        }
    void OpenGLESWindow::endRender()
    {

    }

    void OpenGLESWindow::cleanup()
    {
        F_LogInfo("---------- OpenGLESWindow::cleanup start ----------");
        {
            //0> cleanupSwapChain
            cleanupSwapChain();

            //1> cleanupCustom/cleanupEditor/cleanupImGUI/cleanupDefault/cleanupInternal
            cleanupCustom();
            cleanupEditor();
            cleanupImGUI();
            cleanupDefault();
            cleanupInternal();

            
			//2> Device
			F_DELETE(this->poDebug)
			F_DELETE(this->poShaderInclude)


        }
        F_LogInfo("---------- OpenGLESWindow::cleanup finish ----------");
    }
        void OpenGLESWindow::cleanupDefault()
        {
			this->poDescriptorSetLayoutName = "";
            F_DELETE(this->pDescriptorSetLayout)
            
            cleanupTexture();
            cleanupVertexIndexBuffer();
        }
            void OpenGLESWindow::cleanupTexture()
            {
                F_DELETE(this->poTexture)
            }
            void OpenGLESWindow::cleanupVertexIndexBuffer()
            {
                //1> VertexBuffer/pBufferVertexIndex
                F_DELETE(this->pBufferVertex)
                F_DELETE(this->pBufferVertexIndex)

                //2> Data
                this->poVertexCount = 0;
                this->poVertexBuffer_Size = 0;
                this->poVertexBuffer_Data = nullptr;
                this->poIndexCount = 0;
                this->poIndexBuffer_Size = 0;
                this->poIndexBuffer_Data = nullptr;
            }
        void OpenGLESWindow::cleanupImGUI()
        {
            if (HasConfig_Imgui())
            {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplAndroid_Shutdown();
                ImGui::DestroyContext();
            }
        }
        void OpenGLESWindow::cleanupEditor()
        {
            destroyEditor();
        }
        void OpenGLESWindow::cleanupCustom()
        {

        }

        void OpenGLESWindow::cleanupSwapChain()
        {
            F_LogInfo("----- OpenGLESWindow::cleanupSwapChain start -----");
            {
                size_t count = 0;

                //0> Custom/Editor/Terrain/Default
                cleanupSwapChain_Custom();
                cleanupSwapChain_Editor();
                cleanupSwapChain_Default();
                destroyResourceInternal();

                //1> DepthImage/ColorImage    
                F_DELETE(this->poTextureColor)
				F_DELETE(this->poRenderBufferDepthStencil)
                count = this->poTextureColorLists.size();
                for (size_t i = 0; i < count; i++)
                {
                    F_DELETE(this->poTextureColorLists[i])
                }
                this->poTextureColorLists.clear();
                
				//2> SwapChainFrameBuffers
				count = this->poFrameBuffers.size();
				for (size_t i = 0; i < count; i++)
				{
					F_DELETE(this->poFrameBuffers[i])
				}
				this->poFrameBuffers.clear();

				//3> CommandBuffers


				//4> RenderPass
				F_DELETE(this->poRenderPass)
				
				//5> SwapChainImageViews
				count = this->poSwapChains.size();
				for (size_t i = 0; i < count; i++)
				{
					F_DELETE(this->poSwapChains[i])
				}
				this->poSwapChains.clear();
				 
            }
            F_LogInfo("----- OpenGLESWindow::cleanupSwapChain finish -----");
        }
            void OpenGLESWindow::cleanupSwapChain_Default()
            {
                size_t count = 0;

                //1> ConstBuffers
                count = this->poBuffers_ObjectCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    F_DELETE(this->poBuffers_ObjectCB[i])
                }
                this->poBuffers_ObjectCB.clear();


                //2> Pipelines
                F_DELETE(this->poShaderVertex)
                F_DELETE(this->poShaderFragment)
				F_DELETE(this->poStatePipelineGraphics)

            }
            void OpenGLESWindow::cleanupSwapChain_Editor()
            {
				if (this->pEditorGrid != nullptr)
                {
                    this->pEditorGrid->CleanupSwapChain();
                }
                if (this->pEditorCameraAxis != nullptr)
                {
                    this->pEditorCameraAxis->CleanupSwapChain();
                }
                // if (this->pEditorCoordinateAxis != nullptr)
                // {
                //     this->pEditorCoordinateAxis->CleanupSwapChain();
                // }
                // if (this->pEditorLineFlat2DCollector != nullptr)
                // {
                //     this->pEditorLineFlat2DCollector->CleanupSwapChain();
                // }
                // if (this->pEditorLineFlat3DCollector != nullptr)
                // {
                //     this->pEditorLineFlat3DCollector->CleanupSwapChain();
                // }
            }
            void OpenGLESWindow::cleanupSwapChain_Custom()
            {

            }
        void OpenGLESWindow::recreateSwapChain()
        {
            F_LogInfo("++++++++++ OpenGLESWindow::recreateSwapChain start ++++++++++");
            {
                // int width = 0;
                // int height = 0;
                // glfwGetFramebufferSize(this->pWindow, &width, &height);
                // while (width == 0 || height == 0)
                // {
                //     glfwGetFramebufferSize(this->pWindow, &width, &height);
                //     glfwWaitEvents();
                //     if (glfwWindowShouldClose(this->pWindow)) 
                //     {
                //         //Closing a minimized window
                //         cleanup();
                //         this->isMinimizedWindowNeedClose = true;
                //         return;
                //     }
                // }

				cleanupSwapChain();
				
				createSwapChain();
				createSwapChainImageViews();
				if (HasConfig_MASS())
				{
					createColorResources();
				}  
				if (HasConfig_DepthStencil())
				{
					createDepthStencilResources();
				}
				createRenderPasses();
				createFramebuffers();

				createResourceInternal();
            
            	createConstBuffers();

				createCustomBeforePipeline();
				createGraphicsPipeline();
				createComputePipeline();
				createDescriptorSets();

				recreateSwapChain_Editor();
				recreateSwapChain_Custom();


                cameraReset();
            }
            F_LogInfo("++++++++++ OpenGLESWindow::recreateSwapChain finish ++++++++++");
        }
            void OpenGLESWindow::recreateSwapChain_Editor()
            {
				if (this->pEditorGrid != nullptr)
				{
					this->pEditorGrid->RecreateSwapChain();
				}
				if (this->pEditorCameraAxis != nullptr)
				{
					this->pEditorCameraAxis->RecreateSwapChain();
				}
				// if (this->pEditorCoordinateAxis != nullptr)
				// {
				// 	this->pEditorCoordinateAxis->RecreateSwapChain();
				// }
				// if (this->pEditorLineFlat2DCollector != nullptr)
				// {
				// 	this->pEditorLineFlat2DCollector->RecreateSwapChain();
				// }
				// if (this->pEditorLineFlat3DCollector != nullptr)
				// {
				// 	this->pEditorLineFlat3DCollector->RecreateSwapChain();
				// }
            }
            void OpenGLESWindow::recreateSwapChain_Custom()
            {

            }

}; //LostPeterOpenGLES