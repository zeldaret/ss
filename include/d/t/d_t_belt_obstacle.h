#ifndef D_T_BELT_OBSTACLE_H
#define D_T_BELT_OBSTACLE_H

#include "d/a/obj/d_a_obj_belt_obstacle.h"
#include "d/t/d_tg.h"

class dTgBeltObstacle_c : public dTg_c {
public:
    dTgBeltObstacle_c() {}
    virtual ~dTgBeltObstacle_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    void spawnBeltObstacle();
    void spawnStaminaFruit();

private:
    enum SpawnType_e {
        SPAWN_BELT_OBSTACLE = 0,
        SPAWN_STAMINA_FRUIT = 1,
    };

    /* 0x0FC */ dAcRef_c<dAcObeltObstacle_c> mObstacle;
    /* 0x108 */ f32 mPastState;
    /* 0x10C */ s16 mSpawnTimer;
    /* 0x10E */ u16 mFirstSpawnDelay;
    /* 0x110 */ u16 mSpawnInterval;
    /* 0x112 */ u8 mSpawnType;
    /* 0x113 */ u8 mObstacleParam0;
    /* 0x114 */ u8 mObstacleParam1;
    /* 0x115 */ u8 mObstacleParam2;
    /* 0x116 */ u8 mObstacleParam3;
    /* 0x117 */ u8 mObstacleParam4;
    /* 0x118 */ u8 _0x118[0x11C - 0x118];

    static const f32 lbl_446_rodata_0;
    static const f32 lbl_446_rodata_4;
    static const f32 lbl_446_rodata_8;
};

#endif
