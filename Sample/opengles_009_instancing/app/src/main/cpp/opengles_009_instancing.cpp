/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-03-11
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_009_instancing.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


const String c_strVert = ".vert.spv";
const String c_strFrag = ".frag.spv";

static const int g_ShaderCount = 3;
static const char* g_pathShaderModules[2 * g_ShaderCount] = 
{
    "standard_mesh_opaque.vert.spv", "standard_mesh_opaque.frag.spv", //standard_mesh_opaque
    "standard_mesh_transparent.vert.spv", "standard_mesh_transparent.frag.spv", //standard_mesh_transparent
    "standard_mesh_outline.vert.spv", "standard_mesh_outline.frag.spv", //standard_mesh_outline
};

static const int g_CountLen = 2;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "viking_room",      "Mesh/Model/viking_room/viking_room.obj",    "Texture/Model/viking_room/viking_room.png", //viking_room
    "bunny",            "Mesh/Model/bunny/bunny.obj",                "Texture/Common/default_white.bmp", //bunny
};

static const char* g_pathModelShaderModules[2 * g_CountLen] = 
{
    "standard_mesh_transparent", "standard_mesh_outline", //viking_room
    "standard_mesh_opaque", "standard_mesh_outline", //bunny
};

static float g_instanceGap = 1.5f;

static FVector3 g_tranformModels[3 * g_CountLen] = 
{
	FVector3(   0,   0,    1),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //viking_room
    FVector3(   0,   0,    0),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //bunny
};

static FMatrix4 g_tranformLocalModels[g_CountLen] = 
{
    FMath::RotateX(-90.0f), //viking_room
    FMath::ms_mat4Unit, //bunny
};

static bool g_isTranformLocalModels[g_CountLen] = 
{
    true, //viking_room
    false, //bunny
};

static bool g_isFlipYModels[g_CountLen] = 
{
    false, //viking_room
    false, //bunny
};

static bool g_isTransparentModels[g_CountLen] = 
{
    true, //viking_room
    false, //bunny
};

static float g_TransparentAlpha[g_CountLen] =
{
    0.5f, //viking_room
    1.0f, //bunny
};

static float g_OutlineWidth[g_CountLen] = 
{
    0.02f, //viking_room
    0.02f, //bunny
};

static FVector4 g_OutlineColor[g_CountLen] = 
{
    FVector4(0,1,0,1), //viking_room
    FVector4(1,0,0,1), //bunny
};


OpenGLES_009_Instancing::OpenGLES_009_Instancing(String name)
    : OpenGLESWindow(name)
{
    this->cfg_isDepthStencil = true;
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
	this->cfg_isRotate = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->poTypeVertex = F_MeshVertex_Pos3Color4Normal3Tex2;

    this->cfg_cameraPos = FVector3(0.0f, 20.0f, -10.0f);
}

void OpenGLES_009_Instancing::createCamera()
{
    OpenGLESWindow::createCamera();
    
    cameraReset();
}

