#include "egg/gfx/eggTexture.h"

namespace EGG {

Texture::~Texture() {
    if (getCaptureFlag()) {
        delete[] field_0x3c;
    }
    if (getEmbPaletteDelFlag()) {
        delete mEmbPalette;
    }
}

void Texture::storeTIMG(ResTIMG const *pTIMG, u8 param0) {
    GXTlut tlut;

    if (pTIMG && param0 < GX_BIGTLUT0) {
        mTexInfo = pTIMG;
        mTexData = (void *)((int)mTexInfo + mTexInfo->imageOffset);

        if (mTexInfo->imageOffset == 0) {
            mTexData = (void *)((int)mTexInfo + 0x20);
        }

        mpPalette = NULL;
        mTlutName = 0;
        mWrapS = mTexInfo->wrapS;
        mWrapT = mTexInfo->wrapT;
        mMinFilter = mTexInfo->minFilter;
        mMagFilter = mTexInfo->magFilter;
        mMinLOD = mTexInfo->minLOD;
        mMaxLOD = mTexInfo->maxLOD;
        mLODBias = mTexInfo->LODBias;

        if (mTexInfo->numColors == 0) {
            initTexObj();
        } else {
            if (mTexInfo->numColors > 0x100) {
                tlut = (GXTlut)((param0 % 4) + GX_BIGTLUT0);
            } else {
                tlut = (GXTlut)param0;
            }

            if (mEmbPalette == NULL || !getEmbPaletteDelFlag()) {
                mEmbPalette = new Palette(
                    tlut, (GXTlutFmt)mTexInfo->colorFormat, (JUTTransparency)mTexInfo->alphaEnabled,
                    mTexInfo->numColors, (void *)(&mTexInfo->format + mTexInfo->paletteOffset)
                );
                mFlags = mFlags & 1 | 2;
            } else {
                mEmbPalette->storeTLUT(
                    tlut, (GXTlutFmt)mTexInfo->colorFormat, (JUTTransparency)mTexInfo->alphaEnabled,
                    mTexInfo->numColors, (void *)(&mTexInfo->format + mTexInfo->paletteOffset)
                );
            }
            attachPalette(mEmbPalette);
        }
    }
}

void Texture::attachPalette(Palette *param_0) {
    if (mTexInfo->indexTexture) {
        if (param_0 == NULL && mEmbPalette != NULL) {
            mpPalette = mEmbPalette;
        } else {
            mpPalette = param_0;
        }
        GXTlut name = (GXTlut)mpPalette->getTlutName();
        initTexObj(name);
    }
}

void Texture::initTexObj() {
    GXBool mipmapEnabled;
    if (mTexInfo->mipmapEnabled != 0) {
        mipmapEnabled = 1;
    } else {
        mipmapEnabled = 0;
    }
    u8 *image = ((u8 *)mTexInfo);
    image += (mTexInfo->imageOffset ? mTexInfo->imageOffset : 0x20);
    GXInitTexObj(
        &mTexObj, image, mTexInfo->width, mTexInfo->height, (GXTexFmt)mTexInfo->format, (GXTexWrapMode)mWrapS,
        (GXTexWrapMode)mWrapT, mipmapEnabled
    );
    GXInitTexObjLOD(
        &mTexObj, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter, mMinLOD / 8.0f, mMaxLOD / 8.0f, mLODBias / 100.0f,
        mTexInfo->biasClamp, mTexInfo->doEdgeLOD, (GXAnisotropy)mTexInfo->maxAnisotropy
    );
}

void Texture::initTexObj(GXTlut param_0) {
    GXBool mipmapEnabled;
    if (mTexInfo->mipmapEnabled != 0) {
        mipmapEnabled = 1;
    } else {
        mipmapEnabled = 0;
    }
    mTlutName = param_0;
    u8 *image = ((u8 *)mTexInfo);
    image += (mTexInfo->imageOffset ? mTexInfo->imageOffset : 0x20);
    GXInitTexObjCI(
        &mTexObj, image, mTexInfo->width, mTexInfo->height, (GXCITexFmt)mTexInfo->format, (GXTexWrapMode)mWrapS,
        (GXTexWrapMode)mWrapT, mipmapEnabled, param_0
    );
    GXInitTexObjLOD(
        &mTexObj, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter, mMinLOD / 8.0f, mMaxLOD / 8.0f, mLODBias / 100.0f,
        mTexInfo->biasClamp, mTexInfo->doEdgeLOD, (GXAnisotropy)mTexInfo->maxAnisotropy
    );
}

void Texture::load(GXTexMapID param_0) {
    if (mpPalette) {
        mpPalette->load();
    }
    GXLoadTexObj(&mTexObj, param_0);
}

} // namespace EGG
