#ifndef BLUR_AND_PALETTE_MANAGER_H
#define BLUR_AND_PALETTE_MANAGER_H

#include "common.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "toBeSorted/tlist.h"

struct UnkBlurPaletteListNode {
    TListNode<UnkBlurPaletteListNode> mNode;
};

struct SffSub {
    mColor field_0x00;
    f32 field_0x04;
    f32 field_0x08;
};

struct SpfSub2 {
    mColor field_0x00;
    u8 field_0x04[5];
};

// This is a weird one - mColor / nw4r::ut::Color are assumed
// to be 4-byte-aligned so that all the u32 type punning works
// correctly, but field_0x1D5 is unaligned and uses a separate
// assignment operator! So we'll go with a separate type for now.
struct UnalignedColor {
    UnalignedColor &operator=(const mColor &color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
        return *this;
    }
    u8 r, g, b, a;
};

struct PaletteDefaultMCF {
    /* 0x00 */ mColor MA00_kColor3;
    /* 0x04 */ mColor MA01_tevReg1;
    /* 0x08 */ mColor MA01_kColor2;
    /* 0x0C */ mColor MA02_tevReg1;
    /* 0x10 */ mColor MA02_kColor2;
    /* 0x14 */ mColor MA03_kColor3;
    /* 0x18 */ mColor MA06_kColor3;
    /* 0x1C */ mColor field_0x1C;
};

struct TwoPaletteDefaultMCF {
    /* 0x00 */ PaletteDefaultMCF field_0x00;
    /* 0x20 */ PaletteDefaultMCF field_0x20;
};

// Ghidra had contents of this in an array but that won't work due to dtor codegen.
// Start is correct (dtor offset), needs to at least include field_0x104
struct SpfSub {
    /* 0x000 */ f32 field_0x000;
    /* 0x004 */ mColor grey_787878;
    /* 0x008 */ SffSub white3_ffffff;
    /* 0x014 */ SpfSub2 field_0x014;
    /* 0x020 */ mColor field_0x020[5];
    /* 0x034 */ f32 field_0x034[5];
    /* 0x048 */ f32 field_0x048[5];
    /* 0x05C */ f32 field_0x05C;
    /* 0x060 */ SpfSub2 field_0x060;
    /* 0x06C */ mColor field_0x06C[5];
    /* 0x080 */ f32 field_0x080[5];
    /* 0x094 */ f32 field_0x094[5];
    /* 0x0A8 */ f32 field_0x0A8;
    /* 0x0AC */ SpfSub2 field_0x0AC;
    /* 0x0B8 */ mColor field_0x0B8[5];
    /* 0x0CC */ f32 field_0x0CC[5];
    /* 0x0E0 */ f32 field_0x0E0[5];
    /* 0x0F4 */ f32 field_0x0F4;
    /* 0x0F8 */ SpfSub2 field_0x0F8;
    /* 0x104 */ mColor field_0x104[5];
    /* 0x118 */ f32 field_0x118[5];
    /* 0x12C */ f32 field_0x12C[5];
};

// We assume there's SffSub in here because of weak function order
// The size is correct now, but the default assignment operator doesn't match yet,
// which could have many reasons. Need to tweak the layout.
struct Spf {
    Spf();
    ~Spf() {}

