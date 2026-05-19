/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-05-19
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_011_texturing.h"


void android_main(struct android_app* app)
{
    OpenGLES_011_Texturing sample("LostPeter - OpenGLES_011_Texturing");
    App::Run(app, &sample);
}