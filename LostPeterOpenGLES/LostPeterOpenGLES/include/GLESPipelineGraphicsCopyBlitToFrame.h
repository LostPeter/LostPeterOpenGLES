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

#ifndef _GLES_PIPELINE_GRAPHICS_COPY_BLIT_TO_FRAME_H_
#define _GLES_PIPELINE_GRAPHICS_COPY_BLIT_TO_FRAME_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESPipelineGraphicsCopyBlitToFrame : public Base
    {
    public:
        GLESPipelineGraphicsCopyBlitToFrame(const String& namePipelineGraphics);
        virtual ~GLESPipelineGraphicsCopyBlitToFrame();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;

        CopyBlitObjectConstants objectCB_CopyBlit;
        GLESBufferUniform* pBuffer_CopyBlit;

        GLESShaderProgram* pShaderProgram;
        Mesh* pMeshBlit;

    public:
        void Destroy();
        virtual bool Init(GLESShader* pShaderVertex,
                          GLESShader* pShaderFrag,
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