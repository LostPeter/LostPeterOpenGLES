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

#ifndef _GLES_STATE_PIPELINE_GRAPHICS_H_
#define _GLES_STATE_PIPELINE_GRAPHICS_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESStatePipelineGraphics : public Base
    {
    public:
        GLESStatePipelineGraphics(const String& nameState);
        virtual ~GLESStatePipelineGraphics();

    public:
		String nameDescriptorSetLayout;
		StringVector* poDescriptorSetLayoutNames;

		FMeshVertexType poTypeVertex;

		GLenum poTypePrimitive;
        bool poIsCull;
        GLenum poTypeFrontFace;
        GLenum poTypeCulling;
        GLenum poTypePolygonMode;
		
		bool poDepthEnabled;
		GLenum poDepthFuncCompare;
		bool poDepthTestEnabled;
		bool poDepthWriteEnabled;

		bool poStencilEnabled;
		GLenum poStencil_CompareFunction;
		GLenum poStencil_StencilFailureOp;
		GLenum poStencil_DepthFailureOp;
		GLenum poStencil_DepthStencilPassOp;
		uint32_t poStencil_Ref;
		uint32_t poStencil_Mask;

		bool poBlendEnabled;
		GLenum poBlendColorFactorSrc; 
		GLenum poBlendColorFactorDst;
		GLenum poBlendColorOp;
		GLenum poBlendAlphaFactorSrc; 
		GLenum poBlendAlphaFactorDst;
		GLenum poBlendAlphaOp;

		GLboolean poColorWriteMask_Red;
		GLboolean poColorWriteMask_Green;
		GLboolean poColorWriteMask_Blue;
		GLboolean poColorWriteMask_Alpha;


		GLESShaderProgram* poShaderProgram;
		bool isDeleteShaderProgram;

		Uint2UintMap mapBindIndex2UniformBlockIndex;
		GLESBufferUniformPtrIDMap mapBufferUniform;
		GLESTexturePtrIDMap mapTexture;


    public:
        void Destroy();
		bool Init(GLESShaderProgram* pShaderProgram,
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
				  GLboolean colorWriteMask_Alpha);
		bool Init(GLESShader* pShaderVertex,
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
				  GLboolean colorWriteMask_Alpha);

		virtual void CleanupSwapChain();

	public:
		GLESShaderProgram* GetShaderProgram() const { return this->poShaderProgram; }

		uint32 GetUniformBlockIndex(const String& name);
        void BindUniformBlockBinding(uint32 nUniformBlockIndex, uint32 nUniformBlockBinding);
			
	public:
		void BindBufferUniform(GLESBufferUniform* pBufferUnifom, uint32 nBindingIndex);
		void BindTexture(GLESTexture* pTexture, uint32 nBindingIndex);

    public:
		void BindState();
		void UnBindState();
		void BindShader();
		void BindBufferUniforms();
		void BindTextures();

	protected:
		void bindStateDepth(bool depthEnable);
		void bindStateStencil(bool stencilEnable);
		void bindStateBlend(bool blendEnable);
        
    };

}; //LostPeterOpenGLES

#endif