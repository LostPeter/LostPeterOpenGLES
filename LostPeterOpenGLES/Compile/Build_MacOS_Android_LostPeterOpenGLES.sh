#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-11-30
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}
rebuild=${2}

if [ "$debug" == "debug" ]; then
    name_project="LostPeterOpenGLES_d"
    name_dylib="libLostPeterOpenGL_d.dylib"
else
    name_project="LostPeterOpenGLES"
    name_dylib="libLostPeterOpenGL.dylib"
fi


#1> Build LostPeterFoundation
sh ./Build_MacOS_LostPeterFoundation.sh $debug $rebuild


#2> Build LostPeterOpenGLES
if [ "$rebuild" == "rebuild" ]; then
    rm -rf "../Build/MacOS/"$name_project
fi
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../Plugins/MacOS/"$name_dylib
mkdir -p "../../Plugins/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#dylib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../LostPeterOpenGLES/
else
    cmake ../../../LostPeterOpenGLES/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rfp "../Build/MacOS/"$name_project/$name_dylib "../../Plugins/MacOS/"$name_dylib