/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OPENGLES_WINDOW_H_
#define _OPENGLES_WINDOW_H_

#include "OpenGLESBase.h"
#include "OpenGLESDefine.h"

namespace LostPeterOpenGLES
{
    class openglesExport OpenGLESWindow : public OpenGLESBase
    {
    public:
        OpenGLESWindow(String name);
        virtual ~OpenGLESWindow();

    ///////////////////////// Internal /////////////////////////
    public:
        //RenderPass

        //Uniform ConstantBuffer
        PassConstants passCB;
        GLESBufferUniformPtrVector poBuffers_PassCB;

        //PipelineCompute

        //PipelineGraphics
        GLESPipelineGraphicsCopyBlitToFrame* m_pPipelineGraphics_CopyBlitToFrame;


        //Mesh
        MeshPtrVector m_aMeshes_Internal;
        MeshPtrMap m_mapMeshes_Internal;    

        //Texture
        GLESTexturePtrVector m_aTextures_Internal;
        GLESTexturePtrMap m_mapTextures_Internal;

        //DescriptorSetLayouts
        DescriptorSetLayoutPtrVector m_aDescriptorSetLayouts_Internal;
        DescriptorSetLayoutPtrMap m_mapDescriptorSetLayouts_Internal;
        std::map<String, StringVector> m_mapName2Layouts_Internal;

        //Shader
        GLESShaderPtrVector m_aShaders_Internal;
        GLESShaderPtrMap m_mapShaders_Internal;


    public:
        //Mesh
        virtual Mesh* FindMesh_Internal(const String& nameMesh);

        //Texture
        virtual GLESTexture* FindTexture_Internal(const String& nameTexture);

        //DescriptorSetLayouts
        virtual DescriptorSetLayout* FindDescriptorSetLayout_Internal(const String& nameDescriptorSetLayout);
        virtual StringVector* FindDescriptorSetLayoutNames_Internal(const String& nameDescriptorSetLayout);
        
        //Shader
        virtual GLESShader* FindShader_Internal(const String& nameShader);

        //PipelineCompute

        //PipelineGraphics

        //PipelineGraphics-CopyBlitToFrame
        virtual void UpdateDescriptorSets_Graphics_CopyBlitToFrame();
        virtual void UpdateBuffer_Graphics_CopyBlitToFrame();
        virtual void UpdateBuffer_Graphics_CopyBlitToFrame(const CopyBlitObjectConstants& object);
        virtual void Draw_Graphics_CopyBlitToFrame(GLESFrameBuffer* pFrameBuffer);



    protected:
        virtual void createInternal();
        virtual void cleanupInternal();

        virtual void createResourceInternal();
        virtual void destroyResourceInternal();

        //Mesh
        virtual void destroyMeshes_Internal();
        virtual void createMeshes_Internal();

        //Texture
        virtual void destroyTextures_Internal();
        virtual void createTextures_Internal();

        //DescriptorSetLayouts
        virtual void destroyDescriptorSetLayouts_Internal();
        virtual void createDescriptorSetLayouts_Internal();

        //Shader
        virtual void destroyShaders_Internal();
        virtual void createShaders_Internal();
        
        //Uniform ConstantBuffer
        virtual void destroyUniformCB_Internal();
            virtual void destroyUniform_PassCB();
        virtual void createUniformCB_Internal();
            virtual void createUniform_PassCB();

        //PipelineCompute
        virtual void destroyPipelineCompute_Internal();
            virtual void destroyPipelineCompute_Cull();
            virtual void destroyPipelineCompute_Terrain();
        virtual void createPipelineCompute_Internal();
            virtual void createPipelineCompute_Cull();
            virtual void createPipelineCompute_Terrain();

        //PipelineGraphics
        virtual void destroyPipelineGraphics_Internal();
            virtual void destroyPipelineGraphics_CopyBlitFromFrame();
            virtual void destroyPipelineGraphics_CopyBlitToFrame();
            virtual void destroyPipelineGraphics_DepthShadowMap();
            virtual void destroyPipelineGraphics_DepthHiz();
            virtual void destroyPipelineGraphics_Terrain();
        virtual void createPipelineGraphics_Internal();
            virtual void createPipelineGraphics_CopyBlitFromFrame();
            virtual void createPipelineGraphics_CopyBlitToFrame();
            virtual void createPipelineGraphics_DepthShadowMap();
            virtual void createPipelineGraphics_DepthHiz();
            virtual void createPipelineGraphics_Terrain();
        
