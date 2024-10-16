#ifndef M3D_BLINE_H
#define M3D_BLINE_H

#include "egg/gfx/eggTexture.h"
#include "m/m3d/m_proc.h"
#include "m/m_math.h"
#include "nw4r/ut/ut_Color.h"

namespace m3d {

// The Actual line
class bline_c {
public:
    bline_c() : mpPathArr(0), mpVtxPosArr(0), mpVtxNrmArr(0), mpVtxTexArr(0), mFlags(0) {}
    // This is mainly a Guess, When the array is created, it has both a ctor/dtor
    struct VtxPos {
        mVec3_c pos1;
        mVec3_c pos2;
    };
    struct Vec3u8 {
        u8 x, y, z;
    };
    // This is mainly a Guess, When the array is created, it has only a ctor
    struct VtxNrm {
#pragma warning off(10402)
        union {
            struct {
                Vec3u8 nrm1;
                Vec3u8 nrm2;
            };
            EGG::Vector3s nrm_u16; // There is a short by short copy later
        };
#pragma warning on(10402)
    };
    // This is mainly a Guess, When the array is created, it doesnt use the array alloc
    struct VtxTex {
        f32 tex;
    };

    /* 0x00 */ u8 field_0x00[0x8];

    bool create(EGG::Heap *pHeap, u16 numPts, f32 width, f32 repeat, const nw4r::ut::Color &color);
    void update(mVec3_c *startPos);
    void remove();
    void draw();

    // vt at 0x08
    virtual ~bline_c();

    /* 0x0C */ f32 mWidth; // could be a scale too
    /* 0x10 */ nw4r::ut::Color mColor;
    /* 0x14 */ mVec3_c *mpPathArr;
    /* 0x18 */ VtxPos *mpVtxPosArr;
    /* 0x1C */ VtxNrm *mpVtxNrmArr;
    /* 0x20 */ VtxTex *mpVtxTexArr;
    /* 0x24 */ f32 mTexRepeat; // Higher value causes the texture to repeat more often
    /* 0x28 */ u16 mPathNum;   // Guess
    /* 0x2A */ u16 mVtxNum;    // Guess
    /* 0x2C */ u16 field_0x2C;
    /* 0x2E */ u8 mFlags;
    /* 0x2F    u8 _pad; */
};

class blineMat_c : public proc_c {
public:
    virtual ~blineMat_c();
    virtual void remove() override;
    virtual void drawOpa() override;
    virtual void drawXlu() override;
    virtual void setupGX(bool bTransparent);

    bool create(
        mAllocator_c *pAllocator, int numLines, u16 numLinePts, f32 width, f32 repeat, nw4r::ut::Color &color,
        EGG::ResTIMG *pTex, bool
    );
    void update();
    bline_c *getLine(u16 idx);

    /* 0x1C */ mAllocator_c mAllocator;
    /* 0x34 */ EGG::ResTIMG *mpTex;
    /* 0x38 */ nw4r::ut::List mLines;
    /* 0x44 */ bline_c *mpLineArr;
    /* 0x48 */ short mLineArrNum;
    /* 0x4A */ bool field_0x4A;
};

} // namespace m3d

#endif
