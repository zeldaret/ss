#ifndef D_T_KS_H
#define D_T_KS_H

#include "d/a/d_a_base.h"
#include "d/t/d_tg.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_path.h"
#include "toBeSorted/time_area_mgr.h"

class dAcEKs_c;
class dTgKiesuTag_c : public dTg_c {
public:
    dTgKiesuTag_c() : mStateMgr(*this) {}
    virtual ~dTgKiesuTag_c() {}

    STATE_FUNC_DECLARE(dTgKiesuTag_c, CreateChildren);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlWait);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlReturnToWait);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlMove);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlFighting);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlPathMove);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, DoNothing);

    /* fn_154_8B0 */
    mVec3_c &getSpawnKsPosition();

    void incrementField_0x456() {
        field_0x456++;
    }

    void setField_0x47A() {
        field_0x47A = true;
    }

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dTgKiesuTag_c);
    /* 0x138 */ dAcRef_c<dAcEKs_c> mKsRefs[16];
    /* 0x1F8 */ mMtx_c mAreaMtx;
    /* 0x228 */ ActorOnRail_Ext mRail;
    /* 0x25C */ TimeAreaStruct mTimeArea;
    /* 0x268 */ mMtx_c field_0x268;

    /* 0x31C */ mVec3_c field_0x31C;
    /* 0x328 */ mVec3_c field_0x328;
    /* 0x334 */ mVec3_c field_0x334;
    /* 0x340 */ mVec3_c field_0x340;
    /* 0x34C */ mVec3_c field_0x34C;

    /* 0x37C */ mVec3_c mTargetPosition;
    /* 0x388 */ mVec3_c mSpawnKsPosition;

    /* 0x3A0 */ mAng3_c mSpawnKsAngle;
    /* 0x3A8 */ mAng3_c field_0x3A8;
    /* 0x3AC */ mAng field_0x3AC; // pitch
    /* 0x3AE */ mAng field_0x3AE; // yaw

    /* 0x3B4 */ f32 field_0x3B4;
    /* 0x3B8 */ f32 field_0x3B8; // Y pos offset
    /* 0x3BC */ f32 field_0x3BC; // X/Z scale
    /* 0x3C0 */ f32 field_0x3C0; // Y scale

    /* 0x3D8 */ f32 field_0x3D8; // Y scale
    /* 0x3DC */ f32 field_0x3DC; // X/Z scale

    /* 0x3EC */ mVec3_c field_0x3EC;

    /* 0x44C */ s16 field_0x448;

    // Related to Control?
    /* 0x44C */ s16 field_0x44C;
    /* 0x44E */ s16 field_0x44E;
    /* 0x450 */ s16 field_0x450;

    // Related to ControlFighting?
    /* 0x452 */ s16 field_0x452;
    /* 0x454 */ s16 field_0x454;
    /* 0x456 */ s16 field_0x456;
    /* 0x458 */ s16 field_0x458;
    /* 0x45A */ s16 field_0x45A;
    /* 0x45C */ s16 field_0x45C;
    /* 0x45E */ s16 field_0x45E;

    /* 0x460 */ s16 field_0x460;
    /* 0x462 */ s16 mMaxKsToLink;

    /* 0x468 */ s16 field_0x468;

    /* 0x46C */ s16 mNumKsAlive;
    /* 0x46E */ s16 field435_0x46e;

    /* 0x44A */ s16 mNumKsControl;

    /* 0x474 */ bool field_0x474;
    /* 0x47A */ bool field_0x47A;
    /* 0x47E */ bool mbAllKsActive;

    /* 0x48A */ u8 field_0x48A; // Related to getting spawnPos/ Type?

    /* 0x48E */ u8 mNumKsActive;
};

#endif
