#ifndef AREA_UTILS_H
#define AREA_UTILS_H

#include "m/m_vec.h"
#include "d/d_bzs_types.h"

/* 800a9b00 */ bool checkPosInArea(AREA **area, const mVec3_c &pos);
/* 800a9bd0 */ AREA *getAreaForIndexInRoom(s32 areaIndex, s32 roomId);
/* 800a9c50 */ bool checkPosInArea(s32 areaIndex, s32 roomId, const mVec3_c &pos, AREA *const *area);

#endif
