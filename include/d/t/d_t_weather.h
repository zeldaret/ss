#ifndef D_T_WEATHER_H
#define D_T_WEATHER_H

#include "common.h"
#include "egg/gfx/eggTexture.h"
#include "m/m3d/m_proc.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

// vtable order... maybe move to other header?
struct notsure {
    notsure() : field_0x08(0xFFFFFF), field_0x0C(0), field_0x10(0) {}
    virtual ~notsure() {}

    /* 0x04 */ u8 _0x04[4];
    /* 0x08 */ UNKWORD field_0x08;
    /* 0x0C */ UNKWORD field_0x0C;
    /* 0x10 */ UNKWORD field_0x10;
};

#include "d/t/d_tg.h"

class dWeatherTag_c : public dTg_c {
public:
    dWeatherTag_c() : mStateMgr(*this) {
        sInstance = this;
    }
    ~dWeatherTag_c() {
        sInstance = nullptr;
    }

    bool createHeap() override;
    int create() override;
    int doDelete() override;
    int draw() override;
    int actorExecute() override;
    int actorExecuteInEvent() override;

    // Names completely made up
    class rain_c : public m3d::proc_c {
    public:
        rain_c() {
            field_0x30 = 0;
            field_0x2C = 0;
            field_0x31 = 0;
            field_0x28 = 0.0f;
            mPrevCamPos.setZero();
        }

        virtual void drawXlu() override;
        void calc();

        void setRes(void *res) {
            mpTexture = (EGG::ResTIMG *)res;
        }

        void setField_0x31(u8 val) {
            field_0x31 = val;
        }

    private:
        /* 0x18 */ EGG::ResTIMG *mpTexture;
        /* 0x1C */ mVec3_c mPrevCamPos;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ s32 field_0x2C;
        /* 0x30 */ u8 field_0x30;
        /* 0x31 */ u8 field_0x31;
    };

    // "moya" (texture name) = "mist", when diving or flying in the sky
    class moya_c : public m3d::proc_c {
    private:
        static const int NUM = 100;

    public:
        moya_c() : mpTexture(nullptr) {
            mInitialSet = false;
            field_0xB81 = false;
            field_0xB82 = false;
            for (int i = 0; i < NUM; i++) {
                mParticleBasePos[i].setZero();
                mParticleOffset[i].setZero();
                mMaxAlpha[i] = 0;
                mParticleSpeed[i] = 0.0f;
            }
            mPrevCamPos.setZero();
            mAlphaScale = 0.0f;
        }

        virtual void drawXlu() override;
        void calc();

        void setRes(void *res) {
            mpTexture = (EGG::ResTIMG *)res;
        }

        void setField_0xB81(u8 val) {
            field_0xB81 = val;
        }

    private:
        /* 0x018 */ EGG::ResTIMG *mpTexture;
        /* 0x01C */ mVec3_c mParticleBasePos[NUM];
        /* 0x4CC */ mVec3_c mParticleOffset[NUM];
        /* 0x97C */ mVec3_c mPrevCamPos;
        /* 0x988 */ f32 mParticleSpeed[NUM];
        /* 0xB18 */ f32 mAlphaScale;
        /* 0xB1C */ u8 mMaxAlpha[NUM];
        /* 0xB80 */ bool mInitialSet;
        /* 0xB81 */ bool field_0xB81;
        /* 0xB82 */ bool field_0xB82;
    };

    // Aka "inYourFaceCloud", small clouds periodically appearing right in the screen center if
    class near_cloud_c : public m3d::proc_c {
    private:
        static const int NUM = 8;

    public:
        near_cloud_c() : mpTexture(nullptr), field_0xF4(0), field_0xF5(0), field_0xF6(0) {
            for (int i = 0; i < NUM; i++) {
                field_0x18[i].setZero();
                field_0xE8[i] = 0;
                field_0x78[i] = 0.0f;
                field_0x98[i] = 0.0f;
                field_0xB8[i] = 0.0f;
                field_0xD8[i] = 0;
            }
        }

        virtual void drawXlu() override;
        void calc();

        void setRes(void *res) {
            mpTexture = (EGG::ResTIMG *)res;
        }

        void setField_0xF5(u8 val) {
            field_0xF5 = val;
        }

    private:
        /* 0x18 */ mVec3_c field_0x18[NUM];
        /* 0x78 */ f32 field_0x78[NUM];
        /* 0x98 */ f32 field_0x98[NUM];
        /* 0xB8 */ f32 field_0xB8[NUM];
        /* 0xD8 */ u16 field_0xD8[NUM];
        /* 0xE8 */ u8 field_0xE8[NUM];
        /* 0xF0 */ EGG::ResTIMG *mpTexture;
        /* 0xF4 */ u8 field_0xF4;
        /* 0xF5 */ u8 field_0xF5;
        /* 0xF6 */ u8 field_0xF6;
    };

    class lightning_c : public m3d::proc_c {
    public:
        lightning_c() : field_0x4C8(0), field_0x4CC(0), field_0x4D0(0), field_0x4D4(0) {}

        virtual void drawXlu() override;
        void calc();

    private:
        /* 0x018 */ mVec3_c field_0x18[100];
        /* 0x4C8 */ UNKWORD field_0x4C8;
        /* 0x4CC */ UNKWORD field_0x4CC;
        /* 0x4D0 */ UNKWORD field_0x4D0;
        /* 0x4D4 */ u8 field_0x4D4;
    };