    ///////////////////////// Internal /////////////////////////


    public:
        static const String c_strShaderProgram;

    public:
        static FDynamicLib* s_poDynLoader_EGL;
        static void* GetEGLSymbol(const char* name);

    public:
        GLESDebug* poDebug;
        GLESShaderInclude* poShaderInclude;
        int poMSAASamples;

        int poSwapChainImageFormat;
        int poDepthImageFormat;

        FRectI poViewport;
        FSizeI poOffset;
        FSizeI poExtent;
        FRectI poScissor;

        FVector2 poFramebufferSize;
        FVector2 poWindowContentScale;

        GLESTexturePtrVector poSwapChains;
        GLESTexture* poColor;
        GLESRenderBuffer* poDepthStencil;
        GLESTexturePtrVector poColorLists;
        GLESRenderPass* poRenderPass;
        GLESFrameBufferPtrVector poFrameBuffers;
        size_t poCurrentFrame;
        uint32_t poSwapChainImageIndex;

        uint32_t poVertexCount;
        size_t poVertexBuffer_Size;
        uint8* poVertexBuffer_Data;
        uint32_t poIndexCount;
        size_t poIndexBuffer_Size;
        uint8* poIndexBuffer_Data;
        GLESBufferVertex* pBufferVertex;
        GLESBufferVertexIndex* pBufferVertexIndex;
        FMatrix4 poMatWorld;

        FMeshVertexType poTypeVertex;
        GLESShader* poShaderVertex;
        GLESShader* poShaderFragment;
        GLESShaderProgram* poShaderProgram;

        String poDescriptorSetLayoutName;
        DescriptorSetLayout* pDescriptorSetLayout;

        GLESTexture* poTexture;

        bool isFrameBufferResized;

        //Config
        FVector4 cfg_colorBackground;
        FVector4Vector cfg_colorValues;

        bool cfg_isRenderPassDefaultCustom;

        bool cfg_isMSAA;
        bool cfg_isImgui;
        bool cfg_isWireFrame;
        bool cfg_isRotate;
        
        GLenum cfg_glPrimitiveTopology;
        bool cfg_isCull;
        GLenum cfg_glFrontFace;
        GLenum cfg_glCulling;
        GLenum cfg_glPolygonMode;


        FVector3 cfg_cameraPos;
        FVector3 cfg_cameraLookTarget;
        FVector3 cfg_cameraUp;

        float cfg_cameraFov;
        float cfg_cameraNear;
        float cfg_cameraFar;

        float cfg_cameraSpeedMove;
        float cfg_cameraSpeedZoom;
        float cfg_cameraSpeedRotate;

        //Custom
        String cfg_model_Path;
        String cfg_shaderVertex_Path;
        String cfg_shaderFragment_Path;
        String cfg_texture_Path;
        String cfg_terrain_Path;
        String cfg_terrainTextureDiffuse_Path;
        String cfg_terrainTextureNormal_Path;
        String cfg_terrainTextureControl_Path;
        float cfg_terrainHeightStart;
        float cfg_terrainHeightMax;
        
        //Imgui
        bool imgui_IsEnable;
        int	imgui_MinimalSwapchainImages;
        String imgui_PathIni;
        String imgui_PathLog;

        //Constants Buffer
        std::vector<ObjectConstants> objectCBs;
        GLESBufferUniformPtrVector poBuffers_ObjectCB;

        //Camera
        FCamera* pCamera; //Eye Left
        FCamera* pCameraRight; //Eye Right

        //Light

        //Shadow
        FCamera* pCameraMainLight; //mainLight's shadow camera

        //Mouse
        FVector2 mousePosLast;
        bool mouseButtonDownLeft;
        bool mouseButtonDownRight;
        bool mouseButtonDownMiddle;

