/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _EDITOR_BASE_H_
#define _EDITOR_BASE_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport EditorBase : public Base
    {
    public:
        EditorBase(const String& nameEditor);
        virtual ~EditorBase();
    
    public:
        
    };

}; //LostPeterOpenGLES

#endif