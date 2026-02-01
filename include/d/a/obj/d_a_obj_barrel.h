#ifndef D_A_OBJ_BARREL_H
#define D_A_OBJ_BARREL_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/time_area_mgr.h"

class dAcNpcCeLady_c;
class dAcNpcCeFriend_c;

class dAcOBarrel_c : public dAcObjBase_c {
public:
    enum Type {
        Normal,
        Bomb,
        Type2,
        Kraken,
    };

    dAcOBarrel_c() : mStateMgr(*this) {}
    virtual ~dAcOBarrel_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    virtual void registerInEvent() override;
    virtual void unkVirtFunc_0x6C() override;
    virtual void doInteraction(s32) override;

    /** fn_293_30D0 */
    void fn_293_30D0();

    /** fn_293_31B0 */
    void fn_293_31B0();

    /** fn_293_3560 */
    void fn_293_3560();

    /** fn_293_37B0 */
    void fn_293_37B0();

    /** fn_293_3A20 */
    void updateMtx();

    /** fn_293_3DB0 */
    void fn_293_3DB0();

    /** Break Barrel */
    void fn_293_4200();

    /** Check to see if the Barrel should break */
    bool fn_293_45A0();

    /** fn_293_4A90 */
    bool fn_293_4A90(const mAng &);

    /** Checks if On Lava or Sink Sand */
    bool fn_293_4BC0();

    /** Checks if On Lava */
    bool fn_293_4C20() const;

    /** fn_293_4C60 */
    bool fn_293_4C60();

    /** Check to see if its below some range */
    bool fn_293_4CE0() const;

    /** Check to see if its being moved. (like Conveyor) */
    bool fn_293_4D00();

    /** Check to see if it is being moved upwards? */
    bool fn_293_4ED0() const;

    /** fn_293_4F80 */
    bool fn_293_4F80();

    /** fn_293_50B0 */
    bool fn_293_50B0();

    /** Perform Timeslip */
    void fn_293_5150();

    /** fn_293_5360 */
    void fn_293_5360();

    /** fn_293_5440 */
    void fn_293_5440();

    /** fn_293_54D0 */
    void fn_293_54D0();

    /** fn_293_5850 */
    void fn_293_5850(f32 *, bool);

    /** Set Ground Normal */
    void fn_293_58C0();

    /** fn_293_5910 */
    void fn_293_5910();

    STATE_FUNC_DECLARE(dAcOBarrel_c, Wait);
    STATE_FUNC_DECLARE(dAcOBarrel_c, Grab);
    STATE_FUNC_DECLARE(dAcOBarrel_c, GrabUp);
    STATE_FUNC_DECLARE(dAcOBarrel_c, GrabPut);
    STATE_FUNC_DECLARE(dAcOBarrel_c, Slope);
    STATE_FUNC_DECLARE(dAcOBarrel_c, Water);
    STATE_FUNC_DECLARE(dAcOBarrel_c, Water2);
    STATE_FUNC_DECLARE(dAcOBarrel_c, Explode);
    STATE_FUNC_DECLARE(dAcOBarrel_c, KrakenBorn);
    STATE_FUNC_DECLARE(dAcOBarrel_c, Rebirth);

public:
    bool isType_0() const {
        return mType == Type2 || mType == Kraken;
    }

    bool isTimeshiftableType() const {
        return mType & 1;
    }

    bool field0x17Check() const {
        return field_0xE17 == 0 || field_0xE17 == 1;
    }
    bool isType_1() const {
        return mType & 2;
    }

