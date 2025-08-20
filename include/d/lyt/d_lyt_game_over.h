#ifndef D_LYT_GAME_OVER_H
#define D_LYT_GAME_OVER_H

#include "common.h"
#include "d/d_base.h"
#include "d/lyt/d2d.h"
#include "d/lyt/msg_window/d_lyt_msg_window_select_btn.h"
#include "s/s_State.hpp"
#include "sized_string.h"

class dLytGameOver_c {
public:
    dLytGameOver_c();
    virtual ~dLytGameOver_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    bool requestIn(const char *text, s32 p1, s32 p2);
    bool requestOut(s32 outType);

    s32 getSelectedOption() const {
        return mSelectedOption;
    }

    bool isChangingState() const {
        return mIsChangingState;
    }

    void setField_0x1338(bool v) {
        field_0x1338 = v;
    }


private:
    STATE_FUNC_DECLARE(dLytGameOver_c, Init);
    STATE_FUNC_DECLARE(dLytGameOver_c, In);
    STATE_FUNC_DECLARE(dLytGameOver_c, Wait);
    STATE_FUNC_DECLARE(dLytGameOver_c, Out);

    void init();

    void setAnm(int idx);
    void stopAnm(int idx);
    void applyText();
    void setupButtons();

    /* 0x0004 */ UI_STATE_MGR_DECLARE(dLytGameOver_c);
    /* 0x0044 */ d2d::ResAccIf_c mResAcc1;
    /* 0x03B0 */ d2d::ResAccIf_c mResAcc2;
    /* 0x0720 */ d2d::LytBase_c mLyt;
    /* 0x07B0 */ d2d::AnmGroup_c mAnm[5];
    /* 0x08F0 */ d2d::SubPaneList mSubpaneList;
    /* 0x08FC */ d2d::SubPaneListNode mSubpanes[1];
    /* 0x090C */ dLytMsgWindowSelectBtn_c mSelectBtn;
    /* 0x12E0 */ SizedString<0x40> mLabel;
    /* 0x1320 */ s32 mVariant;
    /* 0x1324 */ s32 mInType;
    /* 0x1328 */ s32 mOutType;
    /* 0x132C */ s32 mStep;
    /* 0x1330 */ s32 mSelectedOption;
    /* 0x1334 */ bool mIsVisible;
    /* 0x1335 */ bool mIsChangingState;
    /* 0x1336 */ bool mInRequest;
    /* 0x1337 */ bool mOutRequest;
    /* 0x1338 */ bool field_0x1338;
};

class dLytGameOverMgr_c : public dBase_c {
public:
    dLytGameOverMgr_c();
    virtual ~dLytGameOverMgr_c();

    int create() override;
    int doDelete() override;
    int execute() override;
    int draw() override;

    static dLytGameOverMgr_c *GetInstance() {
        return sInstance;
    }

private:
    STATE_FUNC_DECLARE(dLytGameOverMgr_c, Init);
    STATE_FUNC_DECLARE(dLytGameOverMgr_c, FadeOut);
    STATE_FUNC_DECLARE(dLytGameOverMgr_c, Select);

    void init();

    static dLytGameOverMgr_c *sInstance;

    /* 0x0068 */ UI_STATE_MGR_DECLARE(dLytGameOverMgr_c);
    /* 0x00A4 */ dLytGameOver_c mMain;
    /* 0x13E0 */ s32 mStep;
    /* 0x13E4 */ s32 mTimer;
};

#endif
