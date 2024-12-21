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

#include "PreInclude.h"
#include "opengles_001_triangle.h"


OpenGLES_001_Triangle::OpenGLES_001_Triangle(String name)
    : OpenGLESWindow(name)
{
    
    this->poTypeVertex = F_MeshVertex_Pos2Color4;
    this->cfg_shaderVertex_Path = getShaderPathRelative("notrans_pos2_color4.vert.spv");
    this->cfg_shaderFragment_Path = getShaderPathRelative("notrans_pos2_color4.frag.spv");
}

void OpenGLES_001_Triangle::loadModel_Custom()
{
    //1> vertices
    this->vertices.push_back(FVertex_Pos2Color4(FVector2( 0.0f,  0.5f), FVector4(1.0f, 0.0f, 0.0f, 1.0f)));
    this->vertices.push_back(FVertex_Pos2Color4(FVector2( 0.5f, -0.5f), FVector4(0.0f, 1.0f, 0.0f, 1.0f)));
    this->vertices.push_back(FVertex_Pos2Color4(FVector2(-0.5f, -0.5f), FVector4(0.0f, 0.0f, 1.0f, 1.0f)));
    this->poVertexCount = (uint32_t)this->vertices.size();
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos2Color4);
    this->poVertexBuffer_Data = (uint8*)(&this->vertices[0]);
    this->poIndexCount = 0;
    this->poIndexBuffer_Size = 0;
    this->poIndexBuffer_Data = nullptr;
}