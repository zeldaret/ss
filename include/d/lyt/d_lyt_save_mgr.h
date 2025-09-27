#ifndef D_LYT_SAVE_MGR_H
#define D_LYT_SAVE_MGR_H

#include "d/d_base.h"
#include "d/lyt/d_lyt_save_msg_window.h"
#include "s/s_State.hpp"

class dLytSaveMgr_c : public dBase_c {
public:
    dLytSaveMgr_c();
    virtual ~dLytSaveMgr_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    static dLytSaveMgr_c *GetInstance() {
        return sInstance;
    }

    
    void init();
    
    enum FadeInPrompt_e {
        PROMPT_SAVEOBJ_NORMAL,
        PROMPT_SAVEOBJ_TOSKY,
        PROMPT_SAVEOBJ_DUNGEON,
        PROMPT_SAVE_NORMAL,
        PROMPT_HERO_MODE,
        PROMPT_HERO_MODE_CONFIRM,

        PROMPT_MAX,
        
        PROMPT_SAVEOBJ_NUM = PROMPT_SAVEOBJ_DUNGEON + 1,
    };
    
    bool setPrompt(s32 prompt, bool setTempflag2);
    bool setSaveobjPrompt(s32 prompt, u8 saveobjId, bool setTempflag2);
    bool unused_weakfnorder();
    bool isBusy() const;

    enum Action_e {
        ACTION_0,
        ACTION_1,
        ACTION_2,
        ACTION_3,
        ACTION_MAX,
    };

    bool isFinishedFadingIn() const {
        return mIsFinishedFadingIn;
    }

private:
    static dLytSaveMgr_c *sInstance;

    STATE_FUNC_DECLARE(dLytSaveMgr_c, Init);
    STATE_FUNC_DECLARE(dLytSaveMgr_c, FadeOut);
    STATE_FUNC_DECLARE(dLytSaveMgr_c, Select);
    STATE_FUNC_DECLARE(dLytSaveMgr_c, FadeIn);

    /* 0x0068 */ UI_STATE_MGR_DECLARE(dLytSaveMgr_c);
    /* 0x00A4 */ dLytSaveMsgWindow_c mWindow;
    /* 0x1184 */ s32 mSavePrompt;
    /* 0x1188 */ s32 mStep;
    /* 0x118C */ s32 mChosenAction;

    /* 0x1190 */ u8 _0x1190[0x119A - 0x1190];

    /* 0x119A */ u8 mSaveobjId;
    /* 0x119B */ bool mFadeOutRequested;
    /* 0x119C */ bool mIsFinishedFadingIn;
    /* 0x119D */ bool mShouldSetTempflag2;
};

#endif
