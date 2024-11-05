#ifndef D_SND_ACTORS_H
#define D_SND_ACTORS_H

#include <nw4r/snd/snd_SoundActor.h>
#include <nw4r/snd/snd_Sound3DActor.h>


class dSndActor_c : public nw4r::snd::SoundActor {
public:
    dSndActor_c(nw4r::snd::SoundArchivePlayer& rStartable);
};

class dSnd3DActor_c : public nw4r::snd::Sound3DActor {
public:
    dSnd3DActor_c(nw4r::snd::SoundArchivePlayer& rPlayer, nw4r::snd::Sound3DManager& rManager);
};

#endif
