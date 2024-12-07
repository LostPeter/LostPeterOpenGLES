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

#ifndef _GLES_RENDER_PASS_H_
#define _GLES_RENDER_PASS_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESRenderPass : public Base
    {
    public:
        GLESRenderPass(const String& nameRenderPass);
        virtual ~GLESRenderPass();

    public:
        GLESFrameBuffer* pFrameBuffer; 

    public:
        void Destroy();
        bool Init();

    public:
        void SetFrameBuffer(GLESFrameBuffer* p) { this->pFrameBuffer = p; }
    };

}; //LostPeterOpenGLES

#endif