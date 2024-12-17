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

#include "../include/GLESShaderInclude.h"
#include "../include/OpenGLESWindow.h"

template<> LostPeterOpenGLES::GLESShaderInclude* LostPeterFoundation::FSingleton<LostPeterOpenGLES::GLESShaderInclude>::ms_Singleton = nullptr;

namespace LostPeterOpenGLES
{
    GLESShaderInclude* GLESShaderInclude::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	GLESShaderInclude& GLESShaderInclude::GetSingleton()
	{  
		F_Assert(ms_Singleton && "GLESShaderInclude::GetSingleton")
		return (*ms_Singleton);     
	}


    String GLESShaderInclude::s_pathInclude = "Shader/Include/";
    String2StringMap GLESShaderInclude::s_mapIncludeKey2Content;
    GLESShaderInclude::GLESShaderInclude()
        : Base("ShaderInclude")
    {

    }
    GLESShaderInclude::~GLESShaderInclude()
    {
        Destroy();
    }

    void GLESShaderInclude::Destroy()
    {

    }

    bool GLESShaderInclude::Init()
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
                F_LogError("*********************** GLESShaderInclude::Init: Load glsl include file failed, path: [%s] !", pathFile.c_str());
                return false;
            }
            if (code.size() <= 0)
            {
                return false;
            }

            String nameKey = "/" + nameFile;
            s_mapIncludeKey2Content[nameKey] = code; 
            F_LogInfo("GLESShaderInclude::Init: Load glsl include file success, name: [%s], path: [%s] !", nameFile.c_str(), pathFile.c_str());
        } 

        return true;
    }

    void GLESShaderInclude::FindIncludes(const String& strSource, StringVector& aIncludeKeys)
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
                F_LogError("*********************** GLESShaderInclude::FindIncludes: glsl include format error, flag1: [%d], flag2: [%d] !", (int)flag1, (int)flag2);
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
    void GLESShaderInclude::ClearIncludes()
    {
        for (String2StringMap::iterator it = s_mapIncludeKey2Content.begin();
             it != s_mapIncludeKey2Content.end(); ++it)
        {
            const String& nameKey = it->first;
            Base::GetWindowPtr()->removeGLShaderGLSLInclude(nameKey);
        }
        s_mapIncludeKey2Content.clear();
    }

    bool GLESShaderInclude::ShaderInclude(const String& strName, 
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
                F_LogError("*********************** GLESShaderInclude::ShaderInclude: Can not find include key: [%s], source name: [%s] !", nameKey.c_str(), strName.c_str());
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
            F_LogInfo("GLESShaderInclude::ShaderInclude: shader source: [%s], contain include key: [%s] !", strName.c_str(), nameKey.c_str());
        }
        return true;
    }
    bool GLESShaderInclude::ShaderInclude(const String& strName, 
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