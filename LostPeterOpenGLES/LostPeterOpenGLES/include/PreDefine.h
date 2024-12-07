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

#ifndef _PRE_DEFINE_H_
#define _PRE_DEFINE_H_

#include "PreIncludeStd.h"
#include "PreIncludeThird.h"

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

#include "ObjectPool.h"

namespace LostPeterOpenGLES
{
////////////////////////////// Define //////////////////////////////
    #define MAX_LIGHT_COUNT 16
    #define MAX_TEXTURE_COUNT 16
    #define MAX_OBJECT_COUNT 1024
    #define MAX_OBJECT_TERRAIN_COUNT 8196
    #define MAX_OBJECT_LINEFLAT_2D_COUNT 2048
    #define MAX_OBJECT_LINEFLAT_3D_COUNT 512
    #define MAX_MATERIAL_COUNT 64
    #define MAX_INSTANCE_COUNT 4096
    #define MAX_CULL_INSTANCE_COUNT 16384
    #define MAX_TERRAIN_SPLAT_COUNT 16
    #define MAX_FRUSTUM_CORNER_COUNT 8
    #define MAX_FRUSTUM_PLANE_COUNT 6
    #define MAX_OBJECT_CULL_COUNT 8196

    #define SHADER_NAME_Vertex                      "vert"
    #define SHADER_NAME_TessellationControl         "tesc"
    #define SHADER_NAME_TessellationEvaluation      "tese"
    #define SHADER_NAME_Geometry                    "geom"
    #define SHADER_NAME_Fragment                    "frag"
    #define SHADER_NAME_Compute                     "comp"


    #define C_THRESH_POINT_ON_PLANE			0.10f
    #define C_THRESH_POINT_ON_SIDE			0.20f
    #define C_THRESH_POINTS_ARE_SAME		0.00002f	
    #define C_THRESH_POINTS_ARE_NEAR		0.015f
    #define C_THRESH_NORMALS_ARE_SAME		0.00002f
    #define C_THRESH_UVS_ARE_SAME			0.0009765625f
    #define C_THRESH_VECTORS_ARE_NEAR		0.0004f
    #define C_THRESH_SPLIT_POLY_WITH_PLANE	0.25f	
    #define C_THRESH_SPLIT_POLY_PRECISELY	0.01f
    #define C_THRESH_ZERO_NORM_SQUARED		0.0001f	
    #define C_THRESH_NORMALS_ARE_PARALLEL	0.999845f	
    #define C_THRESH_NORMALS_ARE_ORTHOGONAL	0.017455f
    #define C_THRESH_VECTOR_NORMALIZED		0.01f
    #define C_THRESH_QUAT_NORMALIZED		0.01f

    #define C_CONFIG_MAX_RENDER_TARGET_MULTIPLE_COUNT       8
    #define C_CONFIG_MAX_TEXTURE_UNIT_COUNT				    16
    #define C_CONFIG_MAX_TEXTURE_COORD_SETS_COUNT		    8
    #define	C_CONFIG_MAX_TEXTURE_UV_SCROLL_ANIM_COUNT	    4
    #define C_CONFIG_MAX_BONE_BLEND_WEIGHTS_COUNT	        4
    #define C_CONFIG_MAX_BONE_COUNT					        256
    #define C_CONFIG_MAX_STREAM_SOURCE_COUNT				16
    #define C_CONFIG_MAX_LIGHT_COUNT						8
    #define C_CONFIG_MAX_SHADER_PARAM_COUNT				    32
    #define C_CONFIG_MAX_GLYPHS_COUNT				        (9030 - 32)
    #define C_CONFIG_GLYPH_INDEX(c)					        c - 33

    #define UTIL_CPU_ALLOCATOR              nullptr

////////////////////////////// Typedef /////////////////////////////
    

    


////////////////////////////// Enum ////////////////////////////////
    enum WindowType
    {
        Window_Main = 0,                         //0:    Main
        Window_Game,                             //1:    Game
        Window_Scene,                            //2:    Scene

