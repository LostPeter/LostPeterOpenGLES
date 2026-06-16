/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2025-05-03
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/GLESStatePipelineCompute.h"
#include "../include/OpenGLESWindow.h"
#include "../include/GLESShaderProgram.h"
#include "../include/GLESShader.h"
#include "../include/GLESBufferUniform.h"
#include "../include/GLESTexture.h"

namespace LostPeterOpenGLES
{
    std::map<uint, String> GLESStatePipelineCompute::s_mapIndex2SamplerName;
    GLESStatePipelineCompute::GLESStatePipelineCompute(const String& nameState)
        : Base(nameState)

        , poDescriptorSetLayout(nullptr)

		, poShaderProgram(nullptr)
		, isDeleteShaderProgram(true)
    {
        if (s_mapIndex2SamplerName.size() <= 0)
		{
			s_mapIndex2SamplerName[0] = "texSampler0";
			s_mapIndex2SamplerName[1] = "texSampler1";
			s_mapIndex2SamplerName[2] = "texSampler2";
			s_mapIndex2SamplerName[3] = "texSampler3";
			s_mapIndex2SamplerName[4] = "texSampler4";
			s_mapIndex2SamplerName[5] = "texSampler5";
			s_mapIndex2SamplerName[6] = "texSampler6";
			s_mapIndex2SamplerName[7] = "texSampler7";
			s_mapIndex2SamplerName[8] = "texSampler8";
			s_mapIndex2SamplerName[9] = "texSampler9";
			s_mapIndex2SamplerName[10] = "texSampler10";
			s_mapIndex2SamplerName[11] = "texSampler11";
			s_mapIndex2SamplerName[12] = "texSampler12";
			s_mapIndex2SamplerName[13] = "texSampler13";
			s_mapIndex2SamplerName[14] = "texSampler14";
			s_mapIndex2SamplerName[15] = "texSampler15";
		}
    }
    GLESStatePipelineCompute::~GLESStatePipelineCompute()
    {
        Destroy();
    }

    void GLESStatePipelineCompute::Destroy()
	{
		CleanupSwapChain();
	}
	
	bool GLESStatePipelineCompute::Init(DescriptorSetLayout* pDescriptorSetLayout,
									    GLESShaderProgram* pShaderProgram,
									    bool deleteShaderProgram)
	{
		this->poShaderProgram = pShaderProgram;
		this->isDeleteShaderProgram = deleteShaderProgram;

		this->poDescriptorSetLayout = pDescriptorSetLayout;
		
		return true;
	}
	bool GLESStatePipelineCompute::Init(DescriptorSetLayout* pDescriptorSetLayout,
									    GLESShader* pShaderCompute)
	{
		String nameShaderProgram = GetName();
		GLESShaderProgram* pShaderProgram = Base::GetWindowPtr()->createShaderProgram(nameShaderProgram,
																				      pShaderCompute);
		if (pShaderProgram == nullptr)
		{
			String msg = "*********************** GLESStatePipelineCompute::Init: Failed to create shader program: " + nameShaderProgram;
            F_LogError("%s", msg.c_str());
			return false;
		}

		return Init(pDescriptorSetLayout,
					pShaderProgram,
					true);
	}

	void GLESStatePipelineCompute::CleanupSwapChain()
	{
		this->poDescriptorSetLayout = nullptr;
		
		if (this->isDeleteShaderProgram)
		{
			F_DELETE(this->poShaderProgram)
		}
		this->poShaderProgram = nullptr;

		this->mapBindIndex2UniformBlockIndex.clear();
		this->mapBufferUniform.clear();
		this->mapTextureCS.clear();
		this->mapTextureImageCS.clear();
	}

	uint32 GLESStatePipelineCompute::GetUniformBlockIndex(const String& name)
	{
		return this->poShaderProgram->GetUniformBlockIndex(name);
	}
	void GLESStatePipelineCompute::BindUniformBlockBinding(uint32 nUniformBlockIndex, uint32 nUniformBlockBinding)
	{
		this->mapBindIndex2UniformBlockIndex[nUniformBlockBinding] = nUniformBlockIndex;
		this->poShaderProgram->SetUniformBlockBinding(nUniformBlockIndex, nUniformBlockBinding);
	}

	void GLESStatePipelineCompute::BindBufferUniform(GLESBufferUniform* pBufferUnifom, uint32 nBindingIndex)
	{
		this->mapBufferUniform[nBindingIndex] = pBufferUnifom;
	}

	void GLESStatePipelineCompute::BindTextureCS(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureCS[nBindingIndex] = pTexture;
	}
	void GLESStatePipelineCompute::BindTextureImageCS(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureImageCS[nBindingIndex] = pTexture;
	}


	void GLESStatePipelineCompute::BindState()
	{

	}
	void GLESStatePipelineCompute::UnBindState()
	{	

		bindTextures(false);
	}
	void GLESStatePipelineCompute::BindShader()
	{
		this->poShaderProgram->BindProgram();
	}
	void GLESStatePipelineCompute::BindBufferUniforms()
	{
		size_t count = this->mapBindIndex2UniformBlockIndex.size();
		if (count > 0)
		{
			for (Uint2UintMap::iterator it = this->mapBindIndex2UniformBlockIndex.begin();
				 it != this->mapBindIndex2UniformBlockIndex.end(); ++it)
			{
				uint32 nBindIndex = it->first;
				GLESBufferUniformPtrIDMap::iterator itFind = this->mapBufferUniform.find(nBindIndex);
				if (itFind != this->mapBufferUniform.end())
				{
					itFind->second->BindBufferUniformBlockIndex();
				}
			}
		}
	}
	void GLESStatePipelineCompute::BindTextures()
	{
		bindTextures(true);
		bindTextureImages(true);
	}

	void GLESStatePipelineCompute::bindTextures(bool enable)
	{
		//CS
		if (this->mapTextureCS.size() > 0)
			bindTexture(this->mapTextureCS, enable);
	}
	void GLESStatePipelineCompute::bindTexture(GLESTexturePtrIDMap& mapTexture, bool enable)
	{
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		for (GLESTexturePtrIDMap::iterator it = mapTexture.begin();
			 it != mapTexture.end(); ++it)
		{
			GLESTexture* pTexture = it->second;
			uint nBindingIndex = it->first;
			pTexture->BindTexture(nBindingIndex, enable);
			pWindow->setUniform1i(this->poShaderProgram->nShaderProgramID, s_mapIndex2SamplerName[nBindingIndex], (int)nBindingIndex);
		}
	}


	void GLESStatePipelineCompute::bindTextureImages(bool enable)
	{
		//Image CS
		if (this->mapTextureImageCS.size() > 0)
			bindTextureImage(this->mapTextureImageCS, enable);
	}
	void GLESStatePipelineCompute::bindTextureImage(GLESTexturePtrIDMap& mapTexture, bool enable)
	{
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		for (GLESTexturePtrIDMap::iterator it = mapTexture.begin();
			 it != mapTexture.end(); ++it)
		{
			GLESTexture* pTexture = it->second;
			uint nBindingIndex = it->first;
			pTexture->BindTextureImage(nBindingIndex, GL_WRITE_ONLY, enable);
		}
	}

}; //LostPeterOpenGLES