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

#include "../include/GLBufferUniform.h"
#include "../include/OpenGLWindow.h"

namespace LostPeterOpenGLES
{
    GLBufferUniform::GLBufferUniform(const String& nameBuffer)
        : GLBuffer(nameBuffer)

        , nBufferSize(0)
        , pBuffer(nullptr)
        , bIsDelete(true)
        , eUsage(GL_DYNAMIC_DRAW)

        , nBindingIndex(0)
        , nBufferUniformID(0)
    {

    }
    GLBufferUniform::~GLBufferUniform()
    {
        Destroy();
    }
        void GLBufferUniform::destroyBuffer()
        {
            if (this->bIsDelete && this->pBuffer != nullptr)
            {
                F_DELETE_T(this->pBuffer)
            }
            this->pBuffer = nullptr;
        }

    void GLBufferUniform::Destroy()
    {
        destroyBuffer();
        if (this->nBufferUniformID > 0)
        {
            Base::GetWindowPtr()->destroyGLBufferUniform(this->nBufferUniformID);
        }
        this->nBufferUniformID = 0;
    }

    bool GLBufferUniform::Init(uint32 bindingIndex,
                               GLenum usage,
                               size_t bufSize, 
                               uint8* pBuf,
                               bool isDelete)
    {
        this->nBufferSize = bufSize;
        this->pBuffer = pBuf;
        this->bIsDelete = isDelete;
        this->eUsage = usage;

        this->nBindingIndex = bindingIndex;

        if (!Base::GetWindowPtr()->createGLBufferUniform(GetName(),
                                                         bindingIndex,
                                                         usage,
                                                         bufSize, 
                                                         pBuf, 
                                                         this->nBufferUniformID))
        {
            F_LogError("*********************** GLBufferUniform::Init: Failed to create buffer uniform: [%s] !", GetName().c_str());
            return false;
        }
        return true;
    }

    void GLBufferUniform::Update(size_t offset,
                                 size_t bufSize, 
                                 uint8* pBuf)
    {
        Base::GetWindowPtr()->updateGLBufferUniform(offset,
                                                    bufSize, 
                                                    pBuf, 
                                                    this->nBufferUniformID);
    }                        

    void* GLBufferUniform::MapBuffer(GLenum access)
    {
        return Base::GetWindowPtr()->mapGLBuffer(this->nBufferUniformID, GL_UNIFORM_BUFFER, access);
    }
    void GLBufferUniform::UnMapBuffer()
    {
        Base::GetWindowPtr()->unMapGLBuffer(GL_UNIFORM_BUFFER);
    }
    void GLBufferUniform::UpdateBuffer(size_t bufSize, 
                                       uint8* pBuf,
                                       GLenum access)
    {
        void* pData = MapBuffer(access);
        F_Assert(pData && "GLBufferUniform::UpdateBuffer")
        memcpy(pData, pBuf, bufSize);
        UnMapBuffer();
    }

    void GLBufferUniform::BindBufferUniform()
    {
        Base::GetWindowPtr()->bindGLBufferUniform(this->nBufferUniformID);
    }
    void GLBufferUniform::BindBufferUniformBlockIndex(uint32 nUniformBlockIndex)
    {
        Base::GetWindowPtr()->bindGLBufferUniformBlockIndex(this->nBufferUniformID, nUniformBlockIndex);
    }

}; //LostPeterOpenGLES