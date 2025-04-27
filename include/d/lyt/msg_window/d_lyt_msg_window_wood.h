#ifndef D_LYT_MSG_WINDOW_WOOD_H
#define D_LYT_MSG_WINDOW_WOOD_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_a_btn.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMsgWindowWood_c : public dLytMsgWindowSubtype {
public:
    dLytMsgWindowWood_c() : mStateMgr(*this) {}
    /* vt 0x08 */ virtual ~dLytMsgWindowWood_c() {}
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

    STATE_FUNC_DECLARE(dLytMsgWindowWood_c, Invisible);
    STATE_FUNC_DECLARE(dLytMsgWindowWood_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowWood_c, Visible);
    STATE_FUNC_DECLARE(dLytMsgWindowWood_c, Out);

private:
    MSG_WINDOW_STATE_MGR_INLINE(dLytMsgWindowWood_c)
    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytMsgWindowWood_c);
    /* 0x04C */ d2d::SubPaneList mSubpaneList;
    /* 0x05C */ d2d::SubPaneListNode mNodes[1];
    /* 0x068 */ d2d::ResAccIf_c mResAcc;
    /* 0x3D8 */ d2d::LytBase_c mLyt;
    /* 0x468 */ d2d::AnmGroup_c mAnm[3];
    /* 0x528 */ dTextBox_c *mpTextboxes[2];
    /* 0x530 */ nw4r::lyt::Pane *mpPanes[2];
    /* 0x538 */ dTagProcessor_c *mpTagProcessor;
    /* 0x53C */ dLytCommonABtn_c mBtn;
    /* 0x6EC */ bool mShouldBeOpen;
};

#endif
