#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-12-02
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_sample=${1}
debug=${2}

if [ "$debug" == "debug" ]; then
    name_apk=$name_sample"_d.apk"
else
    name_apk=$name_sample".apk"
fi
path_apk="../../Bin/MacOS/Android/"$name_apk

# install
adb install $path_apk

# run
adb shell am start -a android.intent.action.MAIN -n com.lostpeter.opengles.$name_sample/com.lostpeter.opengles.$name_sample.MainActivity