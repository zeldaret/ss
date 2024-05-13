#ifndef NW4R_EF_CREATIONQUEUE
#define NW4R_EF_CREATIONQUEUE
#include "common.h"
#include "ef_emitter.h"


namespace nw4r {
namespace ef {
enum CreationType { CreationType_Particle, CreationType_Emitter };

// sizeof(UNK_CREATIONQUEUE) = 0x14
struct Creation {
    u8 mType; // at 0x0
    u16 SHORT_0x2;
    EmitterInheritSetting mInheritSetting; // at 0x4
    Particle *mParticle;                   // at 0x8
    EmitterResource *mResource;            // at 0xc
};

struct CreationQueue {
    CreationQueue();
    void AddParticleCreation(const EmitterInheritSetting *, Particle *, EmitterResource *, u16);
    void AddEmitterCreation(const EmitterInheritSetting *, Particle *, EmitterResource *, u16);

    int mSize;

    Creation mCreations[0x400];

    inline bool IsEmpty() {
        return mSize == 0;
    }

    void Execute();
};
} // namespace ef
} // namespace nw4r

#endif
