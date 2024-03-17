#ifndef EGG_IAUDIO_MANAGER_H
#define EGG_IAUDIO_MANAGER_H

#include "egg/core/eggHeap.h"
#include "egg/egg_types.h"


namespace EGG {
class IAudioMgr {
public:
    struct Arg {
        /* 0x00 */ EGG::Heap *heap;
        /* 0x04 */ char *soundFileName;
        /* 0x08 */ int sndThreadPriority;
        /* 0x0c */ int sndThreadStackSize;
        /* 0x10 */ int dvdThreadPriority;
        /* 0x14 */ int dvdThreadStackSize;
        /* 0x18 */ u32 blocks;
        /* 0x1c */ u8 field_0x1C;
    };
};

class SimpleAudioMgr : public IAudioMgr, public SoundHeapMgr, public ArcPlayer {};
} // namespace EGG

#endif
