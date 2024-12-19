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

#include "../include/App.h"
#include "../include/OpenGLESBase.h"
#include "../include/AndroidWindow.h"


namespace LostPeterOpenGLES
{
    App g_app;
    android_app* App::s_pAndroidApp = nullptr;
    AndroidWindow* App::s_pWindow = nullptr;
    OpenGLESBase* App::s_pBase = nullptr;

    App* App::GetApp()
    {
        return &g_app;
    }

    static void s_AppWndProc(struct android_app* app, int32_t cmd)
    {
        F_LogInfo("s_AppWndProc: cmd: %d", cmd);
        AndroidWindow* pAndroidWindow = App::GetWindow();
        
        switch (cmd)
        {
        case APP_CMD_INPUT_CHANGED: 
            {
                
            }
            break;

        case APP_CMD_INIT_WINDOW:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_INIT_WINDOW 1");
                {
                    pAndroidWindow->m_eglNativeDisplay = EGL_DEFAULT_DISPLAY;
                    pAndroidWindow->m_eglNativeWindow = app->window;
                    
                    if (!pAndroidWindow->CreateWindow(ES_WINDOW_RGB))
                    {
                        F_LogError("*********************** s_AppWndProc: APP_CMD_INIT_WINDOW: EGL create failed !");
                        exit(0);
                    }

                    if (pAndroidWindow->func_Init != nullptr)
                    {
                        pAndroidWindow->func_Init(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_INIT_WINDOW 2");
            }
            break;

        case APP_CMD_TERM_WINDOW:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_TERM_WINDOW 1");
                {
                    if (pAndroidWindow->func_ShutDown != nullptr) 
                    {
                        pAndroidWindow->func_ShutDown(pAndroidWindow);
                    }
                    if (pAndroidWindow->m_pUserData != nullptr)
                    {
                        pAndroidWindow->m_pUserData = nullptr;
                    }
                    pAndroidWindow->Clear();
                }
                F_LogInfo("s_AppWndProc: APP_CMD_TERM_WINDOW 2");
            }
            break;

        case APP_CMD_WINDOW_RESIZED:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_WINDOW_RESIZED 1");
                {
                    if (pAndroidWindow->onWindowResize != nullptr) 
                    {
                        pAndroidWindow->onWindowResize(pAndroidWindow, pAndroidWindow->m_Width, pAndroidWindow->m_Height);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_WINDOW_RESIZED 2");
            }
            break;

        case APP_CMD_WINDOW_REDRAW_NEEDED:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_WINDOW_REDRAW_NEEDED 1");
                {
                    if (pAndroidWindow->onRedrawIfNeeded != nullptr) 
                    {
                        pAndroidWindow->onRedrawIfNeeded(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_WINDOW_REDRAW_NEEDED 2");
            }
            break;

        case APP_CMD_CONTENT_RECT_CHANGED:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_CONTENT_RECT_CHANGED 1");
                {
                    if (pAndroidWindow->onWindowContentChanged != nullptr) 
                    {
                        pAndroidWindow->onWindowContentChanged(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_CONTENT_RECT_CHANGED 2");
            }
            break;

        case APP_CMD_GAINED_FOCUS:
            {
                if (pAndroidWindow->onWindowFocusChanged != nullptr) 
                {
                    pAndroidWindow->onWindowFocusChanged(pAndroidWindow, true);
                }
            }
            break;

        case APP_CMD_LOST_FOCUS:
            {
                pAndroidWindow->m_bIsAnimate = false;
                if (pAndroidWindow->onWindowFocusChanged != nullptr) 
                {
                    pAndroidWindow->onWindowFocusChanged(pAndroidWindow, false);
                }
            }   
            break;

        case APP_CMD_CONFIG_CHANGED:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_CONFIG_CHANGED 1");
                {
                    if (pAndroidWindow->onConfigurationChanged != nullptr) 
                    {
                        pAndroidWindow->onConfigurationChanged(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_CONFIG_CHANGED 2");
            }
            break;

        case APP_CMD_LOW_MEMORY:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_LOW_MEMORY 1");
                {
                    if (pAndroidWindow->onLowMemory != nullptr) 
                    {
                        pAndroidWindow->onLowMemory(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_LOW_MEMORY 2");
            }
            break;

        case APP_CMD_START:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_START 1");
                {
                    if (pAndroidWindow->onStart != nullptr) 
                    {
                        pAndroidWindow->onStart(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_START 2");
            }
            break;

        case APP_CMD_RESUME:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_RESUME 1");
                {
                    if (pAndroidWindow->onResume != nullptr) 
                    {
                        pAndroidWindow->onResume(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_RESUME 2");
            }
            break;

        case APP_CMD_SAVE_STATE:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_SAVE_STATE 1");
                {
                    if (pAndroidWindow->onSavedInstance != nullptr) 
                    {
                        pAndroidWindow->onSavedInstance(pAndroidWindow, app);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_SAVE_STATE 2");
            }
            break;

        case APP_CMD_PAUSE:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_PAUSE 1");
                {
                    if (pAndroidWindow->onPause != nullptr) 
                    {
                        pAndroidWindow->onPause(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_PAUSE 2");
            }
            break;

        case APP_CMD_STOP:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_STOP 1");
                {
                    if (pAndroidWindow->onStop != nullptr) 
                    {
                        pAndroidWindow->onStop(pAndroidWindow);
                    }
                }
                F_LogInfo("s_AppWndProc: APP_CMD_STOP 2");
            }
            break;

        case APP_CMD_DESTROY:
            {
                F_LogInfo("s_AppWndProc: APP_CMD_DESTROY 1");
                {
                    if (pAndroidWindow->onDestroy != nullptr) 
                    {
                        pAndroidWindow->onDestroy(pAndroidWindow);
                    }
                    if (pAndroidWindow->m_pUserData != nullptr) 
                    {
                        pAndroidWindow->m_pUserData = nullptr;
                    }
                    pAndroidWindow->Clear();
                }
                F_LogInfo("s_AppWndProc: APP_CMD_DESTROY 2");
            }
            break;
        
        default:
            break;
        }
    }

    static int32_t s_AppInputProc(struct android_app* app, AInputEvent* event)
    {
        AndroidWindow* pAndroidWindow = App::GetWindow();

        switch (AInputEvent_getType(event))
        {
        case AINPUT_EVENT_TYPE_MOTION:
            {
                F_LogInfo("s_AppInputProc: AINPUT_EVENT_TYPE_MOTION 1");
                {
                    if (pAndroidWindow != nullptr)
                    {
                        if (pAndroidWindow->onTouchEvent != nullptr)
                        {
                            pAndroidWindow->onTouchEvent(pAndroidWindow, event);
                        }
                    }
                }
                F_LogInfo("s_AppInputProc: AINPUT_EVENT_TYPE_MOTION 2");
            }
            break;

        case AINPUT_EVENT_TYPE_KEY:
            {
                F_LogInfo("s_AppInputProc: AINPUT_EVENT_TYPE_KEY 1");
                {
                    if (pAndroidWindow != nullptr)
                    {
                        if (pAndroidWindow->onKeyEvent != nullptr)
                        {
                            pAndroidWindow->onKeyEvent(pAndroidWindow, event);
                        }
                    }
                }
                F_LogInfo("s_AppInputProc: AINPUT_EVENT_TYPE_KEY 2");
            }
            break;
        
        default:
            break;
        }
        return 0;
    }

    int App::Run(android_app* app, OpenGLESBase* pBase)
    {
        App* pApp = GetApp();
        FUtil::ms_pAndroidApp = app;
        String nameApp(pBase->GetTitle());
        F_LogInfo("App::Run: [%s] Enter ***********************", nameApp.c_str());
        s_pAndroidApp = app;
        s_pBase = pBase;

        //1> Parse the command line parameters


        //2> AndroidWindow
        AndroidWindow androidWindow(pBase->GetTitle());
        s_pWindow = &androidWindow;
        pBase->pWindow = s_pWindow;
        s_pWindow->m_pActivity = app->activity;
        s_pWindow->m_pSensorManager = ASensorManager_getInstance();
        s_pWindow->m_pSensorAccelerometer = ASensorManager_getDefaultSensor(s_pWindow->m_pSensorManager, ASENSOR_TYPE_ACCELEROMETER);
        s_pWindow->m_pASensorEventQueue = ASensorManager_createEventQueue(s_pWindow->m_pSensorManager,
                                                                          app->looper, 
                                                                          LOOPER_ID_USER,
                                                                          nullptr, 
                                                                          nullptr);
        if (!s_pWindow->Init())
        {
            F_LogError("*********************** App::Run: CreateWindow error !");
            return 0;
        }

        //3> Data
        s_pAndroidApp->onAppCmd = s_AppWndProc;
        s_pAndroidApp->onInputEvent = s_AppInputProc;
        s_pAndroidApp->userData = pApp;
        if (app->savedState != nullptr)
        {
            s_pWindow->m_pSavedState = app->savedState;
        }
        if (app->config != nullptr)
        {
            s_pWindow->m_pConfig = app->config;
        }
        pApp->initInternal(s_pWindow);

        //3> Main loop
        while (true) 
        {
            int events;
            int ident;
            struct android_poll_source* source;

            //<1> message process
            bool isReturn = false;
            while ((ident = ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0)
            {
                if (source != nullptr)
                {
                    source->process(s_pAndroidApp, source);
                }

                if (s_pAndroidApp->destroyRequested != 0) 
                {
                    if (s_pWindow->func_ShutDown != nullptr) 
                    {
                        s_pWindow->func_ShutDown(s_pWindow);
                    }
                    isReturn = true;
                }
            }
            if (isReturn)
            {
                break;
            }

            //2> wait native egl window and opengles window init
            if (s_pWindow->m_eglNativeWindow == nullptr ||
                !pBase->OnIsInit()) 
            {
                continue;
            }

            //3> animate
            if (s_pWindow->m_bIsAnimate && 
                s_pWindow->onAnimate != nullptr) 
            {
                s_pWindow->onAnimate(s_pWindow);
            }

            //4> app update
            if (s_pWindow->func_Update != nullptr) 
            {
                s_pWindow->func_Update(s_pWindow);
            }

            //5> draw
            if (s_pWindow->func_Render != nullptr) 
            {
                s_pWindow->func_Render(s_pWindow);
                eglSwapBuffers(s_pWindow->m_eglDisplay, s_pWindow->m_eglSurface);
            }

            // pBase->OnKeyboardInput();

            if (pBase->isMinimizedWindowNeedClose) 
            {
                break;
            }
        }

        F_LogInfo("App::Run: [%s] Exit ***********************", nameApp.c_str());
        return 0;
    }

    void App::initInternal(AndroidWindow* pWindow)
    {
        pWindow->func_Init = onInit;
        pWindow->func_Update = onUpdate;
        pWindow->func_Render = onRender;
        pWindow->func_ShutDown = onShutDown;

        pWindow->onWindowResize = onWindowResize;
        pWindow->onTouchEvent = onTouchEvent;
        pWindow->onKeyEvent = onKeyEvent;
    }

    void App::onInit(AndroidWindow* pWindow)
    {
        s_pBase->OnInit(pWindow->GetWindowWidth(), pWindow->GetWindowHeight());
        s_pBase->OnLoad();
    }
    void App::onUpdate(AndroidWindow* pWindow)
    {
        s_pBase->UpdateTimer();
    }
    void App::onRender(AndroidWindow* pWindow)
    {
        if (!s_pBase->isAppPaused)
        {
            s_pBase->CalculateFrameStats(s_pWindow);

            //Compute Before Render
            if (s_pBase->OnBeginCompute_BeforeRender())
            {
                s_pBase->OnUpdateCompute_BeforeRender();
                s_pBase->OnCompute_BeforeRender();
                s_pBase->OnEndCompute_BeforeRender();
            }

            //Render
            if (s_pBase->OnBeginRender())
            {
                s_pBase->OnUpdateRender();
                s_pBase->OnRender();
                s_pBase->OnEndRender();
            }

            //Compute After Render
            if (s_pBase->OnBeginCompute_AfterRender())
            {
                s_pBase->OnUpdateCompute_AfterRender();
                s_pBase->OnCompute_AfterRender();
                s_pBase->OnEndCompute_AfterRender();
            }

            s_pBase->OnPresent();
        }  
    }
    void App::onShutDown(AndroidWindow* pWindow)
    {
        s_pBase->OnDestroy();
    }
    void App::onWindowResize(AndroidWindow* pWindow, int width, int height)
    {
        s_pBase->OnResize(width, height, true);
    }
    int32_t App::onTouchEvent(AndroidWindow* pWindow, AInputEvent* event)
    {

        return 1;
    }
    int32_t App::onKeyEvent(AndroidWindow* pWindow, AInputEvent* event)
    {

        return 1;
    }

}; //LostPeterOpenGLES