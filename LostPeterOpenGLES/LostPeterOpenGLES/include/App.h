/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _APP_H_
#define _APP_H_

#include "PreDefine.h"

namespace LostPeterOpenGLES
{
    class openglExport App
    {
    public:
        static GLFWwindow* s_pWindow;
        static OpenGLBase* s_pBase;

    public:
        static int Run(OpenGLBase* pBase);
        static GLFWwindow* GetGLFWwindow() { return s_pWindow; }
        
    protected:
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double x, double y);
        static void scroll_callback(GLFWwindow* window, double x, double y);
    };

}; //LostPeterOpenGLES

#endif