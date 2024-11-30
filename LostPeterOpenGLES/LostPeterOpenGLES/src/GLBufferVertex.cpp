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

#include "../include/GLBufferVertex.h"
#include "../include/OpenGLWindow.h"

namespace LostPeterOpenGLES
{
    GLBufferVertex::GLBufferVertex(const String& nameBuffer)
        : GLBuffer(nameBuffer)
        , nVAO(0)
        , nVBO(0)

        , typeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
        , nBufferSize(0)
        , pBuffer(nullptr)
        , bIsDelete(true)
    {

    }
    GLBufferVertex::~GLBufferVertex()
    {
        Destroy();
    }

    void GLBufferVertex::Destroy()
    {
        destroyBuffer();
        if (this->nVAO > 0 && this->nVBO > 0)
        {
            Base::GetWindowPtr()->destroyGLBufferVertex(this->nVAO, this->nVBO);
        }
        this->nVAO = 0;
        this->nVBO = 0;
    }
        void GLBufferVertex::destroyBuffer()
        {
            if (this->bIsDelete && this->pBuffer != nullptr)
            {
                F_DELETE_T(this->pBuffer)
            }
            this->pBuffer = nullptr;
        }

    bool GLBufferVertex::Init(FMeshVertexType type,
                              size_t bufSize, 
                              uint8* pBuf,
                              bool isDelete)
    {
        this->typeVertex = type;
        this->nBufferSize = bufSize;
        this->pBuffer = pBuf;
        this->bIsDelete = isDelete;

        if (!Base::GetWindowPtr()->createGLBufferVertex(GetName(),
                                                        type,
                                                        bufSize, 
                                                        pBuf, 
                                                        this->nVAO, 
                                                        this->nVBO))
        {
            F_LogError("*********************** GLBufferVertex::Init: Failed to create buffer vertex: [%s] !", GetName().c_str());
            return false;
        }
        return true;
    }

    void GLBufferVertex::Update(FMeshVertexType type,
                                size_t bufSize, 
                                uint8* pBuf,
                                bool isDelete)
    {
        destroyBuffer();

        this->typeVertex = type;
        this->nBufferSize = bufSize;
        this->pBuffer = pBuf;
        this->bIsDelete = isDelete;

        Base::GetWindowPtr()->updateGLBufferVertex(type,
                                                   bufSize, 
                                                   pBuf, 
                                                   this->nVAO, 
                                                   this->nVBO);
    }

    void GLBufferVertex::BindVertexArray()
    {
        Base::GetWindowPtr()->bindGLVertexArray(this->nVAO);
    }

}; //LostPeterOpenGLES