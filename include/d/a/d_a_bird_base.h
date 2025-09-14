#ifndef D_A_BIRD_BASE_H
#define D_A_BIRD_BASE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_mdl.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_path.h"

class dAcBirdBase_c : public dAcObjBase_c {
    class callback_c : public m3d::callback_c {
    public:
        virtual ~callback_c() {}
        virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

        /* 0x04 */ u8 _0x04[0x94 - 0x04];
    };

public:
    enum Action_e {
        BIRD_ACTION_FLAP,
        BIRD_ACTION_GLIDE,
        BIRD_ACTION_KEEP,
        BIRD_ACTION_DAMAGE,
        BIRD_ACTION_4,
        BIRD_ACTION_DOWN,
        BIRD_ACTION_HOVER,
        BIRD_ACTION_GLIDE_DIVE,
        BIRD_ACTION_SWITCH_DIVE,

        // Certain birds use special actions starting from 9

        BIRD_ACTION_BASE_MAX = 9,
    };

    dAcBirdBase_c() : field_0xF30(0) {}
    virtual ~dAcBirdBase_c() {}

    virtual int doDelete() override;
    virtual int draw() override;

    /* 0x80 */ virtual bool resetBird();
    /* 0x84 */ virtual bool hasPath() const {
        return getPath() >= 0 && getPath() < 0xFF;
    }
    /* 0x88 */ virtual const char* getAnmArcNameBase() const {
        return "BirdAnm";
    }
    /* 0x8C */ virtual const char* getAnmArcNameLink() const {
        return "BirdLinkAnm";
    }
    /* 0x90 */ virtual const char* getAnmArcNameSpecial() const {
        return "BirdLinkAnm";
    }

protected:
    s32 getPath() const {
        return params & 0xFF;
    }

    void updateMatrixBird();
    void loadMdlNodeIds();
    bool chkWallGndRoofHit();
    void handleWallGndRoofHit();
    bool changeBirdAction(const char *animName, s32 action, m3d::playMode_e playMode, f32 blend);
    void executeBirdAction();

    void fn_80176D30();
    void fn_80176F80();
    void fn_80177EA0();
    void fn_80178000();
    void fn_801780C0();
    void fn_80177F30();

    /* 0x330 */ d3d::AnmMdlWrapper mMdl;
    /* 0x3A0 */ s16 mBirdAction;
    /* 0x3A4 */ dShadowCircle_c mShadow;
    /* 0x3AC */ dCcD_Cps mCcCps;
    /* 0x51C */ dCcD_Sph mCcSph;
    /* 0x66C */ dBgS_AcchCir mAcchCir1;
    /* 0x6C8 */ dBgS_ObjAcch mObjAcch1;
    /* 0xA78 */ callback_c mMdlCallback;
    /* 0xB0C */ dBgS_ObjAcch mObjAcch2;
    /* 0xEBC */ dBgS_AcchCir mAcchCir2;
    /* 0xF18 */ mVec3_c field_0xF18;
    /* 0xF24 */ mVec3_c field_0xF24;
    /* 0xF30 */ u16 field_0xF30;
    /* 0xF32 */ s16 mBirdActionTimer;
    /* 0xF34 */ s16 mChkWallGndRoofTimer;
    /* 0xF36 */ u8 _0xF36[0xF40 - 0xF36];
    /* 0xF40 */ f32 field_0xF40;
    /* 0xF44 */ u8 _0xF44[0xF50 - 0xF44];
    /* 0xF50 */ mVec3_c field_0xF50;
    /* 0xF5C */ mVec3_c field_0xF5C;
    /* 0xF68 */ mVec3_c field_0xF68;
    /* 0xF74 */ mVec3_c field_0xF74;
    /* 0xF80 */ mMtx_c field_0xF80;
    /* 0xFB0 */ mQuat_c field_0xFB0;
    /* 0xFC0 */ mVec3_c field_0xFC0;
    /* 0xFCC */ u8 field_0xFCC;
    /* 0xFCE */ s16 field_0xFCE;
    /* 0xFD0 */ u8 _0xFD0[0xFE4 - 0xFD0];
    /* 0xFE4 */ ActorOnRail_Ext mPath;
};

#endif
