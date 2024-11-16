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

class ActorOnRail3 {
    ActorOnRail mPath;
    u16 mSomePntIdx;
    u32 mFlags;

public:
    ActorOnRail3() : mSomePntIdx(0), mFlags(0) {}
};

#endif
