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

#include "../include/GLRenderPass.h"
#include "../include/OpenGLWindow.h"

namespace LostPeterOpenGLES
{
    GLRenderPass::GLRenderPass(const String& nameRenderPass)
        : Base(nameRenderPass)
        , pFrameBuffer(nullptr)
    {
        
    }
    GLRenderPass::~GLRenderPass()
    {
        Destroy();
    }

    void GLRenderPass::Destroy()
    {

    }

    bool GLRenderPass::Init()
    {

        return true;
    }

}; //LostPeterOpenGLES