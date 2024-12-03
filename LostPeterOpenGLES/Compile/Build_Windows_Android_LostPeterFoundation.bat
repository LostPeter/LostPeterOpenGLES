@REM #****************************************************************************
@REM # LostPeterFoundation - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2024-11-30
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

if "%debug%" == "debug" (
    set name_project="LostPeterFoundation_d"
    set name_lib="libLostPeterFoundation_d.so"
    set build_type="Debug"
    set isDebug=1
) else (
    set name_project="LostPeterFoundation"
    set name_lib="libLostPeterFoundation.so"
    set build_type="Release"
    set isDebug=0
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
set plugins_folder_armv8a="..\..\Plugins\Android\"%name_armv8a%
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

cd ..
cd Build
cd Android
cd %name_armv8a%
cd %name_project%


if "%debug%" == "debug" (
    cmake -DDEBUG=1 ../../../../LostPeterFoundation/ -G "Visual Studio 16 2019" -A ARM64 -DCMAKE_TOOLCHAIN_FILE=%NDKPATH%/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%build_type% -DANDROID=1 -DANDROID_ABI=arm64-v8a -DANDROID_NDK=%NDKPATH% -DANDROID_NATIVE_API_LEVEL=%NDKABI_armv8a% -DANDROID_TOOLCHAIN=clang -DANDROID_STL=c++_shared -DANDROID_CPP_FEATURES="rtti exceptions"
    msbuild "%name_project%".sln /p:configuration=debug /p:platform=ARM64
    copy /Y ".\Debug\%name_lib%" "..\..\..\..\..\Plugins\Android\"%name_armv8a%"\"%name_lib%
) else (
    cmake ../../../../LostPeterFoundation/ -G "Visual Studio 16 2019" -A ARM64 -DCMAKE_TOOLCHAIN_FILE=%NDKPATH%/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%build_type% -DANDROID=1 -DANDROID_ABI=arm64-v8a -DANDROID_NDK=%NDKPATH% -DANDROID_NATIVE_API_LEVEL=%NDKABI_armv8a% -DANDROID_TOOLCHAIN=clang -DANDROID_STL=c++_shared -DANDROID_CPP_FEATURES="rtti exceptions"
    msbuild %name_project%.sln /p:configuration=release /p:platform=ARM64
    copy /Y ".\Release\%name_lib%" "..\..\..\..\..\Plugins\Android\"%name_armv8a%"\"%name_lib%
)


cd ..
cd ..
cd ..
cd ..
cd Compile

@echo over