        Window_Count,
    };
    openglesExport const String& Util_GetWindowTypeName(WindowType type);
    openglesExport const String& Util_GetWindowTypeName(int type);
    openglesExport WindowType Util_ParseWindowType(const String& strName);


    enum SwapStatusType
    {
        SwapStatus_Normal = 0,
        SwapStatus_OutOfDate = -1,
        SwapStatus_Lost = -2,
        SwapStatus_Error = -3,
    };


    struct openglesExport PointerBuffer
    {
        void* pBase;
        int nPosStart;
        int nPosEnd;
        
        PointerBuffer()
            : pBase(nullptr)
            , nPosStart(0)
            , nPosEnd(0)
        {

        }
        PointerBuffer(void* p, int posStart, int posEnd)
            : pBase(p)
            , nPosStart(posStart)
            , nPosEnd(posEnd)
        {

        }

        void Clear()
        {
            this->pBase = nullptr;
            this->nPosStart = 0;
            this->nPosEnd = 0;
        }

        void Set(void* p, int posStart, int posEnd)
        {
            this->pBase = p;
            this->nPosStart = posStart;
            this->nPosEnd = posEnd;
        }

        void Offset(int posOffset)
        {
            this->nPosStart += posOffset;
            this->nPosEnd += posOffset;
        }
    };
    typedef std::vector<PointerBuffer*> PointerBufferPtrVector;


    struct openglesExport PixelFormatDes
	{
		String name;
		uint8 nElemBytes;
		uint32 eFlags;
		uint32 eComponentType;
		uint32 nComponentCount;
        bool isSupported;
		
		uint8 nRbits;
		uint8 nGbits;
		uint8 nBbits;
		uint8 nAbits;
		
		uint64 nRmask;
		uint64 nGmask;
		uint64 nBmask;
		uint64 nAmask;
	
		uint8 nRshift;
		uint8 nGshift;
		uint8 nBshift;
		uint8 nAshift;
	};


    enum PixelFormatFlagType
	{
        PixelFormatFlag_IsNative		    = 0x00000001,   //0: IsNative
        PixelFormatFlag_IsCompressed	    = 0x00000002,   //1: IsCompressed
        PixelFormatFlag_IsInteger	        = 0x00000004,   //2: IsInteger
        PixelFormatFlag_IsFloat	            = 0x00000008,   //3: IsFloat
		PixelFormatFlag_IsLuminance	        = 0x00000010,   //4: IsLuminance
        PixelFormatFlag_IsStencil           = 0x00000020,   //5: IsStencil
		PixelFormatFlag_IsDepth		        = 0x00000040,   //6: IsDepth
        PixelFormatFlag_IsDepthStencil      = 0x00000080,   //7: IsDepthStencil
		PixelFormatFlag_HasAlpha		    = 0x00000100,   //8: HasAlpha
	};


    enum PixelFormatComponentType
	{
		PixelFormatComponent_ByteU = 0,                 //0: Byte unsigned
        PixelFormatComponent_ByteS,                     //1: Byte signed
		PixelFormatComponent_ShortU,                    //2: Short unsigned
        PixelFormatComponent_ShortS,                    //3: Short signed
        PixelFormatComponent_IntU,					    //4: Int unsigned
		PixelFormatComponent_IntS,					    //5: Int signed
        PixelFormatComponent_LongU,					    //6: Long unsigned
		PixelFormatComponent_LongS,					    //7: Long signed
		PixelFormatComponent_Float16,                   //8: Float 16
		PixelFormatComponent_Float32,                   //9: Float 32
        PixelFormatComponent_Double,                    //10: Double
	};
    openglesExport const String& Util_GetPixelFormatComponentTypeName(PixelFormatComponentType type);
    openglesExport const String& Util_GetPixelFormatComponentTypeName(int type);


    enum LodType
	{
        Lod_0 = 0,
        Lod_1,
        Lod_2,
        
        Lod_Count,
	};

    enum CullUnitType
    {
        CullUnit_Object = 0,
        CullUnit_Terrain,

        CullUnit_Count,
    };


