#ifndef D_A_OBJ_MUSHROOM_H
#define D_A_OBJ_MUSHROOM_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_light_env.h"
#include "d/d_shadow.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggCamera.h"
#include "m/m3d/m_proc.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_unk_mdl_stuff_2.h"

class dAcOMushRoom_c : public dAcObjBase_c {
public:
    class proc_c : public m3d::proc_c {
    public:
        proc_c() : mpImg(nullptr), mpIdxArr(nullptr), field_0x20(0), mIdxNum(0), mIdxMax(0) {}
        virtual ~proc_c() {
            remove();
        }
        virtual void drawOpa() override;
        void draw();

        bool create(
            EGG::ResTIMG *pImg, u32 drawOpa, mAllocator_c *pAllocator, s32 idxNum, mVec3_c *pVtx, mVec3_c *pNrm,
            u16 *pVtxIdx, u16 *pNrmIdx
        );
        bool entryIdx(u16 idx);
        void clearIdx();

        /* 0x18 */ EGG::ResTIMG *mpImg;
        /* 0x1C */ u16 *mpIdxArr;
        /* 0x20 */ u32 field_0x20;
        /* 0x24 */ s32 mIdxNum;
        /* 0x28 */ s32 mIdxMax;
        /* 0x2C */ u32 mCameraIdx;
        /* 0x30 */ u32 mCameraSubIdx;
        /* 0x34 */ u8 mTev0Alpha;
        /* 0x38 */ mVec3_c *mpVtxArr;
        /* 0x3C */ mVec3_c *mpNrmArr;
        /* 0x40 */ u16 *mpVtxIdxArr;
        /* 0x44 */ u16 *mpNrmIdxArr;
    };

    class camera_c {
    public:
        camera_c() : field_0x1B6(0) {
            for (int i = 0; i < 3; ++i) {
                mAlpha[i] = 0;
                field_0x1B3[i] = 0;
            }
        }
        ~camera_c();

        /* 0x000 */ EGG::OrthoCamera mCam[3];
        /* 0x1B0 */ u8 mAlpha[3];
        /* 0x1B3 */ u8 field_0x1B3[3];
        /* 0x1B6 */ u8 field_0x1B6;
    };
    static camera_c sCameras[8];

    dAcOMushRoom_c() : mStateMgr(*this), mEmitter(this) {}
    virtual ~dAcOMushRoom_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOMushRoom_c, Wait);
    STATE_FUNC_DECLARE(dAcOMushRoom_c, Init);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOMushRoom_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOMushRoom_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOMushRoom_c);

    f32 getUnscaledRadiusX();
    f32 getUnscaledRadiusY();
    f32 getRadiusX();
    f32 getRadiusY();
    f32 getRadiusZ();

    void initCc();
    void setCc();
    void startCloudEmitter();
    void startGlitterEmitter();
    void fn_328_3970();
    bool fn_328_39C0();
    void updateCameras();
    bool fn_328_4B20(const mMtx_c &m, const mVec3_c &v, f32 zHi, f32 zLo);
    bool fn_328_4C30(const f32 &radius, const s16 &angle, cCcD_Obj *ccObj, const f32 &yHi, const f32 &yLo);
    void setLighting();
    void cutLighting();
    void fn_328_4EF0();
    bool checkUnderWater();

    enum SpawnedActorSubtype_e {
        SPAWN_Param = 0,
        SPAWN_MushroomA,
        SPAWN_MushroomB,
        SPAWN_MushroomC,
        SPAWN_MushroomD,
    };
    // The Actual Type
    enum Type_e {
        TYPE_MushroomA,
        TYPE_MushroomB,
        TYPE_MushroomC,
        TYPE_MushroomD,
        TYPE_Max
    };
    s32 getType() {
        s32 type;
        if (mActorSubtype == SPAWN_Param) { // Spawned as "Mushroom"
            switch ((s32)getFromParams(0, 0xF)) {
                case TYPE_MushroomA: type = TYPE_MushroomA; break;
                case TYPE_MushroomB: type = TYPE_MushroomB; break;
                case TYPE_MushroomC: type = TYPE_MushroomC; break;
                case TYPE_MushroomD: type = TYPE_MushroomD; break;
                default:             type = TYPE_MushroomA; break;
            }
        } else { // Spawned as "MushroomA", "MushroomB", "MushroomC", "MushroomD"
            switch (mActorSubtype) {
                case SPAWN_MushroomA: type = TYPE_MushroomA; break;
                case SPAWN_MushroomB: type = TYPE_MushroomB; break;
                case SPAWN_MushroomC: type = TYPE_MushroomC; break;
                case SPAWN_MushroomD: type = TYPE_MushroomD; break;
                default:              type = TYPE_MushroomA; break;
            }
        }
        return type;
    }

    bool isNotParamBit30() {
        return mActorSubtype == SPAWN_Param && (s32)getFromParams(30, 0x1) == 0;
    }

    bool checkGlittering() {
        return getFromParams(31, 1) == 1 && field_0x892;
    }

    dCcD_Sph &getMainCc() {
        return mSph0;
    }
    s32 getSporeParams1() const {
        return field_0x892 != 0 ? 1 : 0;
    }

    int findCameraIdx() {
        for (int i = 0; i < 8; ++i) {
            bool found;
            if (sCameras[i].field_0x1B6 == 0) {
                sCameras[i].field_0x1B6 = 1;
                found = true;
            } else {
                found = false;
            }
            if (found) {
                return i;
            }
        }
        return -1;
    }

    void clearField_0x854() {
        if (mSph0.ChkTgHit() && mSph0.ChkTgAtHitType(AT_TYPE_BELLOWS)) {
            return;
        }

        if (!mSph0.ChkTgHit()) {
            return;
        }
        field_0x854 = 0;
    }

