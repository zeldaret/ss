#ifndef D_T_INSECT_H
#define D_T_INSECT_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/t/d_tg.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

extern "C" bool fn_801BB750(nw4r::math::AABB *aabb, f32 f);

class dTgInsect_c : public dTg_c {
public:
    enum Subtype {
        SUBTYPE_INVALID0,
        SUBTYPE_INVALID1,
        SUBTYPE_INVALID2,
        SUBTYPE_VOLCANIC_LADYBUG,
        SUBTYPE_GERUDO_DRAGONFLY,
        SUBTYPE_WOODLAND_RHINO_BEETLE,
        SUBTYPE_SAND_CICADA,
        SUBTYPE_FARON_GRASSHOPPER,
        SUBTYPE_LANAYRU_ANT,
        SUBTYPE_SKY_STAG_BEETLE,
        SUBTYPE_BUTTERFLY,
        SUBTYPE_SKYLOFT_MANTIS,
        SUBTYPE_EDLIN_ROLLER,
        SUBTYPE_STARRY_FIREFLY,
    };
    enum SpawnSubtype {
        SPAWN_SKYLOFT_BUGKID_TREE = 1,
        SPAWN_BUG_MINIGAME,
        SPAWN_TRIAL_GATE,
        SPAWN_GOSSIP_STONE,
        SPAWN_GODDESS_WALL,
        SPAWN_DEFAULT = 0xF,
    };
    dTgInsect_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgInsect_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    void setKillSignal() {
        mKillSignal = 1;
    }

    void reveal(mVec3_c* pos) {
        mRevealed = 1;
        if (pos != nullptr) {
            mRevealedSpawnPos = *pos;
        }
    }

    static const f32 SCALE_X;
    static const f32 SCALE_Y;

    STATE_FUNC_DECLARE(dTgInsect_c, Wait);
    STATE_FUNC_DECLARE(dTgInsect_c, WaitCreate);
    STATE_FUNC_DECLARE(dTgInsect_c, WaitForceEscape);
    STATE_FUNC_DECLARE(dTgInsect_c, End);

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dTgInsect_c);
    /* 0x138 */ dAcRef_c<dAcBase_c> mLinks[16];
    /* 0x1F8 */ u8 field_0x1f8[16];
    /* 0x208 */ s32 mInsectRespawnTimers[16];
    /* 0x248 */ s32 mInsectCount;
    /* 0x24C */ u8 mRevealed; // used by OBJ_SOIL and OBJ_VSD to signal that the insect should be spawned now
    /* 0x24D */ u8 mKillSignal; // used by goddess walls to signal that the insects should despawn
    /* 0x24E */ u8 field_0x24e;
    /* 0x24F */ u8 field_0x24f;
    /* 0x250 */ u8 mShouldSpawn[16];
    /* 0x260 */ mVec3_c mRevealedSpawnPos;
    /* 0x26C */ dAcRef_c<dAcBase_c> mWarpRef;

    SpawnSubtype getSpawnSubtype() const {
        return (SpawnSubtype)(mParams >> 8 & 0xF);
    }
    bool isSpawnSubtype(SpawnSubtype spanwSubtype) const {
        return getSpawnSubtype() == spanwSubtype;
    }
    Subtype getSubtype() const {
        return (Subtype)(mParams >> 4 & 0xF);
    }
    bool isSubtype(Subtype subtype) const {
        return getSubtype() == subtype;
    }
    s32 getInsectCount() const {
        return (mParams & 0xF);
    }
    void spawnInsect(s32 index);
    bool shouldSpawn();
    void spawnAll();
    bool checkValidSpawnLocation(const mVec3_c &pos, bool updateRotation);
    bool isTrialGateType() const {
        return getSubtype() != SUBTYPE_BUTTERFLY ? false : getSpawnSubtype() == SPAWN_TRIAL_GATE;
    }
    bool isGossipStoneType() const {
        return getSubtype() != SUBTYPE_BUTTERFLY ? false : getSpawnSubtype() == SPAWN_GOSSIP_STONE;
    }
    bool isGoddessWallType() const {
        return getSubtype() != SUBTYPE_BUTTERFLY ? false : getSpawnSubtype() == SPAWN_GODDESS_WALL;
    }
};

#endif
