/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _APP_H_
#define _APP_H_

#include "PreDefine.h"

namespace LostPeterOpenGLES
{
    class openglesExport App
    {
    public:
        static android_app* s_pAndroidApp;
        static AndroidWindow* s_pWindow;
        static OpenGLESBase* s_pBase;

    public:
        static int Run(android_app* app, OpenGLESBase* pBase);

        static App* GetApp();
        static android_app* GetAndroidApp() { return s_pAndroidApp; }
        static AndroidWindow* GetWindow() { return s_pWindow; }

    public:
        static void ShutDown();
        
    protected:
        static void error_callback(int error, const char* description);
        static void key_callback(AndroidWindow* window, int key, int scancode, int action, int mods);
        static void framebuffer_size_callback(AndroidWindow* window, int width, int height);
        static void mouse_button_callback(AndroidWindow* window, int button, int action, int mods);
        static void cursor_position_callback(AndroidWindow* window, double x, double y);
        static void scroll_callback(AndroidWindow* window, double x, double y);
    };

}; //LostPeterOpenGLES

#endif