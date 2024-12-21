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

#ifndef _OPENGLES_004_MODEL_H_
#define _OPENGLES_004_MODEL_H_

#include "OpenGLESWindow.h"
using namespace LostPeterOpenGLES; 

class OpenGLES_004_Model : public OpenGLESWindow
{
public:
    OpenGLES_004_Model(String name);

public:
    std::vector<FVertex_Pos3Color4Tex2> vertices;
    std::vector<uint32_t> indices;

protected:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Default();

        virtual void buildObjectCB();

    //Render/Update
        virtual bool beginRenderImgui();
            virtual void modelConfig();
            
        virtual void endRenderImgui();

private:
    void resetSetting(int index);
    void changeModel(int index);

};


#endif