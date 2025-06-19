#include "d/snd/d_snd_source_py_bird.h"
#include "common.h"
#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_se_sound_2.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"

void dSndSourcePyBird_c::load(void *data, const char *name) {
    if (streq(name, "GlideDive")) {
        mSeqVarValue = 1;
    } else if (streq(name, "SwitchToDive") && mSeqVarValue != 1) {
        mSeqVarValue = 1;
    } else if (streq(name, "Brake")) {
        mSeqVarValue = 2;
        startSound(SE_PyBird_V_BRAKE, nullptr);
    } else if (streq(name, "DamageTornado")) {
        startSound(SE_PyBird_V_TORNADE, nullptr);
    } else if (mSeqVarValue == 1) {
        if (!streq(name, "Attack") && !streq(name, "Accel")) {
            mSeqVarValue = 0;
        }
    } else {
        mSeqVarValue = 0;
    }
    dSndSeSound2_c *glideSound = getHandleType2ForSoundId(SE_PyBird_GLIDE);
    if (glideSound != nullptr && glideSound->getHandle() != nullptr) {
        nw4r::snd::SeqSoundHandle handle(glideSound->getHandle());
        handle.WriteVariable(10, mSeqVarValue);
    }
    mAnimSound.setData(data, name);
}
