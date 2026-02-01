#ifndef D_LYT_AREA_CAPTION_H
#define D_LYT_AREA_CAPTION_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytAreaCaptionParts_c {
public:
    dLytAreaCaptionParts_c() : mStateMgr(*this) {}
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
    bool shouldImmediatelyDismiss() const;

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytAreaCaptionParts_c);
    /* 0x040 */ d2d::ResAccIf_c mResAcc;
    /* 0x0EB */ d2d::dLytSub mLyt;
    /* 0x110 */ d2d::AnmGroup_c mAnmGroups[4];
    /* 0x540 */ dTextBox_c *mTextBoxes[3];
    /* 0x54C */ bool field_0x54C;
    /* 0x54D */ bool field_0x54D;
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

    static void setVisible(bool arg);
    static bool getVisible();

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

    /* 0x000 */ dLytAreaCaptionParts_c mParts[2];
    /* 0xAA8 */ s32 mPartToDraw;
    /* 0xAAC */ bool mVisible;
};

#endif
