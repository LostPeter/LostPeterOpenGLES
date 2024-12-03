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
set debug=%2
set rebuild=%3

echo %name_sample%
echo %debug%
echo %rebuild%

@REM Build Sample
if "%debug%" == "debug" (
    set name_apk=%name_sample%"_d.apk"
) else (
    set name_apk=%name_sample%".apk"
)

@REM Build Sample
set bin_file="..\..\Bin\Windows\Android\"%name_apk%
if exist %bin_file% (
    del /S/Q %bin_file%
)
if not exist "..\..\Bin\Windows\Android" (
    mkdir "..\..\Bin\Windows\Android"
)

cd ..
cd %name_sample%

if "%rebuild%" == "rebuild" (
    call .\gradlew.bat clean
)

if "%debug%" == "debug" (
    call .\gradlew.bat assembleDebug
    copy /Y ".\app\build\outputs\apk\debug\app-debug.apk" "..\..\Bin\Windows\Android\"%name_apk%
) else (
    call .\gradlew.bat assembleRelease
    copy /Y ".\app\build\outputs\apk\release\app-release-unsigned.apk" "..\..\Bin\Windows\Android\"%name_apk%
)

cd ..
cd Compile

@echo over