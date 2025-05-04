/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/EditorCameraAxis.h"
#include "../include/OpenGLESWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/GLESBufferVertexIndex.h"
#include "../include/GLESBufferUniform.h"
#include "../include/GLESShaderProgram.h"
#include "../include/GLESStatePipelineGraphics.h"
#include "../include/GLESShader.h"
#include "../include/GLESTexture.h"
#include "../include/GLESRenderBuffer.h"
#include "../include/GLESFrameBuffer.h"
#include "../include/GLESRenderPass.h"

namespace LostPeterOpenGLES
{
	size_t EditorCameraAxis::s_nMeshConeIndex = 0;
    size_t EditorCameraAxis::s_nMeshAABBIndex = 1;
    size_t EditorCameraAxis::s_nMeshQuadIndex = 2;
    size_t EditorCameraAxis::s_nMeshCameraAxisCount = 2;
    const String EditorCameraAxis::s_strNameShader_CameraAxis_Vert = "vert_editor_camera_axis";
    const String EditorCameraAxis::s_strNameShader_CameraAxis_Frag = "frag_editor_camera_axis";
    const String EditorCameraAxis::s_strNameShader_QuadBlit_Vert = "vert_standard_copy_blit";
    const String EditorCameraAxis::s_strNameShader_QuadBlit_Frag = "frag_standard_copy_blit";
    const float EditorCameraAxis::s_fBlitAreaWidth = 128;
    const float EditorCameraAxis::s_fBlitAreaHeight = 128;
    FMatrix4 EditorCameraAxis::s_aMatrix4Transforms[7] = 
    {
        FMath::FromTRS(FVector3(-2.5f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f, -90.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone X+
        FMath::FromTRS(FVector3( 2.5f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,  90.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone X-
        FMath::FromTRS(FVector3( 0.0f,  2.5f,  0.0f), FVector3(180.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Y+
        FMath::FromTRS(FVector3( 0.0f, -2.5f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Y-
        FMath::FromTRS(FVector3( 0.0f,  0.0f, -2.5f), FVector3( 90.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Z+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  2.5f), FVector3(-90.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Z-
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //AABB
    };
    float EditorCameraAxis::s_fCameraDistance = 10;
    FVector3 EditorCameraAxis::s_vCameraPos = FVector3(0, 0, - EditorCameraAxis::s_fCameraDistance);
    FVector3 EditorCameraAxis::s_vCameraLookTarget = FVector3(0, 0, 0);
    FVector3 EditorCameraAxis::s_vCameraUp = FVector3(0, 1, 0);
    float EditorCameraAxis::s_fCameraFOV = 45.0f;
    float EditorCameraAxis::s_fCameraAspectRatio = 1.0f;
    float EditorCameraAxis::s_fCameraZNear = 0.01f;
    float EditorCameraAxis::s_fCameraZFar = 10000.0f;
    EditorCameraAxis::EditorCameraAxis()
        : EditorBase("EditorCameraAxis")

		//CameraAxis
        , pCamera(nullptr)
        , poColorBackground(0.0f, 0.0f, 0.0f, 0.0f)

		, poBufferUniform_PassCB(nullptr)
		, poBufferUniform_ObjectCB(nullptr)
	#ifdef F_CameraAxisObjectConstants_Separate
		, poBufferUniform_ObjectCB_Last(nullptr)
	#endif

		, poRenderBufferDepthStencil(nullptr)
		, poFrameBufferCameraAxis(nullptr)
		, poRenderPassCameraAxis(nullptr)

        , isNeedUpdate(true)

        //Quad Blit
        , nameDescriptorSetLayout_CopyBlit("")
        , pPipelineGraphics_CopyBlit(nullptr)
		, poBufferUniform_CopyBlitObjectCB(nullptr)
    {

    }
    EditorCameraAxis::~EditorCameraAxis()
    {
		Destroy();
    }

	void EditorCameraAxis::Destroy()
    {
        F_DELETE(this->pCamera)
        CleanupSwapChain();
        destroyMeshes();
    }
    void EditorCameraAxis::Init()
    {
        EditorBase::Init();
    }
    void EditorCameraAxis::UpdateCBs()
    {
        //PassConstants
        {
            FVector3 vDir = Base::GetWindowPtr()->pCamera->GetDir();
            FVector3 vPos = -vDir * s_fCameraDistance;
            this->pCamera->LookAtLH(vPos, s_vCameraLookTarget, s_vCameraUp);
            this->pCamera->UpdateViewMatrix();
            Base::GetWindowPtr()->updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
			this->poBufferUniform_PassCB->UpdateBuffer(sizeof(PassConstants), 
													   (uint8*)(&this->passCB),
													   GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        }
        
        if (!IsNeedUpdate())
            return;
        SetIsNeedUpdate(false);

        //CameraAxis
        {
			poBufferUniform_ObjectCB->UpdateBuffer(sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size(),
												   (uint8*)(&this->cameraAxisObjectCBs[0]),
												   GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		#ifdef F_CameraAxisObjectConstants_Separate
			poBufferUniform_ObjectCB_Last->UpdateBuffer(sizeof(CameraAxisObjectConstants),
													    (uint8*)(&this->cameraAxisObjectCB_Last),
														GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		#endif
        }
        //Quad Blit
        {
			poBufferUniform_CopyBlitObjectCB->UpdateBuffer(sizeof(CopyBlitObjectConstants), 
														   (uint8*)(&this->copyBlitObjectCB),
														   GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        }
    }
    void EditorCameraAxis::Draw()
    {
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		this->pPipelineGraphics->BindState();
		this->pPipelineGraphics->BindShader();
		this->pPipelineGraphics->BindBufferUniforms();
		this->pPipelineGraphics->BindTextures();

        uint32_t instanceStart = 0;
        this->pPipelineGraphics->poShaderProgram->SetInt("uBaseInstance", 0);
        size_t count_mesh = s_nMeshCameraAxisCount;
        for (size_t i = 0; i < count_mesh; i++)
        {
            Mesh* pMesh = this->aMeshes[i];
            size_t count_mesh_sub = pMesh->aMeshSubs.size();
            for (size_t j = 0; j < count_mesh_sub; j++)
            {
                MeshSub* pMeshSub = pMesh->aMeshSubs[j];
				
				pMeshSub->pBufferVertexIndex->BindVertexArray();
			#ifdef F_CameraAxisObjectConstants_Separate
				if (i > 0)
				{
					poBufferUniform_ObjectCB_Last->BindBufferUniformBlockIndex();
				}
				pWindow->drawIndexedInstance(GL_TRIANGLES, (int)pMeshSub->poIndexCount, GL_UNSIGNED_INT, 0, pMeshSub->instanceCount);
			#else
                if (i > 0)
                {
                    this->pPipelineGraphics->poShaderProgram->SetInt("uBaseInstance", (int)instanceStart);
                }
				pWindow->drawIndexedInstancedBaseInstance(GL_TRIANGLES, (int)pMeshSub->poIndexCount, GL_UNSIGNED_INT, 0, pMeshSub->instanceCount, instanceStart);
			#endif
				
                instanceStart += pMeshSub->instanceCount;
            }
        }
    }   
    void EditorCameraAxis::DrawQuad()
    {
		OpenGLESWindow* pWindow = Base::GetWindowPtr();
		this->pPipelineGraphics_CopyBlit->BindState();
		this->pPipelineGraphics_CopyBlit->BindShader();
		this->pPipelineGraphics_CopyBlit->BindBufferUniforms();
		this->pPipelineGraphics_CopyBlit->BindTextures();
		
        Mesh* pMesh = this->aMeshes[s_nMeshQuadIndex];
        MeshSub* pMeshSub = pMesh->aMeshSubs[0];
		pMeshSub->pBufferVertexIndex->BindVertexArray();
		pWindow->drawIndexed(GL_TRIANGLES, (int)pMeshSub->poIndexCount, GL_UNSIGNED_INT, 0);
    }
    void EditorCameraAxis::initConfigs()
    {
        //1> Mesh
        {
            //0: Cone
            MeshInfo* pMICone = new MeshInfo("EditorCameraAxis_Cone",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityCone,
                                             new FMeshCreateParam_EntityCone(0.5f, 2.0f, 0.0f, 16, 1, false, false),
                                             false,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMICone);
            //1: AABB
            MeshInfo* pMIAABB = new MeshInfo("EditorCameraAxis_AABB",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityAABB,
                                             nullptr,
                                             false,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIAABB);
            //2: Quad
            MeshInfo* pMIQuad = new MeshInfo("EditorCameraAxis_Quad",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityQuad,
                                             nullptr,
                                             true,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIQuad);
        }
        //2> Shader
        {
            //CameraAxis
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_CameraAxis_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = Base::GetWindowPtr()->getShaderPathRelative("editor_camera_axis.vert.spv");
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_CameraAxis_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = Base::GetWindowPtr()->getShaderPathRelative("editor_camera_axis.frag.spv");
                this->aShaderModuleInfos.push_back(siFrag);
            }
            //Quad Blit
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_QuadBlit_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = Base::GetWindowPtr()->getShaderPathRelative("standard_copy_blit_to_frame.vert.spv");
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_QuadBlit_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = Base::GetWindowPtr()->getShaderPathRelative("standard_copy_blit_to_frame.frag.spv");
                this->aShaderModuleInfos.push_back(siFrag);
            }
        }
        //3> BufferUniform
        {
            
        }
        //4> DescriptorSetLayout
        {
            //CameraAxis
            {
                this->nameDescriptorSetLayout = "PassConstants-CameraAxisObjectConstants";
                this->aNameDescriptorSetLayouts = FUtilString::Split(this->nameDescriptorSetLayout, "-");
            }
            //Quad Blit
            {
                this->nameDescriptorSetLayout_CopyBlit = "CopyBlitObjectConstants-TextureFrameColor";
                this->aNameDescriptorSetLayouts_CopyBlit = FUtilString::Split(this->nameDescriptorSetLayout_CopyBlit, "-");
            }
        }
        //5> Camera/Viewport
        {
            initCamera();
            initViewport();
        }
    }
        void EditorCameraAxis::initCamera()
        {
            this->pCamera = new FCamera();
            this->pCamera->LookAtLH(s_vCameraPos, s_vCameraLookTarget, s_vCameraUp);
            this->pCamera->PerspectiveLH(s_fCameraFOV, s_fCameraAspectRatio, s_fCameraZNear, s_fCameraZFar);
            this->pCamera->UpdateViewMatrix();
        }
        void EditorCameraAxis::initViewport()
        {
            Base::GetWindowPtr()->createViewport((uint32_t)s_fBlitAreaWidth,
                                                 (uint32_t)s_fBlitAreaHeight,
                                                 this->poViewport,
                                                 this->poScissor);
			this->poOffset.x = 0;
			this->poOffset.y = 0;
			this->poExtent.x = (int32)s_fBlitAreaWidth;
			this->poExtent.y = (int32)s_fBlitAreaHeight;
        }
    void EditorCameraAxis::initBufferUniforms()
    {
		OpenGLESWindow* pWindow = Base::GetWindowPtr();

        //CameraAxis
        {
            //PassConstants
            Base::GetWindowPtr()->updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
			this->poBufferUniform_PassCB = pWindow->createBufferUniform("EditorCameraAxis-PassConstants", 
																		DescriptorSet_PassConstants,
																		GL_DYNAMIC_DRAW,
																		sizeof(PassConstants), 
																		(uint8*)(&this->passCB),
																		false);

            this->cameraAxisObjectCBs.clear();
            int indexConst = 0;
            //Axis Cone
            {
                //X
                {
                    //+
                    CameraAxisObjectConstants xConsts0;
                    xConsts0.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    xConsts0.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(xConsts0);
                    indexConst++;
                    //-
                    CameraAxisObjectConstants xConsts1;
                    xConsts1.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    xConsts1.color = FColor(1.0f, 0.0f, 0.0f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(xConsts1);
                    indexConst++;
                }
                //Y
                {
                    //+
                    CameraAxisObjectConstants yConsts0;
                    yConsts0.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    yConsts0.color = FColor(0.0f, 1.0f, 0.0f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(yConsts0);
                    indexConst++;
                    //-
                    CameraAxisObjectConstants yConsts1;
                    yConsts1.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    yConsts1.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(yConsts1);
                    indexConst++;
                }
                //Z
                {
                    //+
                    CameraAxisObjectConstants zConsts0;
                    zConsts0.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    zConsts0.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(zConsts0);
                    indexConst++;
                    //-
                    CameraAxisObjectConstants zConsts1;
                    zConsts1.g_MatWorld = s_aMatrix4Transforms[indexConst]; 
                    zConsts1.color = FColor(0.0f, 0.0f, 1.0f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(zConsts1);
                    indexConst++;
                }
            }
            //Axis AABB
            {
                CameraAxisObjectConstants constsAABB;
                constsAABB.g_MatWorld = s_aMatrix4Transforms[indexConst];
                constsAABB.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                this->cameraAxisObjectCBs.push_back(constsAABB);
                indexConst++;

			#ifdef F_CameraAxisObjectConstants_Separate
				this->cameraAxisObjectCB_Last = constsAABB;
			#endif
            }
			this->poBufferUniform_ObjectCB = pWindow->createBufferUniform("EditorCameraAxis-CameraAxisObjectConstants", 
																		  DescriptorSet_CameraAxisObjectConstants,
																		  GL_DYNAMIC_DRAW,
																		  sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size(), 
																		  (uint8*)(&this->cameraAxisObjectCBs[0]),
																		  false);
			#ifdef F_CameraAxisObjectConstants_Separate
				this->poBufferUniform_ObjectCB_Last = pWindow->createBufferUniform("EditorCameraAxis-CameraAxisObjectConstants", 
																				   DescriptorSet_CameraAxisObjectConstants,
																				   GL_DYNAMIC_DRAW,
																				   sizeof(CameraAxisObjectConstants), 
																				   (uint8*)(&this->cameraAxisObjectCB_Last),
																				   false);
			#endif

            Mesh* pMesh = this->aMeshes[s_nMeshConeIndex]; //Cone
            MeshSub* pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 6;
            pMesh = this->aMeshes[s_nMeshAABBIndex]; //AABB
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
        }
        //Quad Blit
        {
            float width = (float)Base::GetWindowPtr()->poFramebufferSize.x;
            float height = (float)Base::GetWindowPtr()->poFramebufferSize.y;
			this->copyBlitObjectCB.offsetX = 0.9f - s_fBlitAreaWidth / width;
            this->copyBlitObjectCB.offsetY = 1.0f - s_fBlitAreaHeight / height;
            this->copyBlitObjectCB.scaleX = 2.0f * s_fBlitAreaWidth / width;
            this->copyBlitObjectCB.scaleY = 2.0f * s_fBlitAreaHeight / height;
			this->poBufferUniform_CopyBlitObjectCB = pWindow->createBufferUniform("EditorCameraAxis-CopyBlitObjectConstants", 
																				  DescriptorSet_CopyBlitObjectConstants,
																				  GL_DYNAMIC_DRAW,
																				  sizeof(CopyBlitObjectConstants), 
																				  (uint8*)(&this->copyBlitObjectCB),
																				  false);
        }
        SetIsNeedUpdate(true);
    }
    void EditorCameraAxis::initDescriptorSetLayout()
    {
        EditorBase::initDescriptorSetLayout();
        //Quad Blit
        {   
            
        }   
    }
    void EditorCameraAxis::initPipelineLayout()
    {
        EditorBase::initPipelineLayout();
        //Quad Blit
        {   
            
        }   
    }
    void EditorCameraAxis::initPipelineGraphics()
    {
		OpenGLESWindow* pWindow = Base::GetWindowPtr();

        //GLESStatePipelineGraphics
        {
            //CameraAxis
            {
				//1> Pipeline Graphics
				String namePipelineGraphics = "PipelineGraphics-EditorCameraAxis";
				GLESShader* pShaderVertex = GetShader(s_strNameShader_CameraAxis_Vert); 
				F_Assert("EditorCameraAxis::initPipelineGraphics: Shader Vetex" && pShaderVertex)
				GLESShader* pShaderFragment = GetShader(s_strNameShader_CameraAxis_Frag);
				F_Assert("EditorCameraAxis::initPipelineGraphics: Shader Fragment" && pShaderFragment)
                this->pPipelineGraphics = pWindow->createStatePipelineGraphics(namePipelineGraphics,
																			   pShaderVertex,
																			   nullptr,
																			   nullptr,
																			   nullptr,
																			   pShaderFragment,
																			   F_MeshVertex_Pos3Color4Tex2,
																			   GL_TRIANGLES,
																			   true,
																			   GL_CW,
																			   GL_BACK,
																			   0,
																			   true,
																			   GL_LEQUAL,
																			   true,
																			   true,
																			   false,
																			   GL_LEQUAL,
																			   GL_KEEP,
																			   GL_KEEP,
																			   GL_KEEP,
																			   0,
																			   0,
																			   true,
																			   GL_SRC_ALPHA,
																			   GL_ONE_MINUS_SRC_ALPHA,
																			   GL_FUNC_ADD,
																			   GL_ONE,
																			   GL_ZERO,
																			   GL_FUNC_ADD,
																			   true,
																			   true,
																			   true,
																			   true);
				if (this->pPipelineGraphics == nullptr)
				{
					String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCameraAxis] !";
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg.c_str());
				}
                this->pPipelineGraphics->nameDescriptorSetLayout = this->nameDescriptorSetLayout;
                this->pPipelineGraphics->poDescriptorSetLayoutNames = &this->aNameDescriptorSetLayouts;
				F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis] Create pipeline graphics success !");
				
				//2> TextureTarget/FrameBuffer/RenderPass
				StringVector aPathTexture;
				int w = (int)this->poViewport.Width();
				int h = (int)this->poViewport.Height();
				String nameTextureColor = "TextureColor-EditorCameraAxis";
                GLESTexture* pTexture = pWindow->createTexture(nameTextureColor,
															 aPathTexture,
														     nullptr,
														   	 4,
															 w,
															 h,
															 1,
															 F_Texture_2D,
															 F_TexturePixelFormat_R8G8B8A8_UNORM,
															 F_TextureAddressing_Wrap,
															 F_TextureFilter_Bilinear,
															 F_TextureFilter_Bilinear,
															 F_MSAASampleCount_1_Bit,
															 FColor(0, 0, 0, 1),
															 true,
															 true,
															 false,
															 true,
															 false);
                if (pTexture == nullptr)
                {
                    F_LogError("*********************** EditorCameraAxis::initPipelineGraphics: Failed to create texture color, name: [%s] !", nameTextureColor.c_str());
                    return;
                }
                this->poTextureColors.push_back(pTexture);
				String nameTextureDepthStencil = "Texture-DepthStencil-EditorCameraAxis";
				this->poRenderBufferDepthStencil = pWindow->createRenderBuffer(nameTextureDepthStencil,
																			   w,
																			   h,
																			   GL_DEPTH24_STENCIL8,
																			   GL_DEPTH_STENCIL_ATTACHMENT,
																			   0);
				if (this->poRenderBufferDepthStencil == nullptr)
				{
					F_LogError("*********************** EditorCameraAxis::initPipelineGraphics: Failed to create texture depth stencil, name: [%s] !", nameTextureDepthStencil.c_str());
					return;
				}
				String nameFrameBuffer = "FrameBuffer-EditorCameraAxis";
				this->poFrameBufferCameraAxis = pWindow->createFrameBuffer(nameFrameBuffer,
																	       w,
																		   h,
																		   this->poTextureColors,
																		   this->poRenderBufferDepthStencil,
																		   false,
																		   false);
				if (this->poFrameBufferCameraAxis == nullptr)
				{
					F_LogError("*********************** EditorCameraAxis::initPipelineGraphics: Failed to create framebuffer, name: [%s] !", nameFrameBuffer.c_str());
					return;
				}
				String nameRenderPass("RenderPass-EditorCameraAxis");
				this->poRenderPassCameraAxis = pWindow->createRenderPass(nameRenderPass,
																		 this->poFrameBufferCameraAxis);
				if (this->poRenderPassCameraAxis == nullptr)
				{
					F_LogError("*********************** EditorCameraAxis::initPipelineGraphics: Failed to create renderpass, name: [%s] !", nameRenderPass.c_str());
					return;
				}
            }
            //Quad Blit
            {
				String namePipelineGraphics = "PipelineGraphics-QuadBlit-EditorCameraAxis";
				GLESShader* pShaderVertex = GetShader(s_strNameShader_QuadBlit_Vert); 
				F_Assert("EditorCameraAxis::initPipelineGraphics: Shader Vetex" && pShaderVertex)
				GLESShader* pShaderFragment = GetShader(s_strNameShader_QuadBlit_Frag);
				F_Assert("EditorCameraAxis::initPipelineGraphics: Shader Fragment" && pShaderFragment)
                this->pPipelineGraphics_CopyBlit = pWindow->createStatePipelineGraphics(namePipelineGraphics,
																						pShaderVertex,
																						nullptr,
																						nullptr,
																						nullptr,
																						pShaderFragment,
																						F_MeshVertex_Pos2Color4Tex2,
																						GL_TRIANGLES,
																						true,
																						GL_CW,
																						GL_BACK,
																						0,
																						true,
																						GL_LEQUAL,
																						true,
																						true,
																						false,
																						GL_LEQUAL,
																						GL_KEEP,
																						GL_KEEP,
																						GL_KEEP,
																						0,
																						0,
																						true,
																						GL_SRC_ALPHA,
																						GL_ONE_MINUS_SRC_ALPHA,
																						GL_FUNC_ADD,
																						GL_ONE,
																						GL_ZERO,
																						GL_FUNC_ADD,
																						true,
																						true,
																						true,
																						true);
				if (this->pPipelineGraphics_CopyBlit == nullptr)
				{
					String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCameraAxis_CopyBlit] !";
                    F_LogError("%s", msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                this->pPipelineGraphics_CopyBlit->nameDescriptorSetLayout = this->nameDescriptorSetLayout_CopyBlit;
                this->pPipelineGraphics_CopyBlit->poDescriptorSetLayoutNames = &this->aNameDescriptorSetLayouts_CopyBlit;
				F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis_CopyBlit] Create pipeline graphics success !");
            }
            updateDescriptorSets_Graphics();
        }
    }
    void EditorCameraAxis::updateDescriptorSets_Graphics()
    {
        //CameraAxis
        {
            StringVector* pDescriptorSetLayoutNames = this->pPipelineGraphics->poDescriptorSetLayoutNames;
            F_Assert(pDescriptorSetLayoutNames != nullptr && "EditorCameraAxis::updateDescriptorSets_Graphics")
			uint32_t count_names = (uint32_t)pDescriptorSetLayoutNames->size();
			for (uint32_t i = 0; i < count_names; i++)
			{
				String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[i];

				if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_PassConstants)) //PassConstants
				{
					uint32 nUniformBlockIndex = this->pPipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
					uint32 nBindingIndex = (uint32)DescriptorSet_PassConstants;
					this->pPipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
					this->pPipelineGraphics->BindBufferUniform(this->poBufferUniform_PassCB, nBindingIndex);
				}
				else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_CameraAxisObjectConstants)) //CameraAxisObjectConstants
				{
					uint32 nUniformBlockIndex = this->pPipelineGraphics->GetUniformBlockIndex(nameDescriptorSet);
					uint32 nBindingIndex = (uint32)DescriptorSet_CameraAxisObjectConstants;
					this->pPipelineGraphics->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
					this->pPipelineGraphics->BindBufferUniform(this->poBufferUniform_ObjectCB, nBindingIndex);
				}
				else
				{
					String msg = "*********************** EditorCameraAxis::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
					F_LogError("%s", msg.c_str());
					throw std::runtime_error(msg.c_str());
				}
			}
        }
        //Quad Blit
        {
            StringVector* pDescriptorSetLayoutNames = this->pPipelineGraphics_CopyBlit->poDescriptorSetLayoutNames;
            F_Assert(pDescriptorSetLayoutNames != nullptr && "EditorCameraAxis::updateDescriptorSets_Graphics")
			uint32_t count_names = (uint32_t)pDescriptorSetLayoutNames->size();
			for (uint32_t i = 0; i < count_names; i++)
			{
				String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[i];

				if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_CopyBlitObjectConstants)) //CopyBlitObjectConstants
				{
					uint32 nUniformBlockIndex = this->pPipelineGraphics_CopyBlit->GetUniformBlockIndex(nameDescriptorSet);
					uint32 nBindingIndex = (uint32)DescriptorSet_CopyBlitObjectConstants;
					this->pPipelineGraphics_CopyBlit->BindUniformBlockBinding(nUniformBlockIndex, nBindingIndex);
					this->pPipelineGraphics_CopyBlit->BindBufferUniform(this->poBufferUniform_CopyBlitObjectCB, nBindingIndex);
				}
				else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(DescriptorSet_TextureFrameColor)) //TextureFrameColor
				{
					GLESTexture* pTexture = this->poTextureColors[0];
					this->pPipelineGraphics_CopyBlit->BindTexture(pTexture, 0);
				}
				else
				{
					String msg = "*********************** EditorCameraAxis::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
					F_LogError("%s", msg.c_str());
					throw std::runtime_error(msg.c_str());
				}
			}
        }
    }
    void EditorCameraAxis::destroyBufferUniforms()
    {
        //PassConstants
        {
			F_DELETE(this->poBufferUniform_PassCB)
        }
        //CameraAxis
        {
			F_DELETE(this->poBufferUniform_ObjectCB)
            this->cameraAxisObjectCBs.clear();
		#ifdef F_CameraAxisObjectConstants_Separate
			F_DELETE(this->poBufferUniform_ObjectCB_Last)
		#endif
        }
        //Quad Blit
        {
			F_DELETE(this->poBufferUniform_CopyBlitObjectCB)
        }
    }
    void EditorCameraAxis::destroyPipelineGraphics()
    {
        EditorBase::destroyPipelineGraphics();
		//CameraAxis
		{
			F_DELETE(this->poRenderBufferDepthStencil)
			size_t count = this->poTextureColors.size();
			for (size_t i = 0; i < count; i++)
			{
				F_DELETE(this->poTextureColors[i])
			}
			this->poTextureColors.clear();
			F_DELETE(this->poFrameBufferCameraAxis)
			F_DELETE(this->poRenderPassCameraAxis)
		}
        //Quad Blit
        {   
            F_DELETE(this->pPipelineGraphics_CopyBlit)
        }   
    }
    void EditorCameraAxis::destroyPipelineLayout()
    {
        EditorBase::destroyPipelineLayout();
        //Quad Blit
        {   
			
        }   
    }
    void EditorCameraAxis::destroyDescriptorSetLayout()
    {
        EditorBase::destroyDescriptorSetLayout();
        //Quad Blit
        {   
			
        } 
    }
    void EditorCameraAxis::CleanupSwapChain()
    {
        EditorBase::CleanupSwapChain();

    }
    void EditorCameraAxis::RecreateSwapChain()
    {
        EditorBase::RecreateSwapChain();

    }

}; //LostPeterOpenGLES