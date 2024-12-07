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

#include "../include/PreInclude.h"
#include "../include/OpenGLESBase.h"

namespace LostPeterOpenGLES
{
    FLogManager* OpenGLESBase::ms_pLogManager = FLogManager::GetInstance();
    OpenGLESBase::OpenGLESBase(int width, int height, String name)
        : versionGL_Major(4)
    #if F_PLATFORM == F_PLATFORM_MAC
        , versionGL_Minor(1)
        , versionGLSL("#version 410")
    #else
        , versionGL_Minor(6)
        , versionGLSL("#version 460")
    #endif
        , width(width)
        , height(height)
        , pWindow(nullptr)

        , isAppPaused(false)
        , isMinimized(false)
        , isMaximized(false)
        , isResizing(false)
        , isFullscreenState(false)
        , isMinimizedWindowNeedClose(false)

        , isCreateDevice(false)
        , isLoadAsset(false)

        , pTimer(new FTimer())
        , fFPS(0.0f)
        , nFrameFPS(0)
        , nFrameTotal(0)

        , nameTitle(name)
    {
        this->pathBin = FUtil::GetPathBin() + "/";
        F_LogInfo("Path Bin: [%s] !", this->pathBin.c_str());

        RefreshAspectRatio();
        fTimeLastFPS = pTimer->GetTimeSinceStart();
    }

    OpenGLESBase::~OpenGLESBase()
    {
        F_DELETE(pTimer)
    }

    float OpenGLESBase::RefreshAspectRatio()
    {
        this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        return this->aspectRatio;
    }

    void OpenGLESBase::CalculateFrameStats(AndroidWindow* s_pWindow)
    {

    }

    void OpenGLESBase::UpdateTimer()
    {
        this->pTimer->Tick();
        float timeSinceStart = this->pTimer->GetTimeSinceStart();
        ++ this->nFrameFPS;
        if (timeSinceStart >= this->fTimeLastFPS + 1.0f)
        {
            this->fFPS = this->nFrameFPS / (timeSinceStart - this->fTimeLastFPS);
            this->nFrameFPS = 0;
            this->fTimeLastFPS = timeSinceStart;
        }
    }

    String OpenGLESBase::GetAssetFullPath(const String& assetName)
    {
        return this->pathBin + assetName;
    }

}; //LostPeterOpenGLES