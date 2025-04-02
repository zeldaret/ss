#ifndef D_LYT_AUTO_CAPTION_H
#define D_LYT_AUTO_CAPTION_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytAutoCaption_c : public dLytMsgWindowSubtype {
public:
    /* vt 0x08 */ virtual ~dLytAutoCaption_c() {}
    /* vt 0x0C */ virtual void draw() override;
    /* vt 0x10 */ virtual bool build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) override;
    /* vt 0x14 */ virtual bool remove() override;
    /* vt 0x18 */ virtual bool execute() override;
    /* vt 0x1C */ virtual void open() override;
    /* vt 0x20 */ virtual bool isOpening() const override;
    /* vt 0x24 */ virtual void close() override;
    /* vt 0x28 */ virtual bool isClosing() const override;
    /* vt 0x2C */ virtual bool setText(const wchar_t *text) override;
    /* vt 0x30 */ virtual dTextBox_c *vt_0x30() override {
        return mpTextboxes[0];
    }

    STATE_FUNC_DECLARE(dLytAutoCaption_c, Invisible);
    STATE_FUNC_DECLARE(dLytAutoCaption_c, In);
    STATE_FUNC_DECLARE(dLytAutoCaption_c, OutputText);
    STATE_FUNC_DECLARE(dLytAutoCaption_c, Out);

private:
    static bool checkSomethingMeter();

    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytAutoCaption_c);
    /* 0x068 */ d2d::ResAccIf_c mResAcc;
    /* 0x3C4 */ d2d::LytBase_c mLyt;
    /* 0x44C */ d2d::AnmGroup_c mAnm[4];
    /* 0x54C */ dTextBox_c *mpTextboxes[2];
    /* 0x554 */ nw4r::lyt::Pane *mpPane;
    /* 0x558 */ dTagProcessor_c *mpTagProcessor;
    /* 0x55C */ bool mShouldBeOpen;
};

#endif
