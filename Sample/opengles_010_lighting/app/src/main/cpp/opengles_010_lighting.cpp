/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-03-18
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_010_lighting.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


const String c_strVert = ".vert.spv";
const String c_strFrag = ".frag.spv";

static const int g_ShaderCount = 2;
static const char* g_pathShaderModules[2 * g_ShaderCount] = 
{
   "standard_mesh_opaque_tex2d_lit.vert.spv", "standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit
   "standard_mesh_transparent_lit.vert.spv", "standard_mesh_transparent_lit.frag.spv", //standard_mesh_transparent_lit
};

static const int g_CountLen = 3;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "plane",            "Mesh/Common/plane.fbx",                     "Texture/Common/terrain.png", //plane
    "viking_room",      "Mesh/Model/viking_room/viking_room.obj",    "Texture/Model/viking_room/viking_room.png", //viking_room
    "bunny",            "Mesh/Model/bunny/bunny.obj",                "Texture/Common/default_white.bmp", //bunny
};

static const char* g_pathModelShaderModules[g_CountLen] = 
{
    "standard_mesh_opaque_tex2d_lit", //plane 
    "standard_mesh_transparent_lit", //viking_room
    "standard_mesh_opaque_tex2d_lit", //bunny 
};

static float g_instanceGap = 4.0f;

static int g_instanceExtCount[] =
{
    0, //plane
    5, //viking_room
    5, //bunny
};

static FVector3 g_tranformModels[3 * g_CountLen] = 
{
	FVector3(   0,   0,    0),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //plane
    FVector3(   0,   0,    5),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //viking_room
    FVector3(   0,   0,    0),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //bunny
};

static FMatrix4 g_tranformLocalModels[g_CountLen] = 
{
    FMath::ms_mat4Unit, //plane
    FMath::RotateX(-90.0f), //viking_room
    FMath::ms_mat4Unit, //bunny
};

static bool g_isTranformLocalModels[g_CountLen] = 
{
    false, //plane
    true, //viking_room
    false, //bunny
};

static bool g_isFlipYModels[g_CountLen] = 
{
    true, //plane
    false, //viking_room
    false, //bunny
};

static bool g_isTransparentModels[g_CountLen] = 
{
    false, //plane
    true, //viking_room
    false, //bunny
};

static bool g_isRotateModels[] =
{
    false, //plane
    true, //viking_room
    true, //bunny
};



OpenGLES_010_Lighting::OpenGLES_010_Lighting(String name)
    : OpenGLESWindow(name)
{
    this->cfg_isDepthStencil = true;
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->poTypeVertex = F_MeshVertex_Pos3Color4Normal3Tex2;

    this->cfg_cameraPos = FVector3(0.0f, 15.0f, -20.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
}

void OpenGLES_010_Lighting::createCamera()
{
    OpenGLESWindow::createCamera();
    
    cameraReset();
}

void OpenGLES_010_Lighting::loadModel_Custom()
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

        pModelObject->isRotate = g_isRotateModels[i];
        pModelObject->countInstanceExt = g_instanceExtCount[i];
        pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;
        
        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTransformLocal, g_tranformLocalModels[i]))
        {
            String msg = "*********************** OpenGLES_010_Lighting::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "*********************** OpenGLES_010_Lighting::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
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
bool OpenGLES_010_Lighting::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load 
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        F_LogError("*********************** OpenGLES_010_Lighting::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
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

    F_LogInfo("OpenGLES_010_Lighting::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
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
			F_LogError("*********************** OpenGLES_010_Lighting::loadModel_VertexIndex: create buffer vertex index failed: [%s] !", pModelObject->nameModel.c_str());
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
			F_LogError("*********************** OpenGLES_010_Lighting::loadModel_VertexIndex: create buffer vertex failed: [%s] !", pModelObject->nameModel.c_str());
			return false;
		}
	}

    return true;
}
bool OpenGLES_010_Lighting::loadModel_Texture(ModelObject* pModelObject)
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
			F_LogError("*********************** OpenGLES_010_Lighting::loadModel_Texture: Failed to create texture, path: [%s] !", pModelObject->pathTexture.c_str());
			F_DELETE(this->poTexture)
			return false;
		}
        F_LogInfo("OpenGLES_010_Lighting::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void OpenGLES_010_Lighting::createCustomCB()
{
	rebuildInstanceCBs(true);
}
void OpenGLES_010_Lighting::rebuildInstanceCBs(bool isCreateBuffer)
{
	size_t maxCount = MAX_MATERIAL_COUNT;
	size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//1> Object
		pModelObject->instanceMatWorld.resize(maxCount);
        pModelObject->objectCBs.resize(maxCount);
        pModelObject->materialCBs.resize(maxCount);
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = FMath::FromTRS(g_tranformModels[i * 3 + 0] + FVector3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                        g_tranformModels[i * 3 + 1],
                                                        g_tranformModels[i * 3 + 2]);
            pModelObject->objectCBs[j] = objectConstants;
            pModelObject->instanceMatWorld[j] = objectConstants.g_MatWorld;

            //MaterialConstants
            MaterialConstants materialConstants;
            materialConstants.factorAmbient = FMath::RandomColor(false);
            materialConstants.factorDiffuse = FMath::RandomColor(false);
            materialConstants.factorSpecular = FMath::RandomColor(false);
            materialConstants.lighting.x = FMath::RandF(10.0f, 100.0f);
            materialConstants.lighting.y = FMath::RandF(0.2f, 0.9f);
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
				String msg = "*********************** OpenGLES_010_Lighting::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
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
                String msg = "*********************** OpenGLES_010_Lighting::rebuildInstanceCBs: create buffer uniform: [" + nameBuffer + "] failed !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }
		}
    }
}

