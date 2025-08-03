#ifndef D_LYT_PAUSE_INFO_H
#define D_LYT_PAUSE_INFO_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytCommonTitle_c;

/** 2D UI - Pause menu - title and button help */
class dLytPauseInfo_c {
public:
    dLytPauseInfo_c();
    virtual ~dLytPauseInfo_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    void requestIn();
    void requestOut();
    void updateTitle();

    void setInputInOut(bool inOut);

private:
    void updateCaption();
    void startAnim(int idx);
    void stopAnim(int idx);
    void playBackwards(d2d::AnmGroup_c &anm);

    STATE_FUNC_DECLARE(dLytPauseInfo_c, None);
    STATE_FUNC_DECLARE(dLytPauseInfo_c, In);
    STATE_FUNC_DECLARE(dLytPauseInfo_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseInfo_c, Out);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytPauseInfo_c);
    /* 0x040 */ dLytCommonTitle_c *mpTitle;
    /* 0x044 */ d2d::LytBase_c mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnm[3];
    /* 0x194 */ s32 mStep;
    /* 0x198 */ bool mInRequest;
    /* 0x199 */ bool mOutRequest;
    /* 0x19A */ bool field_0x19A;
    /* 0x19B */ bool field_0x19B;
    /* 0x19C */ bool mInputInOut;
};

#endif
