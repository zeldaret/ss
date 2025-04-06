#ifndef D_LYT_MSG_WINDOW_SWORD_H
#define D_LYT_MSG_WINDOW_SWORD_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_a_btn.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dLytMsgWindowSword_c : public dLytMsgWindowSubtype {
public:
    dLytMsgWindowSword_c() : mStateMgr(*this) {}
    /* vt 0x08 */ virtual ~dLytMsgWindowSword_c() {}
    /* vt 0x0C */ virtual void draw() override;
    /* vt 0x10 */ virtual bool
    build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) override;
    /* vt 0x14 */ virtual bool remove() override;
    /* vt 0x18 */ virtual bool execute() override;
    /* vt 0x1C */ virtual void open(dAcObjBase_c *obj, u32 param) override;
    /* vt 0x20 */ virtual bool isDoneOpening() const override;
    /* vt 0x24 */ virtual void close() override;
    /* vt 0x28 */ virtual bool isDoneClosing() const override;
    /* vt 0x2C */ virtual bool setText(const wchar_t *text) override;
    /* vt 0x30 */ virtual dTextBox_c *getTextBox() override {
        return mpTextboxes[0];
    }
    /* vt 0x34 */ virtual bool startConfirm() override {
        return mBtn.requestIn();
    }
    /* vt 0x38 */ virtual bool startDecide(bool b) override {
        return mBtn.requestOut(b);
    }
    /* vt 0x3C */ virtual bool isDoneDecide() const override {
        return mBtn.isDoneOut();
    }

    STATE_FUNC_DECLARE(dLytMsgWindowSword_c, Invisible);
    STATE_FUNC_DECLARE(dLytMsgWindowSword_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowSword_c, Wait);
    STATE_FUNC_DECLARE(dLytMsgWindowSword_c, Visible);
    STATE_FUNC_DECLARE(dLytMsgWindowSword_c, Out);

    void setSwordType(dLytTextSword::ESwordType type) {
        mSwordType = type;
    }

    dLytMsgWindowCharData *getCharData() {
        return mCharacterData;
    }

private:
    MSG_WINDOW_STATE_MGR_INLINE(dLytMsgWindowSword_c)
    /* 0x0010 */ UI_STATE_MGR_DECLARE(dLytMsgWindowSword_c);
    /* 0x004C */ d2d::SubPaneList mSubpaneList;
    /* 0x005C */ d2d::SubPaneListNode mNodes[1];
    /* 0x0068 */ d2d::ResAccIf_c mResAcc;
    /* 0x03D8 */ d2d::LytBase_c mLyt;
    /* 0x0468 */ d2d::AnmGroup_c mAnm[5];
    /* 0x05A8 */ dTextBox_c *mpTextboxes[2];
    /* 0x05B0 */ nw4r::lyt::Pane *mpPanes[3];
    /* 0x05BC */ dTagProcessor_c *mpTagProcessor;
    /* 0x05C0 */ mVec3_c mTranslationOrig;
    /* 0x05CC */ mVec3_c mTranslationTo;
    /* 0x05D8 */ mVec3_c mTranslationFrom;

    /* 0x05E4 */ mVec2_c mOffset;

    /* 0x05EC */ dAcObjBase_c *mpActor;
    /* 0x05F0 */ s32 mParam;

    /* 0x05F4 */ s32 mWaitDelay;

    /* 0x05F8 */ dLytCommonABtn_c mBtn;

    /* 0x07A8 */ dLytMsgWindowCharData mCharacterData[1023];
    /* 0x5794 */ wchar_t mTextBuf[1023];

    /* 0x5F94 */ dLytTextSword mSwordText;
    /* 0x607C */ dLytTextSword::ESwordType mSwordType;
    /* 0x6080 */ bool mShouldBeOpen;
    /* 0x6081 */ bool mHasDrawnThisTick;
};

#endif
