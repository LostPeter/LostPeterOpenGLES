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

#include "../include/GLESBufferIndirectCommand.h"
#include "../include/OpenGLESWindow.h"

namespace LostPeterOpenGLES
{
    GLESBufferIndirectCommand::GLESBufferIndirectCommand(const String& nameBuffer)
        : GLESBuffer(nameBuffer)

		, eUsage(GL_STATIC_DRAW)
		
		, nBufferIndirectCommandID(0)
    {

    }
    GLESBufferIndirectCommand::~GLESBufferIndirectCommand()
    {
        Destroy();
    }

    void GLESBufferIndirectCommand::Destroy()
    {
		Base::GetWindowPtr()->destroyGLBufferIndirectCommand(this->nBufferIndirectCommandID);
        this->indirectDrawInstanceCommandCBs.clear();
        this->indirectDrawIndexedInstanceCommandCBs.clear();
    }

	bool GLESBufferIndirectCommand::InitIndirectDrawInstance(GLenum usage,
														     int count)
	{
		this->eUsage = usage;
        this->indirectDrawInstanceCommandCBs.resize(count);
        this->indirectDrawIndexedInstanceCommandCBs.clear();

		if (!Base::GetWindowPtr()->createGLBufferIndirectCommand(GetName(),
																 usage,
																 sizeof(DrawArraysIndirectCommand) * count,
																 (uint8*)(this->indirectDrawInstanceCommandCBs.data()), 
																 this->nBufferIndirectCommandID))
        {
            F_LogError("*********************** GLESBufferIndirectCommand::InitIndirectDrawInstance: Failed to create buffer indirect command: [%s] !", GetName().c_str());
            return false;
        }

		return true;
	}
    bool GLESBufferIndirectCommand::InitIndirectDrawIndexedInstance(GLenum usage,
																    int count)
	{
		this->eUsage = usage;
        this->indirectDrawInstanceCommandCBs.clear(); 
        this->indirectDrawIndexedInstanceCommandCBs.resize(count);

		if (!Base::GetWindowPtr()->createGLBufferIndirectCommand(GetName(),
																 usage,
																 sizeof(DrawElementsIndirectCommand) * count,
																 (uint8*)(this->indirectDrawIndexedInstanceCommandCBs.data()), 
																 this->nBufferIndirectCommandID))
        {
            F_LogError("*********************** GLESBufferIndirectCommand::InitIndirectDrawIndexedInstance: Failed to create buffer indirect command: [%s] !", GetName().c_str());
            return false;
        }

		return true;
	}

	void GLESBufferIndirectCommand::UpdateBuffer()
	{
		if (this->indirectDrawInstanceCommandCBs.size() > 0) {
            Base::GetWindowPtr()->updateGLBufferIndirectCommand(0,
																sizeof(DrawArraysIndirectCommand) * this->indirectDrawInstanceCommandCBs.size(),
																(uint8*)(this->indirectDrawInstanceCommandCBs.data()),
																this->nBufferIndirectCommandID);
        } else if (this->indirectDrawIndexedInstanceCommandCBs.size() > 0) {
            Base::GetWindowPtr()->updateGLBufferIndirectCommand(0, 
																sizeof(DrawElementsIndirectCommand) * this->indirectDrawIndexedInstanceCommandCBs.size(),
																(uint8*)(this->indirectDrawIndexedInstanceCommandCBs.data()),
																this->nBufferIndirectCommandID);
        }
	}
	void GLESBufferIndirectCommand::UpdateBuffer(int index, const DrawArraysIndirectCommand& args)
	{
		this->indirectDrawInstanceCommandCBs[index] = args;
        UpdateBuffer();
	}
	void GLESBufferIndirectCommand::UpdateBuffer(const std::vector<DrawArraysIndirectCommand>& args)
	{
		F_Assert(args.size() <= this->indirectDrawInstanceCommandCBs.size() && "GLESBufferIndirectCommand::UpdateBuffer")
        for (size_t i = 0; i < args.size(); i++)
        {
            this->indirectDrawInstanceCommandCBs[i] = args[i];
        }
        UpdateBuffer();
	}
	void GLESBufferIndirectCommand::UpdateBuffer(int index, const DrawElementsIndirectCommand& args)
	{	
		this->indirectDrawIndexedInstanceCommandCBs[index] = args;
        UpdateBuffer();
	}
	void GLESBufferIndirectCommand::UpdateBuffer(const  std::vector<DrawElementsIndirectCommand>& args)
	{
		F_Assert(args.size() <= this->indirectDrawIndexedInstanceCommandCBs.size() && "GLESBufferIndirectCommand::UpdateBuffer")
        for (size_t i = 0; i < args.size(); i++)
        {
            this->indirectDrawIndexedInstanceCommandCBs[i] = args[i];
        }
        UpdateBuffer();
	}

	void GLESBufferIndirectCommand::BindBufferIndirectCommand()
	{
		Base::GetWindowPtr()->bindGLBufferIndirectCommand(this->nBufferIndirectCommandID);
	}

}; //LostPeterOpenGLES