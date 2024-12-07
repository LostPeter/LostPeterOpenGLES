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

#include "../include/GLESBufferVertexIndex.h"
#include "../include/OpenGLESWindow.h"

namespace LostPeterOpenGLES
{
    GLESBufferVertexIndex::GLESBufferVertexIndex(const String& nameBuffer)
        : GLESBuffer(nameBuffer)
        , nVAO(0)
        , nVBO(0)
        , nVEO(0)

        , typeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
        , nBufferSize_Vertex(0)
        , pBuffer_Vertex(nullptr)
        , bIsDelete_Vertex(true)
        , nBufferSize_Index(0)
        , pBuffer_Index(nullptr)
        , bIsDelete_Index(true)
    {

    }
    GLESBufferVertexIndex::~GLESBufferVertexIndex()
    {
        Destroy();
    }

    void GLESBufferVertexIndex::Destroy()
    {
        destroyBuffer();

        if (this->nVAO > 0 && this->nVBO > 0 && this->nVEO > 0)
        {
            Base::GetWindowPtr()->destroyGLBufferVertexIndex(this->nVAO, this->nVBO, this->nVEO);
        }
        this->nVAO = 0;
        this->nVBO = 0;
        this->nVEO = 0;
    }
        void GLESBufferVertexIndex::destroyBuffer()
        {
            if (this->bIsDelete_Vertex && this->pBuffer_Vertex != nullptr)
            {
                F_DELETE_T(this->pBuffer_Vertex)
            }
            this->pBuffer_Vertex = nullptr;
            if (this->bIsDelete_Index && this->pBuffer_Index != nullptr)
            {
                F_DELETE_T(this->pBuffer_Index)
            }
            this->pBuffer_Index = nullptr;
        }

    bool GLESBufferVertexIndex::Init(FMeshVertexType type,
                                   size_t bufSize_Vertex, 
                                   uint8* pBuf_Vertex,
                                   bool isDelete_Vertex,
                                   size_t bufSize_Index, 
                                   uint8* pBuf_Index,
                                   bool isDelete_Index)
    {
        this->typeVertex = type;
        this->nBufferSize_Vertex = bufSize_Vertex;
        this->pBuffer_Vertex = pBuf_Vertex;
        this->bIsDelete_Vertex = isDelete_Vertex;
        this->nBufferSize_Index = bufSize_Index;
        this->pBuffer_Index = pBuf_Index;
        this->bIsDelete_Index = isDelete_Index;

        if (!Base::GetWindowPtr()->createGLBufferVertexIndex(GetName(),
                                                             type,
                                                             bufSize_Vertex, 
                                                             pBuf_Vertex, 
                                                             bufSize_Index,
                                                             pBuf_Index,
                                                             this->nVAO, 
                                                             this->nVBO,
                                                             this->nVEO))
        {
            F_LogError("*********************** GLESBufferVertexIndex::Init: Failed to create buffer vertex index: [%s] !", GetName().c_str());
            return false;
        }
        return true;
    }

    void GLESBufferVertexIndex::Update(FMeshVertexType type,
                                     size_t bufSize_Vertex,
                                     uint8* pBuf_Vertex,
                                     bool isDelete_Vertex,
                                     size_t bufSize_Index,
                                     uint8* pBuf_Index,
                                     bool isDelete_Index)
    {
        destroyBuffer();

        this->typeVertex = type;
        this->nBufferSize_Vertex = bufSize_Vertex;
        this->pBuffer_Vertex = pBuf_Vertex;
        this->bIsDelete_Vertex = isDelete_Vertex;
        this->nBufferSize_Index = bufSize_Index;
        this->pBuffer_Index = pBuf_Index;
        this->bIsDelete_Index = isDelete_Index;

        Base::GetWindowPtr()->updateGLBufferVertexIndex(type,
                                                        bufSize_Vertex, 
                                                        pBuf_Vertex, 
                                                        bufSize_Index,
                                                        pBuf_Index,
                                                        this->nVAO, 
                                                        this->nVBO,
                                                        this->nVEO);
    }

    void GLESBufferVertexIndex::BindVertexArray()
    {
        Base::GetWindowPtr()->bindGLVertexArray(this->nVAO);
    }

}; //LostPeterOpenGLES