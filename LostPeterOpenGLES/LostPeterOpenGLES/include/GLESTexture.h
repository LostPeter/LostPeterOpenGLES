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

#ifndef _GLES_TEXTURE_H_
#define _GLES_TEXTURE_H_

#include "Base.h"

namespace LostPeterOpenGLES
{
    class openglesExport GLESTexture : public Base
    {
    public:
        GLESTexture(const String& _nameTexture,
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
                    bool _isCubeMap,
                    bool _isRenderTarget,
                    bool _isGraphicsComputeShared);
        virtual ~GLESTexture();

    public:
        StringVector aPathTexture;
        FTextureType typeTexture;
        FTexturePixelFormatType typePixelFormat;
        FTextureAddressingType typeAddressing;
        FTextureFilterType typeFilterSizeMin;
        FTextureFilterType typeFilterSizeMag;
        FMSAASampleCountType numSamples;
        FColor borderColor;
        bool isUseBorderColor;
        bool isCubeMap;
        bool isRenderTarget;
        bool isGraphicsComputeShared;
        int width;
        int height;
        int depth;
        int channel;

        int mipMapCount;
        bool isAutoMipmap;

        //Texture 3D
        uint8* pDataRGBA;

        //Texture Animation
        int texChunkMaxX;
        int texChunkMaxY;
        int texChunkIndex;
        int frameCurrent;

        //Texture RenderTarget
        FVector4 rtColorDefault;
        bool rtIsSetColor; 
        

        uint32 nTextureID;

    public:
        void Destroy();
        virtual bool Init();
        virtual bool LoadTexture(int width,
                                 int height,
                                 int depth,
                                 int channel,
                                 uint8* pData);
        virtual void UpdateTexture();

    public:
        void BindTexture();
    };

}; //LostPeterOpenGLES

#endif