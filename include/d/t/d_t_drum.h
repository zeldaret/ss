#ifndef D_T_DRUM_H
#define D_T_DRUM_H

#include "d/a/obj/d_a_obj_drum.h"
#include "d/t/d_tg.h"

class dTgDrum_c : public dTg_c {
public:
    dTgDrum_c() {}
    virtual ~dTgDrum_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

private:
    /* 0x0FC */ dAcRef_c<dAcOdrum_c> mDrum;
    /* 0x108 */ f32 mPastState;
    /* 0x10C */ s16 mSpawnTimer;
    /* 0x10E */ u16 mFirstSpawnDelay;
    /* 0x110 */ u16 mSpawnInterval;
    // Copied into the spawned dAcOdrum_c's params (same bit layout as read by the drum)
    /* 0x112 */ u8 mDrumGroupId;
    /* 0x113 */ u8 mDrumSceneflag;
    /* 0x114 */ u8 mDrumSceneflagInverted;
    /* 0x115 */ u8 mDrumCheckPastState;
    /* 0x116 */ bool mCanSpawn;

    static const f32 sPastStateRadius;
    static const f32 sPastStateThreshold;
    static const f32 sDrumScaleFactor;
};

#endif