         //Editor
        bool cfg_isEditorCreate;
        bool cfg_isEditorGridShow;
        bool cfg_isEditorCameraAxisShow;
        bool cfg_isEditorCoordinateAxisShow;
        FColor cfg_editorGrid_Color;
        float cfg_editorCoordinateAxis_MoveSpeed;
        float cfg_editorCoordinateAxis_RotateSpeed;
        float cfg_editorCoordinateAxis_ScaleSpeed;
        EditorGrid* pEditorGrid;
        EditorCameraAxis* pEditorCameraAxis;
        EditorCoordinateAxis* pEditorCoordinateAxis;
        EditorLineFlat2DCollector* pEditorLineFlat2DCollector;
        EditorLineFlat3DCollector* pEditorLineFlat3DCollector;

    public: 
        static bool s_isEnableValidationLayers;
        static int s_maxFramesInFight;

    public:
        // Common
        virtual void OnInit(int w, int h);
        virtual void OnLoad();
        virtual bool OnIsInit();
        virtual void OnResize(int w, int h, bool force);

        virtual bool OnBeginCompute_BeforeRender();
            virtual void OnUpdateCompute_BeforeRender();
            virtual void OnCompute_BeforeRender();
        virtual void OnEndCompute_BeforeRender();

        virtual bool OnBeginRender();
            virtual void OnUpdateRender();
            virtual void OnRender();
        virtual void OnEndRender();

        virtual bool OnBeginCompute_AfterRender();
            virtual void OnUpdateCompute_AfterRender();
            virtual void OnCompute_AfterRender();
        virtual void OnEndCompute_AfterRender();

        virtual void OnPresent();
        virtual void OnDestroy();

        // Mouse Input
        virtual void OnMouseInput();
        virtual void OnMouseLeftDown(double x, double y);
        virtual void OnMouseLeftUp(double x, double y);
        virtual void OnMouseRightDown(double x, double y);
        virtual void OnMouseRightUp(double x, double y);
        virtual void OnMouseMiddleDown(double x, double y);
        virtual void OnMouseMiddleUp(double x, double y);
        virtual void OnMouseMove(int button, double x, double y);
        virtual void OnMouseHover(double x, double y);
        virtual void OnMouseWheel(double x, double y);

        // Keyboard Input
        virtual void OnKeyboardInput();
        virtual void OnKeyDown(int key);
        virtual void OnKeyUp(int key);

        // Camera Process
        virtual void OnCameraMouseMoveProcess(double newX, double newY, double oldX, double oldY);
        virtual void OnCameraMouseZoomProcess(double zoom);
        virtual void OnCameraMouseKeyboardProcess();

        // Editor Coordinate Process
        virtual void OnEditorCoordinateMouseLeftDown(double x, double y);
        virtual void OnEditorCoordinateMouseMove(double x, double y);
        virtual void OnEditorCoordinateMouseLeftUp(double x, double y);
        virtual void OnEditorCoordinateMouseHover(double x, double y);

    public:
        virtual bool HasConfig_RenderPassDefaultCustom();
        virtual bool HasConfig_MASS();
        virtual bool HasConfig_Imgui();

        virtual bool IsEnable_MASS();
        virtual bool IsEnable_Imgui();

    public:
        virtual void SetIsWireFrame(bool isWireFrame);

        

    public:
        //Create Pipeline
        virtual void createPipeline();
            virtual void createWindowCallback();
            virtual void createDevice();
                virtual void setUpDebugMessenger();
                    virtual bool isExtensionSupported(const char* extension);
                virtual void createSurface();
                virtual void pickPhysicalDevice();
                virtual void setUpEnabledFeatures();
                virtual void createLogicalDevice();

            virtual void createFeatureSupport();

            //Camera/Light
            virtual void createCamera();
            virtual void createLightMain();
            virtual void createShadowLightMain();
            virtual void createTerrain();

            virtual void createCommandObjects();

            virtual void createSwapChainObjects();
                virtual void createSwapChain();
                    virtual void createViewport();
                virtual void createSwapChainImageViews();
                    virtual void createColorResources();
                    virtual void createDepthResources();
                virtual void createColorResourceLists();

            virtual void createDescriptorSetLayouts();
                virtual void createDescriptorSetLayout_Default();
                virtual void createDescriptorSetLayout_Custom();

