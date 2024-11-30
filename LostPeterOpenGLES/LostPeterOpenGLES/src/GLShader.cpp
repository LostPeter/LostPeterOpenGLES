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

#include "../include/GLShader.h"
#include "../include/OpenGLWindow.h"

namespace LostPeterOpenGLES
{
    GLShader::GLShader(const String& nameShader)
        : Base(nameShader)
        , typeShader(F_Shader_Vertex)
        , pathFile("")
        , nShaderID(0)
    {

    }
    GLShader::~GLShader()
    {
        Destroy();
    }

    void GLShader::Destroy()
    {
        if (this->nShaderID > 0)
        {
            Base::GetWindowPtr()->destroyGLShader(this->nShaderID);
        }
        this->nShaderID = 0;
    }

    bool GLShader::Init(const String& path,
                        FShaderType type)
    {   
        this->typeShader = type;
        this->pathFile = path;

        if (!Base::GetWindowPtr()->createGLShader(GetName(),
                                                  path,
                                                  type,
                                                  this->nShaderID))
        {
            F_LogError("*********************** GLShader::Init: createGLShader failed, name: [%s], type: [%d], path: [%s] !", GetName().c_str(), type, path.c_str());
            return false;
        }
        
        return true;
    }

}; //LostPeterOpenGLES