void OpenGLES_009_Instancing::loadModel_Custom()
{
    for (int i = 0; i < g_CountLen; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->nameModel = g_pathModels[3 * i + 0];
        pModelObject->pathModel = g_pathModels[3 * i + 1];
        pModelObject->pathTexture = g_pathModels[3 * i + 2];

        bool isFlipY = g_isFlipYModels[i];
        bool isTransformLocal = g_isTranformLocalModels[i];
		pModelObject->isTransparent = g_isTransparentModels[i];
        
        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTransformLocal, g_tranformLocalModels[i]))
        {
            String msg = "*********************** OpenGLES_009_Instancing::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "*********************** OpenGLES_009_Instancing::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

		m_aModelObjects.push_back(pModelObject);
        if (pModelObject->isTransparent)
            m_aModelObjects_Render.push_back(pModelObject);
        else 
            m_aModelObjects_Render.insert(m_aModelObjects_Render.begin(), pModelObject);
        m_mapModelObjects[pModelObject->nameModel] = pModelObject;
    }
}
bool OpenGLES_009_Instancing::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load 
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        F_LogError("*********************** OpenGLES_009_Instancing::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    pModelObject->vertices.clear();
    pModelObject->vertices.reserve(count_vertex);
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

        pModelObject->vertices.push_back(v);
    }

    int count_index = (int)meshData.indices32.size();
    pModelObject->indices.clear();
    pModelObject->indices.reserve(count_index);
    for (int i = 0; i < count_index; i++)
    {
        pModelObject->indices.push_back(meshData.indices32[i]);
    }
    pModelObject->poVertexCount = (uint32_t)pModelObject->vertices.size();
    pModelObject->poVertexBuffer_Size = pModelObject->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
    pModelObject->poVertexBuffer_Data = &pModelObject->vertices[0];
    pModelObject->poIndexCount = (uint32_t)pModelObject->indices.size();
    pModelObject->poIndexBuffer_Size = pModelObject->poIndexCount * sizeof(uint32_t);
    pModelObject->poIndexBuffer_Data = &pModelObject->indices[0];

    F_LogInfo("OpenGLES_009_Instancing::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
              pModelObject->nameModel.c_str(),
              (int)pModelObject->vertices.size(), 
              (int)pModelObject->indices.size());

    //2> createBufferVertexIndex or createBufferVertex
	if (pModelObject->poIndexBuffer_Size > 0 &&
		pModelObject->poIndexBuffer_Data != nullptr)
	{
		pModelObject->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex("VertexIndex-" + pModelObject->nameModel,
																						 this->poTypeVertex,
																						 pModelObject->poVertexBuffer_Size, 
																						 (uint8*)pModelObject->poVertexBuffer_Data, 
																						 false,
																						 pModelObject->poIndexBuffer_Size, 
																						 (uint8*)pModelObject->poIndexBuffer_Data, 
																						 false);
		if (pModelObject->pBufferVertexIndex == nullptr)
		{
			F_LogError("*********************** OpenGLES_009_Instancing::loadModel_VertexIndex: create buffer vertex index failed: [%s] !", pModelObject->nameModel.c_str());
			return false;
		}
	}
	else
	{
		pModelObject->pBufferVertex = Base::GetWindowPtr()->createBufferVertex("Vertex-" + pModelObject->nameModel,
																			   this->poTypeVertex,
																			   pModelObject->poVertexBuffer_Size, 
																			   (uint8*)pModelObject->poVertexBuffer_Data, 
																			   false);
		if (pModelObject->pBufferVertex == nullptr)
		{
			F_LogError("*********************** OpenGLES_009_Instancing::loadModel_VertexIndex: create buffer vertex failed: [%s] !", pModelObject->nameModel.c_str());
			return false;
		}
	}

    return true;
}
bool OpenGLES_009_Instancing::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        String nameTexture;
        String pathBase;
        FUtilString::SplitFileName(pModelObject->pathTexture, nameTexture, pathBase);
		StringVector aPathTexture;
		aPathTexture.push_back(pModelObject->pathTexture);
		pModelObject->poTexture = new GLESTexture(nameTexture,
												  aPathTexture,
												  F_Texture_2D,
												  F_TexturePixelFormat_R8G8B8A8_UNORM,
												  F_TextureAddressing_Wrap,
												  F_TextureFilter_Bilinear,
												  F_TextureFilter_Bilinear,
												  F_MSAASampleCount_1_Bit,
												  FColor(0, 0, 0, 1),
												  true,
												  true,
												  false,
												  false,
												  false);
		if (!pModelObject->poTexture->Init())
		{
			F_LogError("*********************** OpenGLES_009_Instancing::loadModel_Texture: Failed to create texture, path: [%s] !", pModelObject->pathTexture.c_str());
			F_DELETE(this->poTexture)
			return false;
		}
        F_LogInfo("OpenGLES_009_Instancing::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void OpenGLES_009_Instancing::createCustomCB()
{
	rebuildInstanceCBs(true);
}
void OpenGLES_009_Instancing::rebuildInstanceCBs(bool isCreateBuffer)
{
	size_t maxCount = MAX_OBJECT_COUNT;
	size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//1> Object
		pModelObject->instanceMatWorld.resize(maxCount);
        pModelObject->objectCBs.resize(maxCount);
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = FMath::FromTRS(g_tranformModels[i * 3 + 0] + FVector3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                        g_tranformModels[i * 3 + 1],
                                                        g_tranformModels[i * 3 + 2]);
            pModelObject->objectCBs[j] = objectConstants;
            pModelObject->instanceMatWorld[j] = objectConstants.g_MatWorld;
        }

		if (isCreateBuffer)
		{
			String nameBuffer = "ObjectConstants-" + FUtilString::SaveInt((int)i);
			pModelObject->poBufferUniform = createBufferUniform(nameBuffer,
																DescriptorSet_ObjectConstants,
																GL_DYNAMIC_DRAW,
																sizeof(ObjectConstants) * pModelObject->objectCBs.size(),
																(uint8*)(pModelObject->objectCBs.data()),
																false);
			if (!pModelObject->poBufferUniform)
			{
				String msg = "*********************** OpenGLES_009_Instancing::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError("%s", msg.c_str());
				throw std::runtime_error(msg);
			}
		}

		//2> Material
		if (pModelObject->isTransparent)
		{
			pModelObject->materialCBs.resize(maxCount);
            for (int j = 0; j < pModelObject->countInstance; j++)
            {
                MaterialConstants materialConstants;
                materialConstants.lighting.y = g_TransparentAlpha[i];
                pModelObject->materialCBs[j] = materialConstants;
            }

            if (isCreateBuffer)
			{
				String nameBuffer = "MaterialConstants-" + FUtilString::SaveInt((int)i);
				pModelObject->poBufferUniform_Material = createBufferUniform(nameBuffer,
																			 DescriptorSet_MaterialConstants,
																			 GL_DYNAMIC_DRAW,
																			 sizeof(MaterialConstants) * pModelObject->materialCBs.size(),
																			 (uint8*)(pModelObject->materialCBs.data()),
																			 false);
				if (!pModelObject->poBufferUniform_Material)
				{
					String msg = "*********************** OpenGLES_009_Instancing::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
					F_LogError("%s", msg.c_str());
					throw std::runtime_error(msg);
				}
			}
		}

		//3> Outline
		pModelObject->objectCBs_Outline.resize(maxCount);
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            OutlineObjectConstants objectConstants_Outline;
            objectConstants_Outline.g_MatWorld = pModelObject->instanceMatWorld[j];
            objectConstants_Outline.g_OutlineColor = FMath::RandomColor(false);
            objectConstants_Outline.g_OutlineWidth = g_OutlineWidth[i];
            pModelObject->objectCBs_Outline[j] = objectConstants_Outline;
        }

		if (isCreateBuffer)
		{
			String nameBuffer = "OutlineObjectConstants-" + FUtilString::SaveInt((int)i);
			pModelObject->poBufferUniform_Outline = createBufferUniform(nameBuffer,
																		DescriptorSet_OutlineObjectConstants,
																		GL_DYNAMIC_DRAW,
																		sizeof(OutlineObjectConstants) * pModelObject->objectCBs_Outline.size(),
																		(uint8*)(pModelObject->objectCBs_Outline.data()),
																		false);
			if (!pModelObject->poBufferUniform_Outline)
			{
				String msg = "*********************** OpenGLES_009_Instancing::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError("%s", msg.c_str());
				throw std::runtime_error(msg);
			}
		}
    }
}

