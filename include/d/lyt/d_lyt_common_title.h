#ifndef D_LYT_COMMON_TITLE_H
#define D_LYT_COMMON_TITLE_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "sized_string.h"

class dLytCommonTitle_c {
public:
    dLytCommonTitle_c();
    virtual ~dLytCommonTitle_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    bool set(s32 arg, const char *title, const char *caption);
    const char *setSubTitle(s32 msgIdx);
    const char *setSysTitle(s32 msgIdx);
    const char *setSysCaption(s32 msgIdx);

private:
    void gotoStateNone();
    void startAnim(int idx);
    void stopAnim(int idx);
    void playBackwards(d2d::AnmGroup_c &anm);

    void applyText();
    void applyTextChange(s32 arg);

    void applyTitle(s32 arg);
    void applyCaption(s32 arg);

    d2d::AnmGroup_c &getAnm(int idx) {
        return mAnm[idx];
    }

    STATE_FUNC_DECLARE(dLytCommonTitle_c, None);
    STATE_FUNC_DECLARE(dLytCommonTitle_c, In);
    STATE_FUNC_DECLARE(dLytCommonTitle_c, Wait);
    STATE_FUNC_DECLARE(dLytCommonTitle_c, Out);
    STATE_FUNC_DECLARE(dLytCommonTitle_c, Change);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytCommonTitle_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::ResAccIf_c mResAcc;
    /* 0x440 */ d2d::AnmGroup_c mAnm[8];
    /* 0x640 */ SizedString<0x20> mTitle;
    /* 0x660 */ SizedString<0x20> mCaption;

    /* 0x680 */ UNKWORD field_0x680;
    /* 0x680 */ UNKWORD field_0x684;
    /* 0x688 */ s32 mStep;
    /* 0x68C */ bool mChangeRequest;
    /* 0x68D */ bool field_0x68D;
    /* 0x68E */ bool mVisible;
    /* 0x68F */ bool field_0x68F;
    /* 0x690 */ bool field_0x690;
};

#endif
