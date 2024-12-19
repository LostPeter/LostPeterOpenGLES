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
        ANativeActivity* m_pActivity;
        void* m_pSavedState;
        AConfiguration* m_pConfig;
        void* m_pUserData;

        GLint m_Width;
        GLint m_Height;

        EGLNativeDisplayType m_eglNativeDisplay;
        EGLNativeWindowType m_eglNativeWindow;

        EGLDisplay m_eglDisplay;
        EGLSurface m_eglSurface;
        EGLContext m_eglContext;

        bool m_bIsAnimate;

        ASensorManager* m_pSensorManager;
        ASensorEventQueue* m_pASensorEventQueue;
        const ASensor* m_pSensorAccelerometer;

        bool m_bIsEGLInit;

    public:
        void (* func_Init)(AndroidWindow* eglWindow);
        void (* func_Update)(AndroidWindow* eglWindow);
        void (* func_Render)(AndroidWindow* eglWindow);
        void (* func_ShutDown)(AndroidWindow* eglWindow);
        
        void (* onWindowResize)(AndroidWindow* eglWindow, int width, int height);
        void (* onRedrawIfNeeded)(AndroidWindow* eglWindow);
        void (* onWindowContentChanged)(AndroidWindow* eglWindow);
        void (* onWindowFocusChanged)(AndroidWindow* eglWindow, bool focus);
        void (* onConfigurationChanged)(AndroidWindow* eglWindow);
        void (* onLowMemory)(AndroidWindow* eglWindow);
        void (* onStart)(AndroidWindow* eglWindow);
        void (* onResume)(AndroidWindow* eglWindow);
        void (* onSavedInstance)(AndroidWindow* eglWindow, struct android_app* app);
        void (* onPause)(AndroidWindow* eglWindow);
        void (* onStop)(AndroidWindow* eglWindow);
        void (* onDestroy)(AndroidWindow* eglWindow);
        int32_t (* onTouchEvent)(AndroidWindow* eglWindow, AInputEvent* event);
        int32_t (* onKeyEvent)(AndroidWindow* eglWindow, AInputEvent* event);
        void (* onAnimate)(AndroidWindow* eglWindow);

    public:
        static float GetCurrentTime();

    public:
        void Clear();
        void Destroy();
        bool Init();

    public:
        GLboolean CreateWindow(GLuint flags);
    
        GLint GetContextRenderableType(EGLDisplay eglDisplay);

    public:
        int GetWindowWidth();
        int GetWindowHeight();

    };

}; //LostPeterOpenGLES

#endif