void OpenGLES_009_Instancing::createCustomBeforePipeline()
{
    //1> Shader
    createShaderModules();
}  
void OpenGLES_009_Instancing::createGraphicsPipeline_Custom()
{
    String namePathBase;

    //1> Shader


    //2> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		String pathVertShaderBase = getShaderPathRelative(g_pathModelShaderModules[2 * i + 0] + c_strVert);
        String pathFragShaderBase = getShaderPathRelative(g_pathModelShaderModules[2 * i + 0] + c_strFrag);
        GLESShader* pShaderVertex = findShaderModule(pathVertShaderBase);
        GLESShader* pShaderFragment = findShaderModule(pathFragShaderBase);

        String pathVertShaderOutline = getShaderPathRelative(g_pathModelShaderModules[2 * i + 1] + c_strVert);
        String pathFragShaderOutline = getShaderPathRelative(g_pathModelShaderModules[2 * i + 1] + c_strFrag);
        GLESShader* pShaderVertex_Outline = findShaderModule(pathVertShaderOutline);
        GLESShader* pShaderFragment_Outline = findShaderModule(pathFragShaderOutline);

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

        //poStatePipelineGraphics_Stencil
		String namePipelineGraphics_Stencil = "PipelineGraphics-Stencil-" + pModelObject->nameModel;
        pModelObject->poStatePipelineGraphics_Stencil = createStatePipelineGraphics(namePipelineGraphics_Stencil,
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
																					true,
																					GL_ALWAYS,
																					GL_REPLACE,
																					GL_REPLACE,
																					GL_REPLACE,
																					1,
																					0xFF,
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
        if (pModelObject->poStatePipelineGraphics_Stencil == nullptr)
        {
            String msg = "*********************** OpenGLES_009_Instancing::createGraphicsPipeline_Custom: Failed to create pipeline stencil !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

		//2> poStatePipelineGraphics_Outline
		String namePipelineGraphics_Outline = "PipelineGraphics-Outline-" + pModelObject->nameModel;
        pModelObject->poStatePipelineGraphics_Outline = createStatePipelineGraphics(namePipelineGraphics_Outline,
																					pShaderVertex_Outline,
																					nullptr,
																					nullptr,
																					nullptr,
																					pShaderFragment_Outline,
																					this->poTypeVertex,
																					pModelObject->poTypePrimitive,
																					pModelObject->poIsCull,
																					pModelObject->poTypeFrontFace,
																					pModelObject->poTypeCulling,
																					pModelObject->poTypePolygonMode,
																					pModelObject->poDepthEnabled,
																					pModelObject->poDepthFuncCompare,
																					pModelObject->poDepthTestEnabled,
																					pModelObject->poDepthWriteEnabled,
																					true,
																					GL_NOTEQUAL,
																					GL_KEEP,
																					GL_KEEP,
																					GL_REPLACE,
																					1,
																					0xFF,
																					false,
																					pModelObject->poBlendColorFactorSrc,
																					pModelObject->poBlendColorFactorDst,
																					pModelObject->poBlendColorOp,
																					pModelObject->poBlendAlphaFactorSrc,
																					pModelObject->poBlendAlphaFactorDst,
																					pModelObject->poBlendAlphaOp,
																					pModelObject->poColorWriteMask_Red,
																					pModelObject->poColorWriteMask_Green,
																					pModelObject->poColorWriteMask_Blue,
																					pModelObject->poColorWriteMask_Alpha);
        if (pModelObject->poStatePipelineGraphics_Outline == nullptr)
        {
            String msg = "*********************** OpenGLES_009_Instancing::createGraphicsPipeline_Custom: Failed to create pipeline outline !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

    }

}

void OpenGLES_009_Instancing::destroyShaderModules()
{
    size_t count = this->m_aGLESShaderModules.size();
    for (size_t i = 0; i < count; i++)
    {
        GLESShader* pDXShader = this->m_aGLESShaderModules[i];
        F_DELETE(pDXShader)
    }
    this->m_aGLESShaderModules.clear();
    this->m_mapGLESShaderModules.clear();
}
void OpenGLES_009_Instancing::createShaderModules()
{
    String nameVertexShader;
    String nameFragmentShader;
    String namePathBase;

    for (int i = 0; i < g_ShaderCount; i++)
    {
        String pathVert = getShaderPathRelative(g_pathShaderModules[2 * i + 0]);
        String pathFrag = getShaderPathRelative(g_pathShaderModules[2 * i + 1]);

        //vert
        FUtilString::SplitFileName(pathVert, nameVertexShader, namePathBase);
        GLESShader* pShaderVertex =  createShader(nameVertexShader, pathVert, F_Shader_Vertex);
        if (pShaderVertex == nullptr)
        {
            String msg = "*********************** OpenGLES_009_Instancing::createShaderModules: Failed to create shader vertex: " + pathVert;
			F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShaderVertex);
        this->m_mapGLESShaderModules[pathVert] = pShaderVertex;
        F_LogInfo("OpenGLES_009_Instancing::createShaderModules: create shader [%s] success !", pathVert.c_str());

        //frag
        FUtilString::SplitFileName(pathFrag, nameFragmentShader, namePathBase);
        GLESShader* pShaderFragment = createShader(nameFragmentShader, pathFrag, F_Shader_Fragment);
        if (pShaderFragment == nullptr)
        {
            String msg = "*********************** OpenGLES_009_Instancing::createShaderModules: Failed to create shader fragment: " + pathFrag;
			F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShaderFragment);
        this->m_mapGLESShaderModules[pathFrag] = pShaderFragment;
        F_LogInfo("OpenGLES_009_Instancing::createShaderModules: create shader [%s] success !", pathFrag.c_str());
    }
}
GLESShader* OpenGLES_009_Instancing::findShaderModule(const String& pathShaderModule)
{
    GLESShaderPtrMap::iterator itFind = this->m_mapGLESShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapGLESShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void OpenGLES_009_Instancing::createDescriptorSets_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//(0) PassConstants
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants);
			uint32 nBindingIndex = (uint32)DescriptorSet_PassConstants;

			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics_Stencil->GetUniformBlockIndex(nameDS);
			pModelObject->poStatePipelineGraphics_Stencil->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);

			nUniformBlockIndex = pModelObject->poStatePipelineGraphics_Outline->GetUniformBlockIndex(nameDS);
			pModelObject->poStatePipelineGraphics_Outline->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
		}
		//(1) ObjectConstants
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants);
			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics_Stencil->GetUniformBlockIndex(nameDS);
			uint32 nBindingIndex = (uint32)DescriptorSet_ObjectConstants;
			pModelObject->poStatePipelineGraphics_Stencil->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pModelObject->poStatePipelineGraphics_Stencil->BindBufferUniform(pModelObject->poBufferUniform, nBindingIndex);
		}
		//(2) OutlineObjectConstants
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_OutlineObjectConstants);
			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics_Outline->GetUniformBlockIndex(nameDS);
			uint32 nBindingIndex = (uint32)DescriptorSet_OutlineObjectConstants;
			pModelObject->poStatePipelineGraphics_Outline->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pModelObject->poStatePipelineGraphics_Outline->BindBufferUniform(pModelObject->poBufferUniform_Outline, nBindingIndex);
		}
		//(3) MaterialConstants
		if (pModelObject->poBufferUniform_Material)
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants);
			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics_Stencil->GetUniformBlockIndex(nameDS);
			uint32 nBindingIndex = (uint32)DescriptorSet_MaterialConstants;
			pModelObject->poStatePipelineGraphics_Stencil->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pModelObject->poStatePipelineGraphics_Stencil->BindBufferUniform(pModelObject->poBufferUniform_Material, nBindingIndex);
		}
		//(4) InstanceConstants
		{
			
		}
		//(5) Image
		{
			pModelObject->poStatePipelineGraphics_Stencil->BindTexture(pModelObject->poTexture, 0);
		}
    }
}

