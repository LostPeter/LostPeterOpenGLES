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

name=zziplib-0.13.72
namewrap=zzipwraplib-0.13.72
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_lib_wrap="lib"$namewrap"_d.a"
    build_type="Debug"
    isDebug=1
else
    name_project=$name
    name_lib="lib"$name".a"
    name_lib_wrap="lib"$namewrap".a"
    build_type="Release"
    isDebug=0
fi


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
mkdir -p "../Lib/Android/"$name_armv8a
rm -rf "../Lib/Android/"$name_armv8a"/"$name_lib

# NDK
NDKABI_armv8a=21

cd ..
cd Build
cd Android
cd $name_armv8a
cd $name_project

cmake -DDEBUG=$isDebug ../../../../Sources/$name/ \
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
if [ "$debug" == "debug" ]; then
    cp -rfp "./zzip/libzzip.a" "../../../..//Lib/Android/"$name_armv8a"/"$name_lib
    cp -rfp "./zzipwrap/libzzipwrap.a" "../../../..//Lib/Android/"$name_armv8a"/"$name_lib_wrap
else
    cp -rfp "./zzip/libzzip-0.a" "../../../..//Lib/Android/"$name_armv8a"/"$name_lib
    cp -rfp "./zzipwrap/libzzipwrap-0.a" "../../../..//Lib/Android/"$name_armv8a"/"$name_lib_wrap
fi


cd ..
cd ..
cd ..
cd ..
cd Compile


folderSrc="../Sources/"$name"/zzip"
folderDst="../Include/Android/"$name"/zzip"
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rfp $file $folderDst
done
cp -rfp "../Build/Android/"$name_armv8a"/"$name_project"/zzip/_config.h" $folderDst


folderSrcWrap="../Sources/"$name"/zzipwrap"
folderDstWrap="../Include/Android/"$name"/zzipwrap"
rm -rf $folderDstWrap
mkdir -p $folderDstWrap

for file in ${folderSrcWrap}/*.h
do 
    cp -rfp $file $folderDstWrap
done