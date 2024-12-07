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

#include "../include/GLESShader.h"
#include "../include/OpenGLESWindow.h"

namespace LostPeterOpenGLES
{
    GLESShader::GLESShader(const String& nameShader)
        : Base(nameShader)
        , typeShader(F_Shader_Vertex)
        , pathFile("")
        , nShaderID(0)
    {

    }
    GLESShader::~GLESShader()
    {
        Destroy();
    }

    void GLESShader::Destroy()
    {
        if (this->nShaderID > 0)
        {
            Base::GetWindowPtr()->destroyGLShader(this->nShaderID);
        }
        this->nShaderID = 0;
    }

    bool GLESShader::Init(const String& path,
                        FShaderType type)
    {   
        this->typeShader = type;
        this->pathFile = path;

        if (!Base::GetWindowPtr()->createGLShader(GetName(),
                                                  path,
                                                  type,
                                                  this->nShaderID))
        {
            F_LogError("*********************** GLESShader::Init: createGLShader failed, name: [%s], type: [%d], path: [%s] !", GetName().c_str(), type, path.c_str());
            return false;
        }
        
        return true;
    }

}; //LostPeterOpenGLES