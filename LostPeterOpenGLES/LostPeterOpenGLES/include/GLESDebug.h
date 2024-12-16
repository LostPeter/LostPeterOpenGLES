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

#ifndef _GLES_DEBUG_H_
#define _GLES_DEBUG_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESDebug : public FSingleton<GLESDebug>
                                   , public Base
    {
    public:
        GLESDebug();
        ~GLESDebug();

    public:
        PFNGLDEBUGMESSAGECONTROLPROC func_glDebugMessageControl;
        PFNGLDEBUGMESSAGECALLBACKPROC func_glDebugMessageCallback;
        PFNGLDEBUGMESSAGEINSERTPROC func_glDebugMessageInsert;
        PFNGLOBJECTLABELPROC func_glObjectLabel;
        PFNGLGETOBJECTLABELPROC func_glGetObjectLabel;
        PFNGLPUSHDEBUGGROUPPROC func_glPushDebugGroup;
        PFNGLPOPDEBUGGROUPPROC func_glPopDebugGroup;

        PFNGLDEBUGMESSAGECONTROLKHRPROC func_glDebugMessageControlKHR;
        PFNGLDEBUGMESSAGECALLBACKKHRPROC func_glDebugMessageCallbackKHR;
        PFNGLDEBUGMESSAGEINSERTKHRPROC func_glDebugMessageInsertKHR;
        PFNGLOBJECTLABELKHRPROC func_glObjectLabelKHR;
        PFNGLGETOBJECTLABELKHRPROC func_glGetObjectLabelKHR;
        PFNGLPUSHDEBUGGROUPKHRPROC func_glPushDebugGroupKHR;
        PFNGLPOPDEBUGGROUPKHRPROC func_glPopDebugGroupKHR;

    public:
        static GLESDebug& GetSingleton();
		static GLESDebug* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    public:
        void DebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
        void DebugMessageCallback(GLDEBUGPROCKHR callback, const void* userParam);
        void DebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
        void ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
        void GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar* label);
        void PushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar* message);
        void PopDebugGroup();

    public:
        void BeginRegion(const String& nameMarker, GLuint id);
        void EndRegion();
        
        void SetGLBufferName(uint32 id, const String& name);
        void SetGLVertexArrayName(uint32 id, const String& name);
        void SetGLBufferUniformName(uint32 id, const String& name);

        void SetGLShaderName(uint32 id, const String& name);
        void SetGLShaderProgramName(uint32 id, const String& name);

        void SetGLTextureName(uint32 id, const String& name);
        void SetGLSamplerName(uint32 id, const String& name);

        void SetGLRenderBuffer(uint32 id, const String& name);
        void SetGLFrameBuffer(uint32 id, const String& name);
    };

}; //LostPeterOpenGLES

#endif