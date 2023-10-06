#pragma once

#include "types.h"
#include "rvl/OS.h"

// This is seen in Animal Crossing: City Folk also. Idk where it belongs

// Seen between EGG::Thread and EGG::ConfigurationData (eggThread and eggSystem)
// dSysNandThread (or NandRequestThread ) inherits from eggThread,
//          with part of that thread referencing tsome of these funcs

namespace EGG
{
    class Unknown {
    public:
        u8 _[0xfC-0x00]; // size unsure, but this is the limit seen
        /* 80496de0 */ void fn_80496de0(OSThread*);
        /* 80496e30 */ void fn_80496e30(OSThread*);
        /* 80496f10 */ void fn_80496f10();
        /* 804970e0 */ void fn_804970e0(OSThread*, u8*);
    };
} // namespace EGG
