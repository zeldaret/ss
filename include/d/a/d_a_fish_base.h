#ifndef D_A_FISH_BASE_H
#define D_A_FISH_BASE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmmdl.h"
#include "s/s_State.hpp"

class dAcFishMgrBase_c;

class dAcFishBase_c : public dAcObjBase_c {
public:
    dAcFishBase_c() : mStateMgr(*this) {}
    virtual ~dAcFishBase_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int doDelete() override;
    virtual int draw() override;

protected:
    /* vt 0x80 */ virtual const char *getResFileName() = 0;
    /* vt 0x84 */ virtual const char *getMdlName() = 0;
    /* vt 0x88 */ virtual const char *getAnmName() = 0;
    /* vt 0x8C */ virtual void vt_0x8C();
    /* vt 0x90-0x9C */ STATE_VIRTUAL_FUNC_DECLARE(dAcFishBase_c, Swim);
    /* vt 0x9C-0xA4 */ STATE_VIRTUAL_FUNC_DECLARE(dAcFishBase_c, Escape);
    /* vt 0xA8 */ virtual void vt_0xA8() {}
    /* vt 0xAC */ virtual void vt_0xAC() {}
    /* vt 0xB0 */ virtual UNKWORD vt_0xB0() = 0;
    /* vt 0xB4 */ virtual void vt_0xB4();
    /* vt 0xB8 */ virtual f32 vt_0xB8() = 0;
    /* vt 0xBC */ virtual f32 vt_0xBC() = 0;
    /* vt 0xC0 */ virtual s16 vt_0xC0() const {
        return 100;
    }
    /* vt 0xC4 */ virtual void vt_0xC4() = 0;
    /* vt 0xC8 */ virtual void vt_0xC8() = 0;
    /* vt 0xCC */ virtual f32 vt_0xCC() const {
        return 0.1f;
    }
    /* vt 0xD0 */ virtual f32 vt_0xD0() {
        return 2.0f;
    }
    /* vt 0xD4 */ virtual f32 vt_0xD4() const {
        return 200.0f;
    }
    /* vt 0xD8 */ virtual f32 vt_0xD8() const {
        return 3.0f;
    }
    /* vt 0xDC */ virtual f32 vt_0xDC() const {
        return 5.0f;
    }
    /* vt 0xE0 */ virtual s32 vt_0xE0() const {
        return 90;
    }
    /* vt 0xE4 */ virtual s32 vt_0xE4() const {
        return 180;
    }
    /* vt 0xE8 */ virtual s16 vt_0xE8() const {
        return 15;
    }
    /* vt 0xEC */ virtual f32 vt_0xEC() const {
        return 300.0f;
    }
    /* vt 0xF0 */ virtual f32 vt_0xF0() const {
        return 100.0f;
    }
    /* vt 0xF4 */ virtual s32 vt_0xF4() const {
        return 30;
    }
    /* vt 0xF8 */ virtual s32 vt_0xF8() const {
        return 45;
    }
    /* vt 0xFC */ virtual f32 vt_0xFC() const {
        return 200.0f;
    }
    /* vt 0x100 */ virtual f32 vt_0x100() const {
        return 400.0f;
    }
    /* vt 0x104 */ virtual f32 vt_0x104() const {
        return 15.0f;
    }
    /* vt 0x108 */ virtual f32 vt_0x108() const {
        return 20.0f;
    }
    /* vt 0x10C */ virtual s32 vt_0x10C() const {
        return 30;
    }
    /* vt 0x110 */ virtual s32 vt_0x110() const {
        return 60;
    }
    /* vt 0x114 */ virtual s32 vt_0x114() const {
        return 10;
    }
    /* vt 0x118 */ virtual s32 vt_0x118() const {
        return 20;
    }
    /* vt 0x11C */ virtual s16 vt_0x11C() const {
        return 15;
    }
    /* vt 0x120 */ virtual f32 vt_0x120() const {
        return 500.0f;
    }
    /* vt 0x124 */ virtual f32 vt_0x124() const {
        return 300.0f;
    }

    dAcFishMgrBase_c *getFishMgr();
    // TODO - unknown callback signature for 0x8018c1c0
    void fn_8018C240();
    void fn_8018C250();
    void fn_8018C5A0();
    void fn_8018C760();
    void fn_8018C870();
    void fn_8018CCA0();
    bool fn_8018CFC0(f32 arg);
    bool fn_8018D0D0(f32 arg);
    void fn_8018D190(f32 arg, f32 unused);
    void fn_8018D4D0();
    void fn_8018D5A0();
    bool fn_8018D600();
    void fn_8018D660();
    void fn_8018D6E0();
    void fn_8018D730();
    void fn_8018D760();

    /* 0x330 */ m3d::mdlAnmChr mMdl;
    /* 0x398 */ dShadowCircle_c mShadow;
    /* 0x3A0 */ dBgS_AcchCir mAcch;
    /* 0x3FC */ dBgS_ObjAcch mObjAcch;
    /* 0x7AC */ dCcD_Sph mSph1;
    /* 0x8FC */ dCcD_Sph mSph2;
    /* 0xA4C */ mVec3_c field_0xA4C;
    /* 0xA58 */ u8 field_0xA58;

    /* 0xA59 */ u8 _0xA59[0xA60 - 0xA59];

    /* 0xA60 */ mVec3_c field_0xA60;
    /* 0xA6C */ mVec3_c field_0xA6C;
    /* 0xA78 */ u8 field_0xA78;
    /* 0xA79 */ u8 field_0xA79;
    /* 0xA7A */ u8 field_0xA7A;
    /* 0xA7C */ s16 field_0xA7C;
    /* 0xA7E */ s16 field_0xA7E;
    /* 0xA80 */ s16 field_0xA80;
    /* 0xA82 */ s16 field_0xA82;
    /* 0xA84 */ s16 field_0xA84;
    /* 0xA86 */ s16 field_0xA86;
    /* 0xA88 */ s16 field_0xA88;
    /* 0xA8A */ s16 field_0xA8A;
    /* 0xA86 */ s16 field_0xA8C;
    /* 0xA90 */ f32 field_0xA90;
    /* 0xA94 */ f32 field_0xA94;
    /* 0xA98 */ STATE_MGR_DECLARE(dAcFishBase_c);
};

#endif
