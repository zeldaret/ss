#ifndef AREA_UTILS_H
#define AREA_UTILS_H

#include <m/m_vec.h>

/* 800a9b00 */ bool checkPosInArea(void **AREA, const mVec3_c& pos);
/* 800a9bd0 */ void *getAreaForIndexInRoom(s32 areaIndex, s32 roomId);
/* 800a9c50 */ bool *checkPosInArea(s32 areaIndex, s32 roomId, const mVec3_c& pos, void **AREA);

#endif
