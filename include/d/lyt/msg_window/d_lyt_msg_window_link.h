#ifndef D_LYT_MSG_WINDOW_LINK_H
#define D_LYT_MSG_WINDOW_LINK_H

#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_a_btn.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "s/s_State.hpp"

class dLytMsgWindowLink_c : public dLytMsgWindowSubtype {
public:
    dLytMsgWindowLink_c() : mStateMgr(*this) {}
    /* vt 0x08 */ virtual ~dLytMsgWindowLink_c() {}
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
        return nullptr;
    }
    /* vt 0x34 */ virtual bool startConfirm() override {
        return true;
    }
    /* vt 0x38 */ virtual bool startDecide(bool b) override {
        return true;
    }
    /* vt 0x3C */ virtual bool isDoneDecide() const override {
        return true;
    }

    STATE_FUNC_DECLARE(dLytMsgWindowLink_c, Invisible);
    STATE_FUNC_DECLARE(dLytMsgWindowLink_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowLink_c, Visible);
    STATE_FUNC_DECLARE(dLytMsgWindowLink_c, Out);

    dLytMsgWindowCharData *getCharData() {
        return mBlurRelated.getCharData();
    }

private:
    MSG_WINDOW_STATE_MGR_INLINE(dLytMsgWindowLink_c)
    /* 0x0010 */ UI_STATE_MGR_DECLARE(dLytMsgWindowLink_c);
    /* 0x004C */ d2d::SubPaneList mSubpaneList;
    /* 0x005C */ d2d::SubPaneListNode mNodes[1];
    /* 0x0068 */ d2d::ResAccIf_c mResAcc;
    /* 0x03D8 */ d2d::LytBase_c mLyt;
    /* 0x0468 */ d2d::AnmGroup_c mAnm[4];
    /* 0x0568 */ dTextBox_c *mpTextboxes[2];
    /* 0x0570 */ nw4r::lyt::Pane *mpPanes[5];
    /* 0x0584 */ dTagProcessor_c *mpTagProcessor;
    /* 0x0588 */ u8 _0x0588[0x05C0 - 0x0588];
    /* 0x05C0 */ dLytCommonABtn_c mBtn;
    /* 0x0770 */ dLytTextLight mText;
    /* 0x084C */ MsgWindowBlurRelated mBlurRelated;
    /* 0x583C */ bool mShouldBeOpen;
};

#endif
