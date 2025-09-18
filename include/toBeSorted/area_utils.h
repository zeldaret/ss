#ifndef AREA_UTILS_H
#define AREA_UTILS_H

#include "d/d_bzs_types.h"
#include "m/m_vec.h"

bool checkPosInArea(AREA **area, const mVec3_c &pos);
AREA *getAreaForIndexInRoom(s32 areaIndex, s32 roomId);
bool checkPosInArea(s32 areaIndex, s32 roomId, const mVec3_c &pos, AREA *const *area);

#endif
