@REM #****************************************************************************
@REM # LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2024-11-24
@REM # Github:   https://github.com/LostPeter/LostPeterOpenGLES
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set debug=%1
set rebuild=%2

echo %debug%
echo %rebuild%

@REM mode only can be dx12, gl3, gles3_android, glfw_dx12, glfw_gl3, glfwVulkan
set m_dx12=dx12
set m_gl3=gl3
set m_gles3_android=gles3_android
set m_glfw_dx12=glfw_dx12
set m_glfw_gl3=glfw_gl3
set m_glfw_vulkan=glfw_vulkan

set mode=%m_gles3_android%
if %mode% neq %m_dx12% (
    if %mode% neq %m_gl3% (
        if %mode% neq %m_gles3_android% (
            if %mode% neq %m_glfw_dx12% (
                if %mode% neq %m_glfw_gl3% (
                    if %mode% neq %m_glfw_vulkan% (
                        echo "wrong mode type: [%mode%], only can be [%m_dx12%][%m_gl3%][%m_gles3_android%][%glfw_dx12%][%m_glfw_gl3%][%m_glfw_vulkan%]"
                        exit
                    )
                )     
            )
        )
    )
)
echo "mode: %mode%, %debug%"


set name_base=imgui
set name=%name_base%-1.85

if "%debug%" == "debug" (
    set name_project=%name%"_"%mode%"_d"
    set name_lib="lib"%name%"_"%mode%"_d.a"
    set build_type="Debug"
) else (
    set name_project=%name%"_"%mode%
    set name_lib="lib"%name%"_"%mode%".a"
    set build_type="Release"
)


@rem Plugins folder/file
set lib_folder="..\Lib\Android"
if not exist %lib_folder% (
    mkdir %lib_folder%
)


@REM ################################ Compile armv7a ################################




@REM ################################ Compile armv8a ################################
set name_armv8a="arm64-v8a"

@REM Build
set build_folder_armv8a="..\Build\Android\"%name_armv8a%"\"%name_project%
if exist %build_folder_armv8a% (
    if "%rebuild%" == "rebuild" (
        rmdir /S/Q %build_folder_armv8a%
    )
)
mkdir %build_folder_armv8a%

@REM Lib
set plugins_folder_armv8a=%plugins_folder%"\"%name_armv8a%
if not exist %plugins_folder_armv8a% (
    mkdir %plugins_folder_armv8a%
)
set plugins_libfile_armv8a=%plugins_folder_armv8a%"\"%name_lib%
if exist %plugins_libfile_armv8a% (
    del /S/Q %plugins_libfile_armv8a%
)

@REM NDK
set NDKABI_armv8a=21
set NDKPATH=%ANDROID_NDK%
echo ndk path: %NDKPATH%

cd ..
cd Build
cd Android
cd %name_armv8a%
cd %name_project%


if "%debug%" == "debug" (
    cmake -DDEBUG=1 -DPLATFORM_MODE=%mode% ../../../../Sources/%name%/ -G "Visual Studio 16 2019" -A ARM64 -DCMAKE_TOOLCHAIN_FILE=%NDKPATH%/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%build_type% -DANDROID=1 -DANDROID_ABI=arm64-v8a -DANDROID_NDK=%NDKPATH% -DANDROID_NATIVE_API_LEVEL=%NDKABI_armv8a% -DANDROID_TOOLCHAIN=clang -DBUILD_SHARED_LIBS=0 -DANDROID_ARMV8A=1
    msbuild "%name_project%".sln /p:configuration=debug /p:platform=ARM64
    copy /Y ".\Debug\"%name_lib% "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib%
) else (
    cmake -DPLATFORM_MODE=%mode% ../../../../Sources/%name%/ -G "Visual Studio 16 2019" -A ARM64 -DCMAKE_TOOLCHAIN_FILE=%NDKPATH%/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%build_type% -DANDROID=1 -DANDROID_ABI=arm64-v8a -DANDROID_NDK=%NDKPATH% -DANDROID_NATIVE_API_LEVEL=%NDKABI_armv8a% -DANDROID_TOOLCHAIN=clang -DBUILD_SHARED_LIBS=0 -DANDROID_ARMV8A=1
    msbuild "%name_project%".sln /p:configuration=release /p:platform=ARM64
    copy /Y ".\Release\"%name_lib% "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib%
)


cd ..
cd ..
cd ..
cd ..
cd Compile


set build_folder="..\Include\Android\"%name%"_"%mode%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
)
mkdir %build_folder%

xcopy /S /E /Y /F "..\Sources\%name%\%name_base%\*.h" %build_folder%"\"%name_base%"\"
if %mode% == %m_dx12% (
    xcopy /S /E /Y /F "..\Sources\%name%\impl_dx12\*.h" %build_folder%"\impl_dx12\"
    xcopy /S /E /Y /F "..\Sources\%name%\impl_win32\*.h" %build_folder%"\impl_win32\"
) else if %mode% == %m_gl3% (
    xcopy /S /E /Y /F "..\Sources\%name%\impl_gl3\*.h" %build_folder%"\impl_gl3\"
) else if %mode% == %m_gles3_android% (
    xcopy /S /E /Y /F "..\Sources\%name%\impl_gles3_android\*.h" %build_folder%"\impl_gles3_android\"
) else if %mode% == %m_glfw_dx12% (
    xcopy /S /E /Y /F "..\Sources\%name%\impl_glfw\*.h" %build_folder%"\impl_glfw\"
    xcopy /S /E /Y /F "..\Sources\%name%\impl_dx12\*.h" %build_folder%"\impl_dx12\"
    xcopy /S /E /Y /F "..\Sources\%name%\impl_win32\*.h" %build_folder%"\impl_win32\"
) else if %mode% == %m_glfw_gl3% (
    xcopy /S /E /Y /F "..\Sources\%name%\impl_glfw\*.h" %build_folder%"\impl_glfw\"
    xcopy /S /E /Y /F "..\Sources\%name%\impl_gl3\*.h" %build_folder%"\impl_gl3\"
) else if %mode% == %m_glfw_vulkan% (
    xcopy /S /E /Y /F "..\Sources\%name%\impl_glfw\*.h" %build_folder%"\impl_glfw\"
    xcopy /S /E /Y /F "..\Sources\%name%\impl_vulkan\*.h" %build_folder%"\impl_vulkan\"
)

@echo over