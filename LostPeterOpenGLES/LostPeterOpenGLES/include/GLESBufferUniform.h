/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-16
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _GLES_BUFFER_UNIFORM_H_
#define _GLES_BUFFER_UNIFORM_H_

#include "GLESBuffer.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESBufferUniform : public GLESBuffer
    {
    public:
        GLESBufferUniform(const String& nameBuffer);
        virtual ~GLESBufferUniform();

    public:
        size_t nBufferSize;
        uint8* pBuffer;
        bool bIsDelete;
        GLenum eUsage;
        
        uint32 nBindingIndex;
        uint32 nBufferUniformID;

    public:
        virtual void Destroy();
        virtual bool Init(uint32 bindingIndex,
                          GLenum usage,
                          size_t bufSize, 
                          uint8* pBuf,
                          bool isDelete);
                          
        virtual void Update(size_t offset,
                            size_t bufSize, 
                            uint8* pBuf);

        virtual void* MapBuffer(size_t offset, size_t bufSize, GLenum access);
        virtual void UnMapBuffer();

        virtual void UpdateBuffer(size_t offset,
                                  size_t bufSize, 
                                  uint8* pBuf,
                                  GLenum access);

    public:
        void BindBufferUniform();
        void BindBufferUniformBlockIndex(uint32 nUniformBlockIndex);

    protected:
        virtual void destroyBuffer();
        
    };

}; //LostPeterOpenGLES

#endif