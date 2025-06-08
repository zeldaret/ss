#ifndef BLUR_AND_PALETTE_MANAGER_H
#define BLUR_AND_PALETTE_MANAGER_H

#include "common.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "toBeSorted/tlist.h"

class ActorLighting {
public:
    typedef TList<ActorLighting, 0> ListType;
    /* 0x00 */ TListNode<ActorLighting> mNode;
    static ListType sList;
    static void fn_80026500();
    static const mColor &getLightTev2Color();
    static const mColor &getLightTev1Color();
    static const mColor &getLightTev0Color();
    static const mColor &getLightTevKColor();

public:
    virtual ~ActorLighting();

    void reset();

public:
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ s32 mLightSetIdx;
    /* 0x40 */ mColor mTev0Color;
    /* 0x44 */ mColor mTev1Color;
    /* 0x48 */ mColor mTev2Color;
    /* 0x4C */ mColor mTevPrevColor;
    /* 0x50 */ mColor mTevK0Color;
    /* 0x54 */ mColor mTevK1Color;
    /* 0x58 */ mColor mTevK2Color;
    /* 0x5C */ mColor mTevK3Color;
    /* 0x60 */ u8 mLightingCode;
    /* 0x61 */ u8 field_0x61;
    /* 0x62 */ u8 field_0x62;
    /* 0x63 */ u8 field_0x63;
    /* 0x64 */ u8 field_0x64;
    /* 0x65 */ bool mUseTev0;
    /* 0x66 */ bool mUseTev1;
    /* 0x67 */ bool mUseTev2;
    /* 0x68 */ bool mUseTevPrev;
    /* 0x69 */ bool mUseTevK0;
    /* 0x6A */ bool mUseTevK1;
    /* 0x6B */ bool mUseTevK2;
    /* 0x6C */ bool mUseTevK3;
};

struct LIGHT_INFLUENCE {
    void SetColor(mColor clr) {
        mClr = clr;
    }
    void SetScale(f32 scale) {
        mScale = scale;
    }
    void SetPosition(const mVec3_c &pos) {
        mPos.x = pos.x;
        mPos.y = pos.y;
        mPos.z = pos.z;
    }

    /* 0x00 */ mVec3_c mPos;
    /* 0x0C */ mColor mClr;
    /* 0x10 */ f32 mScale;
    /* 0x14 */ s32 mIdx;
    /* 0x18 */ bool field_0x18;
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

struct ActorPalette {
    /* 0x000 */ mColor mAmbientClr;
    /* 0x004 */ mColor mDiffuseClr;
    /* 0x008 */ mColor mBrightnessClr;
    /* 0x00C */ mColor mSpecularClr;
    /* 0x010 */ mColor mDarkShadowClr;
    /* 0x014 */ mColor mDarkLightClr;
    /* 0x018 */ f32 mSpecular;
    /* 0x01C */ mColor field_0x01C;
    /* 0x020 */ f32 mTemperature; // -1 cool, 1 warm
    /* 0x024 */ f32 field_0x24;
    /* 0x028 */ mColor mShadowClr;
    /* 0x02C */ mColor field_0x02C;
};

struct StagePalette {
    /* 0x000 */ f32 mBrightness;
    /* 0x004 */ mColor mShadowClr;
    /* 0x008 */ mColor mLightClr;
    /* 0x00C */ f32 mSunMoonAngle1;
    /* 0x010 */ f32 mSunMoonAngle2;

    /* 0x014 */ mColor field_0x014;
    /* 0x018 */ u8 field_0x018[5];
    /* 0x020 */ mColor field_0x020[5];
    /* 0x034 */ f32 field_0x034[5];
    /* 0x048 */ f32 field_0x048[5];

    /* 0x05C */ f32 field_0x05C;
    /* 0x060 */ mColor field_0x060;
    /* 0x064 */ u8 field_0x064[5];
    /* 0x06C */ mColor field_0x06C[5];
    /* 0x080 */ f32 field_0x080[5];
    /* 0x094 */ f32 field_0x094[5];