void OpenGLES_010_Lighting::createCustomBeforePipeline()
{
    //1> Shader
    createShaderModules();
}  
void OpenGLES_010_Lighting::createGraphicsPipeline_Custom()
{
    String namePathBase;

    //1> Shader


    //2> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		String pathVertShaderBase = getShaderPathRelative(g_pathModelShaderModules[i] + c_strVert);
        String pathFragShaderBase = getShaderPathRelative(g_pathModelShaderModules[i] + c_strFrag);
        GLESShader* pShaderVertex = findShaderModule(pathVertShaderBase);
        GLESShader* pShaderFragment = findShaderModule(pathFragShaderBase);

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
		String namePipelineGraphics_Stencil = "PipelineGraphics-Stencil-" + pModelObject->nameModel;
        pModelObject->poStatePipelineGraphics = createStatePipelineGraphics(namePipelineGraphics_Stencil,
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
        if (pModelObject->poStatePipelineGraphics == nullptr)
        {
            String msg = "*********************** OpenGLES_010_Lighting::createGraphicsPipeline_Custom: Failed to create pipeline stencil !";
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
}

void OpenGLES_010_Lighting::destroyShaderModules()
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
void OpenGLES_010_Lighting::createShaderModules()
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
            String msg = "*********************** OpenGLES_010_Lighting::createShaderModules: Failed to create shader vertex: " + pathVert;
			F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShaderVertex);
        this->m_mapGLESShaderModules[pathVert] = pShaderVertex;
        F_LogInfo("OpenGLES_010_Lighting::createShaderModules: create shader [%s] success !", pathVert.c_str());

        //frag
        FUtilString::SplitFileName(pathFrag, nameFragmentShader, namePathBase);
        GLESShader* pShaderFragment = createShader(nameFragmentShader, pathFrag, F_Shader_Fragment);
        if (pShaderFragment == nullptr)
        {
            String msg = "*********************** OpenGLES_010_Lighting::createShaderModules: Failed to create shader fragment: " + pathFrag;
			F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aGLESShaderModules.push_back(pShaderFragment);
        this->m_mapGLESShaderModules[pathFrag] = pShaderFragment;
        F_LogInfo("OpenGLES_010_Lighting::createShaderModules: create shader [%s] success !", pathFrag.c_str());
    }
}
GLESShader* OpenGLES_010_Lighting::findShaderModule(const String& pathShaderModule)
{
    GLESShaderPtrMap::iterator itFind = this->m_mapGLESShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapGLESShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void OpenGLES_010_Lighting::createDescriptorSets_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

		//(0) PassConstants
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants);
			uint32 nBindingIndex = (uint32)DescriptorSet_PassConstants;

			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics->GetUniformBlockIndex(nameDS);
			pModelObject->poStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
		}
		//(1) ObjectConstants
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_ObjectConstants);
			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics->GetUniformBlockIndex(nameDS);
			uint32 nBindingIndex = (uint32)DescriptorSet_ObjectConstants;
			pModelObject->poStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pModelObject->poStatePipelineGraphics->BindBufferUniform(pModelObject->poBufferUniform, nBindingIndex);
		}
		//(2) MaterialConstants
		{
			const String& nameDS = Util_GetDescriptorSetTypeName(DescriptorSet_MaterialConstants);
			uint32 nUniformBlockIndex = pModelObject->poStatePipelineGraphics->GetUniformBlockIndex(nameDS);
			uint32 nBindingIndex = (uint32)DescriptorSet_MaterialConstants;
			pModelObject->poStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			pModelObject->poStatePipelineGraphics->BindBufferUniform(pModelObject->poBufferUniform_Material, nBindingIndex);
		}
		//(3) InstanceConstants
		{
			
		}
		//(4) Image
		{
			pModelObject->poStatePipelineGraphics->BindTexture(pModelObject->poTexture, 0);
		}
    }
}

