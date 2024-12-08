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

set debug=%1
set rebuild=%2

call ./Build_Windows_Android_Sample.bat opengles_000_window %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_001_triangle %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_002_imgui %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_003_texture %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_004_model %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_005_camera %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_006_depth %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_007_stencil %debug% %rebuild%
call ./Build_Windows_Android_Sample.bat opengles_008_blend %debug% %rebuild%