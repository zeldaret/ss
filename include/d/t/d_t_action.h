#ifndef D_T_ACTION_H
#define D_T_ACTION_H

#include "d/flag/sceneflag_manager.h"
#include "d/t/d_tg.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"

class dTgAction_c : public dTg_c {
public:
    dTgAction_c() {}
    virtual ~dTgAction_c() {}

    enum Type_e {
        FORCE_FADE_RESTART = 0,
        SHOW_DASH_PROMPT = 1,
        PREVENT_RESPAWN = 4,
        WATER_RELATED = 5,
        FORCE_AUTO_JUMP = 6,
        PREVENT_LOFTWING_CALL = 7,
    };

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    void setActiveArea(f32 scale);
    void resetActiveArea();

    bool checkActive1() const {
        return SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEnableFlag1);
    }
    bool checkActive2() const {
        return (!(mEnableFlag2 < 0xFF) || SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEnableFlag2));
    }

private:
    /* 0x0FC */ mMtx_c mActiveMtx;
    /* 0x12C */ f32 mScaleIncrease;
    /* 0x130 */ u8 mEnableFlag1;
    /* 0x131 */ u8 mEnableFlag2;
    /* 0x132 */ u8 mType;
    /* 0x133 */ u8 _133;
    /* 0x134 */ mAng field_0x134;
};

#endif
