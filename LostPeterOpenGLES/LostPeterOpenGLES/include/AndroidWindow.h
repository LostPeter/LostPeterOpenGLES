/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-12-05
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _ANDROID_WINDOW_H_
#define _ANDROID_WINDOW_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport AndroidWindow : public Base
    {
    public:
        AndroidWindow(const String& nameWindow);
        virtual ~AndroidWindow();

    public:
       

    public:
        

    public:
        virtual void Destroy();
        
    };

}; //LostPeterOpenGLES

#endif