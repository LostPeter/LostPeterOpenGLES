/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-02
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OPENGLES_013_INDIRECTDRAW_H_
#define _OPENGLES_013_INDIRECTDRAW_H_

#include "OpenGLESWindow.h"
using namespace LostPeterOpenGLES; 

class OpenGLES_013_IndirectDraw : public OpenGLESWindow
{
public:
    OpenGLES_013_IndirectDraw(String name);

public:
	/////////////////////////// ModelObjectRend /////////////////////
    struct ModelObject;
    struct ModelObjectRend
    {
		ModelObjectRend(const String& _nameObjectRend,
                        ModelObject* _pModelObject,
                        MeshSub* _pMeshSub,
                        int _indexRend)
            : nameObjectRend(_nameObjectRend)
            , pModelObject(_pModelObject)
            , pMeshSub(_pMeshSub)
            , indexRend(_indexRend)
            , isShow(true)
            , isWireFrame(false)
            , isRotate(false)
            , isLighting(true)
            , isTransparent(false)

            //Uniform
			, countInstanceExt(0)
            , countInstance(1)

			, poBufferUniform_Object(nullptr)
			, poBufferUniform_Material(nullptr)
			, poBufferUniform_Offset(nullptr)
			, isUsedIndirectDraw(false)
			, poBufferUniform_Tessellation(nullptr)
            , isUsedTessellation(false)

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
        ~ModelObjectRend()
        {
            //MeshSub
            this->pMeshSub = nullptr;

			//Uniform
			this->objectCBs.clear();
			this->materialCBs.clear();
			this->tessellationCBs.clear();

            //Texture
            this->mapModelTexturesShaderSort.clear();

            //Clean
            CleanupSwapChain();
        }

        void CleanupSwapChain()
        {
            //Uniform
            F_DELETE(this->poBufferUniform_Object)
			F_DELETE(this->poBufferUniform_Material)
			F_DELETE(this->poBufferUniform_Offset)
			F_DELETE(this->poBufferUniform_Tessellation)

            //Pipeline Graphics
            F_DELETE(this->poStatePipelineGraphics)

            //Pipeline Computes
            size_t count = this->aPipelineComputes.size();
            for (size_t i = 0; i < count; i++)
            {
                F_DELETE(this->aPipelineComputes[i])
            }
			this->aPipelineComputes.clear();
        }

        void RecreateSwapChain()
        {

        }   

        String nameObjectRend;
        ModelObject* pModelObject;
        MeshSub* pMeshSub;
        int indexRend;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;
        bool isTransparent;

		//Texture
		GLESTexturePtrShaderSortMap mapModelTexturesShaderSort;

		//Uniform
		int countInstanceExt;
        int countInstance;

		std::vector<ObjectConstants> objectCBs;
		GLESBufferUniform* poBufferUniform_Object;
		std::vector<FMatrix4> instanceMatWorld;

		std::vector<MaterialConstants> materialCBs;
		GLESBufferUniform* poBufferUniform_Material;

		ValueUIntConstants offsetCBs;
		GLESBufferUniform* poBufferUniform_Offset;
		bool isUsedIndirectDraw;

		std::vector<TessellationConstants> tessellationCBs;
		GLESBufferUniform* poBufferUniform_Tessellation;
        bool isUsedTessellation;

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
    typedef std::vector<ModelObjectRend*> ModelObjectRendPtrVector;
    typedef std::map<String, ModelObjectRend*> ModelObjectRendPtrMap;


    /////////////////////////// ModelObjectRendIndirect /////////////
    struct ModelObjectRendIndirect
    {
		ModelObjectRendIndirect(const String& _nameObjectRendIndirect)
            : nameObjectRendIndirect(_nameObjectRendIndirect)
            , pRend(nullptr)

