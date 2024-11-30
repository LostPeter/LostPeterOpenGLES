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

#ifndef _GL_RENDER_PASS_H_
#define _GL_RENDER_PASS_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport GLRenderPass : public Base
    {
    public:
        GLRenderPass(const String& nameRenderPass);
        virtual ~GLRenderPass();

    public:
        GLFrameBuffer* pFrameBuffer; 

    public:
        void Destroy();
        bool Init();

    public:
        void SetFrameBuffer(GLFrameBuffer* p) { this->pFrameBuffer = p; }
    };

}; //LostPeterOpenGLES

#endif