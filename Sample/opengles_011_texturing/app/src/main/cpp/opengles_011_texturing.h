/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-05-19
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OPENGLES_011_TEXTURING_H_
#define _OPENGLES_011_TEXTURING_H_

#include "OpenGLESWindow.h"
using namespace LostPeterOpenGLES; 

class OpenGLES_011_Texturing : public OpenGLESWindow
{
public:
    OpenGLES_011_Texturing(String name);

public:
	/////////////////////////// ModelMesh ///////////////////////////
    struct ModelMesh
    {
        OpenGLES_011_Texturing* pWindow;
        String nameMesh;
        String pathMesh;
        FMeshType typeMesh;
        FMeshGeometryType typeGeometryType;

		//Vertex/Index
		FMeshVertexType poTypeVertex;
        std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
		uint32_t poVertexCount;
		size_t poVertexBuffer_Size;
		void* poVertexBuffer_Data;
		std::vector<uint32_t> indices;
		uint32_t poIndexCount;
		size_t poIndexBuffer_Size;
		void* poIndexBuffer_Data;
		GLESBufferVertex* pBufferVertex;
		GLESBufferVertexIndex* pBufferVertexIndex;


        ModelMesh(OpenGLES_011_Texturing* _pWindow, 
                  const String& _nameMesh,
                  const String& _pathMesh,
                  FMeshType _typeMesh,
                  FMeshGeometryType _typeGeometryType,
                  FMeshVertexType _poTypeVertex)
            : pWindow(_pWindow)
            , nameMesh(_nameMesh)
            , pathMesh(_pathMesh)
            , typeMesh(_typeMesh)
            , typeGeometryType(_typeGeometryType)

			//Vertex/Index
			, poTypeVertex(_poTypeVertex)
			, poVertexCount(0)
			, poVertexBuffer_Size(0)
			, poVertexBuffer_Data(nullptr)
			, poIndexCount(0)
			, poIndexBuffer_Size(0)
			, poIndexBuffer_Data(nullptr)
			, pBufferVertex(nullptr)
			, pBufferVertexIndex(nullptr)
        {

        }

        ~ModelMesh()
        {
            Destroy();
        }

        void Destroy()
        {
			//Vertex/Index
			F_DELETE(this->pBufferVertex)
			F_DELETE(this->pBufferVertexIndex)
        }


        bool LoadMesh(bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal);

    };
    typedef std::vector<ModelMesh*> ModelMeshPtrVector;
    typedef std::map<String, ModelMesh*> ModelMeshPtrMap;

    
    /////////////////////////// ModelObject /////////////////////////
	struct ModelObject
	{
		ModelObject(OpenGLES_011_Texturing* _pWindow)
			//Common
			: pWindow(_pWindow)

			//Name
			, nameObject("")
            , nameMesh("")
			, isShow(true)
			, isWireFrame(false)
			, isRotate(true)
			, isTransparent(false)
			, isLighting(true)

			//Mesh
            , pMesh(nullptr)
			
			//Uniform
			, countInstanceExt(5)
            , countInstance(11)
			, poBufferUniform(nullptr)
			, poBufferUniform_Material(nullptr)
			
			//Pipeline Graphics
			, poStatePipelineGraphics(nullptr)

            //Pipeline Computes

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

			, poStencilEnabled(false)
			, poStencil_CompareFunction(GL_LEQUAL)
			, poStencil_StencilFailureOp(GL_KEEP)
			, poStencil_DepthFailureOp(GL_KEEP)
			, poStencil_DepthStencilPassOp(GL_KEEP)
			, poStencil_Ref(0)
			, poStencil_Mask(0xFF)
			
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
			//Mesh
            this->pMesh = nullptr;

            //Texture
            this->mapModelTexturesShaderSort.clear();

            //Clean
            CleanupSwapChain();

			//Pipeline
			F_DELETE(this->poStatePipelineGraphics)
		}

		void CleanupSwapChain()
		{
			//Uniform
			F_DELETE(this->poBufferUniform)
			F_DELETE(this->poBufferUniform_Material)


		}

		void recreateSwapChain()
        {

        }

		//Common
		OpenGLES_011_Texturing* pWindow;

		//Name
		int indexModel;
		String nameObject;
        String nameMesh;
		bool isShow;
		bool isWireFrame;
		bool isRotate;
		bool isTransparent;
		bool isLighting;

