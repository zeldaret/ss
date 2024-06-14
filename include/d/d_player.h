#ifndef D_PLAYER_H
#define D_PLAYER_H

#include <common.h>
#include "d/a/obj/d_a_obj_base.h"

class dPlayer: public dAcObjBase_c {
public:
    /* 0x330 */ u8 unk_0x330[0x350 - 0x330];
    /* 0x350 */ u32 someFlags;



    static dPlayer *LINK;
};

#endif
