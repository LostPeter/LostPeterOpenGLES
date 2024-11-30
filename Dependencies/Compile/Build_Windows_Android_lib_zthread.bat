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

set name=zthread-2.3.2
set name_lib_base=%name%".lib"

if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib="lib"%name%"_d.a"
) else (
    set name_project=%name%
    set name_lib="lib"%name%".a"
)


@rem Plugins folder/file
set plugins_folder="..\Lib\Android"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)


@REM ################################ Compile armv7a ################################



@REM ################################ Compile armv8a ################################
set name_armv8a="arm64-v8a"

@REM Build
set build_folder_armv8a="..\Build\Android\"%name_armv8a%"\"%name_project%
if exist %build_folder_armv8a% (
    rmdir /S/Q %build_folder_armv8a%
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
    cmake -DDEBUG=1 ../../../../Sources/%name%/ -G "Visual Studio 16 2019" -A ARM64 -DDEBUG=1 -DBUILD_SHARED_LIBS=0 -DANDROID=1 -DANDROID_NDK=%NDKPATH% -DANDROID_ARMV8A=1 -DPLATFORM_MODE=%mode% 
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_lib_base% "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib%
) else (
    cmake ../../../../Sources/%name%/ -G "Visual Studio 16 2019" -A ARM64 -DBUILD_SHARED_LIBS=0 -DANDROID=1 -DANDROID_NDK=%NDKPATH% -DANDROID_ARMV8A=1 -DPLATFORM_MODE=%mode% 
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_lib_base% "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib%
)


cd ..
cd ..
cd ..
cd ..
cd Compile


set include_folder="..\Include\Android\"%name%"\zthread"
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\Sources\%name%\include\zthread" %include_folder%"\"

@echo over