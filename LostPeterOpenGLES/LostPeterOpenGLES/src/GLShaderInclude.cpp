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

#include "../include/GLShaderInclude.h"
#include "../include/OpenGLWindow.h"

template<> LostPeterOpenGLES::GLShaderInclude* LostPeterFoundation::FSingleton<LostPeterOpenGLES::GLShaderInclude>::ms_Singleton = nullptr;

namespace LostPeterOpenGLES
{
    GLShaderInclude* GLShaderInclude::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	GLShaderInclude& GLShaderInclude::GetSingleton()
	{  
		F_Assert(ms_Singleton && "GLShaderInclude::GetSingleton")
		return (*ms_Singleton);     
	}


    String GLShaderInclude::s_pathInclude = "Assets/Shader/Include/";
    String2StringMap GLShaderInclude::s_mapIncludeKey2Content;
    GLShaderInclude::GLShaderInclude()
        : Base("ShaderInclude")
    {

    }
    GLShaderInclude::~GLShaderInclude()
    {
        Destroy();
    }

    void GLShaderInclude::Destroy()
    {

    }

    bool GLShaderInclude::Init()
    {
        String pathFolder = Base::GetWindowPtr()->GetAssetFullPath(s_pathInclude);
        StringVector aFiles;
        FUtil::EnumFiles(pathFolder, aFiles, false, false);

        s_mapIncludeKey2Content.clear();
        size_t count = aFiles.size();
        for (size_t i = 0; i < count; i++)
        {
            String nameFile = aFiles[i];
            String pathFile = s_pathInclude + nameFile;

            String code;
            if (!FUtil::LoadAssetFileToString(pathFile.c_str(), code))
            {
                F_LogError("*********************** GLShaderInclude::Init: Load glsl include file failed, path: [%s] !", pathFile.c_str());
                return false;
            }
            if (code.size() <= 0)
            {
                return false;
            }

            String nameKey = "/" + nameFile;
            s_mapIncludeKey2Content[nameKey] = code; 
            F_LogInfo("GLShaderInclude::Init: Load glsl include file success, name: [%s], path: [%s] !", nameFile.c_str(), pathFile.c_str());
        } 

        return true;
    }

    void GLShaderInclude::FindIncludes(const String& strSource, StringVector& aIncludeKeys)
    {
        static const String c_strInclude = "#include";
        static const size_t c_sizeInclude = c_strInclude.size();
        size_t offset = 0;
        size_t index = strSource.find(c_strInclude, offset);
        while (index != -1)
        {
            size_t flag1 = strSource.find("\"", index + c_sizeInclude);
            size_t flag2 = strSource.find("\"", flag1 + 1);

            if (flag1 == -1 || flag2 == -1)
            {
                F_LogError("*********************** GLShaderInclude::FindIncludes: glsl include format error, flag1: [%d], flag2: [%d] !", flag1, flag2);
            }
            else
            {
                String nameKey = strSource.substr(flag1 + 1, flag2 - flag1 - 1);
                aIncludeKeys.push_back(nameKey);
            }
            offset = flag2;
            index = strSource.find(c_strInclude, offset);
        }
    }
    void GLShaderInclude::ClearIncludes()
    {
        for (String2StringMap::iterator it = s_mapIncludeKey2Content.begin();
             it != s_mapIncludeKey2Content.end(); ++it)
        {
            const String& nameKey = it->first;
            Base::GetWindowPtr()->removeGLShaderGLSLInclude(nameKey);
        }
        s_mapIncludeKey2Content.clear();
    }

    bool GLShaderInclude::ShaderInclude(const String& strName, 
                                        const String& strSource, 
                                        ConstCharPtrVector& aIncludePaths, 
                                        Int32Vector& aIncludePathLengths)
    {
        StringVector aIncludeKeys;
        FindIncludes(strSource, aIncludeKeys);
        size_t count = aIncludeKeys.size();
        if (count <= 0)
            return true;
        
        for (size_t i = 0; i < count; i++)
        {
            String& nameKey = aIncludeKeys[i];
            String2StringMap::iterator itFind = s_mapIncludeKey2Content.find(nameKey);
            if (itFind == s_mapIncludeKey2Content.end())
            {
                F_LogError("*********************** GLShaderInclude::ShaderInclude: Can not find include key: [%s], source name: [%s] !", nameKey.c_str(), strName.c_str());
                return false;
            }
            const String& strContent = itFind->second;
            Base::GetWindowPtr()->addGLShaderGLSLInclude(nameKey, strContent);

            ShaderInclude(nameKey, 
                          strContent,
                          aIncludePaths,
                          aIncludePathLengths);

            aIncludePaths.push_back(nameKey.c_str());
            aIncludePathLengths.push_back((int)nameKey.size());
            F_LogInfo("GLShaderInclude::ShaderInclude: shader source: [%s], contain include key: [%s] !", strName.c_str(), nameKey.c_str());
        }
        return true;
    }
    bool GLShaderInclude::ShaderInclude(const String& strName, 
                                        const String& strSource)
    {
        ConstCharPtrVector aIncludePaths;
        Int32Vector aIncludePathLengths;
        return ShaderInclude(strName,
                             strSource,
                             aIncludePaths,
                             aIncludePathLengths);
    }

}; //LostPeterOpenGLES