            , isShow(true)
            , isWireFrame(false)
            , isRotate(false)
            , isLighting(true)
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
			, poBufferUniform_Object(nullptr)
			, poBufferUniform_Material(nullptr)
			, poBufferUniform_Offset(nullptr)
			, slotUBO(4)
			, poBufferUniform_Tessellation(nullptr)

			//Pipeline Graphics
			, poStatePipelineGraphics(nullptr)

            //IndirectCommand
            , countIndirectDraw(0)
            , poBuffer_IndirectCommand(nullptr)
        {
            
        }

        ~ModelObjectRendIndirect()
        {
            Destroy();
        }

        void Destroy()
		{
			//Vertex/Index
			F_DELETE(this->pBufferVertex)
			F_DELETE(this->pBufferVertexIndex)

			CleanupSwapChain();

			this->aRends.clear();
			this->aMeshSubs.clear();
			this->pRend = nullptr;
		}

        void CleanupSwapChain()
		{
			//Uniform
			F_DELETE(this->poBufferUniform_Object)
			F_DELETE(this->poBufferUniform_Material)
			F_DELETE(this->poBufferUniform_Offset)
			F_DELETE(this->poBufferUniform_Tessellation)

			F_DELETE(this->poBuffer_IndirectCommand)

			F_DELETE(this->poStatePipelineGraphics)
		}

        void RecreateSwapChain()
        {

        }   


        String nameObjectRendIndirect;
        ModelObjectRendPtrVector aRends;
        MeshSubPtrVector aMeshSubs;
        ModelObjectRend* pRend;

        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;
        bool isTransparent;

        //Vertex
		FMeshVertexType poTypeVertex;
        std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
        size_t poVertexCount;
		size_t poVertexBuffer_PerVertexSize;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;
        std::vector<uint32_t> indices;
        size_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;
        GLESBufferVertex* pBufferVertex;
		GLESBufferVertexIndex* pBufferVertexIndex;

        //Uniform
        std::vector<ObjectConstants> objectCBs;
		GLESBufferUniform* poBufferUniform_Object;

        std::vector<MaterialConstants> materialCBs;
		GLESBufferUniform* poBufferUniform_Material;

		uint32 slotUBO;
		std::vector<ValueUIntConstants> offsetCBs;
		GLESBufferUniform* poBufferUniform_Offset;

        std::vector<TessellationConstants> tessellationCBs;
        GLESBufferUniform* poBufferUniform_Tessellation;

		//Pipeline Graphics
		GLESStatePipelineGraphics* poStatePipelineGraphics;

        //IndirectCommand 
        uint32_t countIndirectDraw;
		std::vector<DrawElementsIndirectCommand> indirectDrawIndexedInstanceCommandCBs;
		GLESBufferIndirectCommand* poBuffer_IndirectCommand;

    ////Pipeline Graphics
		void CreatePipelineGraphics();

	////Command
        void SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends);
        void SetupUniformIndirectCommandBuffer();

        void UpdateUniformBuffer();
        void UpdateIndirectCommandBuffer();
    };
    typedef std::vector<ModelObjectRendIndirect*> ModelObjectRendIndirectPtrVector;
    typedef std::map<String, ModelObjectRendIndirect*> ModelObjectRendIndirectPtrMap;
    

    /////////////////////////// ModelObject /////////////////////////
	struct ModelObject
	{
		ModelObject(OpenGLES_013_IndirectDraw* _pWindow,
					int _index)
			//Common
			: pWindow(_pWindow)
			, index(_index)

			//Name
			, nameObject("")
            , nameMesh("")
			, isShow(true)
			, isWireFrame(false)
			, isRotate(true)
			, isLighting(true)
			, isIndirectDraw(false)
            , isIndirectDrawMulti(false)

			//Mesh
            , pMesh(nullptr)

			//ModelObjectRend
            , pRendIndirect(nullptr)

		{
			
		}
		~ModelObject()
		{
			Destroy();
		}

