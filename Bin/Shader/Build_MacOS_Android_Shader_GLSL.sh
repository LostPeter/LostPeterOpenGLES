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

name_shader=${1}
folder_shader=${2}

folderSrc="./glsl/sample"
folderDst="../Assets/Shader"
fileDst=$folderDst/$folder_shader/$name_shader.spv

cp -rf $folderSrc/$name_shader $fileDst

version=#version
lineSkip=0
while IFS= read -r line || [[ -n ${line} ]]
do
    # echo "$line"
    if [[ $line =~ $version ]]; then
        break
    fi
    ((lineSkip++))
done < "$fileDst"
# echo "$version is in line: $lineSkip"

start=1
end=$lineSkip
sed -i "" "${start},${end}"'d' $fileDst