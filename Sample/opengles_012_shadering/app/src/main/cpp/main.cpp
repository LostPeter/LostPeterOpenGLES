/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-06-15
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_012_shadering.h"


void android_main(struct android_app* app)
{
    OpenGLES_012_Shadering sample("LostPeter - OpenGLES_012_Shadering");
    App::Run(app, &sample);
}