#ifndef D_LYT_SIMPLE_WINDOW_H
#define D_LYT_SIMPLE_WINDOW_H

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "m/m2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "sized_string.h"

class dLytSimpleWindowSubtype {
public:
    /* 0x08 */ virtual ~dLytSimpleWindowSubtype() {}
    /* 0x0C */ virtual bool build(d2d::ResAccIf_c *, dTagProcessor_c *);
    /* 0x10 */ virtual bool remove() {
        return true;
    }
    /* 0x14 */ virtual bool execute() {
        return true;
    }
    /* 0x18 */ virtual bool draw() {
        return true;
    }
    /* 0x1C */ virtual void initializeIn(s32 pos);
    /* 0x20 */ virtual bool isIn() {
        return true;
    }
    /* 0x24 */ virtual void initializeOutputText() {}
    /* 0x28 */ virtual void initializeOut() {}
    /* 0x2C */ virtual bool isOut() {
        return true;
    }
    /* 0x30 */ virtual bool setText(const wchar_t *text);
    /* 0x34 */ virtual dTextBox_c *getTextBox() {
        return nullptr;
    }
    /* 0x38 */ virtual dLytMsgWindowCharData *getCharData() {
        return nullptr;
    }

private:
};

class dLytSimpleWindowTalk_c : public dLytSimpleWindowSubtype {
public:
    /* 0x0C */ virtual bool build(d2d::ResAccIf_c *, dTagProcessor_c *) override;
    /* 0x10 */ virtual bool remove() override;
    /* 0x14 */ virtual bool execute() override;
    /* 0x18 */ virtual bool draw() override;
    /* 0x1C */ virtual void initializeIn(s32 pos) override;
    /* 0x20 */ virtual bool isIn() override;
    /* 0x24 */ virtual void initializeOutputText() override;
    /* 0x28 */ virtual void initializeOut() override;
    /* 0x2C */ virtual bool isOut() override;
    /* 0x30 */ virtual bool setText(const wchar_t *text) override;

    /* 0x34 */ virtual dTextBox_c *getTextBox() override {
        return mpTextboxes[0];
    }
    /* 0x38 */ virtual dLytMsgWindowCharData *getCharData() override {
        return mBlurRelated.getCharData();
    }

private:
    /* 0x0004 */ d2d::ResAccIf_c *mpResAcc;
    /* 0x0008 */ d2d::LytBase_c mLyt;
    /* 0x0098 */ d2d::AnmGroup_c mAnm[4];
    /* 0x0198 */ dTextBox_c *mpTextboxes[2];
    /* 0x01A0 */ nw4r::lyt::Pane *mpPanes[5];
    /* 0x01B4 */ dTagProcessor_c *mpTagProcessor;
    /* 0x01B8 */ dLytTextLight mLightText;
    /* 0x0294 */ MsgWindowBlurRelated mBlurRelated;
    /* 0x5284 */ mVec3_c mOrigPosition;
    /* 0x5290 */ mVec3_c mPosition;
    /* 0x529C */ mVec2_c mOffset;
};

class dLytSimpleWindowSword_c : public dLytSimpleWindowSubtype {
public:
    /* 0x0C */ virtual bool build(d2d::ResAccIf_c *, dTagProcessor_c *) override;
    /* 0x10 */ virtual bool remove() override;
    /* 0x14 */ virtual bool execute() override;
    /* 0x18 */ virtual bool draw() override;
    /* 0x1C */ virtual void initializeIn(s32 pos) override;
    /* 0x20 */ virtual bool isIn() override;
    /* 0x24 */ virtual void initializeOutputText() override;
    /* 0x28 */ virtual void initializeOut() override;
    /* 0x2C */ virtual bool isOut() override;
    /* 0x30 */ virtual bool setText(const wchar_t *text) override;

    /* 0x34 */ virtual dTextBox_c *getTextBox() override {
        return mpTextboxes[0];
    }
    /* 0x38 */ virtual dLytMsgWindowCharData *getCharData() override {
        return mCharData;
    }
    void setSwordType(s32 b) {
        mSwordType = b;
    }

private:
    /* 0x0004 */ d2d::ResAccIf_c *mpResAcc;
    /* 0x0008 */ d2d::LytBase_c mLyt;
    /* 0x0098 */ d2d::AnmGroup_c mAnm[5];
    /* 0x01D8 */ dTextBox_c *mpTextboxes[2];
    /* 0x01E0 */ nw4r::lyt::Pane *mpPanes[3];
    /* 0x01F4 */ dTagProcessor_c *mpTagProcessor;
    /* 0x01F8 */ dLytMsgWindowCharData mCharData[TextWindowUnk::BUF_SIZE];
    /* 0x51DC */ wchar_t mTextBuf[TextWindowUnk::BUF_SIZE];
    /* 0x59DC */ dLytTextSword mSwordText;
    /* 0x5AC4 */ s32 mSwordType;
    /* 0x5AC8 */ u8 field_0x5AC8;
    /* 0x5ACC */ mVec3_c mOrigPosition;
    /* 0x5AD8 */ mVec3_c mPosition;
    /* 0x5AE4 */ mVec2_c mOffset;
};

