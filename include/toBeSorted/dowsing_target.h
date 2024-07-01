#ifndef DOWSING_TARGET_H
#define DOWSING_TARGET_H

#include <d/a/d_a_base.h>
#include <f/f_list_nd.h>
#include <m/m_vec.h>

class DowsingTarget {
public:
    TListNode mLink; // 0x00
private:
    dAcBase_c *mpActor; // 0x08
    u8 mDowsingSlot;    // 0x0C
    u16 field_0x0E;     // 0x0E
    mVec3_c mOffset;    // 0x10

public:
    // TODO
    enum DowsingSlot {
        SLOT_0,
        SLOT_1,
        SLOT_2,
        SLOT_CRYSTALS,
        SLOT_4,
        SLOT_5,
        SLOT_6,
        SLOT_7,
        SLOT_NONE
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

    static DowsingTarget *getDowsingInfo(const mVec3_c &playerPosition, const mVec3_c &dowsingDirection, f32 *, f32 *,
            f32 *intensity, int slot);

    static void init();
    static void execute();

    static u8 getTopDowsingIcon();
    static u8 getLeftDowsingIcon();
};

#endif
