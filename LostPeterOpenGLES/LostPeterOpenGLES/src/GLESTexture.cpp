/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-11-07
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/GLESTexture.h"
#include "../include/OpenGLESWindow.h"

namespace LostPeterOpenGLES
{
    GLESTexture::GLESTexture(const String& _nameTexture,
                             const StringVector& _aPathTexture,
                             FTextureType _typeTexture,
                             FTexturePixelFormatType _typePixelFormat,
                             FTextureAddressingType _typeAddressing,
                             FTextureFilterType _typeFilterSizeMin,
                             FTextureFilterType _typeFilterSizeMag,
                             FMSAASampleCountType _numSamples,
                             const FColor& _borderColor,
                             bool _isUseBorderColor,
                             bool _isAutoMipmap,
                             bool _isGraphicsComputeShared,
                             bool _isRenderTarget,
						     bool _isUnOrderedAccess,
						     const FColor& _rtColor)
        : Base(_nameTexture)
        , aPathTexture(_aPathTexture)
        , typeTexture(_typeTexture)
        , typePixelFormat(_typePixelFormat)
        , typeAddressing(_typeAddressing)
        , typeFilterSizeMin(_typeFilterSizeMin)
        , typeFilterSizeMag(_typeFilterSizeMag)
        , numSamples(_numSamples)
        , borderColor(_borderColor)
        , isUseBorderColor(_isUseBorderColor)
        , isGraphicsComputeShared(_isGraphicsComputeShared)
        , isRenderTarget(_isRenderTarget)
        , isUnOrderedAccess(_isUnOrderedAccess)
		, rtColor(_rtColor)
        
        , width(0)
        , height(0)
        , depth(0)
        , channel(4)

        , mipMapCount(1)
        , isAutoMipmap(_isAutoMipmap)

        //Texture 3D
        , pDataRGBA(nullptr)
        , isDeleteRGBA(true)

        //Texture Animation
        , texChunkMaxX(0)
        , texChunkMaxY(0)
        , texChunkIndex(0)
        , frameCurrent(0)

        //Texture RenderTarget
        , rtColorDefault(0, 0, 0, 1)
        , rtIsSetColor(false)

        , nTextureID(0)
    {

    }
    GLESTexture::~GLESTexture()
    {
        Destroy();
    }

    void GLESTexture::Destroy()
    {
        if (this->nTextureID > 0)
        {
            Base::GetWindowPtr()->destroyGLTexture(this->nTextureID);
        }
        this->nTextureID = 0;
        if (this->isDeleteRGBA)
        {
            F_DELETE_T(this->pDataRGBA)
        }
    }

    bool GLESTexture::Init()
    {
        return LoadTexture(0, 
                           0, 
                           0, 
                           4, 
                           nullptr);
    }

    int GLESTexture::RandomTextureIndex()
    {
        if (this->typeTexture == F_Texture_2DArray)
        {
            int count = (int)this->aPathTexture.size();
            return FMath::Rand(0, count - 1);
        }
        return 0;
    }

