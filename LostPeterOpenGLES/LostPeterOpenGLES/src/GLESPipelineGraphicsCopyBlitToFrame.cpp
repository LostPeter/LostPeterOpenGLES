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

#include "../include/GLESPipelineGraphicsCopyBlitToFrame.h"
#include "../include/OpenGLESWindow.h"
#include "../include/GLESBufferUniform.h"
#include "../include/GLESShader.h"
#include "../include/GLESShaderProgram.h"
#include "../include/Mesh.h"

namespace LostPeterOpenGLES
{
    GLESPipelineGraphicsCopyBlitToFrame::GLESPipelineGraphicsCopyBlitToFrame(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)

        , pBuffer_CopyBlit(nullptr)
        
        , pShaderProgram(nullptr)
        , pMeshBlit(nullptr)
    {

    }

    GLESPipelineGraphicsCopyBlitToFrame::~GLESPipelineGraphicsCopyBlitToFrame()
    {
        Destroy();
    }

    void GLESPipelineGraphicsCopyBlitToFrame::Destroy()
    {
        CleanupSwapChain();
        destroyBufferCopyBlitObject();
    }
        void GLESPipelineGraphicsCopyBlitToFrame::destroyBufferCopyBlitObject()
        {
            F_DELETE(this->pBuffer_CopyBlit)
        }


    bool GLESPipelineGraphicsCopyBlitToFrame::Init(GLESShader* pShaderVertex,
                                                 GLESShader* pShaderFrag,
                                                 Mesh* pMesh,
                                                 const String& descriptorSetLayout,
                                                 StringVector* pDescriptorSetLayoutNames)
    {
        this->pMeshBlit = pMesh;
        this->nameDescriptorSetLayout = descriptorSetLayout;
        this->poDescriptorSetLayoutNames = pDescriptorSetLayoutNames;

        //1> GLESBufferUniform
        if (this->pBuffer_CopyBlit == nullptr)
        {
            if (!createBufferCopyBlitObject())
            {
                F_LogError("*********************** GLESPipelineGraphicsCopyBlitToFrame::Init: createBufferCopyBlitObject failed !");
                return false;
            }
        }

        //2> GLESShaderProgram
        {
            String nameShaderProgram = "ShaderProgram-" + GetName();
            this->pShaderProgram = Base::GetWindowPtr()->createShaderProgram(nameShaderProgram,
                                                                             pShaderVertex,
                                                                             nullptr,
                                                                             nullptr,
                                                                             nullptr,
                                                                             pShaderFrag);
            if (this->pShaderProgram == nullptr)
            {
                F_LogError("*********************** GLESPipelineGraphicsCopyBlitToFrame::Init: createShaderProgram failed, name: [%s] !", nameShaderProgram.c_str());
                return false;
            }
        }

        //3> Binding
        UpdateDescriptorSets();

        return true;
    }
        bool GLESPipelineGraphicsCopyBlitToFrame::createBufferCopyBlitObject()
        {
            this->objectCB_CopyBlit.offsetX = 0.0f;
            this->objectCB_CopyBlit.offsetY = 0.0f;
            this->objectCB_CopyBlit.scaleX = 2.0f;
            this->objectCB_CopyBlit.scaleY = 2.0f;
            Base::GetWindowPtr()->createBufferUniform("CopyBlitObjectConstants-" + this->name, 
                                                      DescriptorSet_ObjectCopyBlit,
                                                      GL_DYNAMIC_DRAW,
                                                      sizeof(CopyBlitObjectConstants), 
                                                      (uint8*)(&this->objectCB_CopyBlit), 
                                                      false);
            return true;
        }

    void GLESPipelineGraphicsCopyBlitToFrame::CleanupSwapChain()
    {
        this->poDescriptorSetLayoutNames = nullptr;
        F_DELETE(this->pShaderProgram)
    }  

    void GLESPipelineGraphicsCopyBlitToFrame::UpdateDescriptorSets()
    {
        String nameCopyBlit = (*this->poDescriptorSetLayoutNames)[0];
        this->pShaderProgram->SetUniformBlockBinding(nameCopyBlit, DescriptorSet_ObjectCopyBlit);
    }

    void GLESPipelineGraphicsCopyBlitToFrame::UpdateBuffer()
    {
        if (this->pBuffer_CopyBlit != nullptr)
        {
            this->pBuffer_CopyBlit->Update(0, sizeof(CopyBlitObjectConstants), (uint8*)(&this->objectCB_CopyBlit));
        }
    }
    void GLESPipelineGraphicsCopyBlitToFrame::UpdateBuffer(const CopyBlitObjectConstants& object)
    {
        this->objectCB_CopyBlit = object;
        UpdateBuffer();
    }

}; //LostPeterOpenGLES