    class sunflare_stars_c : public m3d::proc_c {
    public:
        sunflare_stars_c()
            : field_0x018(nullptr),
              field_0x01C(nullptr),
              field_0x020(nullptr),
              field_0x024(nullptr),
              field_0x028(0.0f, 0.0f, 0.0f),
              field_0x034(0.0f, 0.0f, 0.0f),
              field_0x040(0.0f, 0.0f, 0.0f),
              field_0x04C(0.0f, 0.0f, 0.0f),
              field_0x058(0.0f, 1.0f, 0.0f),
              field_0x064(0),
              field_0x065(0),
              field_0x066(0),
              field_0x067(0),
              field_0x1A8(1.0f),
              field_0x1AC(0.5f),
              field_0x120(0.0f),
              field_0x124(0.0f),
              field_0x128(0.0f),
              field_0x1BC(0),
              field_0x1BE(0) {
            for (int i = 0; i < 16; i++) {
                field_0x068[i].field_0x08 = 0;
            }
            field_0x1C0 = 0;
            calc();
        }
        ~sunflare_stars_c() {
            remove();
        }

        virtual void drawOpa() override;
        virtual void drawXlu() override;
        virtual void remove() override;
        void calc();

        void setResLensFlare1(void *res) {
            field_0x018 = (EGG::ResTIMG *)res;
        }

        void setResStar(void *res) {
            field_0x01C = (EGG::ResTIMG *)res;
        }

        void setResRing(void *res) {
            field_0x020 = (EGG::ResTIMG *)res;
        }

        void setResLensFlare2(void *res) {
            field_0x024 = (EGG::ResTIMG *)res;
        }

        void setField_0x1C0(u8 val) {
            field_0x1C0 = val;
        }

    private:
        /* 0x018 */ EGG::ResTIMG *field_0x018;
        /* 0x01C */ EGG::ResTIMG *field_0x01C;
        /* 0x020 */ EGG::ResTIMG *field_0x020;
        /* 0x024 */ EGG::ResTIMG *field_0x024;
        /* 0x028 */ mVec3_c field_0x028;
        /* 0x034 */ mVec3_c field_0x034;
        /* 0x040 */ mVec3_c field_0x040;
        /* 0x04C */ mVec3_c field_0x04C;
        /* 0x058 */ mVec3_c field_0x058;
        /* 0x064 */ u8 field_0x064;
        /* 0x065 */ u8 field_0x065;
        /* 0x066 */ u8 field_0x066;
        /* 0x067 */ u8 field_0x067;
        /* 0x068 */ notsure field_0x068[16];
        /* 0x1A8 */ f32 field_0x1A8;
        /* 0x1AC */ f32 field_0x1AC;
        /* 0x120 */ f32 field_0x120;
        /* 0x124 */ f32 field_0x124;
        /* 0x128 */ f32 field_0x128;
        /* 0x1BC */ u16 field_0x1BC;
        /* 0x1BE */ u16 field_0x1BE;
        /* 0x1C0 */ u8 field_0x1C0;
    };

    class moon_c : public m3d::proc_c {
    public:
        moon_c() : field_0x18(nullptr), field_0x1C(nullptr), field_0x20(nullptr), field_0x24(0) {}
        ~moon_c() {
            remove();
        }

        virtual void drawOpa() override;
        void doCreate();
        virtual void remove() override;
        void calc();

    private:
        /* 0x18 */ EGG::ResTIMG* field_0x18;
        /* 0x1C */ EGG::ResTIMG* field_0x1C;
        /* 0x20 */ EGG::ResTIMG* field_0x20;
        /* 0x24 */ UNKWORD field_0x24;
    };

    class clouds_c : public m3d::proc_c {
    public:
        clouds_c() {
            init();
        }
        ~clouds_c() {
            remove();
        }

        virtual void drawOpa() override;
        virtual void drawXlu() override;
        void doCreate();
        virtual void remove() override;
        void init();
        void calc();

        void setField_0x2DD9(u8 val) {
            field_0x2DD9 = val;
        }

    private:
        /* 0x0018 */ EGG::ResTIMG *field_0x18;
        /* 0x001C */ EGG::ResTIMG *field_0x1C;
        /* 0x0020 */ EGG::ResTIMG *field_0x20;
        /* 0x0024 */ mVec3_c field_0x0024[300];
        /* 0x0E34 */ mVec3_c field_0x0E34[300];
        /* 0x???? */ u8 _0x[0x1194];
        /* 0x2DD8 */ u8 field_0x2DD8;
        /* 0x2DD9 */ u8 field_0x2DD9;
    };

    static bool spawnWeatherTag();
    static dWeatherTag_c *GetInstance();

private:
    static dWeatherTag_c *sInstance;

    STATE_FUNC_DECLARE(dWeatherTag_c, Wait);

    /* 0x00FC */ STATE_MGR_DECLARE(dWeatherTag_c);
    /* 0x0138 */ moya_c mMoya;
    /* 0x0CBC */ rain_c mRain;
    /* 0x0CF0 */ near_cloud_c mNearCloud;
    /* 0x0DE8 */ sunflare_stars_c mSunFlareStars;
    /* 0x0FAC */ moon_c mMoon;
    /* 0x0FD4 */ clouds_c mClouds;
    /* 0x3DB0 */ lightning_c mLightning;
    /* 0x4288 */ mVec3_c field_0x4288;
    /* 0x4288 */ mVec3_c field_0x4294;
};

#endif
