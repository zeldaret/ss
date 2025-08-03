#ifndef D_LYT_PAUSE_BACK_H
#define D_LYT_PAUSE_BACK_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytPauseBack_c {
public:
    dLytPauseBack_c();
    virtual ~dLytPauseBack_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    void loadBgTexture();
    void requestIn();
    void requestOut();

private:
    void startAnim(int idx);
    void stopAnim(int idx);

    STATE_FUNC_DECLARE(dLytPauseBack_c, None);
    STATE_FUNC_DECLARE(dLytPauseBack_c, In);
    STATE_FUNC_DECLARE(dLytPauseBack_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseBack_c, Out);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytPauseBack_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnm[2];

    /* 0x150 */ s32 mOutStep;
    /* 0x154 */ bool mInRequest;
    /* 0x155 */ bool mOutRequest;
    /* 0x156 */ bool field_0x156;
    /* 0x157 */ bool mVisible;
};

#endif
