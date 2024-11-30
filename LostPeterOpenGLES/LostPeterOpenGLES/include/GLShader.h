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

#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport GLShader : public Base
    {
    public:
        GLShader(const String& nameShader);
        virtual ~GLShader();

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