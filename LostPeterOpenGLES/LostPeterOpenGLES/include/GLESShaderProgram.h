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

#ifndef _GLES_SHADER_PROGRAM_H_
#define _GLES_SHADER_PROGRAM_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESShaderProgram : public Base
    {
    public:
        GLESShaderProgram(const String& nameShaderProgram);
        virtual ~GLESShaderProgram();

    public:
        GLESShader* pShaderVertex;
        GLESShader* pShaderTessellationControl;
        GLESShader* pShaderTessellationEvaluation;
        GLESShader* pShaderGeometry;
        GLESShader* pShaderFragment;
        GLESShader* pShaderCompute;

        uint32 nShaderProgramID;


    public:
        void Destroy();
        bool Init(GLESShader* pShaderVertex,
                  GLESShader* pShaderTessellationControl,
                  GLESShader* pShaderTessellationEvaluation,
                  GLESShader* pShaderGeometry,
                  GLESShader* pShaderFragment);
        bool Init(GLESShader* pShaderCompute);

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