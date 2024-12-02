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

name_sample=${1}
debug=${2}
rebuild=${3}

if [ "$debug" == "debug" ]; then
    name_apk=$name_sample"_d.apk"
else
    name_apk=$name_sample".apk"
fi

#Build Sample
rm -rf "../../Bin/MacOS/Android/"$name_apk
mkdir -p "../../Bin/MacOS/Android"

cd ..
cd $name_sample

if [ "$rebuild" == "rebuild" ]; then
    ./gradlew clean
fi

if [ "$debug" == "debug" ]; then
    ./gradlew assembleDebug
    cp -rfp "./app/build/outputs/apk/debug/app-debug.apk" "../../Bin/MacOS/Android/"$name_apk
else
    ./gradlew assembleRelease
    cp -rfp "./app/build/outputs/apk/release/app-release-unsigned.apk" "../../Bin/MacOS/Android/"$name_apk
fi

cd ..
cd Compile