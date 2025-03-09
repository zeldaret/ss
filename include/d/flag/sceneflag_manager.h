#ifndef SCENEFLAG_MANAGER_H
#define SCENEFLAG_MANAGER_H

#include "common.h"
#include "d/flag/bitwise_flag_helper.h"
#include "d/flag/flag_space.h"


class SceneflagManager {
public:
    FlagSpace mSceneflags;
    FlagSpace mTempflags;
    FlagSpace mZoneflags;
    BitwiseFlagHelper mFlagHelper;
    u16 mSceneIdx;
    u8 mShouldCommit;

    static u16 sTempFlags[4];
    static u16 sSceneFlags[8];
    static u16 sZoneFlags[0xFC];

    static SceneflagManager *sInstance;
    void init();
    void setShouldCommit(u16 flag);
    SceneflagManager();
    s32 isNotTempOrZoneFlag(u16 flag);
    s32 isZoneFlag(u32 flag);
    void updateFlagindex(u16 flagindex);
    void copyFromSave(u32 flagindex);
    void unsetAllTempflags();
    void zoneflagsResetAll();
    void zoneflagsResetForRoom(u16 roomId);
    void unsetZoneAndTempflags();
    void unsetAllZoneflags();
    void unsetZoneflagsForRoom(u16 roomId);
    u16 getZoneflagSlot(u16 roomId, u16 flag);
    u16 getSceneflagSlotGlobal(u16 sceneIdx, u16 flag);
    u16 getSceneflagSlot(u16 flag);
    u16 getTempflagSlot(u16 flag);
    bool checkZoneFlag(u16 roomId, u16 flag);
    bool checkUncommittedZoneflag(u16 roomId, u16 flag);
    u16 checkUncommittedZoneflag2(u16 roomId, u16 flag) {
        return checkUncommittedZoneflag(roomId, flag);
    }
    bool checkUncommittedTempOrSceneflag(u16 flag);
    u16 checkUncommittedTempOrSceneflag2(u16 flag) {
        return checkUncommittedTempOrSceneflag(flag);
    }
    u16 checkFlag(u16 roomId, u16 flag);
    bool checkSceneflagGlobal(u16 sceneIdx, u16 flag);
    u16 checkTempOrSceneflag(u16 flag);
    u16 checkUncommittedFlag(u16 roomId, u16 flag);
    void setZoneflag(u16 roomId, u16 flag);
    void setFlag(u16 roomId, u16 flag);
    void setSceneflagGlobal(u16 sceneIdx, u16 flag);
    void setTempOrSceneflag(u16 flag);
    void unsetZoneflag(u16 roomId, u16 flag);
    void unsetFlag(u16 roomId, u16 flag);
    void unsetSceneflagGlobal(u16 sceneIdx, u16 flag);
    void unsetTempOrSceneflag(u16 flag);
    s32 commit();

    bool checkBoolFlag(u16 roomid, u16 flag) {
        return checkFlag(roomid, flag);
    }
};

#endif