void OpenGLES_009_Instancing::updateCBs_Custom()
{
	GLESBufferUniform* pBufferUniform_Pass = GetUniform_PassCB();
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//0: PassConstants
		pModelObject->poStatePipelineGraphics_Stencil->BindBufferUniform(pBufferUniform_Pass, (uint32)DescriptorSet_PassConstants);
		pModelObject->poStatePipelineGraphics_Outline->BindBufferUniform(pBufferUniform_Pass, (uint32)DescriptorSet_PassConstants);

		//1: ObjectConstants
        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
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

			OutlineObjectConstants& objectCB_Outline = pModelObject->objectCBs_Outline[j];
			objectCB_Outline.g_MatWorld = objectCB.g_MatWorld;

			if (pModelObject->isTransparent)
			{
				MaterialConstants& materialCB = pModelObject->materialCBs[j];
            	materialCB.lighting.y = pModelObject->alpha;
			}
        }
		pModelObject->poBufferUniform->UpdateBuffer(sizeof(ObjectConstants) * count_object,
                                                 	(uint8*)pModelObject->objectCBs.data(),
													 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		pModelObject->poBufferUniform_Outline->UpdateBuffer(sizeof(OutlineObjectConstants) * count_object,
                                                 			(uint8*)pModelObject->objectCBs_Outline.data(),
															 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		if (pModelObject->poBufferUniform_Material)
		{
			pModelObject->poBufferUniform_Material->UpdateBuffer(sizeof(MaterialConstants) * count_object,
                                                 			 	 (uint8*)pModelObject->materialCBs.data(),
															  	 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		}		
    }
}



bool OpenGLES_009_Instancing::beginRenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("OpenGLES_009_Instancing", &windowOpened, 0);
    {
        //0> Common
        commonConfig();
        
        //1> Camera
        cameraConfig();
        
        //2> Model
        modelConfig();

    }
    ImGui::End();

    return true;
}
void OpenGLES_009_Instancing::modelConfig()
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

            String nameModel = FUtilString::SaveInt(i) + " - " + pModelObject->nameModel;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                String nameIsShow = "Is Show - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                String nameIsRotate = "Is Rotate - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                String nameIsOutline = "Is Outline - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsOutline.c_str(), &pModelObject->isOutline);
				String nameIsTransparent = "Is Transparent - " + pModelObject->nameModel;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                if (pModelObject->isTransparent)
                {
                    String nameAlpha = "Alpha - " + pModelObject->nameModel;
                    float fAlpha = pModelObject->alpha;
                    if (ImGui::DragFloat(nameAlpha.c_str(), &fAlpha, 0.001f, 0.0f, 1.0f))
                    {
                        pModelObject->alpha = fAlpha;
                    }
                }
                String nameInstances = "Instance - " + pModelObject->nameModel;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 10);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->poVertexCount, (int)pModelObject->poIndexCount);

                String nameWorld = "Model World - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    int count_instance = pModelObject->countInstance;
                    for (int j = 0; j < count_instance; j++)
                    {
                        OutlineObjectConstants& obj = pModelObject->objectCBs_Outline[j];
                        //Mat
                        const FMatrix4& mat4World = obj.g_MatWorld;
                        String nameTable = FUtilString::SaveInt(j) + " - matWorld - " + pModelObject->nameModel;
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
                        //OutlineWidth
                        String nameOutlineWidth = "Outline Width - " + pModelObject->nameModel;
                        float fOutlineWidth = obj.g_OutlineWidth;
                        if (ImGui::DragFloat(nameOutlineWidth.c_str(), &fOutlineWidth, 0.01f, 0.01f, 1.0f))
                        {
                            obj.g_OutlineWidth = fOutlineWidth;
                        }
                        //OutlineColor
                        String nameOutlineColor = "Outline Color - " + pModelObject->nameModel;
                        if (ImGui::ColorEdit4(nameOutlineColor.c_str(), (float*)&(obj.g_OutlineColor)))
                        {

                        }
                    }
                }
            }
        }
    }
}

