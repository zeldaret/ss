#ifndef D_A_OBJ_TOWER_HAND_D101_H
#define D_A_OBJ_TOWER_HAND_D101_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_item.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/effects_struct.h"

class dAcOTowerHandD101_c : public dAcObjBase_c {
public:
    dAcOTowerHandD101_c() : mStateMgr(*this, sStateID::null), mEffects(this), mEvent(*this, nullptr), mFlags(0) {}
    virtual ~dAcOTowerHandD101_c() {}

    STATE_FUNC_DECLARE(dAcOTowerHandD101_c, RemainOpen);
    STATE_FUNC_DECLARE(dAcOTowerHandD101_c, Close);
    STATE_FUNC_DECLARE(dAcOTowerHandD101_c, RemainClosed);
    STATE_FUNC_DECLARE(dAcOTowerHandD101_c, Open);
    STATE_FUNC_DECLARE(dAcOTowerHandD101_c, Hold);

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    virtual int draw() override;

private:
    int getDirection() const;
    int getHoldFlag() const;
    int getSceneFlag() const;
    u32 getEventId() const;

    inline bool getItem(dAcItem_c *&outItem, bool &outIsWait);
    inline bool getItem(dAcItem_c *&outItem);

    static bool isValidDirectionParam(int param);
    static bool getScale(int direction, mVec3_c &outScale);
    bool getNodeMtx(const char *nodeName, mMtx_c &outMtx) const;
    bool getLoc1Mtx(mMtx_c &outMtx) const;
    bool getLoc2Mtx(mMtx_c &outMtx) const;

    static const mVec3_c &getBoundingLower();
    static const mVec3_c &getBoundingUpper();

    static bool isValidBgWIndex(int index);
    bool getBgW(int index, dBgW *&outBgW);
    bool getBgWMtx(int index, mMtx_c *&outMtx);

    static f32 getFrame1();
    static f32 getFrame2();

    void doSomethingHold(f32 flag);

    void getItemPos(mVec3_c &vec) const;
    ActorEventRelated &getEventStuff();
    void doEvent();

    static f32 getDist1();
    static f32 getDist2();
    static f32 getDist3();

    static f32 getDist1Sq();
    static f32 getDist1PlusDist2();
    static f32 getDist1PlusDist2Sq();
    static f32 getDist3Sq();
    static f32 getCloseRate();
    static f32 getOpenRate();

    bool checkFlag(bool &result) const;
    void setSceneFlag(int flag) const;
    void unsetSceneFlag(int flag) const;
    bool checkSceneFlag1(int flag, bool &result) const;

    void setHoldFlag();
    void unsetHoldFlag();

    static void eventCallback(void *self);

    static void
    transformMtx(const mVec3_c &localTranslation, const mAng3_c &rotation, const mVec3_c &offset, mVec3_c &result);
    static f32 f32Sq(f32 val);

    static f32 getItemOffsetX();
    static f32 getItemOffsetY();
    static f32 getItemOffsetZ();
    static mVec3_c getItemOffset();
    void calcItemPosition(const mVec3_c &offset, mVec3_c &outPosition) const;

    static f32 getLinkOffsetX();
    static f32 getLinkOffsetY();
    static f32 getLinkOffsetZ();
    static void getLinkOffsetNormal(mVec3_c &out);
    static void getLinkOffsetMirror(mVec3_c &out);
    void getLinkOffset(mVec3_c &out) const;

    /* 0x330 */ m3d::mdlAnmChr mMdl;
    /* 0x398 */ EffectsStruct mEffects;
    /* 0x3CC */ dBgW mBgWs[3];
    /* 0x9FC */ STATE_MGR_DECLARE(dAcOTowerHandD101_c);
    /* 0xA38 */ ActorEventRelated mEvent;
    /* 0xA88 */ mMtx_c mLoc1Mtx;
    /* 0xAB8 */ mMtx_c mLoc2Mtx;
    /* 0xAE8 */ dAcRef_c<dAcItem_c> mHeldItem;
    /* 0xAF4 */ f32 field_0xAF4;
    /* 0xAF8 */ u8 field_0xAF8[0xB04 - 0xAF8];
    /* 0xB04 */ u32 mFlags;
    /* 0xB08 */ u16 field_0xB08;
};

#endif
