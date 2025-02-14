/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-03
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _GLES_BUFFER_H_
#define _GLES_BUFFER_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESBuffer : public Base
    {
    public:
        GLESBuffer(const String& nameBuffer);
        virtual ~GLESBuffer();

    public:
        

    public:
        virtual void Destroy() = 0;
        
    };

}; //LostPeterOpenGLES

#endif