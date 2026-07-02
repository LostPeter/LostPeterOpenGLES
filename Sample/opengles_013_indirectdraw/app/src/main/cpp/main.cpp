/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-02
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_013_indirectdraw.h"


void android_main(struct android_app* app)
{
    OpenGLES_013_IndirectDraw sample("LostPeter - OpenGLES_013_IndirectDraw");
    App::Run(app, &sample);
}