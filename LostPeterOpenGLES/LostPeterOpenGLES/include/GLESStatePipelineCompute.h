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

#ifndef _GLES_STATE_PIPELINE_COMPUTE_H_
#define _GLES_STATE_PIPELINE_COMPUTE_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESStatePipelineCompute : public Base
    {
    public:
        GLESStatePipelineCompute(const String& nameState);
        virtual ~GLESStatePipelineCompute();

    public:
		static std::map<uint, String> s_mapIndex2SamplerName;
        
    public:
        DescriptorSetLayout* poDescriptorSetLayout;

		GLESShaderProgram* poShaderProgram;
		bool isDeleteShaderProgram;

		Uint2UintMap mapBindIndex2UniformBlockIndex;
		GLESBufferUniformPtrIDMap mapBufferUniform;
		GLESTexturePtrIDMap mapTextureCS;
		GLESTexturePtrIDMap mapTextureImageCS;

    public:
        virtual void Destroy();
		bool Init(DescriptorSetLayout* pDescriptorSetLayout,
				  GLESShaderProgram* pShaderProgram,
				  bool deleteShaderProgram);
		bool Init(DescriptorSetLayout* pDescriptorSetLayout,
				  GLESShader* pShaderCompute);

		virtual void CleanupSwapChain();
					
	public:
		GLESShaderProgram* GetShaderProgram() const { return this->poShaderProgram; }

		uint32 GetUniformBlockIndex(const String& name);
        void BindUniformBlockBinding(uint32 nUniformBlockIndex, uint32 nUniformBlockBinding);

	public:
		void BindBufferUniform(GLESBufferUniform* pBufferUnifom, uint32 nBindingIndex);
		void BindTextureCS(GLESTexture* pTexture, uint32 nBindingIndex);
		void BindTextureImageCS(GLESTexture* pTexture, uint32 nBindingIndex);

	public:
		void BindState();
		void UnBindState();
		void BindShader();
		void BindBufferUniforms();
		void BindTextures();

	protected:
		void bindTextures(bool enable);
		void bindTexture(GLESTexturePtrIDMap& mapTexture, bool enable);

		void bindTextureImages(bool enable);
		void bindTextureImage(GLESTexturePtrIDMap& mapTexture, bool enable);
    };

}; //LostPeterOpenGLES

#endif