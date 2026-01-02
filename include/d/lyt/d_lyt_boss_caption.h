#ifndef D_LYT_BOSS_CAPTION_H
#define D_LYT_BOSS_CAPTION_H

#include "common.h"

class dLytBossCaption_c {
public:
    static dLytBossCaption_c *GetInstance() {
        return sInstance;
    }

    void show(const char *label, UNKWORD);

    // I believe this is the inline that causes all the Rev0 -> Rev1 pain in RELs...
    void unk_inline() {
        field_0x645 = 0;
        field_0x646 = 0;
    }

private:
    static dLytBossCaption_c *sInstance;

    u8 _0x00[0x645 - 0x00];
    u8 field_0x645;
    u8 field_0x646;
};

#endif
