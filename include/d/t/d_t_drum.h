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
    /* 0x112 */ u8 mDrumParam0;
    /* 0x113 */ u8 mDrumParam1;
    /* 0x114 */ u8 mDrumParam2;
    /* 0x115 */ u8 mDrumParam3;
    /* 0x116 */ bool mCanSpawn;

    static const f32 lbl_447_rodata_0;
    static const f32 lbl_447_rodata_4;
    static const f32 lbl_447_rodata_8;
};

#endif