void OpenGLES_010_Lighting::updateCBs_Custom()
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
        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
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
		pModelObject->poBufferUniform->UpdateBuffer(sizeof(ObjectConstants) * count_object,
                                                 	(uint8*)pModelObject->objectCBs.data(),
													GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		//MaterialConstants
        pModelObject->poBufferUniform_Material->UpdateBuffer(sizeof(MaterialConstants) * count_object,
                                                             (uint8*)pModelObject->materialCBs.data(),
                                                             GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
}



bool OpenGLES_010_Lighting::beginRenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("OpenGLES_010_Lighting", &windowOpened, 0);
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
void OpenGLES_010_Lighting::modelConfig()
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
				String nameIsTransparent = "Is Transparent - " + pModelObject->nameModel;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                
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
                        ObjectConstants& obj = pModelObject->objectCBs[j];
                        MaterialConstants& mat = pModelObject->materialCBs[j];

                        String nameModelInstance = nameModel + " - " + FUtilString::SaveInt(j);
                        if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                        {
                            //ObjectConstants
                            String nameObject = FUtilString::SaveInt(j) + " - Object - " + pModelObject->nameModel;
                            if (ImGui::CollapsingHeader(nameObject.c_str()))
                            {
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
                            }
                            
                            //MaterialConstants
                            String nameMaterial = FUtilString::SaveInt(j) + " - Material - " + pModelObject->nameModel;
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
                                ImGui::Spacing();
                            }
                        }
                    }
                }
            }
        }
    }
}

void OpenGLES_010_Lighting::endRenderImgui()
{
    OpenGLESWindow::endRenderImgui();

}

void OpenGLES_010_Lighting::drawMeshDefault_Custom()
{   
	bool isSetWireFrame = false;
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
		if (pModelObject->pBufferVertex != nullptr)
		{
			pModelObject->pBufferVertex->BindVertexArray();
			drawInstance(pModelObject->poStatePipelineGraphics->poTypePrimitive, 0, pModelObject->poVertexCount, pModelObject->countInstance);
		}
		else if (pModelObject->pBufferVertexIndex != nullptr)
		{
			pModelObject->pBufferVertexIndex->BindVertexArray();
			drawIndexedInstance(pModelObject->poStatePipelineGraphics->poTypePrimitive, pModelObject->poIndexCount, GL_UNSIGNED_INT, 0, pModelObject->countInstance);
		}
		else
		{	
			F_Assert(false && "OpenGLES_010_Lighting::drawMeshDefault_Custom")
		}
    }
}

void OpenGLES_010_Lighting::cleanupCustom()
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