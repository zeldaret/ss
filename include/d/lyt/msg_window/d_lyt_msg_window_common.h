#ifndef D_LYT_MGS_WINDOW_COMMON_H
#define D_LYT_MGS_WINDOW_COMMON_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "m/m2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"

class dLytMsgWindowSubtype : public m2d::Base_c {
public:
    /* vt 0x08 */ virtual ~dLytMsgWindowSubtype() {}
    // vt 0x0C = m2d::Base_c::draw
    /* vt 0x10 */ virtual bool build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor);
    /* vt 0x14 */ virtual bool remove();
    /* vt 0x18 */ virtual bool execute();
    /* vt 0x1C */ virtual void open(dAcObjBase_c *obj, u32 param);
    /* vt 0x20 */ virtual bool isDoneOpening() const;
    /* vt 0x24 */ virtual void close();
    /* vt 0x28 */ virtual bool isDoneClosing() const;
    /* vt 0x2C */ virtual bool setText(const wchar_t *text);
    /* vt 0x30 */ virtual dTextBox_c *getTextBox() {
        return nullptr;
    }
    /* vt 0x34 */ virtual bool startConfirm() {
        return true;
    }
    /* vt 0x38 */ virtual bool startDecide(bool b) {
        return true;
    }
    /* vt 0x3C */ virtual bool isDoneDecide() const {
        return true;
    }
    /* vt 0x40 */ virtual bool vt_0x40() const {
        return true;
    }
};

class dLytTextSword {
public:
    enum ESwordType {
        FI,
        GHIRAHIM,
        LASTBOSS,
    };

    dLytTextSword() {}
    /* vt at 0x00 */
    virtual ~dLytTextSword() {}

    bool build(d2d::ResAccIf_c *pResAcc, ESwordType type);
    bool remove();
    void draw(const wchar_t *str, s8 frame, mVec3_c pos, f32 scale);
    f32 getAnimDuration() const;

private:
    /* 0x04 */ d2d::LytBase_c mLyt;
    /* 0x90 */ d2d::AnmGroup_c mAnm;
    /* 0xD4 */ dTextBox_c *mpTexts[3];
    /* 0xE0 */ nw4r::lyt::Pane *mpPane;
    /* 0xE4 */ ESwordType mType;
};

class dLytTextLight {
public:
    enum ETextType {
        NORMAL,
        DEMO,
    };

    dLytTextLight() {}
    /* vt at 0x00 */
    virtual ~dLytTextLight() {}

    bool build(d2d::ResAccIf_c *pResAcc, ETextType type);
    bool remove();
    void draw(s8 frame, mVec3_c pos, u8 alpha, f32 scale);
    f32 getAnimDuration() const;

private:
    /* 0x04 */ d2d::LytBase_c mLyt;
    // ???
    /* 0x94 */ d2d::AnmGroup_c mAnm;
    /* 0xD4 */ nw4r::lyt::Pane *mpPane;
    /* 0xD8 */ ETextType mType;
};

#endif
