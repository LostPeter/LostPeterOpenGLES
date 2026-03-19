/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-12-01
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_008_blend.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


static const int g_CountLen = 2;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "viking_room",      "Mesh/Model/viking_room/viking_room.obj",    "Texture/Model/viking_room/viking_room.png", //viking_room
    "bunny",            "Mesh/Model/bunny/bunny.obj",                "Texture/Common/default_white.bmp", //bunny
};

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


OpenGLES_008_Blend::OpenGLES_008_Blend(String name)
    : OpenGLESWindow(name)
	, typeVertex(F_MeshVertex_Pos3Color4Normal3Tex2)
	, pShaderVertex(nullptr)
	, pShaderFragment(nullptr)
	, pShaderVertex_Outline(nullptr)
	, pShaderFragment_Outline(nullptr)
{
    this->cfg_isDepthStencil = true;
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
	this->cfg_isRotate = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->shaderVertex_Path = getShaderPathRelative("standard_mesh_transparent.vert.spv");
    this->shaderFragment_Path = getShaderPathRelative("standard_mesh_transparent.frag.spv");
	this->shaderVertex_Path_Outline = getShaderPathRelative("standard_mesh_outline.vert.spv");
    this->shaderFragment_Path_Outline = getShaderPathRelative("standard_mesh_outline.frag.spv");

    this->cfg_cameraPos = FVector3(0.0f, 3.0f, -4.0f);
}

void OpenGLES_008_Blend::createCamera()
{
    OpenGLESWindow::createCamera();
    
    cameraReset();
}

void OpenGLES_008_Blend::loadModel_Custom()
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
            String msg = "*********************** OpenGLES_008_Blend::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        pModelObject->poMatWorld = FMath::FromTRS(g_tranformModels[i * 3 + 0],
                                                  g_tranformModels[i * 3 + 1],
                                                  g_tranformModels[i * 3 + 2]); 

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "*********************** OpenGLES_008_Blend::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
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
bool OpenGLES_008_Blend::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load 
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        F_LogError("*********************** OpenGLES_008_Blend::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
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

    F_LogInfo("OpenGLES_008_Blend::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
              pModelObject->nameModel.c_str(),
              (int)pModelObject->vertices.size(), 
              (int)pModelObject->indices.size());

    //2> createBufferVertexIndex or createBufferVertex
	if (pModelObject->poIndexBuffer_Size > 0 &&
		pModelObject->poIndexBuffer_Data != nullptr)
	{
		pModelObject->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex("VertexIndex-" + pModelObject->nameModel,
																						 this->typeVertex,
																						 pModelObject->poVertexBuffer_Size, 
																						 (uint8*)pModelObject->poVertexBuffer_Data, 
																						 false,
																						 pModelObject->poIndexBuffer_Size, 
																						 (uint8*)pModelObject->poIndexBuffer_Data, 
																						 false);
		if (pModelObject->pBufferVertexIndex == nullptr)
		{
			F_LogError("*********************** OpenGLES_008_Blend::loadModel_VertexIndex: create buffer vertex index failed: [%s] !", pModelObject->nameModel.c_str());
			return false;
		}
	}
	else
	{
		pModelObject->pBufferVertex = Base::GetWindowPtr()->createBufferVertex("Vertex-" + pModelObject->nameModel,
																			   this->typeVertex,
																			   pModelObject->poVertexBuffer_Size, 
																			   (uint8*)pModelObject->poVertexBuffer_Data, 
																			   false);
		if (pModelObject->pBufferVertex == nullptr)
		{
			F_LogError("*********************** OpenGLES_008_Blend::loadModel_VertexIndex: create buffer vertex failed: [%s] !", pModelObject->nameModel.c_str());
			return false;
		}
	}

    return true;
}
bool OpenGLES_008_Blend::loadModel_Texture(ModelObject* pModelObject)
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
			F_LogError("*********************** OpenGLES_008_Blend::loadModel_Texture: Failed to create texture, path: [%s] !", pModelObject->pathTexture.c_str());
			F_DELETE(this->poTexture)
			return false;
		}
        F_LogInfo("OpenGLES_008_Blend::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void OpenGLES_008_Blend::createCustomCB()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//1> Object
        ObjectConstants objectConstants;
        objectConstants.g_MatWorld = pModelObject->poMatWorld;
        pModelObject->objectCBs.push_back(objectConstants);

		String nameBuffer = "ObjectConstants-" + FUtilString::SaveInt((int)i);
		pModelObject->poBufferUniform = createBufferUniform(nameBuffer,
															DescriptorSet_ObjectConstants,
															GL_DYNAMIC_DRAW,
															sizeof(ObjectConstants) * pModelObject->objectCBs.size(),
															(uint8*)(pModelObject->objectCBs.data()),
															false);
		if (!pModelObject->poBufferUniform)
		{
			String msg = "*********************** OpenGLES_008_Blend::createCustomCB: create buffer uniform: [" + nameBuffer + "] failed !";
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg);
		}

		//2> Material
		MaterialConstants materialConstants;
		materialConstants.lighting.y = g_TransparentAlpha[i];
		pModelObject->materialCBs.push_back(materialConstants);

		nameBuffer = "MaterialConstants-" + FUtilString::SaveInt((int)i);
		pModelObject->poBufferUniform_Material = createBufferUniform(nameBuffer,
																	 DescriptorSet_MaterialConstants,
																	 GL_DYNAMIC_DRAW,
																	 sizeof(MaterialConstants) * pModelObject->materialCBs.size(),
																	 (uint8*)(pModelObject->materialCBs.data()),
																	 false);
		if (!pModelObject->poBufferUniform_Material)
		{
			String msg = "*********************** OpenGLES_008_Blend::createCustomCB: create buffer uniform: [" + nameBuffer + "] failed !";
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg);
		}

		//3> Outline
        OutlineObjectConstants objectConstants_Outline;
        objectConstants_Outline.g_MatWorld = pModelObject->poMatWorld;
        objectConstants_Outline.g_OutlineColor = g_OutlineColor[i];
        objectConstants_Outline.g_OutlineWidth = g_OutlineWidth[i];
        pModelObject->objectCBs_Outline.push_back(objectConstants_Outline);

		nameBuffer = "OutlineObjectConstants-" + FUtilString::SaveInt((int)i);
		pModelObject->poBufferUniform_Outline = createBufferUniform(nameBuffer,
																	DescriptorSet_OutlineObjectConstants,
                                                            		GL_DYNAMIC_DRAW,
																	sizeof(OutlineObjectConstants) * pModelObject->objectCBs_Outline.size(),
																	(uint8*)(pModelObject->objectCBs_Outline.data()),
																	false);
		if (!pModelObject->poBufferUniform_Outline)
		{
			String msg = "*********************** OpenGLES_008_Blend::createCustomCB: create buffer uniform: [" + nameBuffer + "] failed !";
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg);
		}
    }
}

