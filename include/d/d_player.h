#ifndef D_PLAYER_H
#define D_PLAYER_H

#include <common.h>
#include "d/a/obj/d_a_obj_base.h"

class dPlayer: public dAcObjBase_c {
public:
    /* 0x330 */ u8 unk_0x330[0x350 - 0x330];
    /* 0x350 */ u32 someFlags_0x350;
    u8 UNK_0x354[0x35C - 0x354];
    /* 0x35C */ u32 mForceOrPreventActionFlags;

    static dPlayer *LINK;
};

#endif
