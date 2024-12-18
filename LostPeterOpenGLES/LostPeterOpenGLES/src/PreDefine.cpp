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

#include "../include/PreDefine.h"
#include "../include/OpenGLESDefine.h"

namespace LostPeterOpenGLES
{   
////////////////////////////// Typedef /////////////////////////////
    


////////////////////////////// Enum ////////////////////////////////
    //WindowType
    static String s_nameWindows[] = 
    {
        "Main",             //0:    Main
        "Game",             //1:    Game
        "Scene",            //2:    Scene
    };
    const String& Util_GetWindowTypeName(WindowType type)
    {
        return s_nameWindows[(int)type];
    }
    const String& Util_GetWindowTypeName(int type)
    {
        return s_nameWindows[(int)type];
    }
    WindowType Util_ParseWindowType(const String& strName)
    {
        for (size_t i = 0; i < (int)Window_Count; i++)
        {
            if (s_nameWindows[i] == strName)
                return (WindowType)(i);
        }
        F_Assert(false && "Util_ParseWindowType: Wrong type name !")
        return Window_Main;
    }


    //PixelFormatComponentType
    static String s_namePixelFormatComponents[] = 
    {
        "ByteU",            //0: Byte unsigned
        "ByteS",            //1: Byte signed
        "ShortU",           //2: Short unsigned
        "ShortS",           //3: Short signed
        "IntU",             //4: Int unsigned
        "IntS",             //5: Int signed
        "LongU",            //6: Long unsigned
        "LongS",            //7: Long signed
        "Float16",          //8: Float 16
        "Float32",          //9: Float 32
        "Double"            //10: Double
    };
    const String& Util_GetPixelFormatComponentTypeName(PixelFormatComponentType type)
    {   
        return s_namePixelFormatComponents[(int)type];
    }
    const String& Util_GetPixelFormatComponentTypeName(int type)
    {
        return s_namePixelFormatComponents[type];
    }


