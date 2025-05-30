#ifndef D_SND_3D_ACTOR_H
#define D_SND_3D_ACTOR_H

#include "nw4r/snd/snd_SoundActor.h"
#include "nw4r/snd/snd_SoundHandle.h"

class dSndActor_c : public nw4r::snd::SoundActor {
public:
    dSndActor_c(nw4r::snd::SoundArchivePlayer &rPlayer);
};

#endif