    /* 0x0A8 */ f32 field_0x0A8;
    /* 0x0AC */ mColor field_0x0AC;
    /* 0x0B0 */ u8 field_0x0B0[5];
    /* 0x0B8 */ mColor field_0x0B8[5];
    /* 0x0CC */ f32 field_0x0CC[5];
    /* 0x0E0 */ f32 field_0x0E0[5];

    /* 0x0F4 */ f32 field_0x0F4;
    /* 0x0F8 */ mColor field_0x0F8;
    /* 0x0FC */ u8 field_0x0FC[5];
    /* 0x104 */ mColor field_0x104[5];
    /* 0x118 */ f32 field_0x118[5];
    /* 0x12C */ f32 field_0x12C[5];
};

struct SkyPalette {
    /* 0x000 */ mColor field_0x00;
    /* 0x004 */ mColor field_0x04;
    /* 0x008 */ mColor field_0x08;
    /* 0x00C */ mColor mCloudColor;
    /* 0x010 */ mColor mSkyColor;
    /* 0x014 */ mColor field_0x14;
    /* 0x018 */ mColor mSkyFilter;
    /* 0x01C */ mVec3_c mSkyCenter;
    /* 0x028 */ mVec3_c mSkyScale;
};

struct SffSub {
    mColor field_0x00;
    f32 field_0x04;
    f32 field_0x08;
};

struct SpfSubUnk0 {
    /* 0x00 */ SffSub field_0x00;
    /* 0x0C */ f32 field_0x0C[3];
};

struct SpfSubUnk1 {
    /* 0x00 */ u8 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ u8 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ UnalignedColor field_0x19; // Bloom Color
    /* 0x1D */ u8 field_0x1D;
};

struct SpfSubUnk2 {
    /* 0x00 */ f32 field_0x00; // Blur Dist
    /* 0x04 */ f32 field_0x04; // Blur Diffuse
    /* 0x08 */ f32 field_0x08; // Softness Distance
    /* 0x0C */ f32 field_0x0C; // Softness diffuse
    /* 0x10 */ u8 field_0x10;
    /* 0x11 */ u8 field_0x11; // Turning this to not 0 disables effects
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C; // Blur?
    /* 0x20 */ f32 field_0x20; // Fog? Shadow?
};

// Broken up in a way to get the assignment operator to match.
// Not 100% certain
struct Spf {
    /* 0x000 */ ActorPalette mActorPalette;
    /* 0x030 */ StagePalette mStagePalette;
    /* 0x170 */ SkyPalette mSkyPalette;
    /* 0x1A4 */ SpfSubUnk0 field_0x1A4;
    /* 0x1BC */ SpfSubUnk1 field_0x1BC;
    /* 0x1DC */ SpfSubUnk2 field_0x1D4;
    /* 0x200 */ mColor mParticleTransparentClr;
    /* 0x204 */ mColor mParticleSolidClr;
};

struct Sff {
    SffSub field_0x00[3];
};

struct EFLIGHT_PROC {
    /* 0x00 */ u8 mState;
    /* 0x01 */ u8 mFrame;
    /* 0x04 */ int mLightType;
    /* 0x08 */ LIGHT_INFLUENCE field_0x8;
};

// Deals with the direction wind particles go?
struct WIND_INFLUENCE {
    /* 0x00 */ mVec3_c mVel;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
};

struct PaletteEAF_smol_entry {
    mColor field_0x00;
    mColor field_0x04;
    mColor field_0x08;
    mColor field_0x0C;
};

struct PaletteEAF_smol {
    PaletteEAF_smol_entry field_0x00[8];
};

struct PaletteEAF_big_entry {
    mColor field_0x00;
    mColor field_0x04;
    mColor field_0x08;
    mColor field_0x0C;
    u8 field_0x10;
};

struct PaletteEAF_big {
    PaletteEAF_big_entry field_0x00[8];
};

// Apparently particle color related
struct Bpm3 {
    PaletteEAF_smol field_0x00[0x20];
};

struct Bpm4 {
    PaletteEAF_big field_0x00[0x20];
};

struct SHADOW_INFLUENCE {
    /* 0x00 */ mVec3_c mPos;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ s16 mIdx;
};
struct Bpm8 {
    /* 0x00 */ mVec3_c mPos;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ s32 field_0x10;
};

struct Bpm9 {
    /* 0x00 */ bool field_0x00;
    /* 0x04 */ mVec3_c mPos;
    /* 0x10 */ f32 mRadius;
};

struct Mist {
    /* 0x00 */ u8 mMode;
    /* 0x01 */ u8 mDecay;
    /* 0x02 */ u8 mAlpha;
    /* 0x04 */ f32 mDirection;
    /* 0x08 */ f32 mSpeed;
};

struct SpfSetting {
    s16 mIdxStart;
    s16 mIdxEnd;
    f32 mRatio;
};

struct SpfTransitionSetting {
    s16 mIdxBefore;
    s16 mIdxAfter;
    s32 mNumFrames;
};

class BlurAndPaletteManager {
public:
    BlurAndPaletteManager();
    virtual ~BlurAndPaletteManager();

