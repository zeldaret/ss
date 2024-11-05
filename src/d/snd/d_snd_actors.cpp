#include <d/snd/d_snd_actors.h>

dSndActor_c::dSndActor_c(nw4r::snd::SoundStartable& rStartable): nw4r::snd::detail::SoundActor(rStartable) {

}

dSnd3DActor_c::dSnd3DActor_c(nw4r::snd::SoundArchivePlayer& rPlayer, nw4r::snd::Sound3DManager& rManager): nw4r::snd::Sound3DActor(rPlayer, rManager) {
    
}
