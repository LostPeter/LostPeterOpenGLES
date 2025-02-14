/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-12-01
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OPENGLES_001_TRIANGLE_H_
#define _OPENGLES_001_TRIANGLE_H_

#include "OpenGLESWindow.h"
using namespace LostPeterOpenGLES; 

class OpenGLES_001_Triangle : public OpenGLESWindow
{
public:
    OpenGLES_001_Triangle(String name);

public:
    std::vector<FVertex_Pos2Color4> vertices;

protected:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Custom();

public:
    

};


#endif