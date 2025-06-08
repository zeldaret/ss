#ifndef EGG_AUDIO_ARC_UTILITY_H
#define EGG_AUDIO_ARC_UTILITY_H

#include "common.h"
#include "egg/audio/eggAudioMgr.h"
#include "nw4r/ut/ut_list.h"

namespace EGG {

// We don't really know much about this since it's
// unused in both NSMBW and SS
struct MultiArcSimpleAudioMgr {
    u8 field_0x000[0x0FC - 0x000];
    s32 field_0x0FC;
    ArcPlayer players[1];

    nw4r::snd::SoundArchivePlayer *getPlayer(int i) {
        return players[i].getPlayer();
    }
};

class AudioUtility {
public:
    class MoveParamMgr {
    public:
        MoveParamMgr();
        static void init();

        nw4r::ut::List mList;
    };

    class HBM {
    public:
        static void init(SimpleAudioMgr *mgr, void (*userCallback)(), u32 frame);
        static void enter();
        static void exit(bool);

        static MultiArcSimpleAudioMgr *sMultiArcSimpleAudioMgr;
        static SimpleAudioMgr *sSimpleAudioMgr;
        static void (*sHBMEffectRestCallback)();
        static void (*sHBMUserCallback)(s32, s32);
        static u32 sHBFadeframe;
    };

    static MoveParamMgr sMoveParamMgr;

    static nw4r::ut::List lbl_80675480;
};

} // namespace EGG

#endif