		//Mesh
        ModelMesh* pMesh;

		//Texture
		GLESTexturePtrShaderSortMap mapModelTexturesShaderSort;

		//Uniform
		int countInstanceExt;
        int countInstance;

		std::vector<ObjectConstants> objectCBs;
		GLESBufferUniform* poBufferUniform;
		std::vector<FMatrix4> instanceMatWorld;

		std::vector<MaterialConstants> materialCBs;
		GLESBufferUniform* poBufferUniform_Material;

		//Pipeline
		GLESStatePipelineGraphics* poStatePipelineGraphics;

		//Pipeline Computes
        GLESStatePipelineComputePtrVector aPipelineComputes;

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

	////Mesh
        void SetMesh(ModelMesh* pMesh)
        {
            this->pMesh = pMesh;
        }
        ModelMesh* GetMesh()
        {
            return this->pMesh;
        }

    ////Textures
        void AddTexture(const String& nameShaderSort, GLESTexture* pTexture)
        {
            GLESTexturePtrVector* pVector = nullptr;
            GLESTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
            {
                GLESTexturePtrVector aMTs;
                this->mapModelTexturesShaderSort[nameShaderSort] = aMTs;
                itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            }
            itFind->second.push_back(pTexture);
        }
        GLESTexture* GetTexture(const String& nameShaderSort, int index)
        {
            GLESTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return itFind->second.at(index);
        }
        GLESTexturePtrVector* GetTextures(const String& nameShaderSort)
        {
            GLESTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return &(itFind->second);
        }

    ////Pipeline Computes
        void AddPipelineCompute(GLESStatePipelineCompute* pPipelineCompute)
        {
            this->aPipelineComputes.push_back(pPipelineCompute);
        }
        GLESStatePipelineCompute* GetPipelineCompute(int index)
        {
            F_Assert (index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObject::GetPipelineCompute")
            return this->aPipelineComputes[index];
        }
	};
	typedef std::vector<ModelObject*> ModelObjectPtrVector;
	typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
	ModelMeshPtrVector m_aModelMesh;
    ModelMeshPtrMap m_mapModelMesh;    

    GLESTexturePtrVector m_aModelTexture;
    GLESTexturePtrMap m_mapModelTexture;

	ModelObjectPtrVector m_aModelObjects;
	ModelObjectPtrVector m_aModelObjects_Render;
	ModelObjectPtrMap m_mapModelObjects;

	GLESShaderPtrVector m_aGLESShaderModules;
    GLESShaderPtrMap m_mapGLESShaderModules;

	DescriptorSetLayoutPtrVector m_aDescriptorSetLayouts;
    DescriptorSetLayoutPtrMap m_mapDescriptorSetLayouts;

protected:
	//Create Pipeline

	//Load Assets
		//Camera
		virtual void createCamera();

		//Geometry/Texture
		virtual void loadModel_Custom();

		//ConstBuffers
		virtual void createCustomCB();

		//Pipeline
		virtual void createCustomBeforePipeline();
		virtual void createGraphicsPipeline_Custom();
		virtual void createComputePipeline_Custom();

		//DescriptorSets
		virtual void createDescriptorSets_Custom();

		//Render/Update
			virtual void updateCBs_Custom();

			virtual bool beginRenderImgui();
				virtual void cameraReset();
				virtual void modelConfig();
				
			virtual void endRenderImgui();

			virtual void drawMeshDefault_Custom();

		//cleanup
			virtual void cleanupCustom();

			virtual void cleanupSwapChain_Custom();
			virtual void recreateSwapChain_Custom();

private:
    void rebuildInstanceCBs(bool isCreateBuffer);

////ModelMesh
    void destroyMeshes();
    void createMeshes();
    ModelMesh* findMesh(const String& nameMesh);

////Texture
    void destroyTextures();
    void createTextures();
    GLESTexture* findTexture(const String& nameTexture);

////ShaderModule
	void destroyShaderModules();
    void createShaderModules();
    GLESShader* findShaderModule(const String& pathShaderModule);

////DescriptorSetLayout
	void destroyDescriptorSetLayouts();
	void createDescriptorSetLayouts();
	DescriptorSetLayout* findDescriptorSetLayout(const String& nameDescriptorSetLayout);
	
};


#endif