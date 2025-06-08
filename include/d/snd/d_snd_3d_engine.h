#ifndef D_SND_3D_ENGINE
#define D_SND_3D_ENGINE

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_Sound3DEngine.h"
#include "nw4r/snd/snd_Sound3DManager.h"

class dSnd3DEngine_c : public nw4r::snd::Sound3DEngine {
public:
    dSnd3DEngine_c();

    virtual void UpdateAmbientParam(
        const nw4r::snd::Sound3DManager *mgr, const nw4r::snd::Sound3DParam *param3d, u32 unk, u32 flags,
        nw4r::snd::SoundAmbientParam *paramAmb
    ) override;
};

#endif
