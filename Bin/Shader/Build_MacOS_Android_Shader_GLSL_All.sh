#****************************************************************************
# LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2024-12-17
# Github:   https://github.com/LostPeter/LostPeterOpenGLES
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name_folder="glsl"
dirSrc="./$name_folder/sample"
dirDst="../Assets/Shader"

mkdir -p $dirDst
rm -rf $dirDst"/Android"

mkdir -p $dirDst"/Android"

function compile_shader() {
    for file in `ls $1`
    do 
        echo $1/$file
        ./Build_MacOS_Android_Shader_GLSL.sh $file $2
    done
}

function read_dir() {
    for file in `ls $1`
    do 
        if [ -d $1"/"$file ]; then
            read_dir $1"/"$file 
        else
            echo $1"/"$file 
        fi
    done
}


echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"
{
    compile_shader $dirSrc Android
}
echo "************** Shader Source .vert/.tesc/.tese/.geom/.frag/.comp **************"


echo "************** Shader Copy ****************************************************"
{
    read_dir $dirDst/Android
}
echo "************** Shader Copy ****************************************************"