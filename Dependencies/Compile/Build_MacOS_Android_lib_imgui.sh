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

# mode only can be gl3, gles3_android, gles3_ios, glfw_gl3, glfw_vulkan, metal
m_gl3="gl3"
m_gles3_android="gles3_android"
m_gles3_ios="gles3_ios"
m_metal="metal"
m_glfw_gl3="glfw_gl3"
m_glfw_vulkan="glfw_vulkan"
m_glfw_metal="glfw_metal"

mode=${m_gles3_android}
if [ [ "$mode" != "$m_gl3" ]\
    && [ "$mode" != "$m_gles3_android" ]\
    && [ "$mode" != "$m_gles3_ios" ]\
    && [ "$mode" != "$m_metal" ]\
    && [ "$mode" != "$m_glfw_gl3" ]\
    && [ "$mode" != "$m_glfw_vulkan" ]\
    && [ "$mode" != "$m_glfw_metal" ] ]; then

    echo "wrong mode type: ["$mode"], only can be [$m_gl3][$m_gles3_android][$m_gles3_ios][$m_metal][$m_glfw_gl3][$m_glfw_vulkan][$m_glfw_metal]"
    exit 1
fi
echo "mode: $mode, $debug"

name_base="imgui"
name="${name_base}-1.85"

if [ "$debug" == "debug" ]; then
    name_project=$name"_"$mode"_d"
    name_lib="lib"$name"_"$mode"_d.a"
    build_type="Debug"
    isDebug=1
else
    name_project=$name"_"$mode
    name_lib="lib"$name"_"$mode".a"
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
    -DPLATFORM_MODE=$mode \
    -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
    -DCMAKE_BUILD_TYPE=$build_type \
    -DANDROID=1 \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_NDK=${ANDROID_NDK} \
    -DANDROID_NATIVE_API_LEVEL=$NDKABI_armv8a \
    -DANDROID_TOOLCHAIN=clang 

make

# Copy
cp -rf "./libimgui-1.85_gles3_android.a" "../../../../Lib/Android/"$name_armv8a"/"$name_lib


cd ..
cd ..
cd ..
cd ..
cd Compile



folderSrc="../Sources/$name/$name_base"
folderDstRoot="../Include/Android/"$name"_"$mode
rm -rf $folderDstRoot
mkdir -p $folderDstRoot
folderDst=$folderDstRoot"/"$name_base
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rfp $file $folderDst
done

if [ "$mode" == "$m_gl3" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_gles3_android" ]; then 
    echo "mode: $mode"

    folderSrc_GlES3_Android="../Sources/"$name"/impl_gles3_android"
    folderDst_GlES3_Android="../Include/Android/"$name"_"$mode"/impl_gles3_android/"
    mkdir -p $folderDst_GlES3_Android

    for file in ${folderSrc_GlES3_Android}/*.h
    do 
        cp -rfp $file $folderDst_GlES3_Android
    done

elif [ "$mode" == "$m_gles3_ios" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_metal" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_glfw_gl3" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../Sources/"$name"/impl_glfw"
    floderSrc_GL3="../Sources/"$name"/impl_gl3"
    folderDst_Glfw="../Include/MacOS/"$name"_"$mode"/impl_glfw/"
    folderDst_GL3="../Include/MacOS/"$name"_"$mode"/impl_gl3/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_GL3

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rfp $file $folderDst_Glfw
    done

    for file in ${floderSrc_GL3}/*.h
    do 
        cp -rfp $file $folderDst_GL3
    done

elif [ "$mode" == "$m_glfw_vulkan" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../Sources/"$name"/impl_glfw"
    floderSrc_Vulkan="../Sources/"$name"/impl_vulkan"
    folderDst_Glfw="../Include/MacOS/"$name"_"$mode"/impl_glfw/"
    folderDst_Vulkan="../Include/MacOS/"$name"_"$mode"/impl_vulkan/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_Vulkan

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rfp $file $folderDst_Glfw
    done

    for file in ${floderSrc_Vulkan}/*.h
    do 
        cp -rfp $file $folderDst_Vulkan
    done

elif [ "$mode" == "$m_glfw_metal" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../Sources/"$name"/impl_glfw"
    floderSrc_Metal="../Sources/"$name"/impl_metal"
    folderDst_Glfw="../Include/MacOS/"$name"_"$mode"/impl_glfw/"
    folderDst_Metal="../Include/MacOS/"$name"_"$mode"/impl_metal/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_Metal

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rfp $file $folderDst_Glfw
    done

    for file in ${floderSrc_Metal}/*.h
    do 
        cp -rfp $file $folderDst_Metal
    done

else 
    echo "mode: $mode, unknown !"
    
fi
