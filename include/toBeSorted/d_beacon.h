#ifndef D_BEACON_H
#define D_BEACON_H

#include "m/m_vec.h"

u32 getMaxBeaconCount();

/**
 * When passed a position, sets the beacon with the given index to the position.
 * When position is nullptr, removes the beacon.
 */
void setBeaconPosition(const mVec3_c *position, u32 beaconIndex);

/**
 * Sets a beacon position similar to `setBeaconPosition`, but corrects the ground level
 */
void setBeaconPositionChecked(const mVec3_c *position, u32 beaconIndex);

/** Checks whether the current stage is ineligible for beacon placement */
bool doesStageForbidBeaconPlacement();

const mVec3_c *getBeaconPosition(u32 beaconIndex);

#endif
