@REM #****************************************************************************
@REM # LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2024-12-02
@REM # Github:   https://github.com/LostPeter/LostPeterOpenGLES
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set name_sample=%1

cd ..
cd %name_sample%

call .\gradlew.bat clean

if exist "./app/src/main/assets" (
    rmdir /S/Q "./app/src/main/assets"
)
if exist "./app/.cxx" (
    rmdir /S/Q "./app/.cxx"
)

cd ..
cd Compile


@echo over