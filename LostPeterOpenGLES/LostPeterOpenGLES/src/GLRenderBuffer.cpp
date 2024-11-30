/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-13
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/GLRenderBuffer.h"
#include "../include/OpenGLWindow.h"

namespace LostPeterOpenGLES
{
    GLRenderBuffer::GLRenderBuffer(const String& nameRenderBuffer)
        : Base(nameRenderBuffer)
        , nWidth(0)
        , nHeight(0)
        , nFormat(0)
        , nAttachment(0)
        , nRenderbuffertarget(0)

        , nRenderBufferID(0)
    {

    }
    GLRenderBuffer::~GLRenderBuffer()
    {
        Destroy();
    }

    void GLRenderBuffer::Destroy()
    {
        if (this->nRenderBufferID > 0)
        {
            Base::GetWindowPtr()->destroyGLRenderBuffer(this->nRenderBufferID);
        }
        this->nRenderBufferID = 0;
    }

    bool GLRenderBuffer::Init(int width,
                              int height,
                              GLenum format,
                              GLenum attachment, 
                              GLenum renderbuffertarget)
    {
        this->nWidth = width;
        this->nHeight = height;
        this->nFormat = format;
        this->nAttachment = attachment;
        this->nRenderbuffertarget = renderbuffertarget;

        if (!Base::GetWindowPtr()->createGLRenderBuffer(GetName(),
                                                        width,
                                                        height,
                                                        format,
                                                        this->nRenderBufferID))
        {
            return false;
        }

        return true;
    }

    void GLRenderBuffer::BindRenderBuffer()
    {
        if (this->nRenderBufferID > 0)
        {
            Base::GetWindowPtr()->bindGLRenderBuffer(this->nRenderBufferID);
        }
    }

}; //LostPeterOpenGLES