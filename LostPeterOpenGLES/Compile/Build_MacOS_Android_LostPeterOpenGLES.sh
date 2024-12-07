#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-12-08
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

if [ "$debug" == "debug" ]; then
    name_project="LostPeterOpenGLES_d"
    name_lib="libLostPeterOpenGLES_d.so"
    build_type="Debug"
    isDebug=1
else
    name_project="LostPeterOpenGLES"
    name_lib="libLostPeterOpenGLES.so"
    build_type="Release"
    isDebug=0
fi


#1> Build LostPeterFoundation
sh ./Build_MacOS_Android_LostPeterFoundation.sh $debug $rebuild


#2> Build LostPeterOpenGLES
################################ Compile armv7a ################################



################################ Compile armv8a ################################
name_armv8a="arm64-v8a"
# Build
mkdir -p "../Build/Android/"$name_armv8a 
if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/Android/"$name_armv8a"/"$name_project
fi
mkdir -p "../Build/Android/"$name_armv8a"/"$name_project

# Lib
mkdir -p ../../Plugins/Android/$name_armv8a
rm -rf "../../Plugins/Android/"$name_armv8a"/"$name_lib

# NDK
NDKABI_armv8a=21

cd ..
cd Build
cd Android
cd $name_armv8a
cd $name_project


cmake -DDEBUG=$isDebug ../../../../LostPeterOpenGLES/ \
    -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
    -DCMAKE_BUILD_TYPE=$build_type \
    -DANDROID=1 \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_NDK=${ANDROID_NDK} \
    -DANDROID_NATIVE_API_LEVEL=$NDKABI_armv8a \
    -DANDROID_TOOLCHAIN=clang \
    -DBUILD_SHARED_LIBS=0 \
    -DANDROID_ARMV8A=1
make


# Copy
cp -rf "./"$name_lib "../../../../../Plugins/Android/"$name_armv8a"/"$name_lib


cd ..
cd ..
cd ..
cd ..
cd Compile