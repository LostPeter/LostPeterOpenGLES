# README #

Author: 

    LostPeter

Target: 

    Learning opengles render api on platform Android, compile in Windows/MacOS with CMake and code the samples !

Github Address:

    LostPeter GitHub: https://github.com/LostPeter/LostPeterOpenGLES

Document Web: 

    LostPeter 知乎: https://www.zhihu.com/people/lostpeter

Usage:

    OpenGLES SDK: 
        
    using gles3.2

    VSCode:
        I'm only using VSCode to edit, compile and debug code in platform Windows/MacOS.
        It is nice and effective to direct compile/distribute apk/debug in vscode.
    

    Build Auto: 
        MacOS:      ./Build_MacOS.sh debug rebuild depends
        Windows:    ./Build_Windows.bat debug rebuild depends

        param1: debug or other is release
        param2: rebuild or other is not rebuild
        param3: depends or other is not build depends


    Build Custom:
    1> Compile Dependencies
        cd Dependencies
        cd Compile
        MacOS:      ./Build_MacOS_Android_lib_all.sh debug rebuild
        Windows:    ./Build_Windows_Android_lib_all.bat debug rebuild

        used dependencies libraries：
            assimp-5.2.4
            glad-3.2
            glm-0.9.9.8
            imgui-1.85
            libcpuid-0.5.1
            libnoise-1.0.0
            libsquish-1.15
            rapidxml-1.13
            spdlog-1.10.0
            stb-0.02
            tinyxml-2.6.2
            zlib-1.2.11
            zziplib-0.13.72

    2> Compile Shader
        cd Bin
        cd Shader
        MacOS:      ./Build_MacOS_Android_Shader_GLSL_All.sh
        Windows:    ./Build_Windows_Android_Shader_GLSL_All.bat

        Always using glsl shader.

    3> Compile LostPeterOpenGLES
        cd LostPeterOpenGLES
        cd Compile
        MacOS:      ./Build_MacOS_Android_LostPeterOpenGLES.sh debug rebuild
        Windows:    ./Build_Windows_Android_LostPeterOpenGLES.bat debug rebuild

    4> Compile Samples
        cd Sample
        cd Compile
        MacOS:      ./Build_MacOS_Android_Sample_All.sh debug rebuild
        Windows:    ./Build_Windows_Android_Sample_All.bat debug rebuild

License:

    MIT License 


### <000> opengles_000_window
[OpenGLES学习例子000: opengles_000_window 基础窗口显示](https://zhuanlan.zhihu.com/p/19289843075)
* sample000：Setup a framework, compile all sort of dependencies libraries, show an Android Window 

![image](https://github.com/LostPeter/LostPeterOpenGLES/blob/main/Images/opengles_000_window.png)