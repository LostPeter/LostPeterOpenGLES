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

#ifndef _GL_SHADER_PROGRAM_H_
#define _GL_SHADER_PROGRAM_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport GLShaderProgram : public Base
    {
    public:
        GLShaderProgram(const String& nameShaderProgram);
        virtual ~GLShaderProgram();

    public:
        GLShader* pShaderVertex;
        GLShader* pShaderTessellationControl;
        GLShader* pShaderTessellationEvaluation;
        GLShader* pShaderGeometry;
        GLShader* pShaderFragment;
        GLShader* pShaderCompute;

        uint32 nShaderProgramID;


    public:
        void Destroy();
        bool Init(GLShader* pShaderVertex,
                  GLShader* pShaderTessellationControl,
                  GLShader* pShaderTessellationEvaluation,
                  GLShader* pShaderGeometry,
                  GLShader* pShaderFragment);
        bool Init(GLShader* pShaderCompute);

    public:
        void BindProgram();

        uint32 GetUniformBlockIndex(const String& name);
        void SetUniformBlockBinding(const String& name, uint32 nUniformBlockBinding);
        void SetUniformBlockBinding(uint32 nUniformBlockIndex, uint32 nUniformBlockBinding);

    public:
        void SetBool(const String& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const FVector2& v2) const;
        void SetVec2(const std::string& name, float x, float y) const;
        void SetVec3(const std::string& name, const FVector3& v3) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;
        void SetVec4(const std::string& name, const FVector4& v4) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const;
        void SetMat2(const std::string& name, const glm::mat2& m2) const;
        void SetMat3(const std::string& name, const FMatrix3& m3) const;
        void SetMat4(const std::string& name, const FMatrix4& m4) const;
    };

}; //LostPeterOpenGLES

#endif