void OpenGLES_009_Instancing::endRenderImgui()
{
    OpenGLESWindow::endRenderImgui();

}

void OpenGLES_009_Instancing::drawMeshDefault_Custom()
{   
	bool isSetWireFrame = false;
	size_t count = this->m_aModelObjects_Render.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects_Render[i];
        if (!pModelObject->isShow)
            continue;
		
		//State/Shader/BufferUniform/Texture
		pModelObject->poStatePipelineGraphics_Stencil->BindState();
		pModelObject->poStatePipelineGraphics_Stencil->BindShader();
		pModelObject->poStatePipelineGraphics_Stencil->BindBufferUniforms();
		pModelObject->poStatePipelineGraphics_Stencil->BindTextures();
		
		//Draw
		if (pModelObject->pBufferVertex != nullptr)
		{
			pModelObject->pBufferVertex->BindVertexArray();
			drawInstance(pModelObject->poStatePipelineGraphics_Stencil->poTypePrimitive, 0, pModelObject->poVertexCount, pModelObject->countInstance);
		}
		else if (pModelObject->pBufferVertexIndex != nullptr)
		{
			pModelObject->pBufferVertexIndex->BindVertexArray();
			drawIndexedInstance(pModelObject->poStatePipelineGraphics_Stencil->poTypePrimitive, pModelObject->poIndexCount, GL_UNSIGNED_INT, 0, pModelObject->countInstance);
		}
		else
		{	
			F_Assert(false && "OpenGLES_009_Instancing::drawMeshDefault_Custom")
		}

		if (pModelObject->isOutline)
		{
			//State/Shader/BufferUniform/Texture
			pModelObject->poStatePipelineGraphics_Outline->BindState();
			pModelObject->poStatePipelineGraphics_Outline->BindShader();
			pModelObject->poStatePipelineGraphics_Outline->BindBufferUniforms();
			pModelObject->poStatePipelineGraphics_Outline->BindTextures();
			
			//Draw
			if (pModelObject->pBufferVertex != nullptr)
			{
				pModelObject->pBufferVertex->BindVertexArray();
				drawInstance(pModelObject->poStatePipelineGraphics_Outline->poTypePrimitive, 0, pModelObject->poVertexCount, pModelObject->countInstance);
			}
			else if (pModelObject->pBufferVertexIndex != nullptr)
			{
				pModelObject->pBufferVertexIndex->BindVertexArray();
				drawIndexedInstance(pModelObject->poStatePipelineGraphics_Outline->poTypePrimitive, pModelObject->poIndexCount, GL_UNSIGNED_INT, 0, pModelObject->countInstance);
			}
			else
			{	
				F_Assert(false && "OpenGLES_009_Instancing::drawMeshDefault_Custom")
			}
		}
    }
}

void OpenGLES_009_Instancing::cleanupCustom()
{
	destroyShaderModules();

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