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

name=zthread-2.3.2
name_lib_base="lib"$name".a"

if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    build_type="Debug"
else
    name_project=$name
    name_lib="lib"$name".a"
    build_type="Release"
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

cmake ../../../../Sources/$name/ \
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
cp -rf "./"$name_lib_base "../../../../Lib/Android/"$name_armv8a"/"$name_lib


cd ..
cd ..
cd ..
cd ..
cd Compile


folderSrc="../Sources/$name/include/zthread"
folderDst="../Include/Android/"$name"/zthread"
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done