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
        static OpenGLESBase* GetOpenGLESBase() { return s_pBase; }

    public:
    protected:
        void initInternal(AndroidWindow* pWindow);

    protected:
        static void onInit(AndroidWindow* pWindow);
        static void onUpdate(AndroidWindow* pWindow);
        static void onRender(AndroidWindow* pWindow);
        static void onShutDown(AndroidWindow* pWindow);

        static void onWindowResize(AndroidWindow* pWindow, int width, int height);
        static int32_t onTouchEvent(AndroidWindow* pWindow, AInputEvent* event);
        static int32_t onKeyEvent(AndroidWindow* pWindow, AInputEvent* event);
    };

}; //LostPeterOpenGLES

#endif