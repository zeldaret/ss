#include "egg/gfx/eggGXUtility.h"

#include "egg/gfx/eggTexture.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

const u8 GXUtility::s_texMtxIds[8] = {30, 33, 36, 39, 42, 45, 48, 51};

int GXUtility::getTexMtxID(int idx) {
    return s_texMtxIds[idx];
}

void GXUtility::setScaleOffsetPerspective(f32 *pOut, f32 f1, f32 f2, f32 f3, f32 f4) {
    pOut[1] *= (1.0f / f1);
    pOut[3] *= (1.0f / f2);
    pOut[2] += f3;
    pOut[4] += f4;
}

void GXUtility::getTexObj(GXTexObj *obj, const ResTIMG &img) {
    u32 offset = img.imageOffset != 0 ? img.imageOffset : sizeof(ResTIMG);
    void *pImage = const_cast<void *>(reinterpret_cast<const void *>(reinterpret_cast<const u8 *>(&img) + offset));
    // Missing a redundant register move here (obj r30->r3)
    GXInitTexObj(
        obj, pImage, img.width, img.height, static_cast<GXTexFmt>(img.format), static_cast<GXTexWrapMode>(img.wrapS),
        static_cast<GXTexWrapMode>(img.wrapT), img.mipmapEnabled != 0
    );
    GXInitTexObjLOD(
        obj, static_cast<GXTexFilter>(img.minFilter), static_cast<GXTexFilter>(img.magFilter), img.minLOD / 8.0f,
        img.maxLOD / 8.0f, img.LODBias / 100.0f, img.biasClamp, img.doEdgeLOD,
        static_cast<GXAnisotropy>(img.maxAnisotropy)
    );
}

void GXUtility::getTexObj(
    GXTexObj *obj, nw4r::g3d::ResTex resTex, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter minFilter,
    GXTexFilter magFilter
) {
    void *pTexData;
    u16 width;
    u16 height;
    GXTexFmt format;
    f32 minLod;
    f32 maxLod;
    GXBool mipMap;
    if (resTex.GetTexObjParam(&pTexData, &width, &height, &format, &minLod, &maxLod, &mipMap)) {
        GXInitTexObj(obj, pTexData, width, height, format, wrapS, wrapT, mipMap);
        GXInitTexObjLOD(obj, minFilter, magFilter, minLod, maxLod, 0.0f, false, false, GX_ANISO_1);
    }
}

void GXUtility::getNormalColor(GXColor &outColor, const Vector3f &vec) {
    // Are there inlines that might work here? Probably!
    f32 fs[3];
    f32 g = vec.z < 0.0f ? vec.y * 128.0f + 128.0f - 0.5f : vec.z * 127.0f + 128.0f + 0.5f;
    f32 b = vec.y < 0.0f ? vec.y * 128.0f + 128.0f - 0.5f : vec.y * 127.0f + 128.0f + 0.5f;
    f32 a = vec.x < 0.0f ? vec.x * 128.0f + 128.0f - 0.5f : vec.x * 127.0f + 128.0f + 0.5f;

    fs[0] = a;
    fs[1] = b;
    fs[2] = g;

    if (fs[0] < 0.0f) {
        fs[0] = 0.0f;
    } else if (fs[0] > 255.0f) {
        fs[0] = 255.0f;
    }
    if (fs[1] < 0.0f) {
        fs[1] = 0.0f;
    } else if (fs[1] > 255.0f) {
        fs[1] = 255.0f;
    }
    if (fs[2] < 0.0f) {
        fs[2] = 0.0f;
    } else if (fs[2] > 255.0f) {
        fs[2] = 255.0f;
    }

    outColor.a = fs[0];
    outColor.b = fs[1];
    outColor.g = fs[2];
}

} // namespace EGG
