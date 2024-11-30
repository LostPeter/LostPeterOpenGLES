/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-08
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/GLDebug.h"
#include "../include/OpenGLWindow.h"

template<> LostPeterOpenGLES::GLDebug* LostPeterFoundation::FSingleton<LostPeterOpenGLES::GLDebug>::ms_Singleton = nullptr;

namespace LostPeterOpenGLES
{
    GLDebug* GLDebug::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	GLDebug& GLDebug::GetSingleton()
	{  
		F_Assert(ms_Singleton && "GLDebug::GetSingleton")
		return (*ms_Singleton);     
	}

    
    GLDebug::GLDebug()
        : Base("GLDebug")

        , func_glDebugMessageControl(nullptr)
        , func_glDebugMessageCallback(nullptr)
        , func_glDebugMessageInsert(nullptr)
        , func_glObjectLabel(nullptr)
        , func_glGetObjectLabel(nullptr)
        , func_glPushDebugGroup(nullptr)
        , func_glPopDebugGroup(nullptr)

        , func_glDebugMessageControlKHR(nullptr)
        , func_glDebugMessageCallbackKHR(nullptr)
        , func_glDebugMessageInsertKHR(nullptr)
        , func_glObjectLabelKHR(nullptr)
        , func_glGetObjectLabelKHR(nullptr)
        , func_glPushDebugGroupKHR(nullptr)
        , func_glPopDebugGroupKHR(nullptr)
    {

    }
    GLDebug::~GLDebug()
    {
        Destroy();
    }

    void GLDebug::Destroy()
    {
        
    } 

    bool GLDebug::Init()
    {
        if (Util_IsSupportExtensionsGL(GLAD_GL_KHR_debug))
        {
            this->func_glDebugMessageControlKHR = glad_glDebugMessageControlKHR;
            this->func_glDebugMessageCallbackKHR = glad_glDebugMessageCallbackKHR;
            this->func_glDebugMessageInsertKHR = glad_glDebugMessageInsertKHR;
            this->func_glObjectLabelKHR = glad_glObjectLabelKHR;
            this->func_glGetObjectLabelKHR = glad_glGetObjectLabelKHR;
            this->func_glPushDebugGroupKHR = glad_glPushDebugGroupKHR;
            this->func_glPopDebugGroupKHR = glad_glPopDebugGroupKHR;
        }
        else
        {
            this->func_glDebugMessageControlKHR = glad_glDebugMessageControl;
            this->func_glDebugMessageCallback = glad_glDebugMessageCallback;
            this->func_glDebugMessageInsert = glad_glDebugMessageInsert;
            this->func_glObjectLabel = glad_glObjectLabel;
            this->func_glGetObjectLabel = glad_glGetObjectLabel;
            this->func_glPushDebugGroup = glad_glPushDebugGroup;
            this->func_glPopDebugGroup = glad_glPopDebugGroup;
        }

        return true;
    }

    
    void GLDebug::DebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled)
    {
        if (this->func_glDebugMessageControlKHR != nullptr)
        {
            this->func_glDebugMessageControlKHR(source, type, severity, count, ids, enabled);
        }
        else if (this->func_glDebugMessageControl != nullptr)
        {
            this->func_glDebugMessageControl(source, type, severity, count, ids, enabled);
        }
    }
    void GLDebug::DebugMessageCallback(GLDEBUGPROCKHR callback, const void* userParam)
    {
        if (this->func_glDebugMessageCallbackKHR != nullptr)
        {
            this->func_glDebugMessageCallbackKHR(callback, userParam);
        }
        else if (this->func_glDebugMessageCallback != nullptr)
        {
            this->func_glDebugMessageCallback(callback, userParam);
        }
    }
    void GLDebug::DebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf)
    {
        if (this->func_glDebugMessageInsertKHR != nullptr)
        {
            this->func_glDebugMessageInsertKHR(source, type, id, severity, length, buf);
        }
        else if (this->func_glDebugMessageInsert != nullptr)
        {
            this->func_glDebugMessageInsert(source, type, id, severity, length, buf);
        }
    }
    void GLDebug::ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar* label)
    {
        if (this->func_glObjectLabelKHR != nullptr)
        {
            this->func_glObjectLabelKHR(identifier, name, length, label);
        }
        else if (this->func_glObjectLabel != nullptr)
        {
            this->func_glObjectLabel(identifier, name, length, label);
        }
    }
    void GLDebug::GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar* label)
    {
        if (this->func_glGetObjectLabelKHR != nullptr)
        {
            this->func_glGetObjectLabelKHR(identifier, name, bufSize, length, label);
        }
        else if (this->func_glGetObjectLabel != nullptr)
        {
            this->func_glGetObjectLabel(identifier, name, bufSize, length, label);
        }
    }
    void GLDebug::PushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar* message)
    {
        if (this->func_glPushDebugGroupKHR != nullptr)
        {
            this->func_glPushDebugGroupKHR(source, id, length, message);
        }
        else if (this->func_glPushDebugGroup != nullptr)
        {
            this->func_glPushDebugGroup(source, id, length, message);
        }
    }
    void GLDebug::PopDebugGroup()
    {
        if (this->func_glPopDebugGroupKHR != nullptr)
        {
            this->func_glPopDebugGroupKHR();
        }
        else if (this->func_glPopDebugGroup != nullptr)
        {
            this->func_glPopDebugGroup();
        }
    }


    void GLDebug::BeginRegion(const String& nameMarker, GLuint id)
    {
        PushDebugGroup(id, 0, -1, nameMarker.c_str());
    }
    void GLDebug::EndRegion()
    {
        PopDebugGroup();
    }

    void GLDebug::SetGLBufferName(uint32 id, const String& name)
    {
        ObjectLabel(GL_BUFFER, id, -1, name.c_str());
    }
    void GLDebug::SetGLVertexArrayName(uint32 id, const String& name)
    {
        ObjectLabel(GL_VERTEX_ARRAY, id, -1, name.c_str());
    }
    void GLDebug::SetGLBufferUniformName(uint32 id, const String& name)
    {
        ObjectLabel(GL_BUFFER, id, -1, name.c_str());
    }

    void GLDebug::SetGLShaderName(uint32 id, const String& name)
    {
        ObjectLabel(GL_SHADER, id, -1, name.c_str());
    }
    void GLDebug::SetGLShaderProgramName(uint32 id, const String& name)
    {
        ObjectLabel(GL_PROGRAM, id, -1, name.c_str());
    }

    void GLDebug::SetGLTextureName(uint32 id, const String& name)
    {
        ObjectLabel(GL_TEXTURE, id, -1, name.c_str());
    }
    void GLDebug::SetGLSamplerName(uint32 id, const String& name)
    {
        ObjectLabel(GL_SAMPLER, id, -1, name.c_str());
    }

    void GLDebug::SetGLRenderBuffer(uint32 id, const String& name)
    {
        ObjectLabel(GL_RENDERBUFFER, id, -1, name.c_str());
    }
    void GLDebug::SetGLFrameBuffer(uint32 id, const String& name)
    {
        ObjectLabel(GL_FRAMEBUFFER, id, -1, name.c_str());
    }

}; //LostPeterOpenGLES