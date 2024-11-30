/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-15
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _GL_PIPELINE_GRAPHICS_COPY_BLIT_TO_FRAME_H_
#define _GL_PIPELINE_GRAPHICS_COPY_BLIT_TO_FRAME_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport GLPipelineGraphicsCopyBlitToFrame : public Base
    {
    public:
        GLPipelineGraphicsCopyBlitToFrame(const String& namePipelineGraphics);
        virtual ~GLPipelineGraphicsCopyBlitToFrame();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;

        CopyBlitObjectConstants objectCB_CopyBlit;
        GLBufferUniform* pBuffer_CopyBlit;

        GLShaderProgram* pShaderProgram;
        Mesh* pMeshBlit;

    public:
        void Destroy();
        virtual bool Init(GLShader* pShaderVertex,
                          GLShader* pShaderFrag,
                          Mesh* pMesh,
                          const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames);

    protected:
        void destroyBufferCopyBlitObject();
        
        bool createBufferCopyBlitObject();

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSets();
        virtual void UpdateBuffer();
        virtual void UpdateBuffer(const CopyBlitObjectConstants& object);
    };  

}; //LostPeterOpenGLES

#endif