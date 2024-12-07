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

#include "../include/GLESRenderPass.h"
#include "../include/OpenGLESWindow.h"

namespace LostPeterOpenGLES
{
    GLESRenderPass::GLESRenderPass(const String& nameRenderPass)
        : Base(nameRenderPass)
        , pFrameBuffer(nullptr)
    {
        
    }
    GLESRenderPass::~GLESRenderPass()
    {
        Destroy();
    }

    void GLESRenderPass::Destroy()
    {

    }

    bool GLESRenderPass::Init()
    {

        return true;
    }

}; //LostPeterOpenGLES