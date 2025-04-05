#ifndef D_LYT_AUTO_EXPLAIN_H
#define D_LYT_AUTO_EXPLAIN_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytAutoExplain_c : public dLytMsgWindowSubtype {
public:
    dLytAutoExplain_c() : mStateMgr(*this) {}
    /* vt 0x08 */ virtual ~dLytAutoExplain_c() {}
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

    STATE_FUNC_DECLARE(dLytAutoExplain_c, Invisible);
    STATE_FUNC_DECLARE(dLytAutoExplain_c, In);
    STATE_FUNC_DECLARE(dLytAutoExplain_c, OutputText);
    STATE_FUNC_DECLARE(dLytAutoExplain_c, Out);

    bool checkSomethingMeter();

private:
    MSG_WINDOW_STATE_MGR_INLINE(dLytAutoExplain_c)
    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytAutoExplain_c);
    /* 0x068 */ d2d::ResAccIf_c mResAcc;
    /* 0x3C4 */ d2d::LytBase_c mLyt;
    /* 0x44C */ d2d::AnmGroup_c mAnm[3];
    /* 0x50C */ dTextBox_c *mpTextboxes[2];
    /* 0x514 */ nw4r::lyt::Pane *mpPane;
    /* 0x518 */ dTagProcessor_c *mpTagProcessor;
    /* 0x51C */ bool mShouldBeOpen;
};

#endif
