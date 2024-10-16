#ifndef D_LYT_AREA_CAPTION_H
#define D_LYT_AREA_CAPTION_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_sub.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytAreaCaptionParts_c {
public:
    dLytAreaCaptionParts_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytAreaCaptionParts_c() {}

    bool init(int first);

    void check();
    bool update();
    bool remove();
    bool draw();
    void setTextIfWaiting(const char *text);

    STATE_FUNC_DECLARE(dLytAreaCaptionParts_c, Wait);
    STATE_FUNC_DECLARE(dLytAreaCaptionParts_c, Start);
    STATE_FUNC_DECLARE(dLytAreaCaptionParts_c, Out);

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytAreaCaptionParts_c);
    /* 0x040 */ d2d::ResAccIf_c mResAcc;
    /* 0x0EB */ dLytSub mLyt;
    /* 0x110 */ d2d::AnmGroup_c mAnmGroups[4];
    /* 0x540 */ dTextBox_c *mTextBoxes[3];
    /* 0x54C */ u8 field_0x54C;
    /* 0x54D */ u8 field_0x54D;
    /* 0x54E */ u8 mFirst;
    /* 0x54F */ bool mAnmCtrlInUse[4];
};

class dLytAreaCaption_c {
public:
    static dLytAreaCaption_c *sInstance;

    static bool create();
    static bool remove();
    static bool update();
    static bool draw();
    static void setTextIfWaiting(const char *);
    static void setArea(int id);
    static void check();

    static void set0xAAC(u8 arg);
    static u8 get0xAAC();

private:
    dLytAreaCaption_c() {
        sInstance = this;
    }
    ~dLytAreaCaption_c() {
        sInstance = nullptr;
    }
    bool init();
    bool deinit();
    bool doUpdate();
    bool doDraw();
    void doSetTextIfWaiting(const char *);
    void doSetArea(int id);
    void doCheck();
    dLytAreaCaptionParts_c mParts[2];

    s32 mPartToDraw;
    u8 field_0xAAC;
};

#endif