    enum LightType
    {
        Light_Directional = 0,
        Light_Point,
        Light_Spot,

    };


    enum LightingType
    {
        Lighting_Node = 0,                                //0:    None
        Lighting_Ambient,                                 //1:    Ambient
        Lighting_DiffuseLambert,                          //2:    DiffuseLambert
        Lighting_SpecularPhong,                           //3:    SpecularPhong
        Lighting_SpecularBlinnPhong,                      //4:    SpecularBlinnPhong
        Lighting_AmbientDiffuseLambert,                   //5:    Ambient + DiffuseLambert
        Lighting_AmbientSpecularPhong,                    //6:    Ambient + SpecularPhong
        Lighting_AmbientSpecularBlinnPhong,               //7:    Ambient + SpecularBlinnPhong
        Lighting_DiffuseLambertSpecularPhong,             //8:    DiffuseLambert + SpecularPhong
        Lighting_DiffuseLambertSpecularBlinnPhong,        //9:    DiffuseLambert + SpecularBlinnPhong
        Lighting_AmbientDiffuseLambertSpecularPhong,      //10:   Ambient + DiffuseLambert + SpecularPhong
        Lighting_AmbientDiffuseLambertSpecularBlinnPhong, //11:   Ambient + DiffuseLambert + SpecularBlinnPhong
    };
    

    enum ShaderSortType
    {
        ShaderSort_Common = 0,      //0: Common
        ShaderSort_Platform,        //1: Platform

    };
    openglesExport const String& Util_GetShaderSortTypeName(ShaderSortType type);
    openglesExport const String& Util_GetShaderSortTypeName(int type);
    openglesExport ShaderSortType Util_ParseShaderSortType(const String& strName);


    enum DescriptorSetType
    {
        DescriptorSet_PassConstants = 0,             //0:  PassConstants
        DescriptorSet_ObjectConstants,               //1:  ObjectConstants
        DescriptorSet_ObjectTerrain,                 //2:  ObjectTerrain
        DescriptorSet_ObjectGrid,                    //3:  ObjectGrid
        DescriptorSet_ObjectCameraAxis,              //4:  ObjectCameraAxis
        DescriptorSet_ObjectCoordinateAxis,          //5:  ObjectCoordinateAxis
        DescriptorSet_ObjectLineFlat2D,              //6:  ObjectLineFlat2D
        DescriptorSet_ObjectLineFlat3D,              //7:  ObjectLineFlat3D
        DescriptorSet_ObjectCopyBlit,                //8:  ObjectCopyBlit
        DescriptorSet_ObjectCull,                    //9:  ObjectCull
        DescriptorSet_Material,                      //10: Material
        DescriptorSet_Instance,                      //11: Instance
        DescriptorSet_TextureCopy,                   //12: TextureCopy
        DescriptorSet_Tessellation,                  //13: Tessellation
        DescriptorSet_Geometry,                      //14: Geometry
        DescriptorSet_TextureVS,                     //15: TextureVS
        DescriptorSet_TextureTESC,                   //16: TextureTESC
        DescriptorSet_TextureTESE,                   //17: TextureTESE
        DescriptorSet_TextureFS,                     //18: TextureFS
        DescriptorSet_TextureFrameColor,             //19: TextureFrameColor
        DescriptorSet_TextureFrameDepth,             //20: TextureFrameDepth
        DescriptorSet_TextureCSR,                    //21: TextureCSR
        DescriptorSet_TextureCSRW,                   //22: TextureCSRW
        DescriptorSet_TextureCSRWSrc,                //23: TextureCSRWSrc
        DescriptorSet_TextureCSRWDst,                //24: TextureCSRWDst
        DescriptorSet_TextureDepthShadow,            //25: TextureDepthShadow
        DescriptorSet_InputAttachRed,                //26: InputAttachRed
        DescriptorSet_InputAttachGreen,              //27: InputAttachGreen
        DescriptorSet_InputAttachBlue,               //28: InputAttachBlue
        DescriptorSet_Terrain,                       //29: Terrain
        DescriptorSet_Cull,                          //30: Cull
        DescriptorSet_HizDepth,                      //31: HizDepth
        DescriptorSet_BufferRWArgsCB,                //32: BufferRWArgsCB
        DescriptorSet_BufferRWLodCB,                 //33: BufferRWLodCB
        DescriptorSet_BufferRWResultCB,              //34: BufferRWResultCB
        DescriptorSet_BufferRWClipCB,                //35: BufferRWClipCB
        DescriptorSet_BufferRWObjectCullInstance,    //36: BufferRWObjectCullInstance
        DescriptorSet_CullInstance,                  //37: CullInstance
        DescriptorSet_BufferObjectLineFlat2D,        //38: BufferObjectLineFlat2D
        DescriptorSet_BufferObjectLineFlat3D,        //39: BufferObjectLineFlat3D

