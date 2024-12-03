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


set name="zziplib-0.13.72"
set namewrap="zzipwraplib-0.13.72"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib="lib"%name%"_d.a"
    set name_lib_wrap="lib"%namewrap%"_d.a"
    set build_type="Debug"
) else (
    set name_project=%name%
    set name_lib="lib"%name%".a"
    set name_lib_wrap="lib"%namewrap%".a"
    set build_type="Release"
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
set NDKPATH=%NDK_ROOT%
echo ndk path: %NDKPATH%


set dirCur=%~dp0
set dirZlibInclude=%dirCur%..\Include\Android\zlib-1.2.11
set dirDependencyLib_d=%dirCur%..\Lib\Android\arm64-v8a\libzlib-1.2.11_d.a
set dirDependencyLib=%dirCur%..\Lib\Android\arm64-v8a\libzlib-1.2.11.a

echo %dirCur%
echo %dirZlibInclude%
echo %dirDependencyLib_d%
echo %dirDependencyLib%



cd ..
cd Build
cd Android
cd %name_armv8a%
cd %name_project%


if "%debug%" == "debug" (
    cmake -DDEBUG=1 -DZLIB_INCLUDE_DIR=%dirZlibInclude% -DZLIB_LIBRARY=%dirDependencyLib_d% ../../../../Sources/%name%/ -G "Visual Studio 16 2019" -A ARM64 -DCMAKE_TOOLCHAIN_FILE=%NDKPATH%/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%build_type% -DANDROID=1 -DANDROID_ABI=arm64-v8a -DANDROID_NDK=%NDKPATH% -DANDROID_NATIVE_API_LEVEL=%NDKABI_armv8a% -DANDROID_TOOLCHAIN=clang -DBUILD_SHARED_LIBS=0 -DANDROID_ARMV8A=1
    msbuild ./zziplib.sln /p:configuration=debug /p:platform=ARM64
    copy /Y ".\zzip\Debug\libzzip.a" "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib%
    copy /Y ".\zzipwrap\Debug\libzzipwrap.a" "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib_wrap%
) else (
    cmake -DZLIB_INCLUDE_DIR=%dirZlibInclude% -DZLIB_LIBRARY=%dirDependencyLib% ../../../../Sources/%name%/ -G "Visual Studio 16 2019" -A ARM64 -DCMAKE_TOOLCHAIN_FILE=%NDKPATH%/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%build_type% -DANDROID=1 -DANDROID_ABI=arm64-v8a -DANDROID_NDK=%NDKPATH% -DANDROID_NATIVE_API_LEVEL=%NDKABI_armv8a% -DANDROID_TOOLCHAIN=clang -DBUILD_SHARED_LIBS=0 -DANDROID_ARMV8A=1
    msbuild ./zziplib.sln /p:configuration=release /p:platform=ARM64
    copy /Y ".\zzip\Release\libzzip-0.a" "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib%
    copy /Y ".\zzipwrap\Release\libzzipwrap-0.a" "..\..\..\..\Lib\Android\"%name_armv8a%"\"%name_lib_wrap%
)


cd ..
cd ..
cd ..
cd ..
cd Compile


set include_folder="..\Include\Android\"%name%"\zzip"
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%
xcopy /S /E /Y /F "..\Sources\%name%\zzip\*.h" %include_folder%"\"
copy /Y "..\Build\Android\%name_armv8a%\%name_project%\zzip\_config.h" %include_folder%"\_config.h"


set include_folder_wrap="..\Include\Android\"%name%"\zzipwrap"
if exist %include_folder_wrap% (
    rmdir /S/Q %include_folder_wrap%
)
mkdir %include_folder_wrap%
xcopy /S /E /Y /F "..\Sources\%name%\zzipwrap\*.h" %include_folder_wrap%"\"