    /* 0x000 */ mColor grey_aaaaaa;
    /* 0x004 */ mColor white_ffffff;
    /* 0x008 */ mColor grey_808080;
    /* 0x00C */ mColor cream_ffffc0;
    /* 0x010 */ mColor grey2_aaaaaa;
    /* 0x014 */ mColor white2_ffffff;
    /* 0x018 */ f32 field_0x18;
    /* 0x01C */ mColor pastel_orange_ffb787;
    /* 0x020 */ f32 field_0x20;
    /* 0x024 */ f32 field_0x24;
    /* 0x028 */ mColor black_000000;
    /* 0x02C */ mColor black2_000000;
    /* 0x030 */ SpfSub mSub;
    /* 0x170 */ mColor grey_787878;
    /* 0x174 */ mColor grey2_787878;
    /* 0x178 */ mColor grey3_787878;
    /* 0x17C */ mColor mCloudColor;
    /* 0x180 */ mColor mSkyColor;
    /* 0x184 */ mColor seafoam_green_b9d8b7;
    /* 0x188 */ mColor white3_ffffff;
    /* 0x18C */ mVec3_c field_0x18C;
    /* 0x198 */ mVec3_c field_0x198;
    /* 0x1A4 */ SffSub MAO5_kColor3; // assuming SffSub here due to initialization with same values
    /* 0x1B0 */ mVec3_c field_0x1B0;
    /* 0x1BC */ u8 field_0x1BC;
    /* 0x1C0 */ f32 field_0x1C0;
    /* 0x1C4 */ f32 field_0x1C4;
    /* 0x1C8 */ u8 field_0x1C8;
    /* 0x1CC */ f32 field_0x1CC;
    /* 0x1D0 */ f32 field_0x1D0;
    /* 0x1D4 */ u8 field_0x1D4;
    /* 0x1D5 */ UnalignedColor field_0x1D5; // pastel_yelow_#fff0b4
    /* 0x1D9 */ u8 field_0x1D9;
    /* 0x1DC */ f32 field_0x1DC;
    /* 0x1E0 */ f32 field_0x1E0;
    /* 0x1E4 */ f32 field_0x1E4;
    /* 0x1E8 */ f32 field_0x1E8;
    /* 0x1EC */ u8 field_0x1EC;
    /* 0x1ED */ u8 field_0x1ED;
    /* 0x1EE */ u16 field_0x1EE;
    /* 0x1F0 */ f32 field_0x1F0;
    /* 0x1F4 */ f32 field_0x1F4;
    /* 0x1F8 */ f32 field_0x1F8;
    /* 0x1FC */ f32 field_0x1FC;
    /* 0x200 */ mColor black3_000000;
    /* 0x204 */ mColor black4_000000;
};

struct Sff {
    Sff();
    ~Sff() {}

    SffSub field_0x00[3];
};

struct Bpm1 {
    Bpm1() {}
    ~Bpm1() {}

    u8 field_0x00[0x14 - 0x00];
    mColor field_0x14;
};

struct Bpm2 {
    Bpm2() {}
    ~Bpm2() {}

    // maybe
    u8 _0x00[0x14 - 0x00];
};

struct PaletteEAF_smol_entry {
    PaletteEAF_smol_entry() {}
    ~PaletteEAF_smol_entry() {}
    mColor field_0x00;
    mColor field_0x04;
    mColor field_0x08;
    mColor field_0x0C;
};

struct PaletteEAF_smol {
    ~PaletteEAF_smol() {}
    PaletteEAF_smol_entry field_0x00[8];
};

struct PaletteEAF_big_entry {
    PaletteEAF_big_entry() {}
    ~PaletteEAF_big_entry() {}
    mColor field_0x00;
    mColor field_0x04;
    mColor field_0x08;
    mColor field_0x0C;
    u8 field_0x10;
};


struct PaletteEAF_big {
    ~PaletteEAF_big() {}
    PaletteEAF_big_entry field_0x00[8];
};

// Apparently particle color related
struct Bpm3 {
    Bpm3() {}
    ~Bpm3() {}

    PaletteEAF_smol field_0x00[0x20];
};

struct Bpm4 {
    Bpm4() {}
    ~Bpm4() {}

    PaletteEAF_big field_0x00[0x20];
};

struct Bpm7 {
    Bpm7() {}
    ~Bpm7() {}

    u8 _0x00[0x14 - 0x00];
};

struct Bpm8 {
    Bpm8() {}
    ~Bpm8() {}

    u8 _0x00[0x14 - 0x00];
};

struct Bpm9 {
    Bpm9() {}
    ~Bpm9() {}

    u8 _0x00[0x14 - 0x00];
};

class BlurAndPaletteManager {
public:
    BlurAndPaletteManager();
    virtual ~BlurAndPaletteManager() {
        sPInstance = nullptr;
    }

    static BlurAndPaletteManager &GetInstance() {
        return sInstance;
    }

    static BlurAndPaletteManager *GetPInstance() {
        return sPInstance;
    }

