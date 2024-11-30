/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-16
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _GL_SHADER_INCLUDE_H_
#define _GL_SHADER_INCLUDE_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglExport GLShaderInclude : public FSingleton<GLShaderInclude>
                                       , public Base
    {
    public:
        GLShaderInclude();
        virtual ~GLShaderInclude();

    public:
        static String s_pathInclude;
        static String2StringMap s_mapIncludeKey2Content;
    
    public:
        static GLShaderInclude& GetSingleton();
		static GLShaderInclude* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    public:
        void FindIncludes(const String& strSource, StringVector& aIncludeKeys); 
        void ClearIncludes();

        bool ShaderInclude(const String& strName, 
                           const String& strSource, 
                           ConstCharPtrVector& aIncludePaths, 
                           Int32Vector& aIncludePathLengths);
        bool ShaderInclude(const String& strName, 
                           const String& strSource);
    };

}; //LostPeterOpenGLES

#endif