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

#ifndef _GLES_SHADER_H_
#define _GLES_SHADER_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESShader : public Base
    {
    public:
        GLESShader(const String& nameShader);
        virtual ~GLESShader();

    public:
        FShaderType typeShader;
        String pathFile;

        uint32 nShaderID;

    public:
        void Destroy();
        bool Init(const String& path,
                  FShaderType type);

    public:
        
    };

}; //LostPeterOpenGLES

#endif