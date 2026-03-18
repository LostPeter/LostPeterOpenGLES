/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-03-18
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_010_lighting.h"


void android_main(struct android_app* app)
{
    OpenGLES_010_Lighting sample("LostPeter - OpenGLES_010_Lighting");
    App::Run(app, &sample);
}