        DescriptorSet_Count,
    };
    openglesExport const String& Util_GetDescriptorSetTypeName(DescriptorSetType type);
    openglesExport const String& Util_GetDescriptorSetTypeName(int type);



////////////////////////////// OpenGL //////////////////////////////

    openglesExport void Util_CreateAttributeDescriptions(FMeshVertexType type);
    openglesExport void Util_EnableAttributeDescriptions(FMeshVertexType type, bool enable);

    openglesExport GLenum Util_Transform2GLShaderType(FShaderType type);
    openglesExport bool Util_IsSupportExtensionsGL(int type);

    openglesExport GLenum Util_Transform2GLTextureType(FTextureType type);
    openglesExport GLenum Util_Transform2GLFormat(FTexturePixelFormatType type);
    openglesExport GLenum Util_Transform2GLFilterSize(FTextureFilterSizeType type);
    openglesExport GLenum Util_Transform2GLFilter(FTextureFilterPixelType type);
    openglesExport GLenum Util_Transform2GLSamplerMipmapMode(FTextureFilterPixelType type);
    openglesExport GLenum Util_Transform2GLFilter(FTextureFilterType typeFilter, FTextureFilterSizeType typeFilterSize);
    openglesExport GLenum Util_Transform2GLSamplerMipmapMode(FTextureFilterType type);
    openglesExport GLenum Util_Transform2GLSamplerAddressMode(FTextureAddressingType type);
    openglesExport int Util_Transform2GLSampleCountFlagBits(FMSAASampleCountType type);

////////////////////////////// Class ///////////////////////////////
    struct LightConstants;
    struct PassConstants;
    struct ObjectConstants;
    struct TerrainObjectConstants;
    struct GridObjectConstants;
    struct CameraAxisObjectConstants;
    struct CoordinateAxisObjectConstants;
    struct LineFlat3DObjectConstants;
    struct CopyBlitObjectConstants;
    struct MaterialConstants;
    struct InstanceConstants;

    struct openglesExport MeshInfo
    {
        MeshInfo();
        MeshInfo(const String& _nameMesh,
                 const String& _pathMesh,
                 FMeshType _typeMesh,
                 FMeshVertexType _typeVertex,
                 FMeshGeometryType _typeGeometryType,
                 FMeshCreateParam* _pMeshCreateParam,
                 bool _isFlipY,
                 bool _isTransformLocal,
                 const FMatrix4& _matTransformLocal);
        ~MeshInfo();

        uint32 group;
        String nameMesh;
        String pathMesh;
        
        FMeshType typeMesh;
        FMeshVertexType typeVertex;
        FMeshGeometryType typeGeometryType;
        FMeshCreateParam* pMeshCreateParam;

        bool isFlipY;
        bool isTransformLocal;
        FMatrix4 matTransformLocal;
    };
    typedef std::vector<MeshInfo*> MeshInfoPtrVector;
    typedef std::map<String, MeshInfo*> MeshInfoPtrMap;


    struct openglesExport ShaderModuleInfo
    {
        String nameShader;
        String nameShaderType;
        String pathShader;
    };
    typedef std::vector<ShaderModuleInfo> ShaderModuleInfoVector;