            virtual void createPipelineObjects();
                virtual void createRenderPasses();
                    virtual void createRenderPass_ShadowMap();
                    virtual void createRenderPass_Default();
                    virtual void createRenderPass_Cull();
                    virtual void createRenderPass_Terrain();
                    virtual void createRenderPass_Custom();
                        virtual GLESRenderPass* createRenderPass_DefaultCustom();
                        virtual GLESRenderPass* createRenderPass_KhrDepth(int formatSwapChain, int formatDepth);
                        virtual GLESRenderPass* createRenderPass_ColorDepthMSAA(int formatColor, int formatDepth, int formatSwapChain, int samples);



                virtual void createFramebuffers();
                    virtual void createFramebuffer_Default();
                    virtual void createFramebuffer_Custom();
                        virtual void createFramebuffer_DefaultCustom();


                    virtual GLESRenderBuffer* createRenderBuffer(const String& nameRenderBuffer,
                                                               int width,
                                                               int height,
                                                               GLenum format,
                                                               GLenum attachment, 
                                                               GLenum renderbuffertarget);

                    virtual bool createGLRenderBuffer(const String& nameRenderBuffer,
                                                      int width,
                                                      int height,
                                                      GLenum format,
                                                      uint32& nRenderBufferID);
                    virtual void bindGLRenderBuffer(uint32 nRenderBufferID);
                    virtual void destroyGLRenderBuffer(uint32 nRenderBufferID);


                    virtual GLESFrameBuffer* createFrameBuffer(const String& nameFrameBuffer,
                                                             int width,
                                                             int height,
                                                             const GLESTexturePtrVector& aColorTexture,
                                                             GLESRenderBuffer* pDepthStencil,
                                                             bool isDeleteColors = false,
                                                             bool isDeleteDepthStencil = false);
                    
                    virtual bool createGLFrameBuffer(const String& nameFrameBuffer,
                                                     const UintType2UintIDMap& mapType2IDs,
                                                     uint32 nDepthStencilID,
                                                     uint32& nFrameBufferID);
                    virtual void bindGLFrameBuffer(uint32 nFrameBufferID);
                    virtual void destroyGLFrameBuffer(uint32 nFrameBufferID);


        //Load Assets
        virtual void loadAssets();

            //Geometry
            virtual void loadGeometry();
                virtual void loadVertexIndexBuffer();
                    virtual void loadModel();
                        virtual void loadModel_Default();
                        virtual void loadModel_Custom();
                    //BufferVertex
                    virtual GLESBufferVertex* createBufferVertex(const String& nameBuffer,
                                                               FMeshVertexType type,
                                                               size_t bufSize, 
                                                               uint8* pBuf,
                                                               bool isDelete);
                    virtual void updateBufferVertex(GLESBufferVertex* pBufferVertex,
                                                    FMeshVertexType type,
                                                    size_t bufSize, 
                                                    uint8* pBuf,
                                                    bool isDelete);

                    //BufferVertexIndex
                    virtual GLESBufferVertexIndex* createBufferVertexIndex(const String& nameBuffer,
                                                                         FMeshVertexType type,
                                                                         size_t bufSize_Vertex, 
                                                                         uint8* pBuf_Vertex,
                                                                         bool isDelete_Vertex,
                                                                         size_t bufSize_Index, 
                                                                         uint8* pBuf_Index,
                                                                         bool isDelete_Index);
                    virtual void updateBufferVertexIndex(GLESBufferVertexIndex* pBufferVertexIndex,
                                                         FMeshVertexType type,
                                                         size_t bufSize_Vertex, 
                                                         uint8* pBuf_Vertex,
                                                         bool isDelete_Vertex,
                                                         size_t bufSize_Index, 
                                                         uint8* pBuf_Index,
                                                         bool isDelete_Index);

                    //BufferUniform
                    virtual GLESBufferUniform* createBufferUniform(const String& nameBuffer,
                                                                 uint32 bindingIndex,
                                                                 GLenum usage,
                                                                 size_t bufSize, 
                                                                 uint8* pBuf,
                                                                 bool isDelete);
                    virtual void updateBufferUniform(GLESBufferUniform* pBufferUniform,
                                                     size_t offset,
                                                     size_t bufSize, 
                                                     uint8* pBuf);


                    virtual bool createGLBufferVertex(const String& nameBuffer,
                                                      FMeshVertexType type,
                                                      size_t bufSize,
                                                      uint8* pBuf,
                                                      uint32& nVAO,
                                                      uint32& nVBO);
                    virtual void updateGLBufferVertex(FMeshVertexType type,
                                                      size_t bufSize,
                                                      uint8* pBuf,
                                                      uint32 nVAO,
                                                      uint32 nVBO);
                    virtual void destroyGLBufferVertex(uint32 nVAO, uint32 nVBO);