private:
    const char *getResFileName();
    const char *getResMdlName();
    const char *getDataBinName();

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ dMdlLOD_c mMdl;
    /* 0x360 */ dShadowCircle_c mShadow;
    /* 0x368 */ dCcD_Sph mSph0;
    /* 0x4B8 */ dCcD_Cyl mCyl;
    /* 0x608 */ dCcD_Sph mSph1;
    /* 0x758 */ STATE_MGR_DECLARE(dAcOMushRoom_c);
    /* 0x794 */ mAng3_c field_0x794;
    /* 0x79C */ u32 field_0x79C;
    /* 0x7A0 */ u8 field_0x7A0;
    /* 0x7A1 */ u8 field_0x7A1;
    /* 0x7A4 */ mVec3_c field_0x7A4;
    /* 0x7B0 */ mVec3_c field_0x7B0;
    /* 0x7BC */ f32 field_0x7BC;
    /* 0x7C0 */ s32 field_0x7C0;
    /* 0x7C4 */ mVec3_c field_0x7C4[3];
    /* 0x7E8 */ mVec3_c field_0x7E8[3];
    /* 0x80C */ mVec3_c field_0x80C[3];
    /* 0x830 */ int mProcAlpha[3];
    /* 0x83C */ u8 field_0x83C[3];
    /* 0x83F */ u8 field_0x83F[3];
    /* 0x844 */ mVec3_c field_0x844;
    /* 0x850 */ s32 mCameraIdx;
    /* 0x854 */ s32 field_0x854;
    /* 0x858 */ LIGHT_INFLUENCE mLightInfluence;
    /* 0x874 */ SHADOW_INFLUENCE mShadowInfluence;
    /* 0x888 */ s32 field_0x888;
    /* 0x88C */ s32 field_0x88C;
    /* 0x890 */ u8 field_0x890;
    /* 0x891 */ u8 field_0x891;
    /* 0x892 */ u8 field_0x892;
    /* 0x894 */ dAcObjRef_c mRef;
    /* 0x8A0 */ dEmitter_c mEmitter;

    /* 0x8D4 */ proc_c mProc[3];

    // Mushroom Bin Data info
    /* 0x9AC */ s32 mData_VecNum;
    /* 0x9B0 */ s32 mData_IdxMax;
    /* 0x9B4 */ mVec3_c *mData_pPosArr;
    /* 0x9B8 */ mVec3_c *mData_pNrmArr;
    /* 0x9BC */ u16 *mData_pPosIdxArr;
    /* 0x9C0 */ u16 *mData_pNrmIdxArr;
};

#endif
