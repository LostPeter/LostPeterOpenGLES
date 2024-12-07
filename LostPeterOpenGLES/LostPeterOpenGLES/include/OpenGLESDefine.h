/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-05
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OPENGLES_DEFINE_H_
#define _OPENGLES_DEFINE_H_

#include "PreDefine.h"
#include "FMath.h"
#include "FMeshVertex.h"

namespace LostPeterOpenGLES
{
/////Struct
    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: F_MeshVertex_Pos2Color4
    struct openglesExport Vertex_Pos2Color4 : FVertex_Pos2Color4
    {

        static void createAttributeDescriptions()
        {
            //Pos2
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FVertex_Pos2Color4), (void*)offsetof(FVertex_Pos2Color4, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FVertex_Pos2Color4), (void*)offsetof(FVertex_Pos2Color4, color));
            glEnableVertexAttribArray(1);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4 //////////////////////////////
    //1: F_MeshVertex_Pos3Color4
    struct openglesExport Vertex_Pos3Color4 : FVertex_Pos3Color4
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4), (void*)offsetof(Vertex_Pos3Color4, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4), (void*)offsetof(Vertex_Pos3Color4, color));
            glEnableVertexAttribArray(1);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3 /////////////////////////////
    //2: F_MeshVertex_Pos3Normal3
    struct openglesExport Vertex_Pos3Normal3 : FVertex_Pos3Normal3
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3), (void*)offsetof(Vertex_Pos3Normal3, pos));
            glEnableVertexAttribArray(0);
            //Normal3
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3), (void*)offsetof(Vertex_Pos3Normal3, normal));
            glEnableVertexAttribArray(1);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3Tex2 /////////////////////////
    //3: F_MeshVertex_Pos3Normal3Tex2
    struct openglesExport Vertex_Pos3Normal3Tex2 : FVertex_Pos3Normal3Tex2
    {
       
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tex2), (void*)offsetof(Vertex_Pos3Normal3Tex2, pos));
            glEnableVertexAttribArray(0);
            //Normal3
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tex2), (void*)offsetof(Vertex_Pos3Normal3Tex2, normal));
            glEnableVertexAttribArray(1);
            //Tex2
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tex2), (void*)offsetof(Vertex_Pos3Normal3Tex2, texCoord));
            glEnableVertexAttribArray(2);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
            }
        }
    };


    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //4: F_MeshVertex_Pos2Color4Tex2
    struct openglesExport Vertex_Pos2Color4Tex2 : FVertex_Pos2Color4Tex2
    {
        
        static void createAttributeDescriptions()
        {
            //Pos2
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos2Color4Tex2), (void*)offsetof(Vertex_Pos2Color4Tex2, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos2Color4Tex2), (void*)offsetof(Vertex_Pos2Color4Tex2, color));
            glEnableVertexAttribArray(1);
            //Tex2
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos2Color4Tex2), (void*)offsetof(Vertex_Pos2Color4Tex2, texCoord));
            glEnableVertexAttribArray(2);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Tex2 //////////////////////////
    //5: F_MeshVertex_Pos3Color4Tex2
    struct openglesExport Vertex_Pos3Color4Tex2 : FVertex_Pos3Color4Tex2
    {
       
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Tex2), (void*)offsetof(Vertex_Pos3Color4Tex2, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Tex2), (void*)offsetof(Vertex_Pos3Color4Tex2, color));
            glEnableVertexAttribArray(1);
            //Tex2
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Tex2), (void*)offsetof(Vertex_Pos3Color4Tex2, texCoord));
            glEnableVertexAttribArray(2);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //6: F_MeshVertex_Pos3Color4Normal3Tex2 
    struct openglesExport Vertex_Pos3Color4Normal3Tex2 : FVertex_Pos3Color4Normal3Tex2
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tex2, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tex2, color));
            glEnableVertexAttribArray(1);
            //Normal3
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tex2, normal));
            glEnableVertexAttribArray(2);
            //Tex2
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tex2, texCoord));
            glEnableVertexAttribArray(3);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tex4 ///////////////////
    //7: F_MeshVertex_Pos3Color4Normal3Tex4
    struct openglesExport Vertex_Pos3Color4Normal3Tex4 : FVertex_Pos3Color4Normal3Tex4
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tex4, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tex4, color));
            glEnableVertexAttribArray(1);
            //Normal3
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tex4, normal));
            glEnableVertexAttribArray(2);
            //Tex4
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tex4, texCoord));
            glEnableVertexAttribArray(3);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex2 ///////////////
    //8: F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
    struct openglesExport Vertex_Pos3Color4Normal3Tangent3Tex2 : FVertex_Pos3Color4Normal3Tangent3Tex2
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, color));
            glEnableVertexAttribArray(1);
            //Normal3
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, normal));
            glEnableVertexAttribArray(2);
            //Tangent3
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, tangent));
            glEnableVertexAttribArray(3);
            //Tex2
            glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, texCoord));
            glEnableVertexAttribArray(4);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
                glDisableVertexAttribArray(4);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex4 ///////////////
    //9: F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
    struct openglesExport Vertex_Pos3Color4Normal3Tangent3Tex4 : FVertex_Pos3Color4Normal3Tangent3Tex4
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, color));
            glEnableVertexAttribArray(1);
            //Normal3
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, normal));
            glEnableVertexAttribArray(2);
            //Tangent3
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, tangent));
            glEnableVertexAttribArray(3);
            //Tex4
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, texCoord));
            glEnableVertexAttribArray(4);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
                glDisableVertexAttribArray(4);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3Tangent3BlendWI8Tex2 //////////
    //10: F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
    struct Vertex_Pos3Normal3Tangent3BlendWI8Tex2 : FVertex_Pos3Normal3Tangent3BlendWI8Tex2
    {
        
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, pos));
            glEnableVertexAttribArray(0);
            //Normal3
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, normal));
            glEnableVertexAttribArray(1);
            //Tangent3
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, tangent));
            glEnableVertexAttribArray(2);
            //BlendW
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, blendweight));
            glEnableVertexAttribArray(3);
            //BlendI
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, blendindex));
            glEnableVertexAttribArray(4);
            //Tex2
            glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, texCoord));
            glEnableVertexAttribArray(5);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
                glEnableVertexAttribArray(5);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
                glDisableVertexAttribArray(4);
                glDisableVertexAttribArray(5);
            }
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 ////
    //11: F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    struct Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 : FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    {
       
        static void createAttributeDescriptions()
        {
            //Pos3
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, pos));
            glEnableVertexAttribArray(0);
            //Color4
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, color));
            glEnableVertexAttribArray(1);
            //Normal3
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, normal));
            glEnableVertexAttribArray(2);
            //Tangent3
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, tangent));
            glEnableVertexAttribArray(3);
            //BlendW
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, blendweight));
            glEnableVertexAttribArray(4);
            //BlendI
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, blendindex));
            glEnableVertexAttribArray(5);
            //Tex2
            glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2), (void*)offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, texCoord));
            glEnableVertexAttribArray(6);
        }

        static void enableAttributeDescriptions(bool enable)
        {
            if (enable)
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
                glEnableVertexAttribArray(5);
                glEnableVertexAttribArray(6);
            }
            else
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(3);
                glDisableVertexAttribArray(4);
                glDisableVertexAttribArray(5);
                glDisableVertexAttribArray(6);
            }
        }
    };


    //////////////////////////////// TransformConstants /////////////////////////////
    struct openglesExport TransformConstants
    {
        FMatrix4 mat4View;
        FMatrix4 mat4View_Inv;
        FMatrix4 mat4Proj;
        FMatrix4 mat4Proj_Inv;
        FMatrix4 mat4ViewProj;
        FMatrix4 mat4ViewProj_Inv;

        TransformConstants()
            : mat4View(FMath::Identity4x4())
            , mat4View_Inv(FMath::Identity4x4())
            , mat4Proj(FMath::Identity4x4())
            , mat4Proj_Inv(FMath::Identity4x4())
            , mat4ViewProj(FMath::Identity4x4())
            , mat4ViewProj_Inv(FMath::Identity4x4())
        {

        }
    };

    //////////////////////////////// CameraConstants ////////////////////////////////
    struct openglesExport CameraConstants
    {
        FVector3 posEyeWorld;    
        float fNearZ;
        float fFarZ;
        float fReserved1;
        float fReserved2;
        float fReserved3;

        CameraConstants()
            : posEyeWorld(0.0f, 0.0f, 0.0f)
            , fNearZ(0.0f)
            , fFarZ(0.0f)
            , fReserved1(0.0f)
            , fReserved2(0.0f)
            , fReserved3(0.0f)
        {

        }
    }; 

    //////////////////////////////// LightConstants /////////////////////////////////
    struct openglesExport LightConstants
    {
        FVector4 common;    // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
        FVector3 position;  // directional/point/spot
        float falloffStart;  // point/spot light only
        FVector3 direction; // directional/spot light only
        float falloffEnd;    // point/spot light only
        FVector4 ambient;   // ambient
        FVector4 diffuse;   // diffuse
        FVector4 specular;  // specular

        FMatrix4 depthMVP;  // depthMVP

        LightConstants()
            : common(0, 0, 0, 64)
            , position(0.0f, 100.0f, 0.0f)
            , falloffStart(1.0f)
            , direction(0.0f, -1.0f, 0.0f)
            , falloffEnd(10.0f)
            , ambient(1.0f, 1.0f, 1.0f, 1.0f)
            , diffuse(1.0f, 1.0f, 1.0f, 1.0f)
            , specular(1.0f, 1.0f, 1.0f, 1.0f)
            
            , depthMVP(FMath::Identity4x4())
        {

        }
    };

    //////////////////////////////// ShadowConstants ////////////////////////////////
    struct openglesExport ShadowConstants
    {
        float fov; //For Light ShadowMap's depthMVP
        float zNear; //For Light ShadowMap's depthMVP
	    float zFar; //For Light ShadowMap's depthMVP
        uint32_t depthSize; //For Light ShadowMap's depth size
	    float depthBiasConstant; //For Light ShadowMap's constant depth bias factor
	    float depthBiasSlope; //For Light ShadowMap's slope depth bias factor

        ShadowConstants()
            : fov(45.0f)
            , zNear(1.0f)
            , zFar(200.0f)
#if F_PLATFORM == F_PLATFORM_WINDOW || F_PLATFORM == F_PLATFORM_MAC 
            , depthSize(2048)
#else
            , depthSize(1024)
#endif
            , depthBiasConstant(1.25f)
            , depthBiasSlope(1.75f)
        {

        }
    };

    //////////////////////////////// PassConstants //////////////////////////////////
    struct openglesExport PassConstants
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
        FVector2 g_RenderTargetSize;   
        FVector2 g_RenderTargetSize_Inv;

        //Material
        FVector4 g_AmbientLight;

        //Light
        LightConstants g_MainLight;
        LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];

        PassConstants()
            : g_TotalTime(0.0f)
            , g_DeltaTime(0.0f)
            , g_Pad1(1.0f)
            , g_Pad2(1.0f)
            , g_RenderTargetSize(0.0f, 0.0f)
            , g_RenderTargetSize_Inv(0.0f, 0.0f)
            , g_AmbientLight(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };

    //////////////////////////////// ObjectConstants ////////////////////////////////
    struct openglesExport ObjectConstants
    {
        FMatrix4 g_MatWorld;
        
        ObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
        {

        }
    };
    struct openglesExport GridObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        GridObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct openglesExport CameraAxisObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        CameraAxisObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct openglesExport CoordinateAxisObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        CoordinateAxisObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct openglesExport LineFlat2DObjectConstants
    {
        FColor color;
        
        LineFlat2DObjectConstants()
            : color(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };
    struct openglesExport LineFlat3DObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;
        
        LineFlat3DObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };
    struct openglesExport CopyBlitObjectConstants
    {
        float offsetX;
        float offsetY;
        float scaleX;
        float scaleY;

        CopyBlitObjectConstants()
            : offsetX(0.0f)
            , offsetY(0.0f)
            , scaleX(1.0f)
            , scaleY(1.0f)
        {
            
        }
    };
    
    //////////////////////////////// TextureConstants ///////////////////////////////
    struct openglesExport TextureConstants
    {
        float texWidth;
        float texHeight;
        float texDepth;
        float indexTextureArray;

        float texSpeedU;
        float texSpeedV;
        float texSpeedW;
        float reserve0;

        float texChunkMaxX;
        float texChunkMaxY;
        float texChunkIndexX;
        float texChunkIndexY;

        TextureConstants()
            : texWidth(512.0f)
            , texHeight(512.0f)
            , texDepth(512.0f)
            , indexTextureArray(0.0f)

            , texSpeedU(0.0f)
            , texSpeedV(0.0f)
            , texSpeedW(0.0f)
            , reserve0(0.0f)
            
            , texChunkMaxX(0.0f)
            , texChunkMaxY(0.0f)
            , texChunkIndexX(0.0f)
            , texChunkIndexY(0.0f)
        {

        }
    };
    
    //////////////////////////////// MaterialConstants //////////////////////////////
    struct openglesExport MaterialConstants
    {
        FVector4 factorAmbient;
        FVector4 factorDiffuse;
        FVector4 factorSpecular;

        float shininess;
        float alpha;
        float lighting;
        float castshadow;
        float receiveshadow;
        float reserve0;
        float reserve1;
        float reserve2;

        TextureConstants aTexLayers[MAX_TEXTURE_COUNT];

        MaterialConstants()
            : factorAmbient(1.0f, 1.0f, 1.0f, 1.0f)
            , factorDiffuse(1.0f, 1.0f, 1.0f, 1.0f)
            , factorSpecular(1.0f, 1.0f, 1.0f, 1.0f)
            , shininess(20.0f)
            , alpha(1.0f)
            , lighting(1.0f)
            , castshadow(0.0f)
            , receiveshadow(0.0f)
            , reserve0(0.0f)
            , reserve1(0.0f)
            , reserve2(0.0f)
        {
            
        }

        static MaterialConstants RandomMaterialConstants()
        {
            MaterialConstants mc;
            RandomMaterialConstants(mc);
            return mc;
        }
        static void RandomMaterialConstants(MaterialConstants& mc)
        {
            mc.factorAmbient = FMath::RandomColor(false);
            mc.factorDiffuse = FMath::RandomColor(false);
            mc.factorSpecular = FMath::RandomColor(false);
            mc.shininess = FMath::RandF(10.0f, 100.0f);
            mc.alpha = FMath::RandF(0.2f, 0.9f);
        }
    };
    typedef std::vector<MaterialConstants> MaterialConstantsVector;
    typedef std::vector<MaterialConstants*> MaterialConstantsPtrVector;

    //////////////////////////////// InstanceConstants //////////////////////////////
    struct openglesExport InstanceConstants
    {
        int offsetObject;
        int indexObject;
        int offsetMaterial;
        int indexMaterial;

        InstanceConstants()
            : offsetObject(0)
            , indexObject(0)
            , offsetMaterial(0)
            , indexMaterial(0)
        {

        }  
    };

    //////////////////////////////// TextureCopyConstants ///////////////////////////
    struct openglesExport TextureCopyConstants
    {
        FVector4 texInfo;
        FVector4 texOffset;
        FVector4 texIndexArray;
        FVector4 texClearColor;

        TextureCopyConstants()
            : texInfo(0, 0, 0, 0)
            , texOffset(0, 0, 0, 0)
            , texIndexArray(0, 0, 0, 0)
            , texClearColor(0, 0, 0, 1)
        {

        }
    };

    //////////////////////////////// TessellationConstants //////////////////////////
    struct openglesExport TessellationConstants
    {
    ////tesc
        float tessLevelOuter;
        float tessLevelInner;

    ////tese
        float tessAlpha;
        float tessStrength;

        TessellationConstants()
            : tessLevelOuter(3.0f)
            , tessLevelInner(3.0f)
            , tessAlpha(1.0f)
            , tessStrength(0.1f)
        {

        }
    };

    //////////////////////////////// GeometryConstants //////////////////////////////
    struct openglesExport GeometryConstants
    {
        float width;
        float height;
        float length;
        float reserve;

        GeometryConstants()
            : width(0.05f)
            , height(3.0f)
            , length(0.02f)
            , reserve(0.0f)
        {

        }
    };

    //////////////////////////////// TerrainConstants ///////////////////////////////
    struct openglesExport TerrainSplatConstants
    {
        float splatSizeX; //size x
        float splatSizeY; //size y
        float splatOffsetX; //offset x
        float splatOffsetY; //offset y

        FVector4 diffuseRemapScale; //diffuse remap scale
        float normalRemapScale; //normal remap scale
        float reserve0;
        float reserve1;
        float reserve2;

        TerrainSplatConstants()
            : splatSizeX(100)
            , splatSizeY(100)
            , splatOffsetX(0)
            , splatOffsetY(0)
            , diffuseRemapScale(1.0f, 1.0f, 1.0f, 1.0f)
            , normalRemapScale(1.0f)
        {

        }
    };
    struct openglesExport TerrainConstants
    {
        FMatrix4 matWorld; //Matrix world
        float textureX; //HeightMap/NarmalMap Texture Size X
        float textureZ; //HeightMap/NarmalMap Texture Size Z
        float textureX_Inverse; //1/textureX
        float textureZ_Inverse; //1/textureZ
        float heightStart; //Height Low Start 
        float heightMax; //Height Max (from heightStart, heightEnd = heightStart + heightMax)
        float terrainSizeX; //Terrain Size X
        float terrainSizeZ; //Terrain Size Z
        
        TerrainSplatConstants aSplats[MAX_TERRAIN_SPLAT_COUNT];

        TerrainConstants()
            : matWorld(FMath::Identity4x4())
            , textureX(1025)
            , textureZ(1025)
            , textureX_Inverse(1.0f / 1024)
            , textureZ_Inverse(1.0f / 1024)
            , heightStart(0)
            , heightMax(200)
            , terrainSizeX(1024)
            , terrainSizeZ(1024)
        {

        }
    };
    struct openglesExport TerrainObjectConstants
    {
        float offsetX; //Instance offset X
        float offsetZ; //Instance offset Z

        TerrainObjectConstants()
            : offsetX(0)
            , offsetZ(0)
        {

        }
    };
    
    //////////////////////////////// CullConstants //////////////////////////////////
    struct openglesExport CullConstants
    {
        uint32 nMaxLodCount; //Max Lod Count
        uint32 nMaxRenderCount; //Max Render Count
        uint32 nMaxObjectCount; //Max Object Count
        uint32 nIsNoCulling; //Is No Culling?

        FMatrix4 mat4VPLast;  //VP Last
        FVector4 v4FrustumPlanes[MAX_FRUSTUM_PLANE_COUNT]; //6 Frustum Planes
        FVector4 v4ParamComon[4]; ///Param common (0: CameraPos; 1: Min FrustumPlanes; 2: Max FrustumPlanes; 3: Hiz ScreenRes)
        FVector4 v4ParamRender; //Param Render
        FVector4 v4PosPlayer; //Pos Player

        CullConstants()
            : nMaxLodCount((int)0)
            , nMaxRenderCount(200)
            , nMaxObjectCount(10000)
            , nIsNoCulling(0)

            , mat4VPLast(FMath::Identity4x4())
        {

        }
    };
    struct openglesExport CullObjectConstants
    {
        FVector4 vPos; //xyz = pos, w = object offset 
        FVector4 vExt; //xyz = ext, w = isNoHizTest
        uint32 nRenderIndex; //Render Index
        uint32 nRenderCount; //Render Count
        uint32 nObjectOffset; //Object Offset
        uint32 nIsNoHizTest; //IsNoHizTest

        CullObjectConstants()
            : vPos(FMath::ms_v4Zero)
            , vExt(FMath::ms_v4Zero)
            , nRenderIndex(0)
            , nRenderCount(0)
            , nObjectOffset(0)
            , nIsNoHizTest(0)
        {

        }
    };
    typedef std::vector<CullObjectConstants> CullObjectConstantsVector;
    typedef std::vector<CullObjectConstants*> CullObjectConstantsPtrVector;
    struct openglesExport CullObjectInstanceConstants
    {
        FMatrix4 mat4Object2World;

        CullObjectInstanceConstants()
            : mat4Object2World(FMath::Identity4x4())
        {

        }
    };
    typedef std::vector<CullObjectInstanceConstants> CullObjectInstanceConstantsVector;
    typedef std::vector<CullObjectInstanceConstants*> CullObjectInstanceConstantsPtrVector;
    struct openglesExport CullInstanceConstants
    {
        uint32 nObjectOffset;
        float reserve0;
        float reserve1;
        float reserve2;

        CullInstanceConstants()
            : nObjectOffset(0)
            , reserve0(0.0f)
            , reserve1(0.0f)
            , reserve2(0.0f)
        {

        }  
    };
    typedef std::vector<CullInstanceConstants> CullInstanceConstantsVector;
    typedef std::vector<CullInstanceConstants*> CullInstanceConstantsPtrVector;
    
    //////////////////////////////// HizDepthConstants //////////////////////////////
    struct openglesExport HizDepthConstants
    {
        FVector4 vRtDepthSize;

        HizDepthConstants()
            : vRtDepthSize(0,0,0,0)
        {

        }
    };


}; //LostPeterOpenGLES

#endif