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

#ifndef _GL_FRAME_BUFFER_H_
#define _GL_FRAME_BUFFER_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport GLFrameBuffer : public Base
    {
    public:
        GLFrameBuffer(const String& nameFrameBuffer);
        virtual ~GLFrameBuffer();

    public:
        static UintID2UintTypeMap s_mapID2Types;

    public:
        int nWidth;
        int nHeight;
        GLTexturePtrVector aColorTexture;
        UintType2UintIDMap mapType2IDs;
        GLRenderBuffer* pDepthStencil;
        bool bIsDeleteColors;
        bool bIsDeleteDepthStencil;

        uint32 nFrameBufferID;

    public:
        void Destroy();
        bool Init(int width,
                  int height,
                  const GLTexturePtrVector& aColors,
                  GLRenderBuffer* pDS,
                  bool isDeleteColors = false,
                  bool isDeleteDepthStencil = false);

    public:
        void BindFrameBuffer();

    public:
        GLTexture* GetColorTexture(int index);
    };

}; //LostPeterOpenGLES

#endif