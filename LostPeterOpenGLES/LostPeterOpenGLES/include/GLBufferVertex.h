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

#ifndef _GL_BUFFER_VERTEX_H_
#define _GL_BUFFER_VERTEX_H_

#include "GLBuffer.h"

namespace LostPeterOpenGLES
{
    class openglExport GLBufferVertex : public GLBuffer
    {
    public:
        GLBufferVertex(const String& nameBuffer);
        virtual ~GLBufferVertex();

    public:
        uint32 nVAO;
        uint32 nVBO;

        FMeshVertexType typeVertex;
        size_t nBufferSize;
        uint8* pBuffer;
        bool bIsDelete;

    public:
        virtual void Destroy();
        virtual bool Init(FMeshVertexType type,
                          size_t bufSize, 
                          uint8* pBuf,
                          bool isDelete);

        virtual void Update(FMeshVertexType type,
                            size_t bufSize, 
                            uint8* pBuf,
                            bool isDelete);

    public:
        void BindVertexArray();

    protected:
        virtual void destroyBuffer();
    };

}; //LostPeterOpenGLES

#endif