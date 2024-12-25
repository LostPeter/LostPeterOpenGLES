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

#include "../include/GLESBufferUniform.h"
#include "../include/OpenGLESWindow.h"

namespace LostPeterOpenGLES
{
    GLESBufferUniform::GLESBufferUniform(const String& nameBuffer)
        : GLESBuffer(nameBuffer)

        , nBufferSize(0)
        , pBuffer(nullptr)
        , bIsDelete(true)
        , eUsage(GL_DYNAMIC_DRAW)

        , nBindingIndex(0)
        , nBufferUniformID(0)
        , nUniformBlockIndex(0)
    {

    }
    GLESBufferUniform::~GLESBufferUniform()
    {
        Destroy();
    }
        void GLESBufferUniform::destroyBuffer()
        {
            if (this->bIsDelete && this->pBuffer != nullptr)
            {
                F_DELETE_T(this->pBuffer)
            }
            this->pBuffer = nullptr;
        }

    void GLESBufferUniform::Destroy()
    {
        destroyBuffer();
        if (this->nBufferUniformID > 0)
        {
            Base::GetWindowPtr()->destroyGLBufferUniform(this->nBufferUniformID);
        }
        this->nBufferUniformID = 0;

        this->nBindingIndex = 0;
        this->nUniformBlockIndex = 0;
    }

    bool GLESBufferUniform::Init(uint32 bindingIndex,
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
            F_LogError("*********************** GLESBufferUniform::Init: Failed to create buffer uniform: [%s] !", GetName().c_str());
            return false;
        }
        return true;
    }

    void GLESBufferUniform::Update(size_t offset,
                                   size_t bufSize, 
                                   uint8* pBuf)
    {
        Base::GetWindowPtr()->updateGLBufferUniform(offset,
                                                    bufSize, 
                                                    pBuf, 
                                                    this->nBufferUniformID);
    }                        

    void* GLESBufferUniform::MapBuffer(size_t offset, size_t bufSize, GLenum access)
    {
        return Base::GetWindowPtr()->mapGLBufferRange(this->nBufferUniformID, this->nBindingIndex, GL_UNIFORM_BUFFER, offset, bufSize, access);
    }
    void GLESBufferUniform::UnMapBuffer()
    {
        Base::GetWindowPtr()->unMapGLBufferRange(GL_UNIFORM_BUFFER);
    }
    void GLESBufferUniform::UpdateBuffer(size_t offset,
                                         size_t bufSize, 
                                         uint8* pBuf,
                                         GLenum access)
    {
        void* pData = MapBuffer(offset, bufSize, access);
        if (pData == nullptr)
        {
            F_LogError("*********************** GLESBufferUniform::UpdateBuffer: Failed to MapBuffer uniform: [%s] !", GetName().c_str());
            return;
        }
        memcpy(pData, (void*)pBuf, bufSize);
        UnMapBuffer();
    }

    void GLESBufferUniform::BindBufferUniform()
    {
        Base::GetWindowPtr()->bindGLBufferUniform(this->nBufferUniformID);
    }
    void GLESBufferUniform::BindBufferUniformBlockIndex(uint32 nBlockIndex)
    {
        this->nUniformBlockIndex = nBlockIndex;
        Base::GetWindowPtr()->bindGLBufferUniformBlockIndex(this->nBufferUniformID, nBlockIndex);
    }

}; //LostPeterOpenGLES