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

set name_folder=glsl
set folderSrc=.\%name_folder%\sample
set folderDst=..\Assets\Shader

if not exist %folderDst% (
    mkdir %folderDst%
)

if exist %folderDst%\Android (
    rmdir /S/Q %folderDst%\Android
)
mkdir %folderDst%\Android



echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"
call :buildShader %folderSrc% Android
echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"

echo "************** Shader Copy ****************************************************"
for %%i in (%folderDst%\Android\*.*) do echo %%i
echo "************** Shader Copy ****************************************************"

exit /b %errorlevel%
:buildShader
    echo folder is: %~1

	for /F %%i in ('Dir %~1\*.* /B') do (
		echo %~1\%%i

        call ./Build_Windows_Android_Shader_GLSL.bat %%i %~2
	) 

exit /b 0 
