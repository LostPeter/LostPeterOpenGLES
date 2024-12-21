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

#ifndef _OPENGLES_003_TEXTURE_H_
#define _OPENGLES_003_TEXTURE_H_

#include "OpenGLESWindow.h"
using namespace LostPeterOpenGLES; 

class OpenGLES_003_Texture : public OpenGLESWindow
{
public:
    OpenGLES_003_Texture(String name);

public:
    std::vector<FVertex_Pos2Color4Tex2> vertices;

protected:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Custom();

    //Render/Update
        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

public:

};


#endif