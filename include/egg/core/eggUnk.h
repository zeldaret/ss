#ifndef EGG_UNK_H
#define EGG_UNK_H

#include "common.h"

#include "rvl/OS.h" // IWYU pragma: export

// This is seen in Animal Crossing: City Folk also. Idk where it belongs

// Seen between EGG::Thread and EGG::ConfigurationData (eggThread and eggSystem)
// dSysNandThread (or NandRequestThread ) inherits from eggThread,
//          with part of that thread referencing tsome of these funcs

namespace EGG {
class Unknown {
public:
    u8 _[0xfC - 0x00]; // size unsure, but this is the limit seen
    void fn_80496de0(OSThread *);
    void fn_80496e30(OSThread *);
    void fn_80496f10();
    void fn_804970e0(OSThread *, u8 *);
};
} // namespace EGG

#endif