                    virtual bool createGLBufferVertexIndex(const String& nameBuffer,
                                                           FMeshVertexType type,
                                                           size_t bufSize_Vertex,
                                                           uint8* pBuf_Vertex,
                                                           size_t bufSize_Index,
                                                           uint8* pBuf_Index,
                                                           uint32& nVAO,
                                                           uint32& nVBO,
                                                           uint32& nVEO);
                    virtual void updateGLBufferVertexIndex(FMeshVertexType type,
                                                           size_t bufSize_Vertex,
                                                           uint8* pBuf_Vertex,
                                                           size_t bufSize_Index,
                                                           uint8* pBuf_Index,
                                                           uint32 nVAO,
                                                           uint32 nVBO,
                                                           uint32 nVEO);
                    virtual void bindGLVertexArray(uint32 nVAO);
                    virtual void destroyGLBufferVertexIndex(uint32 nVAO, uint32 nVBO, uint32 nVEO);

                    virtual bool createGLBufferUniform(const String& nameBuffer,
                                                       uint32 bindingIndex,
                                                       GLenum usage,
                                                       size_t bufSize, 
                                                       uint8* pBuf,
                                                       uint32& nBufferUniformID);
                    virtual void updateGLBufferUniform(size_t offset,
                                                       size_t bufSize,
                                                       uint8* pBuf,
                                                       uint32 nBufferUniformID);
                    virtual void bindGLBufferUniform(uint32 nBufferUniformID);
                    virtual void bindGLBufferUniformBlockIndex(uint32 nBufferUniformID, uint32 nUniformBlockIndex);
                    virtual void destroyGLBufferUniform(uint32 nBufferUniformID);

                    virtual void* mapGLBufferRange(uint32 nBufferID, uint32 nBlockIndex, GLenum target, size_t offset, size_t bufSize, GLbitfield access);
                        virtual void flushGLMappedBufferRange(uint32 nBufferID, size_t offset, size_t bufSize);
                    virtual void unMapGLBufferRange(GLenum target);


                virtual void loadTexture();
                    virtual void loadTexture_Default();
                    virtual void loadTexture_Custom();

                    virtual GLESTexture* createTexture(const String& nameTexture,
                                                     const StringVector& aPathTexture,
                                                     uint8* pData,
                                                     int channel,
                                                     int width, 
                                                     int height,
                                                     int depth,
                                                     FTextureType typeTexture,
                                                     FTexturePixelFormatType typePixelFormat,
                                                     FTextureAddressingType typeAddressing,
                                                     FTextureFilterType typeFilterSizeMin,
                                                     FTextureFilterType typeFilterSizeMag,
                                                     FMSAASampleCountType numSamples,
                                                     const FColor& borderColor,
                                                     bool isUseBorderColor,
                                                     bool isAutoMipmap,
                                                     bool isCubeMap,
                                                     bool isRenderTarget,
                                                     bool isGraphicsComputeShared);

                    virtual bool createTexture2D(const String& nameTexture,
                                                 const String& pathAsset_Tex,
                                                 int& mipMapCount, 
                                                 bool isAutoMipmap,
                                                 FTextureType typeTexture, 
                                                 bool isCubeMap,
                                                 FTexturePixelFormatType typePixelFormat,
                                                 FTextureAddressingType typeAddressing,
                                                 FTextureFilterType typeFilterSizeMin,
                                                 FTextureFilterType typeFilterSizeMag,
                                                 FMSAASampleCountType numSamples, 
                                                 const FColor& borderColor,
                                                 bool isUseBorderColor,
                                                 bool isGraphicsComputeShared,
                                                 uint32& nTextureID);

                    
                    virtual bool createTextureRenderTarget2D(const String& nameTexture,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             int channel,
                                                             int width, 
                                                             int height,
                                                             int& mipMapCount, 
                                                             bool isAutoMipmap,
                                                             FTextureType typeTexture, 
                                                             bool isCubeMap,
                                                             FTexturePixelFormatType typePixelFormat,
                                                             FTextureAddressingType typeAddressing,
                                                             FTextureFilterType typeFilterSizeMin,
                                                             FTextureFilterType typeFilterSizeMag,
                                                             FMSAASampleCountType numSamples, 
                                                             const FColor& borderColor,
                                                             bool isUseBorderColor,
                                                             bool isGraphicsComputeShared,
                                                             uint32& nTextureID);
                    virtual bool createTextureRenderTarget2D(const String& nameTexture,
                                                             uint8* pData,
                                                             int channel,
                                                             int width, 
                                                             int height,
                                                             int& mipMapCount, 
                                                             bool isAutoMipmap,
                                                             FTextureType typeTexture, 
                                                             bool isCubeMap,
                                                             FTexturePixelFormatType typePixelFormat,
                                                             FTextureAddressingType typeAddressing,
                                                             FTextureFilterType typeFilterSizeMin,
                                                             FTextureFilterType typeFilterSizeMag,
                                                             FMSAASampleCountType numSamples, 
                                                             const FColor& borderColor,
                                                             bool isUseBorderColor,
                                                             bool isGraphicsComputeShared,
                                                             uint32& nTextureID);


