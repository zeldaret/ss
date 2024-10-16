#ifndef NW4R_G3D_RESMAT_H
#define NW4R_G3D_RESMAT_H
#include "common.h"
#include "nw4r/g3d/g3d_rescommon.h"
#include "nw4r/math/math_types.h" // IWYU pragma: export
#include "rvl/GX.h"               // IWYU pragma: export

namespace nw4r {
namespace g3d {

struct ResTevColorDL {
    union {
        struct {
            u8 tevColor[3][20];  // offset 0x0, size 0x3C
            u8 _0[4];            // offset 0x3C, size 0x4
            u8 tevKColor[4][10]; // offset 0x40, size 0x28
            u8 _1[24];           // offset 0x68, size 0x18
        } dl;
        u8 data[128];
    };
};

struct ResMatTevColor : public ResCommon<ResTevColorDL> {
    ResMatTevColor(void *vptr) : ResCommon<ResTevColorDL>(vptr) {}
    void DCStore(bool sync);
    void CallDisplayList(bool bSync) const;
    ResMatTevColor CopyTo(void *p) const;
    void GXGetTevColor(GXTevRegID id, GXColor *color);
    void GXSetTevColor(GXTevRegID id, GXColor color);
    void GXSetTevKColor(GXTevKColorID id, GXColor color);
};

struct ResPixDL {
    union {
        struct {
            unsigned char alphaCompare[5]; // offset 0x0, size 0x5
            unsigned char zMode[5];        // offset 0x5, size 0x5
            unsigned char blendMode[10];   // offset 0xA, size 0xA
            unsigned char setDstAlpha[5];  // offset 0x14, size 0x5
            unsigned char _[7];            // offset 0x19, size 0x7
        } dl;                              // offset 0x0, size 0x20
        unsigned char data[32];            // offset 0x0, size 0x20
    };
};

struct ResMatPix : public ResCommon<ResPixDL> {
    ResMatPix(void *vptr) : ResCommon(vptr) {}
    void DCStore(bool sync);
    void GXGetDstAlpha(u8 *, u8 *) const;
    void GXSetDstAlpha(u8, u8);
    void CallDisplayList(bool bSync) const;
    ResMatPix CopyTo(void *p) const;

    void GXSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
};

struct ResTexCoordGenDL {};

struct ResMatTexCoordGen : public ResCommon<ResTexCoordGenDL> {
    inline ResMatTexCoordGen(void *vptr) : ResCommon(vptr) {}
    bool IsValid() const {
        return ResCommon::IsValid();
    }

    bool GXGetTexCoordGen2(GXTexCoordID, GXTexGenType *, GXTexGenSrc *, u8 *, u32 *);
    void GXSetTexCoordGen2(GXTexCoordID, GXTexGenType, GXTexGenSrc, u8, u32);
    void DCStore(bool);
};

struct ResTexSrtData {
    union {
        struct {
            unsigned char texCoordGen[8][18]; // offset 0x0, size 0x90
            unsigned char _[16];              // offset 0x90, size 0x10
        } dl;                                 // offset 0x0, size 0xA0
        unsigned char data[160];              // offset 0x0, size 0xA0
    };
};

struct ResTexSrt : public ResCommon<ResTexSrtData> {
    inline ResTexSrt(void *vptr) : ResCommon(vptr) {}

    void SetMapMode(u32, u32, int, int);
};

struct ResTexPlttInfoData {
    u32 mNumOffsets; // at 0x0

    struct InfoOffset {
        u32 mResOffset; // at 0x0
        u32 mTexOffset; // at 0x4
    } mOffsets[];       // at 0x4
};

struct ResTexPlttInfo {
    ResCommon<ResTexPlttInfoData> mInfo;