void OpenGLES_008_Blend::createGraphicsPipeline_Custom()
{
    String namePathBase;

    //1> Shader
    String nameVertexShader;
	FUtilString::SplitFileName(this->shaderVertex_Path, nameVertexShader, namePathBase);
	this->pShaderVertex = createShader(nameVertexShader, this->shaderVertex_Path, F_Shader_Vertex);
	if (this->pShaderVertex == nullptr)
	{
		String msg = "*********************** OpenGLES_008_Blend::createGraphicsPipeline_Custom: Failed to create shader vertex: " + this->shaderVertex_Path;
		F_LogError("%s", msg.c_str());
		throw std::runtime_error(msg);
	}
	
	String nameFragmentShader;
	FUtilString::SplitFileName(this->shaderFragment_Path, nameFragmentShader, namePathBase);
	this->pShaderFragment = createShader(nameFragmentShader, this->shaderFragment_Path, F_Shader_Fragment);
	if (this->pShaderFragment == nullptr)
	{
		String msg = "*********************** OpenGLES_008_Blend::createGraphicsPipeline_Custom: Failed to create shader fragment: " + this->shaderFragment_Path;
		F_LogError("%s", msg.c_str());
		throw std::runtime_error(msg);
	}

	FUtilString::SplitFileName(this->shaderVertex_Path_Outline, nameVertexShader, namePathBase);
	this->pShaderVertex_Outline = createShader(nameVertexShader, this->shaderVertex_Path_Outline, F_Shader_Vertex);
	if (this->pShaderVertex_Outline == nullptr)
	{
		String msg = "*********************** OpenGLES_008_Blend::createGraphicsPipeline_Custom: Failed to create shader vertex: " + this->shaderVertex_Path_Outline;
		F_LogError("%s", msg.c_str());
		throw std::runtime_error(msg);
	}

	FUtilString::SplitFileName(this->shaderFragment_Path_Outline, nameFragmentShader, namePathBase);
	this->pShaderFragment_Outline = createShader(nameFragmentShader, this->shaderFragment_Path_Outline, F_Shader_Fragment);
	if (this->pShaderFragment_Outline == nullptr)
	{
		String msg = "*********************** OpenGLES_008_Blend::createGraphicsPipeline_Custom: Failed to create shader fragment: " + this->shaderFragment_Path_Outline;
		F_LogError("%s", msg.c_str());
		throw std::runtime_error(msg);
	}

    //2> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        //poStatePipelineGraphics_Stencil
		String namePipelineGraphics_Stencil = "PipelineGraphics-Stencil-" + pModelObject->nameModel;
        pModelObject->poStatePipelineGraphics_Stencil = createStatePipelineGraphics(namePipelineGraphics_Stencil,
																					this->pShaderVertex,
																					nullptr,
																					nullptr,
																					nullptr,
																					this->pShaderFragment,
																					this->typeVertex,
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
																					GL_ALWAYS,
																					GL_REPLACE,
																					GL_REPLACE,
																					GL_REPLACE,
																					1,
																					0xFF,
																					true,
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
        if (pModelObject->poStatePipelineGraphics_Stencil == nullptr)
        {
            String msg = "*********************** OpenGLES_008_Blend::createGraphicsPipeline_Custom: Failed to create pipeline stencil !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

		//2> poStatePipelineGraphics_Outline
		String namePipelineGraphics_Outline = "PipelineGraphics-Outline-" + pModelObject->nameModel;
        pModelObject->poStatePipelineGraphics_Outline = createStatePipelineGraphics(namePipelineGraphics_Outline,
																					this->pShaderVertex_Outline,
																					nullptr,
																					nullptr,
																					nullptr,
																					this->pShaderFragment_Outline,
																					this->typeVertex,
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
            String msg = "*********************** OpenGLES_008_Blend::createGraphicsPipeline_Custom: Failed to create pipeline outline !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

    }

}

void OpenGLES_008_Blend::createDescriptorSets_Custom()
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

void OpenGLES_008_Blend::updateCBs_Custom()
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
                objectCB.g_MatWorld = glm::rotate(pModelObject->poMatWorld, 
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->poMatWorld;
            }

			OutlineObjectConstants& objectCB_Outline = pModelObject->objectCBs_Outline[j];
			objectCB_Outline.g_MatWorld = objectCB.g_MatWorld;

			MaterialConstants& materialCB = pModelObject->materialCBs[j];
            materialCB.lighting.y = pModelObject->alpha;
        }
		pModelObject->poBufferUniform->UpdateBuffer(sizeof(ObjectConstants) * count_object,
                                                 	(uint8*)pModelObject->objectCBs.data(),
													 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		pModelObject->poBufferUniform_Outline->UpdateBuffer(sizeof(OutlineObjectConstants) * count_object,
                                                 			(uint8*)pModelObject->objectCBs_Outline.data(),
															 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		pModelObject->poBufferUniform_Material->UpdateBuffer(sizeof(MaterialConstants) * count_object,
                                                 			 (uint8*)pModelObject->materialCBs.data(),
															  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
}



bool OpenGLES_008_Blend::beginRenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("OpenGLES_008_Blend", &windowOpened, 0);
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
void OpenGLES_008_Blend::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
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
				String nameAlpha = "Alpha - " + pModelObject->nameModel;
				ImGui::DragFloat(nameAlpha.c_str(), &pModelObject->alpha, 0.01f, 0.0f, 1.0f);

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->poVertexCount, (int)pModelObject->poIndexCount);

                String nameWorld = "Model World - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    const FMatrix4& mat4World = pModelObject->objectCBs[0].g_MatWorld;
                    String nameTable = FUtilString::SaveInt(i) + " - split_model_world";
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

				String nameObject = "Model Object - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameObject.c_str()))
				{
					OutlineObjectConstants& obj = pModelObject->objectCBs_Outline[0];
					
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

void OpenGLES_008_Blend::endRenderImgui()
{
    OpenGLESWindow::endRenderImgui();

}

void OpenGLES_008_Blend::drawMeshDefault_Custom()
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
			drawInstance(pModelObject->poStatePipelineGraphics_Stencil->poTypePrimitive, 0, pModelObject->poVertexCount, 1);
		}
		else if (pModelObject->pBufferVertexIndex != nullptr)
		{
			pModelObject->pBufferVertexIndex->BindVertexArray();
			drawIndexedInstance(pModelObject->poStatePipelineGraphics_Stencil->poTypePrimitive, pModelObject->poIndexCount, GL_UNSIGNED_INT, 0, 1);
		}
		else
		{	
			F_Assert(false && "OpenGLES_008_Blend::drawMeshDefault_Custom")
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
				drawInstance(pModelObject->poStatePipelineGraphics_Outline->poTypePrimitive, 0, pModelObject->poVertexCount, 1);
			}
			else if (pModelObject->pBufferVertexIndex != nullptr)
			{
				pModelObject->pBufferVertexIndex->BindVertexArray();
				drawIndexedInstance(pModelObject->poStatePipelineGraphics_Outline->poTypePrimitive, pModelObject->poIndexCount, GL_UNSIGNED_INT, 0, 1);
			}
			else
			{	
				F_Assert(false && "OpenGLES_008_Blend::drawMeshDefault_Custom")
			}
		}
    }
}

void OpenGLES_008_Blend::cleanupCustom()
{
	F_DELETE(this->pShaderVertex)
    F_DELETE(this->pShaderFragment)
	F_DELETE(this->pShaderVertex_Outline)
    F_DELETE(this->pShaderFragment_Outline)

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