    //DescriptorSetType
    static String s_nameDescriptorSetTypes[] = 
    {
        "PassConstants",                    //0:   PassConstants
        "ObjectConstants",                  //1:   ObjectConstants
        "ObjectTerrain",                    //2:   ObjectTerrain
        "ObjectGrid",                       //3:   ObjectGrid
        "ObjectCameraAxis",                 //4:   ObjectCameraAxis
        "ObjectCoordinateAxis",             //5:   ObjectCoordinateAxis
        "ObjectLineFlat2D",                 //6:   ObjectLineFlat2D
        "ObjectLineFlat3D",                 //7:   ObjectLineFlat3D
        "ObjectCopyBlit",                   //8:   ObjectCopyBlit
        "ObjectCull",                       //9:   ObjectCull
        "Material",                         //10:  Material
        "Instance",                         //11:  Instance
        "TextureCopy",                      //12:  TextureCopy
        "Tessellation",                     //13:  Tessellation
        "Geometry",                         //14:  Geometry
        "TextureVS",                        //15:  TextureVS
        "TextureTESC",                      //16:  TextureTESC
        "TextureTESE",                      //17:  TextureTESE
        "TextureFS",                        //18:  TextureFS
        "TextureFrameColor",                //19:  TextureFrameColor
        "TextureFrameDepth",                //20:  TextureFrameDepth
        "TextureCSR",                       //21:  TextureCSR
        "TextureCSRW",                      //22:  TextureCSRW
        "TextureCSRWSrc",                   //23:  TextureCSRWSrc
        "TextureCSRWDst",                   //24:  TextureCSRWDst
        "TextureDepthShadow",               //25:  TextureDepthShadow
        "InputAttachRed",                   //26:  InputAttachRed
        "InputAttachGreen",                 //27:  InputAttachGreen
        "InputAttachBlue",                  //28:  InputAttachBlue
        "Terrain",                          //29:  Terrain
        "Cull",                             //30:  Cull
        "HizDepth",                         //31:  HizDepth
        "BufferRWArgsCB",                   //32:  BufferRWArgsCB
        "BufferRWLodCB",                    //33:  BufferRWLodCB
        "BufferRWResultCB",                 //34:  BufferRWResultCB
        "BufferRWClipCB",                   //35:  BufferRWClipCB
        "BufferRWObjectCullInstance",       //36:  BufferRWObjectCullInstance
        "CullInstance",                     //37:  CullInstance
        "BufferObjectLineFlat2D",           //38:  BufferObjectLineFlat2D
        "BufferObjectLineFlat3D",           //39:  BufferObjectLineFlat3D
    };
    const String& Util_GetDescriptorSetTypeName(DescriptorSetType type)
    {
        return s_nameDescriptorSetTypes[(int)type];
    }
    const String& Util_GetDescriptorSetTypeName(int type)
    {
        return s_nameDescriptorSetTypes[type];
    }
    DescriptorSetType Util_ParseDescriptorSetType(const String& strName)
    {
        for (size_t i = 0; i < (int)DescriptorSet_Count; i++)
        {
            if (s_nameDescriptorSetTypes[i] == strName)
                return (DescriptorSetType)(i);
        }
        F_Assert(false && "Util_ParseDescriptorSetType: Wrong type name !")
        return DescriptorSet_PassConstants;
    }


////////////////////////////// OpenGL //////////////////////////////   
    void Util_CreateAttributeDescriptions(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4: 
                {
                    Vertex_Pos2Color4::createAttributeDescriptions(); 
                    return;
                }
            case F_MeshVertex_Pos3Color4:
                {
                    Vertex_Pos3Color4::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Normal3:
                {
                    Vertex_Pos3Normal3::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Normal3Tex2:
                {
                    Vertex_Pos3Normal3Tex2::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos2Color4Tex2:
                {
                    Vertex_Pos2Color4Tex2::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Color4Tex2:
                {
                    Vertex_Pos3Color4Tex2::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                {
                    Vertex_Pos3Color4Normal3Tex2::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                {
                    Vertex_Pos3Color4Normal3Tex4::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                {
                    Vertex_Pos3Color4Normal3Tangent3Tex2::createAttributeDescriptions();
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                {
                    Vertex_Pos3Color4Normal3Tangent3Tex4::createAttributeDescriptions();
                    return;
                }
        }
        F_Assert(false && "Util_CreateAttributeDescriptions: Wrong type !")
    }
    void Util_EnableAttributeDescriptions(FMeshVertexType type, bool enable)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4: 
                {
                    Vertex_Pos2Color4::enableAttributeDescriptions(enable); 
                    return;
                }
            case F_MeshVertex_Pos3Color4:
                {
                    Vertex_Pos3Color4::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Normal3:
                {
                    Vertex_Pos3Normal3::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Normal3Tex2:
                {
                    Vertex_Pos3Normal3Tex2::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos2Color4Tex2:
                {
                    Vertex_Pos2Color4Tex2::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Color4Tex2:
                {
                    Vertex_Pos3Color4Tex2::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                {
                    Vertex_Pos3Color4Normal3Tex2::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                {
                    Vertex_Pos3Color4Normal3Tex4::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                {
                    Vertex_Pos3Color4Normal3Tangent3Tex2::enableAttributeDescriptions(enable);
                    return;
                }
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                {
                    Vertex_Pos3Color4Normal3Tangent3Tex4::enableAttributeDescriptions(enable);
                    return;
                }
        }
        F_Assert(false && "Util_EnableAttributeDescriptions: Wrong type !")
    }

    GLenum Util_Transform2GLShaderType(FShaderType type)
    {
        switch ((int)type)
        {
            case F_Shader_Vertex:
                return GL_VERTEX_SHADER;
            case F_Shader_TessellationControl:
                return GL_TESS_CONTROL_SHADER;
            case F_Shader_TessellationEvaluation:
                return GL_TESS_EVALUATION_SHADER;
            case F_Shader_Geometry:
                return GL_GEOMETRY_SHADER;
            case F_Shader_Fragment:
                return GL_FRAGMENT_SHADER;
            case F_Shader_Compute:
        #if F_PLATFORM == F_PLATFORM_MAC
                F_Assert(false && "Util_Transform2GLShaderType: MacOS only support 4.1, can not support compute shader !")
                return GL_VERTEX_SHADER;
        #else
                return GL_COMPUTE_SHADER;
        #endif
        }
        F_Assert(false && "Util_Transform2GLShaderType: Wrong type !")
        return GL_VERTEX_SHADER;
    }
    
    bool Util_IsSupportExtensionsGL(int type)
    {
        return type > 0;
    }

    GLenum Util_Transform2GLTextureType(FTextureType type)
    {
        switch ((int)type)
        {
        case F_Texture_1D:          return GL_TEXTURE_2D; //GL_TEXTURE_1D;
        case F_Texture_2D:          return GL_TEXTURE_2D;
        case F_Texture_2DArray:     return GL_TEXTURE_2D_ARRAY;
        case F_Texture_3D:          return GL_TEXTURE_3D;
        case F_Texture_CubeMap:     return GL_TEXTURE_CUBE_MAP;
        }
        F_Assert(false && "Util_Transform2GLTextureType: Wrong type !")
        return GL_TEXTURE_2D;
    }
    GLenum Util_Transform2GLFormat(FTexturePixelFormatType type)
    {
        switch ((int32)type)
        {
            case F_TexturePixelFormat_R8_UNORM:          return GL_R8;
            case F_TexturePixelFormat_R16_UNORM:         return GL_R8; //GL_R16;
            case F_TexturePixelFormat_R8G8B8A8_SRGB:     return GL_RGBA;
            case F_TexturePixelFormat_R8G8B8A8_UNORM:    return GL_RGBA;
        }
        F_Assert(false && "Util_Transform2GLFormat: Wrong type !")
        return GL_RGBA;
    }
    GLenum Util_Transform2GLFilterSize(FTextureFilterSizeType type)
    {
        switch ((int)type)
        {
        case F_TextureFilterSize_Min:              return GL_TEXTURE_MIN_FILTER;
        case F_TextureFilterSize_Mag:              return GL_TEXTURE_MAG_FILTER;
        }
        F_Assert(false && "Util_Transform2GLFilterSize: Wrong type !")
        return GL_TEXTURE_MIN_FILTER;
    }
    GLenum Util_Transform2GLFilter(FTextureFilterPixelType type)
    {
        switch ((int)type)
        {
        case F_TextureFilterPixel_None:            return GL_NEAREST;
        case F_TextureFilterPixel_Point:           return GL_NEAREST;
        case F_TextureFilterPixel_Linear:          return GL_LINEAR;
        case F_TextureFilterPixel_Anisotropic:     return GL_LINEAR;
        }
        F_Assert(false && "Util_Transform2GLFilter: Wrong type !")
        return GL_NEAREST;
    }
    GLenum Util_Transform2GLSamplerMipmapMode(FTextureFilterPixelType type)
    {
        switch ((int)type)
        {
        case F_TextureFilterPixel_None:            return GL_NEAREST_MIPMAP_NEAREST;
        case F_TextureFilterPixel_Point:           return GL_NEAREST_MIPMAP_LINEAR;
        case F_TextureFilterPixel_Linear:          return GL_LINEAR_MIPMAP_NEAREST;
        case F_TextureFilterPixel_Anisotropic:     return GL_LINEAR_MIPMAP_LINEAR;
        }
        F_Assert(false && "Util_Transform2GLSamplerMipmapMode: Wrong type !")
        return GL_NEAREST_MIPMAP_NEAREST;
    }
    GLenum Util_Transform2GLFilter(FTextureFilterType typeFilter, FTextureFilterSizeType typeFilterSize)
    {
        switch ((int)typeFilter)
        {
        case F_TextureFilter_None:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return GL_NEAREST;
                    case F_TextureFilterSize_Mag:        return GL_NEAREST;
                    F_Assert(false && "Util_Transform2GLFilter: Wrong FTextureFilterSizeType type !")
                }   
            }   
            break;
        case F_TextureFilter_Bilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return GL_LINEAR;
                    case F_TextureFilterSize_Mag:        return GL_LINEAR;
                    F_Assert(false && "Util_Transform2GLFilter: Wrong FTextureFilterSizeType type !")
                }
            }
            break;
        case F_TextureFilter_Trilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return GL_LINEAR;
                    case F_TextureFilterSize_Mag:        return GL_LINEAR;
                    F_Assert(false && "Util_Transform2GLFilter: Wrong FTextureFilterSizeType type !")
                }
            }
            break;
        case F_TextureFilter_Anisotropic:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return GL_LINEAR;
                    case F_TextureFilterSize_Mag:        return GL_LINEAR;
                    F_Assert(false && "Util_Transform2GLFilter: Wrong FTextureFilterSizeType type !")
                }
            }
            break;
        } 
        F_Assert(false && "Util_Transform2GLFilter: Wrong FTextureFilterType type !")
        return GL_NEAREST;
    }
    GLenum Util_Transform2GLSamplerMipmapMode(FTextureFilterType type)
    {
        switch ((int)type)
        {
        case F_TextureFilterPixel_None:            return GL_NEAREST_MIPMAP_NEAREST;
        case F_TextureFilterPixel_Point:           return GL_NEAREST_MIPMAP_NEAREST;
        case F_TextureFilterPixel_Linear:          return GL_LINEAR_MIPMAP_NEAREST;
        case F_TextureFilterPixel_Anisotropic:     return GL_LINEAR_MIPMAP_NEAREST;
        }
        F_Assert(false && "Util_Transform2GLSamplerMipmapMode: Wrong type !")
        return GL_NEAREST_MIPMAP_NEAREST;
    }
    GLenum Util_Transform2GLSamplerAddressMode(FTextureAddressingType type)
    {
        switch((int)type)
        {
        case F_TextureAddressing_Wrap:      return GL_REPEAT;
        case F_TextureAddressing_Mirror:    return GL_MIRRORED_REPEAT;
        case F_TextureAddressing_Clamp:     return GL_CLAMP_TO_EDGE;
        case F_TextureAddressing_Border:    return GL_CLAMP_TO_BORDER;
        }
        F_Assert(false && "Util_Transform2GLSamplerAddressMode: Wrong type !")
        return GL_REPEAT;
    }
    int Util_Transform2GLSampleCountFlagBits(FMSAASampleCountType type)
    {
        switch((int)type)
        {
        case F_MSAASampleCount_1_Bit:      return 1;
        case F_MSAASampleCount_2_Bit:      return 2;
        case F_MSAASampleCount_4_Bit:      return 4;
        case F_MSAASampleCount_8_Bit:      return 8;
        case F_MSAASampleCount_16_Bit:     return 16;
        case F_MSAASampleCount_32_Bit:     return 32;
        case F_MSAASampleCount_64_Bit:     return 64;
        }
        F_Assert(false && "Util_Transform2GLSampleCountFlagBits: Wrong type !")
        return 1;
    }


////////////////////////////// Class ///////////////////////////////
    MeshInfo::MeshInfo()
        : group(0)
        , nameMesh("")
        , pathMesh("")
        , typeMesh(F_Mesh_File)
        , typeVertex(F_MeshVertex_Pos3Color4Tex2)
        , typeGeometryType(F_MeshGeometry_EntityGrid)
        , pMeshCreateParam(nullptr)
        , isFlipY(false)
        , isTransformLocal(false)
        , matTransformLocal(FMath::ms_mat4Unit)
    {

    }
    MeshInfo::MeshInfo(const String& _nameMesh,
                       const String& _pathMesh,
                       FMeshType _typeMesh,
                       FMeshVertexType _typeVertex,
                       FMeshGeometryType _typeGeometryType,
                       FMeshCreateParam* _pMeshCreateParam,
                       bool _isFlipY,
                       bool _isTransformLocal,
                       const FMatrix4& _matTransformLocal)
        : group(0)
        , nameMesh(_nameMesh)
        , pathMesh(_pathMesh)
        , typeMesh(_typeMesh)
        , typeVertex(_typeVertex)
        , typeGeometryType(_typeGeometryType)
        , pMeshCreateParam(_pMeshCreateParam)
        , isFlipY(_isFlipY)
        , isTransformLocal(_isTransformLocal)
        , matTransformLocal(_matTransformLocal)
    {

    }
    MeshInfo::~MeshInfo()
    {
        F_DELETE(this->pMeshCreateParam)
    }


  
    void DescriptorSetLayout::Init(const String& nameLayout)
    {
        this->nameDescriptorSetLayout = nameLayout;
        this->aLayouts = FUtilString::Split(nameLayout, "-");
        int count_ds = (int)this->aLayouts.size();
        for (int i= 0; i < count_ds; i++)
        {
            this->mapName2Index[this->aLayouts[i]] = i;
        }
    }

}; //LostPeterOpenGLES