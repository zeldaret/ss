#ifndef D_T_MAP_MARK_H
#define D_T_MAP_MARK_H

#include "d/t/d_tg.h"

class dTgMapMark_c : public dTg_c {
public:
    dTgMapMark_c() {}
    virtual ~dTgMapMark_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    /** How mSceneflag / mTriggerStoryFlag affect whether the mark is active */
    enum FlagCondition_e {
        FLAG_COND_SET = 0,
        FLAG_COND_UNSET = 1,
        FLAG_COND_ALWAYS = 3,
    };

    /* 0x0FC */ u16 mTriggerStoryFlag;
    /* 0x0FE */ u16 field_0x0FE;
    /* 0x100 */ u8 mSceneflag;
    /* 0x101 */ u8 mToggleSceneflag;
    /* 0x102 */ u8 mSceneflagCondition;
    /* 0x103 */ u8 mStoryflagCondition;
    /* 0x104 */ u8 field_0x104;
    /* 0x105 */ u8 field_0x105;
    /* 0x108 */ s32 mMarkType;
    /* 0x10C */ bool mStoryflagOk;
    /* 0x10D */ bool mSceneflagOk;
    /* 0x10E */ bool mIsActive;
    /* 0x10F */ bool mShowPlusBtnHint;
    /* 0x110 */ bool mDisableOnToggle;
};

#endif
