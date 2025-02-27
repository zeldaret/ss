#ifndef ACTOR_ON_RAIL_H
#define ACTOR_ON_RAIL_H

#include "common.h"
#include "m/m_vec.h"

class ActorOnRail {
private:
    void *mpPathPtr;
    int mRoomIndex;
    u8 mPathSubtype;

public:
    /* 800A6690 */
    ActorOnRail();
    /* 800A66D0 */
    virtual ~ActorOnRail();

    /* 800A6D90 */
    bool init(int pathIndex, int roomId, int pathSubtype);
    /* 800A68B0 */
    mVec3_c *getPntPosForIndex(int index);
};

class ActorOnRail_Ext : public ActorOnRail {
public:
    ActorOnRail_Ext();
    virtual ~ActorOnRail_Ext();

    void setSegment(u16 segmentIndex, f32 segmentFraction);
    bool initExt(int pathIndex, int roomId, int, int pathSegment, int pathSubtype, f32, f32, f32);

    f32 getSegmentFraction() const {
        return mSegmentFraction;
    }

    bool CheckFlag(u32 flag) const {
        return (field_0x1C & flag) != 0;
    }

    void ClearFlag(u32 flag) {
        field_0x1C &= ~flag;
    }

    void SetFlag(u32 flag) {
        field_0x1C |= flag;
    }

    const mVec3_c &getPosition() const {
        return mPosition;
    }


private:
    s32 mSegmentIndex;
    f32 mSegmentFraction;
    UNKWORD field_0x18;
    u32 field_0x1C;
    f32 field_0x20;
    UNKWORD field_0x24;
    mVec3_c mPosition;
};

#endif