		void Destroy()
		{
			//Mesh
            this->pMesh = nullptr;
            this->aMeshSubUsed.clear();

            //ObjectRend
            CleanupSwapChain();
            size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                delete pRend;
            }
            this->aRends.clear();
            F_DELETE(pRendIndirect)
		}

		void CleanupSwapChain()
		{
			size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                pRend->CleanupSwapChain();
            }
            if (pRendIndirect != nullptr)
            {
                pRendIndirect->CleanupSwapChain();
            }
		}

		void RecreateSwapChain()
        {
			size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                pRend->RecreateSwapChain();
            }
            if (pRendIndirect != nullptr)
            {
                pRendIndirect->RecreateSwapChain();
            }
        }

		//Common
		OpenGLES_013_IndirectDraw* pWindow;
		int index;

		//Name
		int indexModel;
		String nameObject;
        String nameMesh;
		bool isShow;
		bool isWireFrame;
		bool isRotate;
		bool isLighting;
		bool isIndirectDraw;
        bool isIndirectDrawMulti;

		//Mesh
        Mesh* pMesh;

		std::vector<int> aMeshSubUsed;

        //ModelObjectRend
        ModelObjectRendPtrVector aRends;
        ModelObjectRendIndirect* pRendIndirect;

	////Mesh
        void SetMesh(Mesh* pMesh)
        {
            this->pMesh = pMesh;
        }
        Mesh* GetMesh()
        {
            return this->pMesh;
        }

	////ModelObjectRend
        void AddObjectRend(ModelObjectRend* pRend)
        {
            this->aRends.push_back(pRend);
        }
	};
	typedef std::vector<ModelObject*> ModelObjectPtrVector;
	typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
	MeshPtrVector m_aModelMesh;
    MeshPtrMap m_mapModelMesh;    

    GLESTexturePtrVector m_aModelTexture;
    GLESTexturePtrMap m_mapModelTexture;

	ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrMap m_mapModelObjects;
    ModelObjectRendPtrVector m_aModelObjectRends_All;
    ModelObjectRendPtrVector m_aModelObjectRends_Opaque;
    ModelObjectRendPtrVector m_aModelObjectRends_Transparent;
    bool m_isDrawIndirect;
    bool m_isDrawIndirectMulti;

	GLESShaderPtrVector m_aGLESShaderModules;
    GLESShaderPtrMap m_mapGLESShaderModules;

	DescriptorSetLayoutPtrVector m_aDescriptorSetLayouts;
    DescriptorSetLayoutPtrMap m_mapDescriptorSetLayouts;

protected:
	//Create Pipeline
		virtual void setUpEnabledFeatures();


	//Load Assets
		//Camera
		virtual void createCamera();

		//Geometry/Texture
		virtual void loadModel_Custom();
			void createIndirectCommands();

		//ConstBuffers
		virtual void createCustomCB();

		//Pipeline
		virtual void createCustomBeforePipeline();
		virtual void createGraphicsPipeline_Custom();
		virtual void createComputePipeline_Custom();

		//DescriptorSets
		virtual void createDescriptorSets_Custom();
			void createDescriptorSets_Graphics(ModelObjectRend* pRend,
											   GLESStatePipelineGraphics* pStatePipelineGraphics,
											   GLESBufferUniform* pBufferUniform_Object,
											   GLESBufferUniform* pBufferUniform_Material,
											   GLESBufferUniform* pBufferUniform_Offset,
											   GLESBufferUniform* pBufferUniform_Tessellation);

	//Compute/Update
		virtual void updateCompute_BeforeRender_Custom();

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

////Mesh
    void destroyMeshes();
    void createMeshes();
    Mesh* findMesh(const String& nameMesh);

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
	
////Draw
    void drawModelObjectRends(ModelObjectRendPtrVector& aRends);
    void drawModelObjectRend(ModelObjectRend* pRend);

	void drawModelObjectRendIndirects(ModelObjectRendPtrVector& aRends);
    void drawModelObjectRendIndirect(ModelObjectRendIndirect* pRendIndirect);
};


#endif