    static BlurAndPaletteManager &GetInstance() {
        return sInstance;
    }

    static BlurAndPaletteManager *GetPInstance() {
        return sPInstance;
    }

    void get_vectle_calc(const mVec3_c *, const mVec3_c *, mVec3_c *);
    void sphere_to_cartesian(f32 ang_y, f32 ang_xz, mVec3_c *);

    void efplight_set(LIGHT_INFLUENCE *pLightInfo);
    void efplight_cut(LIGHT_INFLUENCE *pLightInfo);
    LIGHT_INFLUENCE *eflight_influence(const mVec3_c *);

    void SordFlush_set(const mVec3_c *pPos, s32 lightType); // Name is guess based on closest func in tp

    void plight_set(LIGHT_INFLUENCE *pLightInfo);
    void plight_cut(LIGHT_INFLUENCE *pLightInfo);
    LIGHT_INFLUENCE *light_influence(const mVec3_c *, bool param2);
    LIGHT_INFLUENCE *light_influence2(const mVec3_c *, bool param2);

    void shadow_set(SHADOW_INFLUENCE *pShadowInfo);
    void shadow_cut(SHADOW_INFLUENCE *pShadowInfo);
    SHADOW_INFLUENCE *shadow_influence(const mVec3_c *);

    void setLightFilter(f32 ratio);
    void set0x35B0(f32 f);

    // Params are guesses
    void setBPM8(const mVec3_c *pos, u32 type, f32 radius);
    void setBPM8_Type4(const mVec3_c *pos);
    void setBPM8_Type6(const mVec3_c *pos);
    void setBPM8_Type10(const mVec3_c *pos);
    void setBPM8_Type10_2(const mVec3_c *pos);
    void setBPM8_Type6_2(const mVec3_c *pos);
    bool check_BPM8(const mVec3_c *pos, mVec3_c *pOutPos);
    bool check_BPM8_1001(const mVec3_c *pos, mVec3_c *pOutPos);
    bool check_BPM8_501(const mVec3_c *pos, mVec3_c *pOutPos);

    bool setUnk(const mVec3_c *pos, f32 radius);
    void setBPM9(const mVec3_c *pos, f32 radius);
    bool check_BPM9(const mVec3_c *pos);

    s32 checkBPM9_Entity(mVec3_c *pOutPos);

    void setAlterateRoomId(s32 roomId);
    s32 getAlterateRoomId();

    mColor color_ratio_set(const mColor &start, const mColor &end, f32 ratio);
    mVec3_c vec_ratio_set(const mVec3_c &start, const mVec3_c &end, f32 ratio);
    f32 f32_ratio_set(const f32 start, const f32 end, f32 ratio);
    u16 u16_ratio_set(const u16 start, const u16 end, f32 ratio);
    u8 u8_ratio_set(const u8 start, const u8 end, f32 ratio);

    void spf_ratio_set(Spf &out, const Spf &start, const Spf &end, f32 ratio);
    void light_influence_ratio_set(
        LIGHT_INFLUENCE &out, const LIGHT_INFLUENCE &start, const LIGHT_INFLUENCE &end, f32 ratio
    );
    void sff_ratio_set(Sff &out, const Sff &start, const Sff &end, f32 ratio);

