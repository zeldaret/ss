#ifndef D_LYT_NOTE_H
#define D_LYT_NOTE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

// size 0x27C
class dLytNote_c {
public:
    dLytNote_c() : mStateMgr(*this) {}
    virtual ~dLytNote_c() {}

    bool build(d2d::ResAccIf_c *);
    bool remove();
    void execute(u8 alpha);
    void draw();

    void setTransform(nw4r::lyt::Pane &p);

    f32 getField0x260() const;
    void setFrame(f32);

private:
    STATE_FUNC_DECLARE(dLytNote_c, Wait);
    STATE_FUNC_DECLARE(dLytNote_c, In);
    STATE_FUNC_DECLARE(dLytNote_c, Move);
    STATE_FUNC_DECLARE(dLytNote_c, Out);

    /* 0x004 */ STATE_MGR_DECLARE(dLytNote_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[6];
    /* 0x250 */ nw4r::lyt::Pane *mpPane;
    /* 0x254 */ mVec3_c mTranslate;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ s32 mOutAnim;
    /* 0x268 */ s32 mInAnim;
    /* 0x26C */ s32 field_0x26C;
    /* 0x270 */ u8 field_0x270;
    /* 0x271 */ u8 field_0x271;
    /* 0x272 */ u8 field_0x272;
    /* 0x273 */ u8 field_0x273;
    /* 0x274 */ u8 field_0x274;
};

#endif