                    virtual bool createGLTexture(const String& nameTexture,
                                                 uint8* pData,
                                                 int channel,
                                                 int width, 
                                                 int height, 
                                                 int depth, 
                                                 int numArray,
                                                 int mipMapCount, 
                                                 bool isAutoMipmap,
                                                 FTextureType typeTexture, 
                                                 bool isCubeMap,
                                                 FTexturePixelFormatType typePixelFormat,
                                                 FTextureAddressingType typeAddressing,
                                                 FTextureFilterType typeFilterSizeMin,
                                                 FTextureFilterType typeFilterSizeMag,
                                                 FMSAASampleCountType numSamples, 
                                                 const FColor& borderColor,
                                                 bool isUseBorderColor,
                                                 bool isGraphicsComputeShared,
                                                 uint32& nTextureID);
                    virtual void bindGLTexture(FTextureType typeTexture, uint32 nTextureID);
                    virtual void destroyGLTexture(uint32 nTextureID);


                virtual void createConstBuffers();
                    virtual void createObjectCB();
                        virtual void buildObjectCB();
                    virtual void createMaterialCB();
                        virtual void buildMaterialCB();
                    virtual void createInstanceCB();
                        virtual void buildInstanceCB();
                    virtual void createCustomCB();

                virtual GLESShader* createShader(const String& nameShader, const String& pathFile, FShaderType typeShader);
                virtual String getShaderPathRelative(const String& nameShader);
                virtual String getShaderPath(const String& nameShader);

                virtual bool createGLShader(const String& nameShader, const String& pathFile, FShaderType typeShader, uint32& nShaderID);
                virtual bool createGLShader(const String& nameShader, const String& strTypeShader, const String& pathFile, FShaderType typeShader, uint32& nShaderID);
                virtual void destroyGLShader(uint32 nShaderID);
                

                virtual GLESShaderProgram* createShaderProgram(const String& nameShaderProgram,
                                                             GLESShader* pShaderVertex,
                                                             GLESShader* pShaderTessellationControl,
                                                             GLESShader* pShaderTessellationEvaluation,
                                                             GLESShader* pShaderGeometry,
                                                             GLESShader* pShaderFragment);
                virtual GLESShaderProgram* createShaderProgram(const String& nameShaderProgram,
                                                             GLESShader* pShaderCompute);

                virtual uint32 createGLShaderProgram();
                virtual bool createGLShaderProgram(const String& nameShaderProgram,
                                                   uint32 nShaderVertexID,
                                                   uint32 nShaderTessellationControlID,
                                                   uint32 nShaderTessellationEvaluationID,
                                                   uint32 nShaderGeometryID,
                                                   uint32 nShaderFragmentID,
                                                   uint32& nShaderProgramID);
                virtual bool createGLShaderProgram(const String& nameShaderProgram,
                                                   uint32 nShaderComputeID,
                                                   uint32& nShaderProgramID);
                    virtual void bindGLShaderProgram(uint32 nShaderProgramID);

                    virtual uint32 getUniformBlockIndex(uint32 nShaderProgramID, const String& name);
                    virtual void setUniformBlockBinding(uint32 nShaderProgramID, uint32 nUniformBlockIndex, uint32 nUniformBlockBinding);