    void fn_800247D0(mVec3_c, f32);
    void fn_80022AF0(f32);
    void setField_0x2F20(f32 arg) {
        field_0x2F20 = arg;
    }
    void fn_800223A0(void *);
    void fn_80022440(void *);
    // light pillar related
    void fn_80024240(s16, s16, s16);

    u8 get0x2DE8() const {
        return field_0x2DE0[8];
    }

    const PaletteDefaultMCF &GetCurrentDefaultMcf() const {
        return field_0x5CE4;
    }

    const Spf &GetCurrentSph() const {
        return currentSpf;
    }

    const PaletteEAF_smol_entry& getSmallEAF(s32 idx1, s32 idx2) {
        return field_0x38E4.field_0x00[idx1].field_0x00[idx2];
    }

    f32 getfield_0x2F14() const {
        return field_0x2F14;
    }

    f32 getfield_0x2F18() const {
        return field_0x2F18;
    }

    f32 getfield_0x2F1C() const {
        return field_0x2F1C;
    }

    static mColor &getLightColor1();
    static mColor &getLightColor2();

private:
    mColor combineColors(const mColor &c1, const mColor &c2, f32 ratio);

    /* 0x0004 */ Spf currentSpf;
    /* 0x020C */ Spf spfs[20];
    /* 0x2AAC */ Sff currentSff;
    /* 0x2AD0 */ Sff sffs[20];
    /* 0x2DA0 */ u8 field_0x2DA0[0x2DD4 - 0x2DA0];
    /* 0x2DD4 */ mVec3_c field_0x2DD4;
    /* 0x2DE0 */ u8 field_0x2DE0[0x2DEC - 0x2DE0];
    /* 0x2DEC */ mColor field_0x2DEC;
    /* 0x2DF0 */ u8 field_0x2DF0[0x2DF4 - 0x2DF0];
    /* 0x2DF4 */ mColor field_0x2DF4;
    /* 0x2DF8 */ mVec3_c field_0x2DF8;
    /* 0x2E04 */ u8 field_0x2E04[0x2E08 - 0x2E04];
    /* 0x2E08 */ mVec3_c field_0x2E08[5];
    /* 0x2E44 */ mVec3_c field_0x2E44[5];
    /* 0x2E80 */ mVec3_c field_0x2E80[5];
    /* 0x2EBC */ mVec3_c field_0x2EBC[5];
    /* 0x2EF8 */ mColor field_0x2EF8;
    /* 0x2EFC */ mColor field_0x2EFC;
    /* 0x2F00 */ mVec3_c field_0x2F00;
    /* 0x2F0C */ s16 field_0x2F0C;
    /* 0x2F0E */ s16 field_0x2F0E;
    /* 0x2F10 */ u8 field_0x2F10;
    /* 0x2F14 */ f32 field_0x2F14;
    /* 0x2F18 */ f32 field_0x2F18;
    /* 0x2F1C */ f32 field_0x2F1C;
    /* 0x2F20 */ f32 field_0x2F20;
    /* 0x2F24 */ u8 _0x2F24[0x357C - 0x2F24];
    /* 0x357C */ Bpm1 field_0x357C;
    /* 0x3594 */ u8 _0x3594[0x35A0 - 0x3594];
    /* 0x35A0 */ Bpm2 field_0x35A0;
    /* 0x35B4 */ Bpm7 field_0x35B4[8];
    /* 0x3654 */ Bpm8 field_0x3654[10];
    /* 0x371C */ Bpm9 field_0x371C[20];
    /* 0x38AC */ u8 _0x38AC[0x38B4 - 0x38AC];
    /* 0x38B4 */ mVec3_c field_0x38B4;
    /* 0x38C0 */ u8 _0x38C0[0x38C8 - 0x38C0];
    /* 0x38C8 */ mVec3_c field_0x38C8;
    /* 0x38D4 */ u8 _0x38D4[0x38E4 - 0x38D4];
    /* 0x38E4 */ Bpm3 field_0x38E4;
    /* 0x48E4 */ Bpm4 field_0x48E4;
    /* 0x5CE4 */ PaletteDefaultMCF field_0x5CE4;
    /* 0x5D04 */ TwoPaletteDefaultMCF field_0x5D04;

    static BlurAndPaletteManager sInstance;
    static BlurAndPaletteManager *sPInstance;
};

#endif
