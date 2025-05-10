#ifndef DOWSING_TARGET_H
#define DOWSING_TARGET_H

#include "d/a/d_a_base.h"
#include "f/f_list_nd.h"
#include "m/m_vec.h"

class DowsingTarget {
public:
    TListNode<DowsingTarget> mLink; // 0x00
private:
    dAcBase_c *mpActor; // 0x08
    u8 mDowsingSlot;    // 0x0C
    u16 field_0x0E;     // 0x0E
    mVec3_c mOffset;    // 0x10

public:
    enum DowsingSlot {
        SLOT_STORY_EVENT = 0,
        SLOT_RUPEE = 1,
        SLOT_QUEST = 2,
        SLOT_CRYSTAL = 3,
        SLOT_HEART = 4,
        SLOT_GODDESS_CUBE = 5,
        SLOT_LOOK = 6,
        SLOT_TREASURE = 7,
        SLOT_NONE = 8,
    };

    DowsingTarget(dAcBase_c *ac, u8 dowsingSlot) : mpActor(ac), mDowsingSlot(dowsingSlot) {}
    // vt offset 0x1C
    virtual ~DowsingTarget();            // 0x08
    virtual void getPosition(mVec3_c &); // 0x0C

    bool doRegister();
    bool doUnregister();
    void initialize(DowsingSlot type, u16 unk, const mVec3_c *offset, f32 f);
    void reinitialize(DowsingSlot type);

    inline DowsingSlot getSlot() {
        return (DowsingSlot)mDowsingSlot;
    }

    static bool hasZeldaDowsing();
    static bool hasKikwiDowsing();
    static bool hasKeyPieceDowsing();
    static bool hasDesertNodeDowsing();
    static bool hasAnyTrialDowsing();
    static bool hasSacredWaterDowsing();
    static bool hasSandshipDowsing();
    static bool hasTadtoneDowsing();
    static bool hasPropellerDowsing();
    static bool hasWaterBasinDowsing();
    static bool hasCrystalBallDowsing();
    static bool hasPumpkinDowsing();
    static bool hasNewPlantSpeciesDowsing();
    static bool hasPartyWheelDowsing();

    static bool hasDowsingInSlot(int slot);

    static DowsingTarget *getDowsingInfo(
        const mVec3_c &playerPosition, const mVec3_c &dowsingDirection, f32 *, f32 *, f32 *intensity, int slot
    );

    static void init();
    static void execute();

    static s32 getTopDowsingIcon();
    static s32 getLeftDowsingIcon();
};

#endif