                    virtual void setUniform1i(uint32 nShaderProgramID, const String& name, int value);
                    virtual void setUniform1f(uint32 nShaderProgramID, const String& name, float value);
                    virtual void setUniform2f(uint32 nShaderProgramID, const String& name, float x, float y);
                    virtual void setUniform3f(uint32 nShaderProgramID, const String& name, float x, float y, float z);
                    virtual void setUniform4f(uint32 nShaderProgramID, const String& name, float x, float y, float z, float w);
                    virtual void setUniform2fv(uint32 nShaderProgramID, const String& name, const FVector2& v2);
                    virtual void setUniform3fv(uint32 nShaderProgramID, const String& name, const FVector3& v3);
                    virtual void setUniform4fv(uint32 nShaderProgramID, const String& name, const FVector4& v4);
                    virtual void setUniformMatrix2fv(uint32 nShaderProgramID, const String& name, const glm::mat2& m2);
                    virtual void setUniformMatrix3fv(uint32 nShaderProgramID, const String& name, const FMatrix3& m3);
                    virtual void setUniformMatrix4fv(uint32 nShaderProgramID, const String& name, const FMatrix4& m4);

                    virtual int getUniformLocation(uint32 nShaderProgramID, const String& name);
                    virtual int getUniformLocation(uint32 nShaderProgramID, const char* name);

                    virtual void setUniform1i(int location, int value);
                    virtual void setUniform1f(int location, float value);
                    virtual void setUniform2f(int location, float x, float y);
                    virtual void setUniform3f(int location, float x, float y, float z);
                    virtual void setUniform4f(int location, float x, float y, float z, float w);
                    virtual void setUniform2fv(int location, const FVector2& v2);
                    virtual void setUniform3fv(int location, const FVector3& v3);
                    virtual void setUniform4fv(int location, const FVector4& v4);
                    virtual void setUniformMatrix2fv(int location, const glm::mat2& m2);
                    virtual void setUniformMatrix3fv(int location, const FMatrix3& m3);
                    virtual void setUniformMatrix4fv(int location, const FMatrix4& m4);

                    virtual bool hasGLShaderGLSLInclude(const String& key);
                    virtual void addGLShaderGLSLInclude(const String& key, const String& content);
                    virtual void removeGLShaderGLSLInclude(const String& key);

                virtual void destroyGLShaderProgram(uint32 nShaderProgramID);
                virtual bool checkGLShaderCompileErrors(uint32 nShader, const String& type);    
                virtual void getGLShaderCompileErrors(uint32 nShader, std::vector<GLchar>& errorLog);
                virtual void getGLShaderProgramCompileErrors(uint32 nShader, std::vector<GLchar>& errorLog);


                virtual void createCustomBeforePipeline();
                virtual void createGraphicsPipeline();
                    virtual void createGraphicsPipeline_Default();
                    virtual void createGraphicsPipeline_Custom();    


                virtual void createComputePipeline();
                    virtual void createComputePipeline_Default();
                    virtual void createComputePipeline_Custom();


                virtual void createDescriptorSets();
                    virtual void createDescriptorSets_Default();
                    virtual void createDescriptorSets_Terrain();
                    virtual void createDescriptorSets_Custom();
                        virtual void updateDescriptorSets(DescriptorSetLayout* pDSL, GLESShaderProgram* pSP); 



            //Imgui
            virtual void createImgui();
                virtual void createImgui_Init();

            //Editor
            virtual void createEditor();
                virtual void createEditor_Grid();
                virtual void createEditor_CameraAxis();
                virtual void createEditor_CoordinateAxis();      
                virtual void createEditor_LineFlat2DCollector();
                virtual void createEditor_LineFlat3DCollector();  
            virtual void destroyEditor();


        //Resize
        virtual void resizeWindow(int w, int h, bool force);


        //Compute Before Render


        //Compute After Render



