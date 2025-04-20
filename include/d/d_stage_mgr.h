#ifndef D_STAGE_MGR_H
#define D_STAGE_MGR_H

#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_base.h"
#include "d/d_dylink.h"
#include "d/d_fader.h"
#include "d/d_bzs_types.h"
#include "egg/gfx/eggScreen.h"
#include "m/m_allocator.h"
#include "m/m_dvd.h"
#include "nw4r/g3d/g3d_camera.h"
#include "s/s_FPhase.h"
#include "s/s_State.hpp"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/time_area_mgr.h"

class RelObjCtrl {
public:
    dDynamicModuleControl mRelCtrl;
    ObjectArcControl mOarcCtrl;

    void doLoad();
    bool isLoaded() const;
};

class dStageMgr_c : public dBase_c {
public:
    dStageMgr_c();
    virtual ~dStageMgr_c();

    STATE_FUNC_DECLARE(dStageMgr_c, ReadStageRes);
    STATE_FUNC_DECLARE(dStageMgr_c, ReadRoomRes);
    STATE_FUNC_DECLARE(dStageMgr_c, ReadObjectRes);
    STATE_FUNC_DECLARE(dStageMgr_c, ReadStageLayerRes);
    STATE_FUNC_DECLARE(dStageMgr_c, ReadLayerObjectRes);
    STATE_FUNC_DECLARE(dStageMgr_c, SoundLoadSceneData);
    STATE_FUNC_DECLARE(dStageMgr_c, CreateObject);
    STATE_FUNC_DECLARE(dStageMgr_c, ReadObjectSound);
    STATE_FUNC_DECLARE(dStageMgr_c, SceneChangeSave);
    STATE_FUNC_DECLARE(dStageMgr_c, RestartSceneWait);
    STATE_FUNC_DECLARE(dStageMgr_c, RestartScene);

public:
    bool isAreaTypeDungeon() const;
    bool isAreaTypeSky() const;
    bool isAreaTypeHouse() const;
    bool isAreaTypeDungeonOrBoss() const;
    bool isAreaTypeOverworldOrSkyloft() const;
    s32 getSTIFunk1() const;

    s32 getSTIFbyte4() const;
    bool isSTIFbyte4_2or3or4() const;
    bool isSTIFbyte4_5or6() const;
    bool isSTIFbyte4_7or8or9or10or11() const;
    bool isSTIFbyte4_1() const {
        return getSTIFbyte4() == 1;
    }
    bool isSTIFbyte4_0() const {
        return getSTIFbyte4() == 0;
    }

    s32 getAreaType() const;
    s32 getMapNameId() const;

    bool fn_80199250();

    static dStageMgr_c *GetInstance() {
        return sInstance;
    }

    bool checkFlag0x88A0(u32 flag) const {
        return (field_0x88A0 & flag) != 0;
    }

    void initUnkWithWater(u32 val, d3d::UnkWithWater *waterThing);
    void destroyUnkWithWater(u32 val, d3d::UnkWithWater *waterThing);

    const RMPL *getRmpl() const {
        return mpRmpl;
    }

    u16 getRmplCount() const {
        return mRmplCount;
    }

    void setRmpl(const RMPL *pcam, u16 count) {
        mRmplCount = count;
        mpRmpl = pcam;
    }

    const PCAM *getPcam() const {
        return mpPcam;
    }

    u16 getPcamCount() const {
        return mPcamCount;
    }

    void setPcam(const PCAM *pcam, u16 count) {
        mPcamCount = count;
        mpPcam = pcam;
    }

    void setLyse(const LYSE *lyse, u16 count) {
        mLyseCount = count;
        mpLyse = lyse;
    }

    void overrideLayer0WithLyse();

    void setStif(const STIF *stif);
    s16 getFlagIndex() const;

    void setDemoName(const SizedString<16>& name) {
        mDemoName = name;
    }

    const char *getDemoName() const {
        return mDemoName;
    }

    void addLayerArcn(const char *path);
    void addStageArcn(const char *path);
    void addObjId(u16 id);
    void addActorId(u16 actorId);

    nw4r::g3d::Camera getCamera(s32 idx);

    dFader_c *getFader() {
        return &mFader;
    }

    bool isFaderSettled() const {
        return mFader.isSettled();
    }

    bool isInLastBoss() const {
        return mIsInLastBoss;
    }

private:
    static void lastExecuteCallback();

    void triggerFade(s32 fadeType, u8 fadeFrames);

    static dStageMgr_c *sInstance;
    static sFPhase<dStageMgr_c>::phaseCallback sCallbacks[];

    /* 0x0068 */ mHeapAllocator_c mAlloc;
    /* 0x0084 */ sFPhase<dStageMgr_c> mPhase;
    /* 0x0098 */ STATE_MGR_DECLARE(dStageMgr_c);
    /* 0x00D4 */ RelObjCtrl mStageObjCtrl;
    /* 0x0324 */ RelObjCtrl mLayerObjCtrl;
    /* 0x0574 */ LayoutArcControl mLayoutArcCtrl1;
    /* 0x0580 */ LayoutArcControl mLayoutArcCtrl2;
    /* 0x058C */ dBgS mBg;
    // ...
    u8 _0x00001[0x7D0];
    /* 0x3E40 */ dCcS mCc;
    // ...
    /* 0x7930 */ dFader_c mFader;
    
    /* 0x7954 */ u16 mPcamCount;
    /* 0x7956 */ u16 mLyseCount;
    /* 0x7958 */ u16 mRmplCount;
    
    /* 0x795C */ const PCAM *mpPcam;
    /* 0x7960 */ const LYSE *mpLyse;
    /* 0x7964 */ const STIF *mpStif;
    /* 0x7968 */ const RMPL *mpRmpl;
    
    /* 0x799C */ mDvd_callback_c *mpDvdCallback;
    /* 0x79A0 */ mDvd_callback_c *mpDvdCallback2;

    /* 0x79A4 */ EGG::Screen mScreens[2];

    /* 0x7AB4 */ AttentionManager mAttention;
    /* 0x8694 */ dTimeAreaMgr_c mTimeAreaMgr;

    // ...
    u8 ___[446];

    /* 0x88A0 */ u32 field_0x88A0;
    /* 0x88A4 */ SizedString<16> mDemoName;
    /* 0x88B4 */ u8 _0x88B4[0x88BC - 0x88B4];
    /* 0x88BC */ bool field_0x88BC;
    /* 0x88BD */ u8 field_0x88BD;
    /* 0x88BE */ u8 field_0x88BE;
    /* 0x88BF */ u8 field_0x88BF;
    /* 0x88C0 */ u8 field_0x88C0;
    /* 0x88C1 */ bool mIsInLastBoss;
};
#endif
