#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-11-24
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

./Build_MacOS_Android_lib_glm.sh
./Build_MacOS_Android_lib_rapidxml.sh
./Build_MacOS_Android_lib_spdlog.sh
./Build_MacOS_Android_lib_stb.sh

./Build_MacOS_Android_lib_libcpuid.sh $debug $rebuild
./Build_MacOS_Android_lib_libnoise.sh $debug $rebuild
./Build_MacOS_Android_lib_libsquish.sh $debug $rebuild
./Build_MacOS_Android_lib_tinyxml.sh $debug $rebuild
./Build_MacOS_Android_lib_zlib.sh $debug $rebuild
./Build_MacOS_Android_lib_zziplib.sh $debug $rebuild
./Build_MacOS_Android_lib_glad.sh 3.2 $debug $rebuild
./Build_MacOS_Android_lib_imgui.sh $debug $rebuild
./Build_MacOS_Android_lib_assimp.sh $debug $rebuild
./Build_MacOS_Android_lib_zthread.sh $debug $rebuild
