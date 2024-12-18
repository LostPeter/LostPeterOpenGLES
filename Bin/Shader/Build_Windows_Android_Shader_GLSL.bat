@REM #****************************************************************************
@REM # LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author:   LostPeter
@REM # Time:     2024-12-17
@REM # Github:   https://github.com/LostPeter/LostPeterOpenGLES
@REM # Document: https://www.zhihu.com/people/lostpeter/posts
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
setlocal enabledelayedexpansion

set name_shader=%1
set folder_shader=%2

set folderSrc=.\glsl\sample
set folderDst=..\Assets\Shader
set fileDst=%folderDst%\%folder_shader%\%name_shader%.spv

copy %folderSrc%\%name_shader% %fileDst%

set version=#version
set lineSkip=0
for /f "delims=" %%i in (%fileDst%) do (
    set /a lineSkip+=1
    echo %%i| findstr %version% >nul && (
        goto abc
    )
)
:abc
echo "%version% is in line: %lineSkip%"

more +%lineSkip% "%fileDst%" > "%fileDst%.txt"
move /y %fileDst%.txt %fileDst%

endlocal