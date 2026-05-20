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

#include "../include/EditorBase.h"
#include "../include/OpenGLESWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/GLESBufferVertexIndex.h"
#include "../include/GLESStatePipelineGraphics.h"
#include "../include/GLESShader.h"

namespace LostPeterOpenGLES
{
    EditorBase::EditorBase(const String& nameEditor)
        : Base(nameEditor)

		//DescriptorSetLayouts
        , nameDescriptorSetLayout("")
        , pDescriptorSetLayout(nullptr)

        //GLESStatePipelineGraphics
        , pPipelineGraphics(nullptr)
    {

    }
    EditorBase::~EditorBase()
    {
       
    }

	GLESShader* EditorBase::GetShader(const String& nameShaer)
	{
		GLESShaderPtrMap::iterator itFind = this->mapShaders.find(nameShaer);
		if (itFind == this->mapShaders.end())
			return nullptr;
		return itFind->second;
	}

    void EditorBase::destroyInternal()
	{
		destroyPipelineGraphics();
        destroyPipelineLayout();
        destroyDescriptorSetLayout();
        destroyShaders();
        destroyBufferUniforms();
	}

	void EditorBase::Init()
    {
        //0> initConfigs
        initConfigs();

        //1> initMeshes
        initMeshes();

        //2> initShader
        initShaders();

        //3> initBufferUniforms
        initBufferUniforms();

        //4> initDescriptorSetLayout
        initDescriptorSetLayout();

        //5> initPipelineLayout
        initPipelineLayout();

        //6> initPipelineGraphics
        initPipelineGraphics();
    }
    void EditorBase::initMeshes()
    {
        if (this->aMeshInfos.size() <= 0)
            return;
        Base::GetWindowPtr()->CreateMeshes(this->aMeshInfos, this->aMeshes, this->mapMeshes);
    }
    void EditorBase::initShaders()
    {   
        if (this->aShaderModuleInfos.size() <= 0)
            return;
        Base::GetWindowPtr()->CreateShaders(this->aShaderModuleInfos, this->aShaders, this->mapShaders);
    }
    void EditorBase::initDescriptorSetLayout()
    {
		
    }
    void EditorBase::initPipelineLayout()
    {
        
    }
    void EditorBase::destroyMeshes()
    {
        size_t count = this->aMeshes.size();
        for (size_t i = 0; i < count; i++)
        {
            Mesh* pMesh = this->aMeshes[i];
            F_DELETE(pMesh)
        }
        this->aMeshes.clear();
        this->mapMeshes.clear();

        count = this->aMeshInfos.size();
        for (size_t i = 0; i < count; i++)
        {
            MeshInfo* pMI = this->aMeshInfos[i];
            F_DELETE(pMI)
        }
        this->aMeshInfos.clear();
    }
    void EditorBase::destroyShaders()
    {
        size_t count = this->aShaders.size();
        for (size_t i = 0; i < count; i++)
        {
            GLESShader* pShader = this->aShaders[i];
			F_DELETE(pShader)
        }
        this->aShaders.clear();
        this->mapShaders.clear();
    }
    void EditorBase::destroyPipelineGraphics()
    {
        F_DELETE(this->pPipelineGraphics)
    }
    void EditorBase::destroyPipelineLayout()
    {
        
    }
    void EditorBase::destroyDescriptorSetLayout()
    {
		F_DELETE(this->pDescriptorSetLayout)
    }   

    void EditorBase::Draw()
    {
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		this->pPipelineGraphics->BindState();
		this->pPipelineGraphics->BindShader();
		BindUniformPass();
		this->pPipelineGraphics->BindBufferUniforms();

        size_t count_mesh = this->aMeshes.size();
        for (size_t i = 0; i < count_mesh; i++)
        {
            Mesh* pMesh = this->aMeshes[i];
            size_t count_mesh_sub = pMesh->aMeshSubs.size();
            for (size_t j = 0; j < count_mesh_sub; j++)
            {
                MeshSub* pMeshSub = pMesh->aMeshSubs[j];
				
				pMeshSub->pBufferVertexIndex->BindVertexArray();
				pWindow->drawIndexed(GL_TRIANGLES, (int)pMeshSub->poIndexCount, GL_UNSIGNED_INT, 0);
            }
        }
		this->pPipelineGraphics->UnBindState();
    }   

    void EditorBase::CleanupSwapChain()
    {
        
    }
    void EditorBase::RecreateSwapChain()
    {
        
    }

}; //LostPeterOpenGLES