    bool checkFlag_0xE18() {
        return field_0xE18 < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, field_0xE18);
    }

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShadow;
    /* 0x358 */ dBgS_AcchCir mAcchCir;
    /* 0x3B4 */ dBgS_ObjAcch mObjAcch;
    /* 0x764 */ cBgS_PolyInfo mPolyInfo;
    /* 0x790 */ dBgW mBgW;
    /* 0x9A0 */ dCcD_Cyl mCyl;
    /* 0xAF0 */ dCcD_Sph mSph;
    /* 0xC40 */ STATE_MGR_DECLARE(dAcOBarrel_c);
    /* 0xC7C */ TimeAreaStruct mTimeArea;
    /* 0xC88 */ dAcRef_c<dAcNpcCeLady_c> mCeLady;
    /* 0xC94 */ dAcRef_c<dAcNpcCeFriend_c> mCeFriend;
    /* 0xCA0 */ dEmitter_c mEmitter0;
    /* 0xCD4 */ dEmitter_c mEmitter1;
    /* 0xD08 */ dWaterEffect_c mWaterEffect;
    /* 0xD50 */ mQuat_c quat_0xD50;
    /* 0xD60 */ mQuat_c quat_0xD60;
    /* 0xD70 */ mQuat_c quat_0xD70;
    /* 0xD80 */ mQuat_c quat_0xD80;
    /* 0xD90 */ mQuat_c quat_0xD90;
    /* 0xDA0 */ mQuat_c quat_0xDA0;
    /* 0xDB0 */ mVec3_c field_0xDB0; //< player direction vector
    /* 0xDBC */ mVec3_c field_0xDBC;
    /* 0xDC8 */ mVec3_c field_0xDC8;
    /* 0xDD4 */ f32 field_0xDD4;
    /* 0xDD8 */ f32 field_0xDD8;
    /* 0xDDC */ f32 field_0xDDC;
    /* 0xDE0 */ f32 field_0xDE0;
    /* 0xDE4 */ mAng3_c field_0xDE4;
    /* 0xDEA */ mAng field_0xDEA;
    /* 0xDEC */ mAng field_0xDEC;
    /* 0xDEE */ mAng field_0xDEE;
    /* 0xDF0 */ mAng field_0xDF0;
    /* 0xDF2 */ mAng field_0xDF2;
    /* 0xDF4 */ mAng field_0xDF4;
    /* 0xDF6 */ mAng field_0xDF6;
    /* 0xDF8 */ mAng field_0xDF8;
    /* 0xDFA */ mAng field_0xDFA;
    /* 0xDFC */ u16 field_0xDFC;
    /* 0xDFE */ u16 field_0xDFE;
    /* 0xE00 */ bool field_0xE00; //< Can Damage ?
    /* 0xE01 */ bool field_0xE01;
    /* 0xE02 */ bool field_0xE02;
    /* 0xE03 */ bool field_0xE03;
    /* 0xE04 */ bool field_0xE04;
    /* 0xE05 */ bool field_0xE05;
    /* 0xE06 */ bool field_0xE06;
    /* 0xE07 */ bool field_0xE07;
    /* 0xE08 */ bool field_0xE08;
    /* 0xE09 */ bool field_0xE09; //< for OBJ_VSD Present
    /* 0xE0A */ bool field_0xE0A;
    /* 0xE0B */ bool field_0xE0B;
    /* 0xE0C */ bool field_0xE0C; //< Timeslip Active
    /* 0xE0D */ bool field_0xE0D; //< Timeslip_rev Active
    /* 0xE0E */ bool field_0xE0E;
    /* 0xE0F */ u8 mType;
    /* 0xE10 */ u8 field_0xE10;
    /* 0xE11 */ u8 field_0xE11;
    /* 0xE12 */ u8 field_0xE12;
    /* 0xE13 */ u8 field_0xE13;
    /* 0xE14 */ u8 field_0xE14;
    /* 0xE15 */ u8 field_0xE15;
    /* 0xE16 */ u8 field_0xE16;
    /* 0xE17 */ u8 field_0xE17;
    /* 0xE18 */ u8 field_0xE18;
    /* 0xE19 */ u8 mBrokenFlag;
    /* 0xE1C */ s32 mDropItem;
};

#endif
