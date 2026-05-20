/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-03-18
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OPENGLES_010_LIGHTING_H_
#define _OPENGLES_010_LIGHTING_H_

#include "OpenGLESWindow.h"
using namespace LostPeterOpenGLES; 

class OpenGLES_010_Lighting : public OpenGLESWindow
{
public:
    OpenGLES_010_Lighting(String name);

public:
	struct ModelObject
	{
		ModelObject(OpenGLES_010_Lighting* _pWindow)
			//Common
			: pWindow(_pWindow)

			//Name
			, nameModel("")
			, pathModel("")
			, pathTexture("")
			, isShow(true)
			, isWireFrame(false)
			, isRotate(true)
			, isTransparent(false)

			//Vertex/Index
			, poVertexCount(0)
			, poVertexBuffer_Size(0)
			, poVertexBuffer_Data(nullptr)
			, poIndexCount(0)
			, poIndexBuffer_Size(0)
			, poIndexBuffer_Data(nullptr)
			, pBufferVertex(nullptr)
			, pBufferVertexIndex(nullptr)
			
			//Uniform
			, countInstanceExt(5)
            , countInstance(11)
			, poBufferUniform(nullptr)
			, poBufferUniform_Material(nullptr)
			
			//Texture
			, poTexture(nullptr)

			//Pipeline
			, poStatePipelineGraphics(nullptr)

			//State
			, poTypePrimitive(GL_TRIANGLES)
			, poIsCull(true)
			, poTypeFrontFace(GL_CW)
			, poTypeCulling(GL_BACK)
			, poTypePolygonMode(0)

			, poDepthEnabled(true)
			, poDepthFuncCompare(GL_LEQUAL)
			, poDepthTestEnabled(true)
			, poDepthWriteEnabled(true)

			, poStencilEnabled(true)
			, poStencil_CompareFunction(GL_LEQUAL)
			, poStencil_StencilFailureOp(GL_KEEP)
			, poStencil_DepthFailureOp(GL_KEEP)
			, poStencil_DepthStencilPassOp(GL_KEEP)
			, poStencil_Ref(0)
			, poStencil_Mask(0)
			
			, poBlendEnabled(false)
			, poBlendColorFactorSrc(GL_ONE)
			, poBlendColorFactorDst(GL_ZERO)
			, poBlendColorOp(GL_FUNC_ADD)
			, poBlendAlphaFactorSrc(GL_ONE)
			, poBlendAlphaFactorDst(GL_ZERO)
			, poBlendAlphaOp(GL_FUNC_ADD)

			, poColorWriteMask_Red(true)
			, poColorWriteMask_Green(true)
			, poColorWriteMask_Blue(true)
			, poColorWriteMask_Alpha(true)
		{
			
		}
		~ModelObject()
		{
			//Vertex/Index
			F_DELETE(this->pBufferVertex)
			F_DELETE(this->pBufferVertexIndex)
			//Uniform
			this->objectCBs.clear();
			F_DELETE(this->poBufferUniform)
			this->materialCBs.clear();
			F_DELETE(this->poBufferUniform_Material)

			//Texture
			F_DELETE(this->poTexture)

			//Pipeline
			F_DELETE(this->poStatePipelineGraphics)
		}

		//Common
		OpenGLES_010_Lighting* pWindow;

		//Name
		String nameModel;
		String pathModel;
		String pathTexture;
		bool isShow;
		bool isWireFrame;
		bool isRotate;
		bool isTransparent;

		//Vertex/Index
		std::vector<FVertex_Pos3Color4Normal3Tex2> vertices;
		uint32_t poVertexCount;
		size_t poVertexBuffer_Size;
		void* poVertexBuffer_Data;
		std::vector<uint32_t> indices;
		uint32_t poIndexCount;
		size_t poIndexBuffer_Size;
		void* poIndexBuffer_Data;
		GLESBufferVertex* pBufferVertex;
		GLESBufferVertexIndex* pBufferVertexIndex;

		//Uniform
		int countInstanceExt;
        int countInstance;

		std::vector<ObjectConstants> objectCBs;
		GLESBufferUniform* poBufferUniform;
		std::vector<FMatrix4> instanceMatWorld;

		std::vector<MaterialConstants> materialCBs;
		GLESBufferUniform* poBufferUniform_Material;

		//Texture
		GLESTexture* poTexture;

		//Pipeline
		GLESStatePipelineGraphics* poStatePipelineGraphics;

		//State
		GLenum poTypePrimitive;
		bool poIsCull;
		GLenum poTypeFrontFace;
		GLenum poTypeCulling;
		GLenum poTypePolygonMode;

		bool poDepthEnabled;
		GLenum poDepthFuncCompare;
		bool poDepthTestEnabled;
		bool poDepthWriteEnabled;

		bool poStencilEnabled;
		GLenum poStencil_CompareFunction;
		GLenum poStencil_StencilFailureOp;
		GLenum poStencil_DepthFailureOp;
		GLenum poStencil_DepthStencilPassOp;
		uint32_t poStencil_Ref;
		uint32_t poStencil_Mask;

		bool poBlendEnabled;
		GLenum poBlendColorFactorSrc; 
		GLenum poBlendColorFactorDst;
		GLenum poBlendColorOp;
		GLenum poBlendAlphaFactorSrc; 
		GLenum poBlendAlphaFactorDst;
		GLenum poBlendAlphaOp;

		GLboolean poColorWriteMask_Red;
		GLboolean poColorWriteMask_Green;
		GLboolean poColorWriteMask_Blue;
		GLboolean poColorWriteMask_Alpha;
	};
	typedef std::vector<ModelObject*> ModelObjectPtrVector;
	typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
	ModelObjectPtrVector m_aModelObjects;
	ModelObjectPtrVector m_aModelObjects_Render;
	ModelObjectPtrMap m_mapModelObjects;

	String nameDescriptorSetLayoutLocal;
	DescriptorSetLayout* pDescriptorSetLayoutLocal;

	GLESShaderPtrVector m_aGLESShaderModules;
    GLESShaderPtrMap m_mapGLESShaderModules;

protected:
	//Create Pipeline

	//Load Assets
	//Camera
	virtual void createCamera();

	//Geometry/Texture
	virtual void loadModel_Custom();
		bool loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal);
		bool loadModel_Texture(ModelObject* pModelObject);

	//ConstBuffers
	virtual void createCustomCB();

	//Pipeline
	virtual void createCustomBeforePipeline();
	virtual void createGraphicsPipeline_Custom();

	//DescriptorSets
	virtual void createDescriptorSets_Custom();

	//Render/Update
		virtual void updateCBs_Custom();

		virtual bool beginRenderImgui();
			virtual void modelConfig();
			
		virtual void endRenderImgui();

		virtual void drawMeshDefault_Custom();

	//cleanup
		virtual void cleanupCustom();

private:
    void rebuildInstanceCBs(bool isCreateBuffer);

	void destroyShaderModules();
    void createShaderModules();
    GLESShader* findShaderModule(const String& pathShaderModule);
};


#endif