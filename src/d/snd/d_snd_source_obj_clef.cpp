#include "d/snd/d_snd_source_obj_clef.h"

#include "common.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundHandle.h"

struct SndTadtoneGroup {
    u8 count;
    u8 pitchVariables[];
};

static const SndTadtoneGroup grp1 = {
    1,
    {0x41},
};

static const SndTadtoneGroup grp2 = {
    4,
    {0x3C, 0x40, 0x41, 0x43},
};

static const SndTadtoneGroup grp3 = {
    2,
    {0x43, 0x37},
};

static const SndTadtoneGroup grp4 = {
    8,
    {0x34, 0x43, 0x41, 0x43, 0x45, 0x35, 0x39, 0x3C},
};

static const SndTadtoneGroup grp5 = {
    1,
    {0x41},
};

static const SndTadtoneGroup grp6 = {
    8,
    {0x48, 0x34, 0x37, 0x3C, 0x43, 0x4A, 0x4C, 0x48},
};

static const SndTadtoneGroup grp7 = {
    1,
    {0x41},
};

static const SndTadtoneGroup grp8 = {
    4,
    {0x3C, 0x40, 0x41, 0x43},
};

static const SndTadtoneGroup grp9 = {
    8,
    {0x43, 0x3C, 0x37, 0x3C, 0x34, 0x43, 0x41, 0x43},
};

static const SndTadtoneGroup grp10 = {
    1,
    {0x45},
};

static const SndTadtoneGroup grp11 = {
    4,
    {0x41, 0x35, 0x39, 0x3C},
};

static const SndTadtoneGroup grp12 = {
    8,
    {0x48, 0x34, 0x37, 0x3C, 0x43, 0x4A, 0x4C, 0x48},
};

static const SndTadtoneGroup grp13 = {
    1,
    {0x41},
};

static const SndTadtoneGroup grp14 = {
    4,
    {0x3C, 0x40, 0x41, 0x43},
};

static const SndTadtoneGroup grp15 = {
    2,
    {0x43, 0x37},
};

static const SndTadtoneGroup grp16 = {
    4,
    {0x37, 0x43, 0x41, 0x43},
};

static const SndTadtoneGroup grp17 = {
    16,
    {0x45, 0x39, 0x3E, 0x41, 0x35, 0x39, 0x3E, 0x41, 0x48, 0x34, 0x37, 0x3C, 0x43, 0x4A, 0x4C, 0x48},
};

static const SndTadtoneGroup *sGroups[] = {
    &grp1,  &grp2,  &grp3,  &grp4,  &grp5,  &grp6,  &grp7,  &grp8,  &grp9,
    &grp10, &grp11, &grp12, &grp13, &grp14, &grp15, &grp16, &grp17,
};

static nw4r::snd::SoundHandle clefSoundHandle;

bool dSndSourceObjClef_c::startTadtoneSound(u32 groupIdx, u32 noteIdx) {
    if (groupIdx > ARRAY_LENGTH(sGroups)) {
        return false;
    }

    if (groupIdx < 1) {
        return false;
    }

    const SndTadtoneGroup *grp = sGroups[groupIdx - 1];

    if (noteIdx >= grp->count) {
        return false;
    }

    s16 var = grp->pitchVariables[noteIdx];

    clefSoundHandle.Stop(10);

    nw4r::snd::SoundHandle *pHandle = startSound(SE_Clef_VOICE, &clefSoundHandle);
    if (pHandle != nullptr) {
        nw4r::snd::SeqSoundHandle seqHandle(pHandle);
        seqHandle.WriteVariable(0, var);
        return true;
    }

    return false;
}