    bool GLESTexture::LoadTexture(int width,
                                int height,
                                int depth,
                                int channel,
                                uint8* pData)
    {   
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->channel = channel;

        OpenGLESWindow* pWindow = Base::GetWindowPtr();

        if (!this->isRenderTarget)
        {
            if (this->typeTexture == F_Texture_1D)
            {
                if (!pWindow->createTexture1D(this->name,
											  this->aPathTexture[0], 
											  this->mipMapCount,
											  this->isAutoMipmap,
											  this->typeTexture,
											  this->typePixelFormat,
											  this->typeAddressing,
											  this->typeFilterSizeMin,
											  this->typeFilterSizeMag,
											  this->numSamples,
											  this->borderColor,
											  this->isUseBorderColor,
											  this->isGraphicsComputeShared,
											  this->isUnOrderedAccess,
											  this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture 1d, name: [%s], path: [%s] !", this->name.c_str(), this->aPathTexture[0].c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_2D)
            {
                if (!pWindow->createTexture2D(this->name,
                                              this->aPathTexture[0], 
                                              this->mipMapCount,
                                              this->isAutoMipmap,
                                              this->typeTexture,
                                              this->typePixelFormat,
                                              this->typeAddressing,
                                              this->typeFilterSizeMin,
                                              this->typeFilterSizeMag,
                                              this->numSamples,
                                              this->borderColor,
                                              this->isUseBorderColor,
                                              this->isGraphicsComputeShared,
                                              this->isUnOrderedAccess,
                                              this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture 2d, name: [%s], path: [%s] !", this->name.c_str(), this->aPathTexture[0].c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_2DArray)
            {
                if (!pWindow->createTexture2DArray(this->name,
												   this->aPathTexture, 
												   this->mipMapCount,
												   this->isAutoMipmap,
												   this->typeTexture,
												   this->typePixelFormat,
												   this->typeAddressing,
												   this->typeFilterSizeMin,
												   this->typeFilterSizeMag,
												   this->numSamples,
												   this->borderColor,
												   this->isUseBorderColor,
												   this->isGraphicsComputeShared,
												   this->isUnOrderedAccess,
												   this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture 2d array, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_3D)
            {
                if (!pWindow->createTexture3D(this->name,
											  this->aPathTexture[0], 
											  this->mipMapCount,
											  this->isAutoMipmap,
											  this->typeTexture,
											  this->typePixelFormat,
											  this->typeAddressing,
											  this->typeFilterSizeMin,
											  this->typeFilterSizeMag,
											  this->numSamples,
											  this->borderColor,
											  this->isUseBorderColor,
											  this->isGraphicsComputeShared,
											  this->isUnOrderedAccess,
											  this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture 3d, name: [%s], path: [%s] !", this->name.c_str(), this->aPathTexture[0].c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_CubeMap)
            {
                if (!pWindow->createTextureCubeMap(this->name,
												   this->aPathTexture, 
												   this->mipMapCount,
												   this->isAutoMipmap,
												   this->typeTexture,
												   this->typePixelFormat,
												   this->typeAddressing,
												   this->typeFilterSizeMin,
												   this->typeFilterSizeMag,
												   this->numSamples,
												   this->borderColor,
												   this->isUseBorderColor,
												   this->isGraphicsComputeShared,
												   this->isUnOrderedAccess,
												   this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture cubemap, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else
            {
                String msg = "*********************** GLESTexture::LoadTexture: Wrong texture type, name: [" + this->name + "] !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        else
        {
            if (this->typeTexture == F_Texture_1D)
            {
                if (!pWindow->createTextureRenderTarget1D(this->name,
														  pData, 
														  channel,
														  width,
														  height,
														  this->mipMapCount,
														  this->isAutoMipmap,
														  this->typeTexture,
														  this->typePixelFormat,
														  this->typeAddressing,
														  this->typeFilterSizeMin,
														  this->typeFilterSizeMag,
														  this->numSamples,
														  this->borderColor,
														  this->isUseBorderColor,
														  this->isGraphicsComputeShared,
														  this->isUnOrderedAccess,
														  this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture RenderTarget1D, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_2D)
            {
                if (!pWindow->createTextureRenderTarget2D(this->name,
                                                          pData, 
                                                          channel,
                                                          width,
                                                          height,
                                                          this->mipMapCount,
                                                          this->isAutoMipmap,
                                                          this->typeTexture,
                                                          this->typePixelFormat,
                                                          this->typeAddressing,
                                                          this->typeFilterSizeMin,
                                                          this->typeFilterSizeMag,
                                                          this->numSamples,
                                                          this->borderColor,
                                                          this->isUseBorderColor,
                                                          this->isGraphicsComputeShared,
                                                          this->isUnOrderedAccess,
                                                          this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture RenderTarget2D, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_2DArray)
            {
                if (!pWindow->createTextureRenderTarget2DArray(this->name,
														   	   pData, 
															   channel,
															   width,
															   height,
															   depth,
															   this->mipMapCount,
															   this->isAutoMipmap,
															   this->typeTexture,
															   this->typePixelFormat,
															   this->typeAddressing,
															   this->typeFilterSizeMin,
															   this->typeFilterSizeMag,
															   this->numSamples,
															   this->borderColor,
															   this->isUseBorderColor,
															   this->isGraphicsComputeShared,
															   this->isUnOrderedAccess,
															   this->rtColorDefault,
															   this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture RenderTarget2DArray, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_3D)
            {
                if (pData == nullptr)
                {
                    uint32_t size = width * height * depth * channel;
                    this->pDataRGBA = new uint8[size];
                    memset(this->pDataRGBA, 0, (size_t)size);
                    updateNoiseTextureData();
                    this->isDeleteRGBA = true;
                }
                else 
                {
                    this->pDataRGBA = pData;
                    this->isDeleteRGBA = false;
                }
				if (!pWindow->createTextureRenderTarget3D(this->name,
														  this->pDataRGBA, 
														  channel,
														  width,
														  height,
														  depth,
														  this->mipMapCount,
														  this->isAutoMipmap,
														  this->typeTexture,
														  this->typePixelFormat,
														  this->typeAddressing,
														  this->typeFilterSizeMin,
														  this->typeFilterSizeMag,
														  this->numSamples,
														  this->borderColor,
														  this->isUseBorderColor,
														  this->isGraphicsComputeShared,
														  this->isUnOrderedAccess,
														  this->rtColorDefault,
														  this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture RenderTarget3D, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else if (this->typeTexture == F_Texture_CubeMap)
            {
                if (!pWindow->createTextureRenderTargetCubeMap(this->name,
														   	   pData, 
															   channel,
															   width,
															   height,
															   depth,
															   this->mipMapCount,
															   this->isAutoMipmap,
															   this->typeTexture,
															   this->typePixelFormat,
															   this->typeAddressing,
															   this->typeFilterSizeMin,
															   this->typeFilterSizeMag,
															   this->numSamples,
															   this->borderColor,
															   this->isUseBorderColor,
															   this->isGraphicsComputeShared,
															   this->isUnOrderedAccess,
															   this->rtColorDefault,
															   this->nTextureID))
                {
                    F_LogError("*********************** GLESTexture::LoadTexture: Failed to create texture RenderTargetCubeMap, name: [%s] !", this->name.c_str());
                    return false;
                }
            }
            else
            {
                String msg = "*********************** GLESTexture::LoadTexture: Wrong texture type, name: [" + this->name + "] !";
                F_LogError("%s", msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        return true;
    }

    void GLESTexture::UpdateTexture()
    {
        if (this->typeTexture == F_Texture_3D)
        {
            updateNoiseTexture();
        }
    }
    void GLESTexture::updateNoiseTextureData()
    {
        // Perlin noise
        noise::module::Perlin modulePerlin;
        for (int z = 0; z < this->depth; z++)
        {
            for (int y = 0; y < this->height; y++)
            {
                for (int x = 0; x < this->width; x++)
                {
                    float nx = (float)x / (float)this->width;
                    float ny = (float)y / (float)this->height;
                    float nz = (float)z / (float)this->depth;

                    float n = 20.0f * (float)(modulePerlin.GetValue(nx, ny, nz));
                    n = n - floor(n);
                    uint8 v = static_cast<uint8>(floor(n * 255));
                    int address = x + y * this->width + z * this->width * this->height;
                    this->pDataRGBA[address + 0] = v;
                    if (channel > 1)
                         this->pDataRGBA[address + 1] = v;
                    if (channel > 2)
                         this->pDataRGBA[address + 2] = v;
                    if (channel > 3)
                         this->pDataRGBA[address + 3] = v;
                }
            }
        }
    }
    void GLESTexture::updateNoiseTexture()
    {
        //1> updateNoiseTextureData
        updateNoiseTextureData();

        
    }

    void GLESTexture::BindTexture(uint slot, bool enable)
    {
        if (this->nTextureID > 0)
        {
            if (enable)
                Base::GetWindowPtr()->bindGLTexture(this->typeTexture, slot, this->nTextureID);
            else
                Base::GetWindowPtr()->bindGLTexture(this->typeTexture, slot, 0);
        }
    }

    void GLESTexture::BindTextureImage(uint slot, uint32 access, bool enable)
    {
        if (this->nTextureID > 0)
        {
            if (enable)
                Base::GetWindowPtr()->bindGLTextureImage(this->typeTexture, slot, this->nTextureID, access, Util_Transform2GLFormatInternal(this->typePixelFormat));
            else
                Base::GetWindowPtr()->bindGLTextureImage(this->typeTexture, slot, 0, access, GL_RGBA8);
        }
    }

}; //LostPeterOpenGLES