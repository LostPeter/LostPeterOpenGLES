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

#include "../include/AndroidWindow.h"
#include "../include/App.h"

namespace LostPeterOpenGLES
{
    AndroidWindow::AndroidWindow(const String& nameWindow)
        : Base(nameWindow)
    {

    }
    AndroidWindow::~AndroidWindow()
    {
        Destroy();
    }

    void AndroidWindow::Destroy()
    {

    }

    



}; //LostPeterOpenGLES