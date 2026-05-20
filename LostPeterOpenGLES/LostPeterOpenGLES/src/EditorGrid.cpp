/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/EditorGrid.h"
#include "../include/OpenGLESWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/GLESBufferUniform.h"
#include "../include/GLESStatePipelineGraphics.h"
#include "../include/GLESShader.h"

namespace LostPeterOpenGLES
{
	const String EditorGrid::s_strNameShader_Grid_Vert = "vert_editor_grid";
    const String EditorGrid::s_strNameShader_Grid_Frag = "frag_editor_grid";
    EditorGrid::EditorGrid()
        : EditorBase("EditorGrid")

		, pBufferUniform(nullptr)
        , isNeedUpdate(true)

    {

    }
    EditorGrid::~EditorGrid()
    {
		Destroy();
    }
	void EditorGrid::Destroy()
    {
        destroyInternal();
        destroyMeshes();
    }
    void EditorGrid::Init()
    {
        EditorBase::Init();
    }
    void EditorGrid::UpdateCBs()
    {
        if (!IsNeedUpdate())
            return;
        SetIsNeedUpdate(false);

		this->pBufferUniform->UpdateBuffer(sizeof(GridObjectConstants), 
										   (uint8*)(&this->gridObjectCB),
										   GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
	void EditorGrid::BindUniformPass()
	{
		GLESBufferUniform* pBufferUniform_Pass = Base::GetWindowPtr()->GetUniform_PassCB();
		this->pPipelineGraphics->BindBufferUniform(pBufferUniform_Pass, pBufferUniform_Pass->GetBindingIndex());
	}
    void EditorGrid::initConfigs()
    {
        //1> Mesh
        {
            MeshInfo* pMI = new MeshInfo("EditorGrid",
                                         "",
                                         F_Mesh_Geometry,
                                         F_MeshVertex_Pos3Color4Tex2,
                                         F_MeshGeometry_EntityGrid,
                                         nullptr,
                                         false,
                                         true,
                                         FMath::RotateX(90.0f));
            this->aMeshInfos.push_back(pMI);
        }
        //2> Shader
        {
            //Vert
            ShaderModuleInfo siVert;
            siVert.nameShader = s_strNameShader_Grid_Vert;
            siVert.nameShaderType = "vert";
            siVert.pathShader = Base::GetWindowPtr()->getShaderPathRelative("editor_grid.vert.spv");
            this->aShaderModuleInfos.push_back(siVert);
            //Frag
            ShaderModuleInfo siFrag;
            siFrag.nameShader = s_strNameShader_Grid_Frag;
            siFrag.nameShaderType = "frag";
            siFrag.pathShader = Base::GetWindowPtr()->getShaderPathRelative("editor_grid.frag.spv");
            this->aShaderModuleInfos.push_back(siFrag);
        }
        //3> BufferUniform
        {
            this->gridObjectCB.g_MatWorld = FMath::FromTRS(FVector3(0.0f, 0.0f, 0.0f),
                                                           FVector3(0.0f, 0.0f, 0.0f),
                                                           FVector3(1024.0f, 1024.0f, 1024.0f));
        }
        //4> DescriptorSetLayout
        {
            this->nameDescriptorSetLayout = "PassConstants-GridObjectConstants";
            this->pDescriptorSetLayout = new DescriptorSetLayout();
			this->pDescriptorSetLayout->Init(this->nameDescriptorSetLayout);
        }
    }
    void EditorGrid::initBufferUniforms()
    {
		this->pBufferUniform = Base::GetWindowPtr()->createBufferUniform("EditorGrid-GridObjectConstants",
																		 DescriptorSet_GridObjectConstants,
																		 GL_DYNAMIC_DRAW,
																		 sizeof(GridObjectConstants),
																		 (uint8*)(&this->gridObjectCB),
																		 false);
        SetIsNeedUpdate(true);
    }
    void EditorGrid::initPipelineGraphics()
    {
		OpenGLESWindow* pWindow = Base::GetWindowPtr();

        String namePipelineGraphics = "PipelineGraphics-" + GetName();
		GLESShader* pShaderVertex = GetShader(s_strNameShader_Grid_Vert); 
		F_Assert("EditorGrid::initPipelineGraphics: Shader Vetex" && pShaderVertex)
		GLESShader* pShaderFragment = GetShader(s_strNameShader_Grid_Frag);
		F_Assert("EditorGrid::initPipelineGraphics: Shader Fragment" && pShaderFragment)
		this->pPipelineGraphics = pWindow->createStatePipelineGraphics(namePipelineGraphics,
                                                                       this->pDescriptorSetLayout,
																	   pShaderVertex,
																	   nullptr,
																	   nullptr,
																	   nullptr,
																	   pShaderFragment,
																	   F_MeshVertex_Pos3Color4Tex2,
																	   GL_TRIANGLES,
																	   true,
																	   GL_CW,
																	   GL_BACK,
																	   0,
																	   false,
																	   GL_LEQUAL,
																	   false,
																	   false,
																	   false,
																	   GL_LEQUAL,
																	   GL_KEEP,
																	   GL_KEEP,
																	   GL_KEEP,
																	   0,
																	   0,
																	   true,
																	   GL_SRC_ALPHA,
																	   GL_ONE_MINUS_SRC_ALPHA,
																	   GL_FUNC_ADD,
																	   GL_ONE,
																	   GL_ZERO,
																	   GL_FUNC_ADD,
																	   true,
																	   true,
																	   true,
																	   true);
		if (this->pPipelineGraphics == nullptr)
		{
			String msg = "*********************** EditorGrid::initPipelineGraphics: Failed to create pipeline graphics for [EditorGrid] !";
			F_LogError("%s", msg.c_str());
			throw std::runtime_error(msg.c_str());
		}			
		F_LogInfo("EditorGrid::initPipelineGraphics: [EditorGrid] Create pipeline graphics success !");

		updateDescriptorSets_Graphics();
    }
    void EditorGrid::updateDescriptorSets_Graphics()
    {   
		uint32_t count_names = (uint32_t)this->pDescriptorSetLayout->aLayouts.size();
		for (uint32_t i = 0; i < count_names; i++)
		{
			const String& nameDescriptorSet = this->pDescriptorSetLayout->aLayouts[i];
			
			if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants)) //PassConstants
			{
				uint32 nUniformBlockIndex = this->pPipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
				uint32 nBindingIndex = (uint32)DescriptorSet_PassConstants;
				this->pPipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
			}
			else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_GridObjectConstants)) //GridObjectConstants
			{
				uint32 nUniformBlockIndex = this->pPipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
				uint32 nBindingIndex = (uint32)DescriptorSet_GridObjectConstants;
				this->pPipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
				this->pPipelineGraphics->BindBufferUniform(this->pBufferUniform, nBindingIndex);
			}
			else
			{
				String msg = "*********************** EditorGrid::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
				F_LogError("%s", msg.c_str());
				throw std::runtime_error(msg.c_str());
			}
		}
    }
    void EditorGrid::destroyBufferUniforms()
    {
		F_DELETE(this->pBufferUniform)
    }
    void EditorGrid::destroyPipelineGraphics()
    {
        EditorBase::destroyPipelineGraphics();
    }

    void EditorGrid::CleanupSwapChain()
    {   
        EditorBase::CleanupSwapChain();

    }
    void EditorGrid::RecreateSwapChain()
    {
        EditorBase::RecreateSwapChain();

    }

}; //LostPeterOpenGLES