    inline ResTexPlttInfo(void *vptr) : mInfo(vptr) {}
};

struct ResMatDLData {
    ResPixDL dlPix;                                      // offset 0x0, size 0x20
    ResTevColorDL dlTevColor;                            // offset 0x20, size 0x80
    /* ResIndMtxAndScaleDL */ u8 dlIndMtxAndScale[0x40]; // offset 0xA0, size 0x40
    ResTexCoordGenDL dlTexCoordGen;                      // offset 0xE0, size 0xA0
};

struct ResGenModeData {
    u8 nTexGens;         // offset 0x0, size 0x1
    u8 nChans;           // offset 0x1, size 0x1
    u8 nTevs;            // offset 0x2, size 0x1
    u8 nInds;            // offset 0x3, size 0x1
    GXCullMode cullMode; // offset 0x4, size 0x4
};

struct ResGenMode : public ResCommon<ResGenModeData> {
    ResGenMode(void *vptr) : ResCommon(vptr) {}
    void DCStore(bool sync);
    ResGenMode CopyTo(void *p) const;
    GXCullMode GXGetCullMode();
    u8 GXGetNumIndStages() const;
    u8 GXGetNumTevStages() const;
    u8 GXGetNumChans() const;
    u8 GXGetNumTexGens() const;
    void GXSetCullMode(GXCullMode);
};

struct ResTexObjData {};

struct ResTexObj {
    ResCommon<ResTexObjData> mTexObj;
    inline ResTexObj(void *vptr) : mTexObj(vptr) {}

    GXTexObj *GetTexObj(GXTexMapID);
};

struct ResMatData {
    u32 size;                                  // offset 0x0, size 0x4
    s32 toResMdlData;                          // offset 0x4, size 0x4
    s32 name;                                  // offset 0x8, size 0x4
    u32 id;                                    // offset 0xC, size 0x4
    u32 flag;                                  // offset 0x10, size 0x
    ResGenModeData genMode;                    // offset 0x14, size 0x8
    /* ResMatMiscData */ u8 misc[0xC];         // offset 0x1C, size 0xC
    s32 toResTevData;                          // offset 0x28, size 0x4
    u32 numResTexPlttInfo;                     // offset 0x2C, size 0x4
    s32 toResTexPlttInfo;                      // offset 0x30, size 0x4
    s32 toResMatFurData;                       // offset 0x34, size 0x4
    u32 toResUserData;                         // offset 0x38, size 0x4
    u32 toResMatDLData;                        // offset 0x3C, size 0x4
    /* ResTexObjData */ u8 texObjData[0x104];  // offset 0x40, size 0x104
    /* ResTlutObjData */ u8 tlutObjData[0x64]; // offset 0x144, size 0x64
    ResTexSrtData texSrtData;                  // offset 0x1A8, size 0x248
    /* ResChanData */ u8 chan[0x20];           // offset 0x3F0, size 0x28
};

struct ResMat : public ResCommon<ResMatData> {
    ResMat(void *ptr) : ResCommon<ResMatData>(ptr) {}

    ResMatTevColor GetResMatTevColor() {
        return ResMatTevColor(&ofs_to_ptr<ResMatDLData>(ref().toResMatDLData)->dlTevColor);
    }

    ResMatPix GetResMatPix() {
        return ResMatPix(&ofs_to_ptr<ResMatDLData>(ref().toResMatDLData)->dlPix);
    }

    ResGenMode GetResGenMode() {
        return ResGenMode(&ref().genMode);
    }

    ResTexSrt GetResTexSrt() {
        return ResTexSrt(&ref().texSrtData);
    }

    ResTexObj GetResTexObj() {
        return ResTexObj(&ref().texObjData);
    }

    ResMatTexCoordGen GetResMatTexCoordGen() {
        return ResMatTexCoordGen(&ofs_to_ptr<ResMatDLData>(ref().toResMatDLData)->dlTexCoordGen);
    }

    bool IsOpaque() const;

    bool Bind(ResFile);
    UNKTYPE Release();
    UNKTYPE Init();
};
} // namespace g3d
} // namespace nw4r

#endif
