/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-06
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/GLESShaderProgram.h"
#include "../include/OpenGLESWindow.h"
#include "../include/GLESShader.h"

namespace LostPeterOpenGLES
{
    GLESShaderProgram::GLESShaderProgram(const String& nameShaderProgram)
        : Base(nameShaderProgram)
        , pShaderVertex(nullptr)
        , pShaderTessellationControl(nullptr)
        , pShaderTessellationEvaluation(nullptr)
        , pShaderGeometry(nullptr)
        , pShaderFragment(nullptr)
        , pShaderCompute(nullptr)

        , nShaderProgramID(0)
    {

    }
    GLESShaderProgram::~GLESShaderProgram()
    {
        Destroy();
    }

    void GLESShaderProgram::Destroy()
    {
        if (this->nShaderProgramID > 0)
        {
            Base::GetWindowPtr()->destroyGLShaderProgram(this->nShaderProgramID);
        }
        this->nShaderProgramID = 0;

        this->pShaderVertex = nullptr;
        this->pShaderTessellationControl = nullptr;
        this->pShaderTessellationEvaluation = nullptr;
        this->pShaderGeometry = nullptr;
        this->pShaderFragment = nullptr;
        this->pShaderCompute = nullptr;
    }

    bool GLESShaderProgram::Init(GLESShader* pShaderVertex,
                               GLESShader* pShaderTessellationControl,
                               GLESShader* pShaderTessellationEvaluation,
                               GLESShader* pShaderGeometry,
                               GLESShader* pShaderFragment)
    {
        this->pShaderVertex = pShaderVertex;
        this->pShaderTessellationControl = pShaderTessellationControl;
        this->pShaderTessellationEvaluation = pShaderTessellationEvaluation;
        this->pShaderGeometry = pShaderGeometry;
        this->pShaderFragment = pShaderFragment;

        if (!Base::GetWindowPtr()->createGLShaderProgram(GetName(),
                                                         pShaderVertex != nullptr ? pShaderVertex->nShaderID : 0,
                                                         pShaderTessellationControl != nullptr ? pShaderTessellationControl->nShaderID : 0,
                                                         pShaderTessellationEvaluation != nullptr ? pShaderTessellationEvaluation->nShaderID : 0,
                                                         pShaderGeometry != nullptr ? pShaderGeometry->nShaderID : 0,
                                                         pShaderFragment != nullptr ? pShaderFragment->nShaderID : 0,
                                                         this->nShaderProgramID))
        {
            F_LogError("*********************** GLESShaderProgram::Init: Failed to create shader program: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }

    bool GLESShaderProgram::Init(GLESShader* pShaderCompute)
    {
        if (pShaderCompute == nullptr)
            return false;
        this->pShaderCompute = pShaderCompute;

        if (!Base::GetWindowPtr()->createGLShaderProgram(GetName(),
                                                         pShaderCompute->nShaderID,
                                                         this->nShaderProgramID))
        {
            F_LogError("*********************** GLESShaderProgram::Init: Failed to create shader program [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }

    void GLESShaderProgram::BindProgram()
    {
        Base::GetWindowPtr()->bindGLShaderProgram(this->nShaderProgramID);
    }

    uint32 GLESShaderProgram::GetUniformBlockIndex(const String& name)
    {
        return Base::GetWindowPtr()->getUniformBlockIndex(this->nShaderProgramID, name);
    }
    void GLESShaderProgram::SetUniformBlockBinding(const String& name, uint32 nUniformBlockBinding)
    {
        uint32 nUniformBlockIndex = GetUniformBlockIndex(name);
        SetUniformBlockBinding(nUniformBlockIndex, nUniformBlockBinding);
    }
    void GLESShaderProgram::SetUniformBlockBinding(uint32 nUniformBlockIndex, uint32 nUniformBlockBinding)
    {
        Base::GetWindowPtr()->setUniformBlockBinding(this->nShaderProgramID, nUniformBlockIndex, nUniformBlockBinding);
    }

    void GLESShaderProgram::SetBool(const String& name, bool value) const
    {
        Base::GetWindowPtr()->setUniform1i(this->nShaderProgramID, name, (int)value);
    }
    void GLESShaderProgram::SetInt(const std::string& name, int value) const
    {
        Base::GetWindowPtr()->setUniform1i(this->nShaderProgramID, name, value);
    }
    void GLESShaderProgram::SetFloat(const std::string& name, float value) const
    {
        Base::GetWindowPtr()->setUniform1f(this->nShaderProgramID, name, value);
    }
    void GLESShaderProgram::SetVec2(const std::string& name, const FVector2& v2) const
    {
        Base::GetWindowPtr()->setUniform2fv(this->nShaderProgramID, name, v2);
    }
    void GLESShaderProgram::SetVec2(const std::string& name, float x, float y) const
    {
        Base::GetWindowPtr()->setUniform2f(this->nShaderProgramID, name, x, y);
    }
    void GLESShaderProgram::SetVec3(const std::string& name, const FVector3& v3) const
    {
        Base::GetWindowPtr()->setUniform3fv(this->nShaderProgramID, name, v3);
    }
    void GLESShaderProgram::SetVec3(const std::string& name, float x, float y, float z) const
    {
        Base::GetWindowPtr()->setUniform3f(this->nShaderProgramID, name, x, y, z);
    }
    void GLESShaderProgram::SetVec4(const std::string& name, const FVector4& v4) const
    {
        Base::GetWindowPtr()->setUniform4fv(this->nShaderProgramID, name, v4);
    }
    void GLESShaderProgram::SetVec4(const std::string& name, float x, float y, float z, float w) const
    {
        Base::GetWindowPtr()->setUniform4f(this->nShaderProgramID, name, x, y, z, w);
    }
    void GLESShaderProgram::SetMat2(const std::string& name, const glm::mat2& m2) const
    {
        Base::GetWindowPtr()->setUniformMatrix2fv(this->nShaderProgramID, name, m2);
    }
    void GLESShaderProgram::SetMat3(const std::string& name, const FMatrix3& m3) const
    {
        Base::GetWindowPtr()->setUniformMatrix3fv(this->nShaderProgramID, name, m3);
    }
    void GLESShaderProgram::SetMat4(const std::string& name, const FMatrix4& m4) const
    {
        Base::GetWindowPtr()->setUniformMatrix4fv(this->nShaderProgramID, name, m4);
    }

}; //LostPeterOpenGLES