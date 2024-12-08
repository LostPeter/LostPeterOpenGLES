/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-12-01
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "opengles_006_depth.h"


void android_main(struct android_app* app)
{
    OpenGLES_006_Depth sample(1280, 720, "LostPeter - OpenGLES_006_Depth");
    App::Run(app, &sample);
}