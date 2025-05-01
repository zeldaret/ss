#ifndef D_S_BOOT_H
#define D_S_BOOT_H

#include "d/d_fader.h"
#include "d/d_scene.h"
#include "d/lyt/d2d.h"
#include "m/m2d.h"
#include "m/m_dvd.h"
#include "s/s_FPhase.h"
#include "s/s_State.hpp"
#include "sized_string.h"

class dScBoot_c : public dScene_c {
public:
    dScBoot_c();
    virtual ~dScBoot_c();

    STATE_FUNC_DECLARE(dScBoot_c, Init);
    STATE_FUNC_DECLARE(dScBoot_c, Strap);
    STATE_FUNC_DECLARE(dScBoot_c, Connect);
    STATE_FUNC_DECLARE(dScBoot_c, Save);

    static dScBoot_c *GetInstance() {
        return sInstance;
    }

private:
    static dScBoot_c *sInstance;
    class strap_c {
    public:
        strap_c();

        bool create();
        bool execute();
        bool remove();
        void draw();
        void init();

    private:
        /* 0x000 */ SizedString<64> mArcName;
        /* 0x040 */ SizedString<64> mStr2;
        /* 0x080 */ SizedString<64> mStr3;
        /* 0x0C0 */ d2d::ResAccIf_c mResAcc;
        /* 0x430 */ m2d::Simple_c mLyt;
        /* 0x4C8 */ m2d::Anm_c mAnm;
        /* 0x4EC */ u8 field_0x4EC;
        /* 0x4EE */ u16 field_0x4EE;
        /* 0x4F0 */ u8 field_0x4F0;
    };

    sFPhaseBase::sFPhaseState executeLoadPhase();

    virtual int create() override;
    virtual int execute() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual void deleteReady() override;

    bool checkDone();

    sFPhaseBase::sFPhaseState cb1();
    sFPhaseBase::sFPhaseState cb2();
    sFPhaseBase::sFPhaseState cb3();
    sFPhaseBase::sFPhaseState cb4();
    sFPhaseBase::sFPhaseState cb5();
    sFPhaseBase::sFPhaseState cb6();
    sFPhaseBase::sFPhaseState cb7();
    sFPhaseBase::sFPhaseState cb8();
    sFPhaseBase::sFPhaseState cb9();

    static void *dvdCallback(void *data);

    static sFPhase<dScBoot_c>::phaseCallback sCallbacks[];

    /* 0x068 */ strap_c mStrapScreen;
    /* 0x55C */ sFPhase<dScBoot_c> mPhases;
    /* 0x570 */ mDvd_callback_c *mpDvdCallback;
    /* 0x574 */ STATE_MGR_DECLARE(dScBoot_c);
    /* 0x5B4 */ dFader_c mFader;
    /* 0x5D4 */ s32 field_0x5D4;
    /* 0x5D8 */ u32 field_0x5D8;
    /* 0x5DC */ u8 field_0x5DC;
    /* 0x5DD */ u8 field_0x5DD;
    /* 0x5DE */ u8 field_0x5DE;
    /* 0x5DF */ u8 field_0x5DF;
    /* 0x5E0 */ u8 field_0x5E0;
    /* 0x5E1 */ u8 field_0x5E1;
    /* 0x5E2 */ u8 field_0x5E2;
    /* 0x5E3 */ u8 field_0x5E3;
    /* 0x5E4 */ u8 field_0x5E4;
};

#endif
