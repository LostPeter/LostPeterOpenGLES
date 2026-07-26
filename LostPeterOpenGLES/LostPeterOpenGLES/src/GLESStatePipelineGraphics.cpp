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

#include "../include/GLESStatePipelineGraphics.h"
#include "../include/OpenGLESWindow.h"
#include "../include/GLESShaderProgram.h"
#include "../include/GLESShader.h"
#include "../include/GLESBufferUniform.h"
#include "../include/GLESTexture.h"

namespace LostPeterOpenGLES
{
	std::map<uint, String> GLESStatePipelineGraphics::s_mapIndex2SamplerName;
    GLESStatePipelineGraphics::GLESStatePipelineGraphics(const String& nameState)
        : Base(nameState)

        , poDescriptorSetLayout(nullptr)

		, poTypeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)

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
    GLESStatePipelineGraphics::~GLESStatePipelineGraphics()
    {
		Destroy();
    }

	void GLESStatePipelineGraphics::Destroy()
	{
		CleanupSwapChain();
	}

	bool GLESStatePipelineGraphics::Init(DescriptorSetLayout* pDescriptorSetLayout,
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
		this->poShaderProgram = pShaderProgram;
		this->isDeleteShaderProgram = deleteShaderProgram;

		this->poDescriptorSetLayout = pDescriptorSetLayout;
		this->poTypeVertex = typeVertex;
		
		this->poTypePrimitive = typePrimitive;
        this->poIsCull = isCull;
        this->poTypeFrontFace = typeFrontFace;
        this->poTypeCulling = typeCulling;
        this->poTypePolygonMode = typePolygonMode;

		this->poDepthEnabled = depthEnabled;
		this->poDepthFuncCompare = depthFuncCompare;
		this->poDepthTestEnabled = depthTestEnabled;
		this->poDepthWriteEnabled = depthWriteEnabled;

		this->poStencilEnabled = stencilEnabled;
		this->poStencil_CompareFunction = stencil_CompareFunction;
		this->poStencil_StencilFailureOp = stencil_StencilFailureOp;
		this->poStencil_DepthFailureOp = stencil_DepthFailureOp;
		this->poStencil_DepthStencilPassOp = stencil_DepthStencilPassOp;
		this->poStencil_Ref = stencil_Ref;
		this->poStencil_Mask = stencil_Mask;

		this->poBlendEnabled = blendEnabled;
		this->poBlendColorFactorSrc = blendColorFactorSrc; 
		this->poBlendColorFactorDst = blendColorFactorDst;
		this->poBlendColorOp = blendColorOp;
		this->poBlendAlphaFactorSrc = blendAlphaFactorSrc; 
		this->poBlendAlphaFactorDst = blendAlphaFactorDst;
		this->poBlendAlphaOp = blendAlphaOp;

		this->poColorWriteMask_Red = colorWriteMask_Red;
		this->poColorWriteMask_Green = colorWriteMask_Green;
		this->poColorWriteMask_Blue = colorWriteMask_Blue;
		this->poColorWriteMask_Alpha = colorWriteMask_Alpha;

		return true;
	}

	bool GLESStatePipelineGraphics::Init(DescriptorSetLayout* pDescriptorSetLayout,
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
		String nameShaderProgram = GetName();
		GLESShaderProgram* pShaderProgram = Base::GetWindowPtr()->createShaderProgram(nameShaderProgram,
																				      pShaderVertex,
																				      pShaderTessellationControl,
																				      pShaderTessellationEvaluation,
																				      pShaderGeometry,
																				      pShaderFragment);
		if (pShaderProgram == nullptr)
		{
			String msg = "*********************** GLESStatePipelineGraphics::Init: Failed to create shader program: " + nameShaderProgram;
			F_LogError("%s", msg.c_str());
			return false;
		}

		return Init(pDescriptorSetLayout,
					pShaderProgram,
					true,
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
					colorWriteMask_Alpha);
	}

	void GLESStatePipelineGraphics::CleanupSwapChain()
	{
		this->poDescriptorSetLayout = nullptr;
		
		if (this->isDeleteShaderProgram)
		{
			F_DELETE(this->poShaderProgram)
		}
		this->poShaderProgram = nullptr;

		this->mapBindIndex2UniformBlockIndex.clear();
		this->mapBufferUniform.clear();
		this->mapTextureVS.clear();
		this->mapTextureFS.clear();
		this->mapTextureTESC.clear();
		this->mapTextureTESE.clear();
		this->mapTextureGS.clear();
		this->mapTextureCS.clear();
	}

	uint32 GLESStatePipelineGraphics::GetUniformBlockIndex(const String& name)
	{
		return this->poShaderProgram->GetUniformBlockIndex(name);
	}
	void GLESStatePipelineGraphics::BindUniformBlockBinding(uint32 nUniformBlockIndex, uint32 nUniformBlockBinding)
	{
		this->mapBindIndex2UniformBlockIndex[nUniformBlockBinding] = nUniformBlockIndex;
		this->poShaderProgram->SetUniformBlockBinding(nUniformBlockIndex, nUniformBlockBinding);
	}

	GLESStatePipelineGraphics* GLESStatePipelineGraphics::NewStatePipelineGraphics(const String& nameStatePipelineGraphics)
	{
		GLESStatePipelineGraphics* pStatePipelineGraphics = Base::GetWindowPtr()->createStatePipelineGraphics(nameStatePipelineGraphics,
                                                                                                              this->poDescriptorSetLayout,
																											  this->poShaderProgram,
																											  false,
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
        if (pStatePipelineGraphics == nullptr)
        {
            String msg = "*********************** GLESStatePipelineGraphics::NewStatePipelineGraphics: Failed to create pipeline graphics: " + nameStatePipelineGraphics;
            F_LogError("%s", msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        return pStatePipelineGraphics;
	}

	void GLESStatePipelineGraphics::BindBufferUniform(GLESBufferUniform* pBufferUnifom, uint32 nBindingIndex)
	{
		this->mapBufferUniform[nBindingIndex] = pBufferUnifom;
	}
	void GLESStatePipelineGraphics::BindTextureVS(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureVS[nBindingIndex] = pTexture;
	}
	void GLESStatePipelineGraphics::BindTextureFS(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureFS[nBindingIndex] = pTexture;
	}
	void GLESStatePipelineGraphics::BindTextureTESC(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureTESC[nBindingIndex] = pTexture;
	}
	void GLESStatePipelineGraphics::BindTextureTESE(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureTESE[nBindingIndex] = pTexture;
	}
	void GLESStatePipelineGraphics::BindTextureGS(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureGS[nBindingIndex] = pTexture;
	}
	void GLESStatePipelineGraphics::BindTextureCS(GLESTexture* pTexture, uint32 nBindingIndex)
	{
		this->mapTextureCS[nBindingIndex] = pTexture;
	}

	void GLESStatePipelineGraphics::BindState()
	{
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		pWindow->setFrontFace(this->poTypeFrontFace);
		pWindow->setEnable(GL_CULL_FACE, this->poIsCull);
		pWindow->setCullFace(this->poTypeCulling);
		//pWindow->setPolygonMode(GL_FRONT_AND_BACK, this->poTypePolygonMode);

		bindStateDepth(this->poDepthEnabled);
		bindStateStencil(this->poStencilEnabled);
		bindStateBlend(this->poBlendEnabled);
		pWindow->setColorMask(this->poColorWriteMask_Red, this->poColorWriteMask_Green, this->poColorWriteMask_Blue, this->poColorWriteMask_Alpha);
	}
	void GLESStatePipelineGraphics::UnBindState()
	{
		bindStateDepth(false);
		bindStateStencil(false);
		bindStateBlend(false);
		bindTextures(false);
	}
	void GLESStatePipelineGraphics::bindStateDepth(bool depthEnable)
	{
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		pWindow->setEnable(GL_DEPTH_TEST, depthEnable);
		if (depthEnable)
		{
			if (!this->poDepthTestEnabled)
				pWindow->setDepthFunc(GL_ALWAYS);
			else
				pWindow->setDepthFunc(this->poDepthFuncCompare);
			pWindow->setDepthWrite(this->poDepthWriteEnabled);
		}
	}
	void GLESStatePipelineGraphics::bindStateStencil(bool stencilEnable)
	{
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		pWindow->setEnable(GL_STENCIL_TEST, stencilEnable);
		if (stencilEnable)
		{
			pWindow->setStencilFunc(this->poStencil_CompareFunction, this->poStencil_Ref, this->poStencil_Mask);
			pWindow->setStencilOp(this->poStencil_StencilFailureOp, this->poStencil_DepthFailureOp, this->poStencil_DepthStencilPassOp);
			pWindow->setStencilMask(this->poStencil_Mask);
		}
	}
	void GLESStatePipelineGraphics::bindStateBlend(bool blendEnable)
	{
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		pWindow->setEnable(GL_BLEND, blendEnable);
		if (blendEnable)
		{
			pWindow->setBlendFunc(this->poBlendColorFactorSrc, this->poBlendColorFactorDst);
		}
	}

	void GLESStatePipelineGraphics::BindShader()
	{	
		this->poShaderProgram->BindProgram();
	}

	void GLESStatePipelineGraphics::BindBufferUniforms()
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
	void GLESStatePipelineGraphics::BindTextures()
	{
		bindTextures(true);
	}
	void GLESStatePipelineGraphics::bindTextures(bool enable)
	{
		//VS
		if (this->mapTextureVS.size() > 0)
			bindTexture(this->mapTextureVS, enable);
		//FS
		if (this->mapTextureFS.size() > 0)
			bindTexture(this->mapTextureFS, enable);
		//TESC
		if (this->mapTextureTESC.size() > 0)
			bindTexture(this->mapTextureTESC, enable);
		//TESE
		if (this->mapTextureTESE.size() > 0)
			bindTexture(this->mapTextureTESE, enable);
		//GS
		if (this->mapTextureGS.size() > 0)
			bindTexture(this->mapTextureGS, enable);
		//CS
		if (this->mapTextureCS.size() > 0)
			bindTexture(this->mapTextureCS, enable);
	}
	void GLESStatePipelineGraphics::bindTexture(GLESTexturePtrIDMap& mapTexture, bool enable)
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

}; //LostPeterOpenGLES