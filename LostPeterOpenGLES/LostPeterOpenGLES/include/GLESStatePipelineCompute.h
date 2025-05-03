/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2025-05-03
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _GLES_STATE_PIPELINE_COMPUTE_H_
#define _GLES_STATE_PIPELINE_COMPUTE_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESStatePipelineCompute : public Base
    {
    public:
        GLESStatePipelineCompute(const String& nameState);
        virtual ~GLESStatePipelineCompute();

    public:
        

    public:
        virtual void Destroy() = 0;
        
    };

}; //LostPeterOpenGLES

#endif