class dLytSimpleWindowLink_c : public dLytSimpleWindowSubtype {
public:
    /* 0x0C */ virtual bool build(d2d::ResAccIf_c *, dTagProcessor_c *) override;
    /* 0x10 */ virtual bool remove() override;
    /* 0x14 */ virtual bool execute() override;
    /* 0x18 */ virtual bool draw() override;
    /* 0x1C */ virtual void initializeIn(s32 pos) override;
    /* 0x20 */ virtual bool isIn() override;
    /* 0x24 */ virtual void initializeOutputText() override;
    /* 0x28 */ virtual void initializeOut() override;
    /* 0x2C */ virtual bool isOut() override;
    /* 0x30 */ virtual bool setText(const wchar_t *text) override;

    /* 0x34 */ virtual dTextBox_c *getTextBox() override {
        return mpTextboxes[0];
    }
    /* 0x38 */ virtual dLytMsgWindowCharData *getCharData() override {
        return mBlurRelated.getCharData();
    }

private:
    /* 0x0004 */ d2d::ResAccIf_c *mpResAcc;
    /* 0x0008 */ d2d::LytBase_c mLyt;
    /* 0x0098 */ d2d::AnmGroup_c mAnm[4];
    /* 0x0198 */ dTextBox_c *mpTextboxes[2];
    /* 0x01A0 */ nw4r::lyt::Pane *mpPanes[5];
    /* 0x01B4 */ dTagProcessor_c *mpTagProcessor;
    /* 0x01B8 */ dLytTextLight mLightText;
    /* 0x0294 */ MsgWindowBlurRelated mBlurRelated;
    /* 0x5??? */ mVec3_c mOrigPosition;
    /* 0x5??? */ mVec3_c mPosition;
    /* 0x5??? */ mVec2_c mOffset;
};

/** 2D UI - Message window - Window without a confirm button, automatically closes (on timer or event) */
class dLytSimpleWindow_c : public m2d::Base_c {
public:
    dLytSimpleWindow_c() : mStateMgr(*this) {
        sInstance = this;
    }
    virtual ~dLytSimpleWindow_c() {
        sInstance = nullptr;
    }
    virtual void draw() override;

    bool build();
    bool remove();
    bool execute();

    static dLytSimpleWindow_c *getInstance() {
        return sInstance;
    }

    bool set(const char *label, s32 timer, bool global);

    bool isVisible();
    bool isOutputText();
    bool isInEvent();

    STATE_FUNC_DECLARE(dLytSimpleWindow_c, Invisible);
    STATE_FUNC_DECLARE(dLytSimpleWindow_c, In);
    STATE_FUNC_DECLARE(dLytSimpleWindow_c, OutputText);
    STATE_FUNC_DECLARE(dLytSimpleWindow_c, Out);

private:
    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytSimpleWindow_c);
    /* 0x04C */ d2d::ResAccIf_c mResAcc;

    /* 0x3BC */ TextWindowUnk *mpUnk;
    /* 0x3C0 */ dTagProcessor_c *mpTagProcessor;
    /* 0x3C4 */ dLytSimpleWindowSubtype *mpCurrentWindow;
    /* 0x3C8 */ dLytSimpleWindowTalk_c *mpWindowTalk;
    /* 0x3CC */ dLytSimpleWindowSword_c *mpWindowSword;
    /* 0x3D0 */ dLytSimpleWindowLink_c *mpWindowLink;
    /* 0x3D4 */ UNKWORD field_0x3D4;
    /* 0x3D8 */ UNKWORD field_0x3D8;
    /* 0x3DC */ UNKWORD field_0x3DC;
    /* 0x3E0 */ UNKWORD field_0x3E0;
    /* 0x3E4 */ UNKWORD mTimer;
    /* 0x3E8 */ u16 field_0x3E8;
    /* 0x3EA */ SizedString<0x40> mLabel;
    /* 0x42A */ SizedString<0x40> field_0x42A;
    /* 0x46A */ bool mInRequest;
    /* 0x46B */ bool mWaitForEvent;

    static dLytSimpleWindow_c *sInstance;
};

#endif
