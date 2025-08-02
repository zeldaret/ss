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

    bool getField_0x5E2() const {
        return field_0x5E2;
    }
    void setField_0x5E2(bool val) {
        field_0x5E2 = val;
    }
    bool getField_0x5E3() const {
        return field_0x5E3;
    }
    void setField_0x5E3(bool val) {
        field_0x5E3 = val;
    }
    bool getField_0x5E4() const {
        return field_0x5E4;
    }
    void setField_0x5E4(bool val) {
        field_0x5E4 = val;
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
        /* 0x040 */ SizedString<64> mLytFileName;
        /* 0x080 */ SizedString<64> mAnimFileName;
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
    bool tryLoadTitleScreen();

    static void drawCallback();

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
    /* 0x5D4 */ s32 mProgressStage;
    /* 0x5D8 */ s32 field_0x5D8;
    /* 0x5DC */ bool mStrapScreenSeen;
    /* 0x5DD */ bool mCalibrationDone;
    /* 0x5DE */ bool mAllDataLoaded;
    /* 0x5DF */ bool mObjectDataLoaded;
    /* 0x5E0 */ bool field_0x5E0;
    /* 0x5E1 */ bool mStrapScreenVisible;
    /* 0x5E2 */ bool field_0x5E2;
    /* 0x5E3 */ bool field_0x5E3;
    /* 0x5E4 */ bool field_0x5E4;
};

#endif
