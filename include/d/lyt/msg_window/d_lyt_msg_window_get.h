#ifndef D_LYT_MSG_WINDOW_GET_H
#define D_LYT_MSG_WINDOW_GET_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "s/s_State.hpp"

class dLytMsgWindowGet_c : public dLytMsgWindowSubtype {
public:
    dLytMsgWindowGet_c() : mStateMgr(*this) {}
    /* vt 0x08 */ virtual ~dLytMsgWindowGet_c() {}
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

    STATE_FUNC_DECLARE(dLytMsgWindowGet_c, Invisible);
    STATE_FUNC_DECLARE(dLytMsgWindowGet_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowGet_c, Visible);
    STATE_FUNC_DECLARE(dLytMsgWindowGet_c, Out);

private:
    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytMsgWindowGet_c);
};

#endif