    void set_override_spf(s16 start, s16 end, f32 ratio);

    void setMist(u8 mode, u8 decay, u8 alpha, f32 dir, f32 speed);
    void setMistMode3(const mVec3_c &vel, f32 value);

    void setDOF(f32 dof);
    void disableDOF();

    void setWind(mVec3_c mPos, f32 f);

    void setField_0x2F20(f32 arg) {
        field_0x2F20 = arg;
    }

    // light pillar related
    void set_palette_transition(s16 before, s16 after, s32 numFrames);

    // All related
    void fn_80024740(u8, u8);
    bool fn_80024770(u8);
    bool fn_800247A0(u8);

    u8 get0x2DE8() const {
        return field_0x2DE8[0];
    }

    const PaletteDefaultMCF &GetCurrentDefaultMcf() const {
        return field_0x5CE4;
    }

    const Spf &GetCurrentSpf() const {
        return currentSpf;
    }

    const PaletteEAF_smol_entry &getSmallEAF(s32 idx1, s32 idx2) {
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

private:
    /* 0x0004 */ Spf currentSpf;
    /* 0x020C */ Spf spfs[20];
    /* 0x2AAC */ Sff currentSff;
    /* 0x2AD0 */ Sff sffs[20];
    /* 0x2DA0 */ SpfSetting mCurrentSpfSetting;
    /* 0x2DA8 */ u8 field_0x2DA8[0x2DB4 - 0x2DA8];
    /* 0x2DB4 */ SpfTransitionSetting mTransitionSpfSetting;
    /* 0x2DBC */ u8 field_0x2DBC[0x2DC0 - 0x2DBC];
    /* 0x2DC0 */ SpfSetting mOverrideSpfSetting;
    /* 0x2DC8 */ Mist mMistInfo;
    /* 0x2DD4 */ mVec3_c mMistPos;
    /* 0x2DE0 */ bool mDofEnabled;
    /* 0x2DE4 */ f32 mDof;
    /* 0x2DE8 */ u8 field_0x2DE8[0x2DEC - 0x2DE8];
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
    /* 0x2F24 */ u8 field_0x2F24[4];
    /* 0x2F28 */ LIGHT_INFLUENCE *efplight[5];
    /* 0x2F3C */ LIGHT_INFLUENCE *pointlight[200];
    /* 0x325C */ SHADOW_INFLUENCE *pshadow[200];
    /* 0x357C */ EFLIGHT_PROC eflight;
    /* 0x35A0 */ WIND_INFLUENCE mWind;
    /* 0x35B4 */ SHADOW_INFLUENCE field_0x35B4[8];
    /* 0x3654 */ Bpm8 field_0x3654[10];
    /* 0x371C */ Bpm9 field_0x371C[20];
    /* 0x387C */ u32 field_0x387C;
    /* 0x38B0 */ bool field_0x38B0;
    /* 0x38B4 */ mVec3_c field_0x38B4;
    /* 0x38C0 */ f32 field_0x38C0;
    /* 0x38C4 */ s32 field_0x38C4;
    /* 0x38C8 */ mVec3_c field_0x38C8;
    /* 0x38D4 */ u32 mPlayerRoomId;
    /* 0x38D8 */ s32 mAlternateRoomId;
    /* 0x38D8 */ u32 field_0x38DC;
    /* 0x38D8 */ s32 field_0x38E0;
    /* 0x38E4 */ Bpm3 field_0x38E4;
    /* 0x48E4 */ Bpm4 field_0x48E4;
    /* 0x5CE4 */ PaletteDefaultMCF field_0x5CE4;
    /* 0x5D04 */ TwoPaletteDefaultMCF field_0x5D04;
    /* 0x5D44 */ u8 field_0x5D44[0x5D59 - 0x5D44];
    /* 0x5D5A */ struct { // Anonymous until more is known
        bool mEnabled;
        u8 mValue;
    } field_0x5D59[9];
    /* 0x5D60 */ u8 field_0x5D6C[0x5D70 - 0x5D6C];

    static BlurAndPaletteManager sInstance;
    static BlurAndPaletteManager *sPInstance;
};

#endif
