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

#ifndef _GLES_RENDER_BUFFER_H_
#define _GLES_RENDER_BUFFER_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESRenderBuffer : public Base
    {
    public:
        GLESRenderBuffer(const String& nameRenderBuffer);
        virtual ~GLESRenderBuffer();

    public:
        int nWidth;
        int nHeight;
        GLenum nFormat;
        GLenum nAttachment; 
        GLenum nRenderbuffertarget;

        uint32 nRenderBufferID;

    public:
        void Destroy();
        bool Init(int width,
                  int height,
                  GLenum format,
                  GLenum attachment, 
                  GLenum renderbuffertarget);

    public:
        void BindRenderBuffer();
    };

}; //LostPeterOpenGLES

#endif