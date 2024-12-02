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

if "%debug%" == "debug" (
    set name_apk=%name_sample%"_d.apk"
) else (
    set name_apk=%name_sample%".apk"
)
set path_apk="../../Bin/Windows/Android/"%name_apk%

@REM install
adb install %path_apk%

@REM run
adb shell am start -a android.intent.action.MAIN -n com.lostpeter.opengles.%name_sample%/com.lostpeter.opengles.%name_sample%.MainActivity

@echo over