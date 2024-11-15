#ifndef AREA_UTILS_H
#define AREA_UTILS_H

#include "m/m_angle.h"
#include "m/m_vec.h"

struct AREA {
    /* 0x00 */ mVec3_c mPos;
    /* 0x0C */ mVec3_c mScale;
    /* 0x18 */ mAng mAngleY;
    /* 0x1A */ mAng mAngleLink;
    /* 0x1C */ u8 field_0x1C;
};

/* 800a9b00 */ bool checkPosInArea(AREA **area, const mVec3_c &pos);
/* 800a9bd0 */ AREA *getAreaForIndexInRoom(s32 areaIndex, s32 roomId);
/* 800a9c50 */ bool checkPosInArea(s32 areaIndex, s32 roomId, const mVec3_c &pos, AREA *const *area);

#endif
