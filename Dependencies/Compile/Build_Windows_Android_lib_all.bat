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

set debug=%1
set rebuild=%2

call ./Build_Windows_Android_lib_glm.bat
call ./Build_Windows_Android_lib_rapidxml.bat
call ./Build_Windows_Android_lib_spdlog.bat
call ./Build_Windows_Android_lib_stb.bat

call ./Build_Windows_Android_lib_libcpuid.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_libnoise.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_libsquish.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_tinyxml.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_zlib.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_zziplib.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_glad.bat 3.2 %debug% %rebuild%
call ./Build_Windows_Android_lib_imgui.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_assimp.bat %debug% %rebuild%
call ./Build_Windows_Android_lib_zthread.bat %debug% %rebuild%