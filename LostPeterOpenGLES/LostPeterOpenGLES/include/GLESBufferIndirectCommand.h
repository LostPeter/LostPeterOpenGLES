/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-02
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _GLES_BUFFER_INDIRECT_COMMAND_H_
#define _GLES_BUFFER_INDIRECT_COMMAND_H_

#include "GLESBuffer.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESBufferIndirectCommand : public GLESBuffer
    {
    public:
        GLESBufferIndirectCommand(const String& nameBuffer);
        virtual ~GLESBufferIndirectCommand();

    public:
        std::vector<DrawArraysIndirectCommand> indirectDrawInstanceCommandCBs;
        std::vector<DrawElementsIndirectCommand> indirectDrawIndexedInstanceCommandCBs;
        GLenum eUsage;
			
        uint32 nBufferIndirectCommandID;

    public:
        virtual void Destroy();
        bool InitIndirectDrawInstance(GLenum usage,
						  			  int count);
        bool InitIndirectDrawIndexedInstance(GLenum usage,
											 int count);
        
    public:
		F_FORCEINLINE int GetBufferIndirectDrawInstanceSize() const { return (int)this->indirectDrawInstanceCommandCBs.size() * sizeof(DrawArraysIndirectCommand); }
        F_FORCEINLINE void* GetBufferIndirectDrawInstance() const { return (void*)this->indirectDrawInstanceCommandCBs.data(); }
        F_FORCEINLINE int GetBufferIndirectDrawIndexedInstanceSize() const { return (int)this->indirectDrawIndexedInstanceCommandCBs.size() * sizeof(DrawElementsIndirectCommand); }
        F_FORCEINLINE void* GetBufferIndirectDrawIndexedInstance() const { return (void*)this->indirectDrawIndexedInstanceCommandCBs.data(); }

    public:
        void UpdateBuffer();
        void UpdateBuffer(int index, const DrawArraysIndirectCommand& args);
        void UpdateBuffer(const std::vector<DrawArraysIndirectCommand>& args);
        void UpdateBuffer(int index, const DrawElementsIndirectCommand& args);
        void UpdateBuffer(const  std::vector<DrawElementsIndirectCommand>& args);

    public:
        void BindBufferIndirectCommand(); 
    };

}; //LostPeterOpenGLES

#endif