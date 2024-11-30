/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-03
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/GLFrameBuffer.h"
#include "../include/OpenGLWindow.h"
#include "../include/GLTexture.h"
#include "../include/GLRenderBuffer.h"

namespace LostPeterOpenGLES
{
    UintID2UintTypeMap GLFrameBuffer::s_mapID2Types;
    GLFrameBuffer::GLFrameBuffer(const String& nameFrameBuffer)
        : Base(nameFrameBuffer)

        , nWidth(0)
        , nHeight(0)

        , pDepthStencil(nullptr)
        , bIsDeleteColors(false)
        , bIsDeleteDepthStencil(false)

        , nFrameBufferID(0)
    {
        if (s_mapID2Types.empty())
        {
            s_mapID2Types[0] = GL_COLOR_ATTACHMENT0;
            s_mapID2Types[1] = GL_COLOR_ATTACHMENT1;
            s_mapID2Types[2] = GL_COLOR_ATTACHMENT2;
            s_mapID2Types[3] = GL_COLOR_ATTACHMENT3;
            s_mapID2Types[4] = GL_COLOR_ATTACHMENT4;
            s_mapID2Types[5] = GL_COLOR_ATTACHMENT5;
            s_mapID2Types[6] = GL_COLOR_ATTACHMENT6;
            s_mapID2Types[7] = GL_COLOR_ATTACHMENT7;

            s_mapID2Types[8] = GL_COLOR_ATTACHMENT8;
            s_mapID2Types[9] = GL_COLOR_ATTACHMENT9;
            s_mapID2Types[10] = GL_COLOR_ATTACHMENT10;
            s_mapID2Types[11] = GL_COLOR_ATTACHMENT11;
            s_mapID2Types[12] = GL_COLOR_ATTACHMENT12;
            s_mapID2Types[13] = GL_COLOR_ATTACHMENT13;
            s_mapID2Types[14] = GL_COLOR_ATTACHMENT14;
            s_mapID2Types[15] = GL_COLOR_ATTACHMENT15;
        }
    }
    GLFrameBuffer::~GLFrameBuffer()
    {
        Destroy();
    }

    void GLFrameBuffer::Destroy()
    {
        if (this->bIsDeleteColors)
        {
            size_t count = this->aColorTexture.size();
            for (size_t i = 0; i < count; i++)
            {
                F_DELETE(this->aColorTexture[i])
            }
        }
        this->aColorTexture.clear();
        this->mapType2IDs.clear();
        if (this->bIsDeleteDepthStencil)
        {
            F_DELETE(this->pDepthStencil)
        }

        if (this->nFrameBufferID > 0)
        {
            Base::GetWindowPtr()->destroyGLFrameBuffer(this->nFrameBufferID);
        }
        this->nFrameBufferID = 0;
    }

    bool GLFrameBuffer::Init(int width,
                             int height,
                             const GLTexturePtrVector& aColors,
                             GLRenderBuffer* pDS,
                             bool isDeleteColors /*= false*/,
                             bool isDeleteDepthStencil /*= false*/)
    {
        this->nWidth = width;
        this->nHeight = height;
        this->aColorTexture.clear();
        this->mapType2IDs.clear();
        size_t count = aColors.size();
        for (size_t i = 0; i < count; i++)
        {
            GLTexture* pGLTexture = aColors[i];
            this->aColorTexture.push_back(pGLTexture);
            uint32 type = s_mapID2Types[(uint32)i];
            this->mapType2IDs[type] = pGLTexture->nTextureID;
        }
        this->pDepthStencil = pDS;
        this->bIsDeleteColors = isDeleteColors;
        this->bIsDeleteDepthStencil = isDeleteDepthStencil;

        if (!Base::GetWindowPtr()->createGLFrameBuffer(GetName(),
                                                       this->mapType2IDs,
                                                       pDS->nRenderBufferID,
                                                       this->nFrameBufferID))
        {
            return false;
        }
        return true;
    }

    void GLFrameBuffer::BindFrameBuffer()
    {
        if (this->nFrameBufferID > 0)
        {
            Base::GetWindowPtr()->bindGLFrameBuffer(this->nFrameBufferID);
        }
    }

    GLTexture* GLFrameBuffer::GetColorTexture(int index)
    {
        if (index < 0 || index > (int)this->aColorTexture.size())
            return nullptr;
        return this->aColorTexture[index];
    }

}; //LostPeterOpenGLES