#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-12-01
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

./Build_MacOS_Android_Sample.sh opengles_000_window $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_001_triangle $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_002_imgui $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_003_texture $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_004_model $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_005_camera $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_006_depth $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_007_stencil $debug $rebuild
./Build_MacOS_Android_Sample.sh opengles_008_blend $debug $rebuild
