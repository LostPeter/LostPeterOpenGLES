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

name=stb-0.02

folderSrc="../Sources/$name"
folderDst="../Include/Android/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done