#include "d/snd/d_snd_actor.h"

#include "d/snd/d_snd_checkers.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"

dSndActor_c::dSndActor_c(nw4r::snd::SoundArchivePlayer &rStartable) : nw4r::snd::SoundActor(rStartable) {}

void dSndActor_c::doesSomethingWithForeachSound() {
    SoundPropertiesChecker checker(-1);
    ForEachSound(checker, false);
}
