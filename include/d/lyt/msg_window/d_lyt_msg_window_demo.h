#ifndef D_LYT_MSG_WINDOW_DEMO_H
#define D_LYT_MSG_WINDOW_DEMO_H

#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "sized_string.h"
#include "toBeSorted/lyt_vec2f.h"

class dLytMsgWindowDemo_c : public dLytMsgWindowSubtype {
public:
    dLytMsgWindowDemo_c() : mStateMgr(*this) {}
    /* vt 0x08 */ virtual ~dLytMsgWindowDemo_c() {}
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

    dLytMsgWindowCharData *getCharData() {
        return mBlurRelated.getCharData();
    }

    STATE_FUNC_DECLARE(dLytMsgWindowDemo_c, Invisible);
    STATE_FUNC_DECLARE(dLytMsgWindowDemo_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowDemo_c, Visible);
    STATE_FUNC_DECLARE(dLytMsgWindowDemo_c, Out);

private:
    static SizedString<64> getTextLabel();

    MSG_WINDOW_STATE_MGR_INLINE(dLytMsgWindowDemo_c)
    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytMsgWindowDemo_c);
    /* 0x04C */ d2d::ResAccIf_c mResAcc;
    /* 0x3BC */ d2d::LytBase_c mLyt;
    /* 0x44C */ d2d::AnmGroup_c mAnm[2];

    /* 0x4CC */ nw4r::lyt::Pane *mpPane;
    /* 0x4D0 */ dTextBox_c *mpTextboxes[2];
    /* 0x4D8 */ dTagProcessor_c *mpTagProcessor;
    /* 0x4DC */ LytVec2f mOffsets[2];
    /* 0x4EC */ dLytTextLight mText;
    /* 0x5C8 */ MsgWindowBlurRelated mBlurRelated;

    /* 0x55B8 */ bool mShouldBeOpen;
};

#endif