        //Render/Update
        virtual bool beginRender();
            virtual void updateRender();
                virtual void updateCBs_Default();
                    virtual void updateCBs_Pass();
                        virtual void updateCBs_PassTransformAndCamera(PassConstants& pass, FCamera* pCam, int nIndex);
                    virtual void updateCBs_Objects();
                        virtual void updateCBs_ObjectsContent();
                    virtual void updateCBs_Materials();
                        virtual void updateCBs_MaterialsContent();
                    virtual void updateCBs_Instances();
                        virtual void updateCBs_InstancesContent();
                virtual void updateCBs_Terrain();
                virtual void updateCBs_ImGUI();
                    virtual bool beginRenderImgui();
                        //Common
                        virtual void commonConfig();
                            virtual void commonShowConfig();
                            virtual void commonEditorConfig();
                        //Camera
                        virtual void cameraConfig();
                            virtual void cameraReset();
                        //Light
                        virtual void lightConfig();
                            virtual void lightConfigItem(LightConstants& lc, const String& name, int index, bool canChangeType, bool bIsMainLight);
                            virtual void lightMainReset();
                        //Shadow
                        virtual void shadowConfig();
                            virtual void shadowConfigItem(ShadowConstants& sc, const String& name, bool bIsMainLight);
                            virtual void shadowReset();
                         //Cull
                        virtual void cullConfig();
                        //Terrain
                        virtual void terrainConfig();
                            virtual void terrainConfigItem(TerrainConstants& tc, const String& name);
                                virtual bool terrainConfigSplatItem(TerrainSplatConstants& tsc, const String& name);
                            virtual void terrainReset();
                        //PassConstants
                        virtual void passConstantsConfig();
                        //Model
                        virtual void modelConfig();
                        
                    virtual void endRenderImgui();
                virtual void updateCBs_Editor();
                virtual void updateCBs_Custom();

                virtual void updateRenderCommandBuffers_CustomBeforeDefault();
                virtual void updateRenderCommandBuffers_Default();

                    virtual void updateRenderPass_CustomBeforeDefault();
                    virtual void updateRenderPass_Default();
                        virtual void updateMeshDefault_Before();
                            virtual void drawMeshDefault();
                            virtual void drawMeshTerrain();
                            virtual void drawMeshDefault_Custom();
                            virtual void drawMeshDefault_Editor();
                            virtual void drawMeshDefault_CustomBeforeImgui();
                            virtual void drawMeshDefault_Imgui();
                        virtual void updateMeshDefault_After();
                    virtual void updateRenderPass_CustomAfterDefault();


                    virtual void beginRenderPass(const String& nameRenderPass,
                                                 GLESRenderPass* pRenderPass,
                                                 const FSizeI& offset,
                                                 const FSizeI& extent,
                                                 const FVector4& clBg,
                                                 float depth,
                                                 int stencil);

                        virtual void setEnable(GLenum cap, bool enable);
                        virtual void setEnableDepthTest(bool enable);
                        virtual void setClearColor(float r, float g, float b, float a);
                        virtual void setClearColor(const FVector4& color);
                        virtual void setClearDepth(float depth);
                        virtual void setClearStencil(int stencil);
                        virtual void setClearColorDepthStencil(float r, float g, float b, float a, float depth, int stencil);
                        virtual void setClearColorDepthStencil(const FVector4& color, float depth, int stencil);
                        virtual void clear(GLbitfield mask);
                        virtual void setFrontFace(GLenum mode);
                        virtual void setCullFace(GLenum mode);
                        virtual void setPolygonMode(GLenum face, GLenum mode);
                        virtual void setBlendFunc(GLenum sfactor, GLenum dfactor);
                    
                        virtual void draw(GLenum mode, GLint first, GLsizei count);
                        virtual void drawIndexed(GLenum mode, GLsizei count, GLenum type, const void* indices);

                    virtual void endRenderPass(GLESRenderPass* pRenderPass);


                virtual void updateRenderCommandBuffers_CustomAfterDefault();

            virtual void render();
        virtual void endRender();

        //cleanup
        virtual void cleanup();
            virtual void cleanupDefault();
                virtual void cleanupTexture();
                virtual void cleanupVertexIndexBuffer();
            virtual void cleanupImGUI();
            virtual void cleanupEditor();
            virtual void cleanupCustom();

            virtual void cleanupSwapChain();
                virtual void cleanupSwapChain_Default();
                virtual void cleanupSwapChain_Editor();
                virtual void cleanupSwapChain_Custom();
            virtual void recreateSwapChain();
                virtual void recreateSwapChain_Editor();
                virtual void recreateSwapChain_Custom();

    private:


    };

}; //LostPeterOpenGLES

#endif