    typedef std::map<String, uint32> DescriptorSetName2IndexMap;
    struct openglesExport DescriptorSetLayout
    {
        String nameDescriptorSetLayout;
        StringVector aLayouts;
        DescriptorSetName2IndexMap mapName2Index;

        void Init(const String& nameLayout);
    };
    typedef std::vector<DescriptorSetLayout> DescriptorSetLayoutVector;
    typedef std::vector<DescriptorSetLayout*> DescriptorSetLayoutPtrVector;
    typedef std::map<String, DescriptorSetLayout*> DescriptorSetLayoutPtrMap;


    class AndroidWindow;
    class App;
    class OpenGLESBase;
    class OpenGLESWindow;

    class GLESDebug;
    class GLESBuffer;
    class GLESBufferStorage;
    class GLESBufferUniform; 
    class GLESBufferVertex;
    class GLESBufferVertexIndex;
    class GLESFrameBuffer;
    class GLESPipelineGraphicsCopyBlitToFrame;
    class GLESRenderBuffer;
    class GLESRenderPass;
    class GLESShader;
    class GLESShaderInclude;
    class GLESShaderProgram;
    class GLESTexture;

    typedef std::map<uint32, uint32> UintID2UintTypeMap;
    typedef std::map<uint32, uint32> UintType2UintIDMap;

    typedef std::vector<GLESBuffer*> GLESBufferPtrVector;
    typedef std::map<String, GLESBuffer*> GLESBufferPtrMap;
    typedef std::vector<GLESBufferStorage*> GLESBufferStoragePtrVector;
    typedef std::map<String, GLESBufferStorage*> GLESBufferStoragePtrMap;
    typedef std::vector<GLESBufferUniform*> GLESBufferUniformPtrVector;
    typedef std::map<String, GLESBufferUniform*> GLESBufferUniformPtrMap;
    typedef std::vector<GLESBufferVertex*> GLESBufferVertexPtrVector;
    typedef std::map<String, GLESBufferVertex*> GLESBufferVertexPtrMap;
    typedef std::vector<GLESBufferVertexIndex*> GLESBufferVertexIndexPtrVector;
    typedef std::map<String, GLESBufferVertexIndex*> GLESBufferVertexIndexPtrMap;

    typedef std::vector<GLESRenderBuffer*> GLESRenderBufferPtrVector;
    typedef std::map<String, GLESRenderBuffer*> GLESRenderBufferPtrMap;

    typedef std::vector<GLESFrameBuffer*> GLESFrameBufferPtrVector;
    typedef std::map<String, GLESFrameBuffer*> GLESFrameBufferPtrMap;

    typedef std::vector<GLESRenderPass*> GLESRenderPassPtrVector;
    typedef std::map<String, GLESRenderPass*> GLESRenderPassPtrMap;

    typedef std::vector<GLESShader*> GLESShaderPtrVector;
    typedef std::map<String, GLESShader*> GLESShaderPtrMap;
    typedef std::map<int32, GLESShader*> GLESShaderPtrIDMap;
    typedef std::vector<GLESShaderProgram*> GLESShaderProgramPtrVector;
    typedef std::map<String, GLESShaderProgram*> GLESShaderProgramPtrMap;
    typedef std::map<int32, GLESShaderProgram*> GLESShaderProgramPtrIDMap;

    typedef std::vector<GLESTexture*> GLESTexturePtrVector;
    typedef std::map<String, GLESTexture*> GLESTexturePtrMap;


    class Base;
    class EditorBase;
    class EditorCameraAxis;
    class EditorCoordinateAxis;
    class EditorGrid;
    class EditorLineFlat2DCollector;
    class EditorLineFlat3DCollector;
    class Mesh;
    class MeshSub;

    typedef std::vector<MeshSub*> MeshSubPtrVector;
    typedef std::map<String, MeshSub*> MeshSubPtrMap;
    
    typedef std::vector<Mesh*> MeshPtrVector;
    typedef std::map<String, Mesh*> MeshPtrMap;


}; //LostPeterOpenGLES


#endif