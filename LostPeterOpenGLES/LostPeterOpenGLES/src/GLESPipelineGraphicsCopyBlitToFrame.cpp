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
#include "../include/GLESStatePipelineGraphics.h"
#include "../include/Mesh.h"

namespace LostPeterOpenGLES
{
    GLESPipelineGraphicsCopyBlitToFrame::GLESPipelineGraphicsCopyBlitToFrame(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)

        , pBuffer_CopyBlit(nullptr)
        
        , pStatePipelineGraphics(nullptr)
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
                                                   GLESShader* pShaderFragment,
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

        //2> GLESStatePipelineGraphics
        {
			OpenGLESWindow* pWindow = Base::GetWindowPtr();
            String nameStatePipelineGraphics = "StatePipelineGraphics-" + GetName();
			this->pStatePipelineGraphics = pWindow->createStatePipelineGraphics(nameStatePipelineGraphics,
																				pShaderVertex,
																				nullptr,
																				nullptr,
																				nullptr,
																				pShaderFragment,
																				F_MeshVertex_Pos2Color4Tex2,
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
																				false,
																				GL_ONE,
																				GL_ZERO,
																				GL_FUNC_ADD,
																				GL_ONE,
																				GL_ZERO,
																				GL_FUNC_ADD,
																				true,
																				true,
																				true,
																				true);
			if (this->pStatePipelineGraphics == nullptr)
			{
				F_LogError("*********************** GLESPipelineGraphicsCopyBlitToFrame::Init: StatePipelineGraphics failed, name: [%s] !", nameStatePipelineGraphics.c_str());
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
            this->pBuffer_CopyBlit = Base::GetWindowPtr()->createBufferUniform("CopyBlitObjectConstants-" + this->name, 
																			   DescriptorSet_CopyBlitObjectConstants,
																			   GL_DYNAMIC_DRAW,
																			   sizeof(CopyBlitObjectConstants), 
																			   (uint8*)(&this->objectCB_CopyBlit), 
																			   false);
            return true;
        }

    void GLESPipelineGraphicsCopyBlitToFrame::CleanupSwapChain()
    {
        this->poDescriptorSetLayoutNames = nullptr;
        F_DELETE(this->pStatePipelineGraphics)
    }  

    void GLESPipelineGraphicsCopyBlitToFrame::UpdateDescriptorSets()
    {
        String nameCopyBlit = (*this->poDescriptorSetLayoutNames)[0];
		uint32 nUniformBlockIndex = this->pStatePipelineGraphics->GetUniformBlockIndex(nameCopyBlit);
        this->pStatePipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, DescriptorSet_CopyBlitObjectConstants);
		this->pStatePipelineGraphics->BindBufferUniform(this->pBuffer_CopyBlit, this->pBuffer_CopyBlit->GetBindingIndex());
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