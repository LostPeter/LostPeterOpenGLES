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
    float AndroidWindow::GetCurrentTime()
    {
        struct timespec clockRealTime;
        clock_gettime(CLOCK_MONOTONIC, &clockRealTime);
        double timeInSeconds = clockRealTime.tv_sec + (double)clockRealTime.tv_nsec / 1e9;
        return (float) timeInSeconds;
    }

    AndroidWindow::AndroidWindow(const String& nameWindow)
        : Base(nameWindow)
    {
        Clear();
    }
    AndroidWindow::~AndroidWindow()
    {
        Destroy();
    }

    void AndroidWindow::Clear()
    {
        m_pActivity = nullptr;
        m_pSavedState = nullptr;
        m_pConfig = nullptr;
        m_pUserData = nullptr;

        m_Width = 0;
        m_Height = 0;

        m_eglNativeDisplay = 0;
        m_eglNativeWindow = 0;

        m_eglDisplay = 0;
        m_eglSurface = 0;
        m_eglContext = 0;

        m_bIsAnimate = false;

        m_pSensorManager = nullptr;
        m_pASensorEventQueue= nullptr;
        m_pSensorAccelerometer= nullptr;

        m_bIsEGLInit = false;

        func_Init = nullptr;
        func_Update = nullptr;
        func_Render = nullptr;    
        func_ShutDown = nullptr;

        onWindowResize = nullptr;
        onRedrawIfNeeded = nullptr;
        onWindowContentChanged = nullptr;
        onWindowFocusChanged = nullptr;
        onConfigurationChanged = nullptr;
        onLowMemory = nullptr;
        onStart = nullptr;
        onResume = nullptr;
        onSavedInstance = nullptr;
        onPause = nullptr;
        onStop = nullptr;
        onDestroy = nullptr;
        onTouchEvent = nullptr;
        onKeyEvent = nullptr;
        onAnimate = nullptr;
    }

    void AndroidWindow::Destroy()
    {

    }

    bool AndroidWindow::Init()
    {

        return true;
    }

    GLboolean AndroidWindow::CreateWindow(GLuint flags)
    {
        //1> EGL
        EGLConfig config;
        EGLint majorVersion;
        EGLint minorVersion;
        EGLint contextAttribs[] = 
        {
            EGL_CONTEXT_CLIENT_VERSION,
            3,
            EGL_NONE
        };

        //2> Width/Height
        this->m_Width = ANativeWindow_getWidth(this->m_eglNativeWindow);
        this->m_Height = ANativeWindow_getHeight(this->m_eglNativeWindow);

        //3> eglGetDisplay，Default is EGL_DEFAULT_DISPLAY
        this->m_eglDisplay = eglGetDisplay(this->m_eglNativeDisplay);

        //4> EGL_NO_DISPLAY ?
        if (this->m_eglDisplay == EGL_NO_DISPLAY) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglGetDisplay failed !");
            return GL_FALSE;
        }

        //5> eglInitialize
        if (!eglInitialize(this->m_eglDisplay, &majorVersion, &minorVersion)) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglInitialize failed !");
            return GL_FALSE;
        }

        //6> Surface Config
        EGLint numConfigs = 0;
        EGLint attribList[] = {
                EGL_RED_SIZE, 5,
                EGL_GREEN_SIZE, 6,
                EGL_BLUE_SIZE, 5,
                EGL_ALPHA_SIZE, (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
                EGL_DEPTH_SIZE, (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
                EGL_STENCIL_SIZE, (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
                EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
                EGL_RENDERABLE_TYPE, GetContextRenderableType(this->m_eglDisplay),
                EGL_NONE
        };

        //7> Select Config
        if (!eglChooseConfig(this->m_eglDisplay, attribList, &config, 1, &numConfigs)) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglChooseConfig failed !");
            return GL_FALSE;
        }
        if (numConfigs < 1) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglChooseConfig numConfigs < 1 !");
            return GL_FALSE;
        }

        //8> Get EGLConfig
        EGLint format = 0;
        eglGetConfigAttrib(this->m_eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(this->m_eglNativeWindow, 0, 0, format);

        //9> eglCreateWindowSurface
        this->m_eglSurface = eglCreateWindowSurface(this->m_eglDisplay, config,
                                                    this->m_eglNativeWindow, nullptr);
        if (this->m_eglSurface == EGL_NO_SURFACE) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglCreateWindowSurface failed !");
            return GL_FALSE;
        }

        //10> eglCreateContext
        this->m_eglContext = eglCreateContext(this->m_eglDisplay, config,
                                            EGL_NO_CONTEXT, contextAttribs);
        if (this->m_eglContext == EGL_NO_CONTEXT) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglCreateContext failed !");
            return GL_FALSE;
        }

        //11> eglMakeCurrent
        if (!eglMakeCurrent(this->m_eglDisplay, this->m_eglSurface,
                            this->m_eglSurface, this->m_eglContext)) 
        {
            F_LogError("*********************** AndroidWindow::CreateWindow: eglMakeCurrent failed !");
            return GL_FALSE;
        }

        F_LogInfo("AndroidWindow::CreateWindow: CreateWindow success !");
        m_bIsEGLInit = true;
        return GL_TRUE;
    }

    GLint AndroidWindow::GetContextRenderableType(EGLDisplay eglDisplay)
    {
    #ifdef EGL_KHR_create_context
        const char *extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS);
        if (extensions != nullptr && strstr(extensions, "EGL_KHR_create_context")) 
        {
            return EGL_OPENGL_ES3_BIT_KHR;
        }
    #endif
        return EGL_OPENGL_ES2_BIT;
    }


}; //LostPeterOpenGLES