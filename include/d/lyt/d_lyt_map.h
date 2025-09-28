#ifndef D_LYT_MAP_H
#define D_LYT_MAP_H

#include "common.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_message.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_map_capture.h"
#include "d/lyt/d_lyt_map_global.h"
#include "egg/core/eggColorFader.h"
#include "m/m2d.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_flow_mgr.h"

struct dMapFootPrintEntry {
    /* 0x00 */ dMapFootPrintEntry *pPrev;
    /* 0x04 */ dMapFootPrintEntry *pNext;
    /* 0x08 */ mVec3_c position;
    /* 0x14 */ mAng rotation;
};

/** Generic footprints queue */
class dMapFootPrintsQueue_c {
public:
    void init();
    void addStep(const mVec3_c &pos, const mAng &rot);
    dMapFootPrintEntry *getFirst() const;
    dMapFootPrintEntry *getNext(const dMapFootPrintEntry *it) const;

    bool isEmpty() const {
        return mCount == 0;
    }

private:
    /* 0x000 */ dMapFootPrintEntry mEntries[40];
    /* 0x3C0 */ dMapFootPrintEntry *mpFirst;
    /* 0x3C4 */ dMapFootPrintEntry *mpLast;
    /* 0x3C8 */ u32 mCount;
};

/** Records Link's footsteps */
class dMapFootPrintsMgr_c {
public:
    dMapFootPrintsMgr_c() {
        mQueue.init();
    }
    virtual ~dMapFootPrintsMgr_c() {}

    void execute();

    const dMapFootPrintsQueue_c *getQueue() const {
        return &mQueue;
    }

private:
    /* 0x004 */ dMapFootPrintsQueue_c mQueue;
    /* 0x3D0 */ f32 mMinStepDistanceSq;
};

/** 2D UI - Map - Link footprint markers */
class dLytMapFootPrints_c {
public:
    dLytMapFootPrints_c(dLytMapGlobal_c *global);
    virtual ~dLytMapFootPrints_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void draw();

private:
    dLytMapGlobal_c *getGlobal() const;

    /* 0x004 */ dLytMapGlobal_c *mpGlobal;
    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ nw4r::lyt::Pane *mpPane;
    /* 0x0A0 */ f32 field_0xA0;
    /* 0x0A4 */ mVec2_c mFootprintPositions[30];
    /* 0x194 */ u16 mMaxNumSteps;
    /* 0x196 */ u16 mCurrentNumSteps;
    /* 0x198 */ f32 field_0x198;
    /* 0x19C */ f32 field_0x19C;
    /* 0x1A0 */ u8 field_0x1A0;
};

struct LytMap0x80520B5C {
    LytMap0x80520B5C() : field_0x04(false), field_0x05(false) {}
    virtual ~LytMap0x80520B5C() {}

    bool field_0x04;
    bool field_0x05;
};

struct LytMapFourAnimGroups {
    /* 0x00 */ d2d::AnmGroup_c mGroup1;
    /* 0x40 */ d2d::AnmGroup_c mGroup2;
    /* 0x80 */ d2d::AnmGroup_c mGroup3;
    /* 0xC0 */ d2d::AnmGroup_c mGroup4;
};

// Size 0x1E0
class dLytMapPinIcon_c {
public:
    friend class dLytMapPinIconAggregate_c;

    dLytMapPinIcon_c()
        : mStateMgr(*this, sStateID::null),
          mpBounding(nullptr),
          field_0x1BC(0),
          field_0x1C0(0.0f, 0.0f, 0.0f),
          field_0x1CC(0),
          field_0x1D0(nullptr),
          field_0x1D4(0),
          mIndex(0),
          field_0x1DC(0) {}
    virtual ~dLytMapPinIcon_c();

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool draw();
    bool execute();

    void fn_8012EC30();

    bool isSelect() const {
        return *mStateMgr.getStateID() == dLytMapPinIcon_c::StateID_ToSelect ||
             *mStateMgr.getStateID() == dLytMapPinIcon_c::StateID_Select;
    }

    bool isRemove() const {
        return *mStateMgr.getStateID() == dLytMapPinIcon_c::StateID_Remove;
    }

    STATE_FUNC_DECLARE(dLytMapPinIcon_c, Wait);
    STATE_FUNC_DECLARE(dLytMapPinIcon_c, ToSelect);
    STATE_FUNC_DECLARE(dLytMapPinIcon_c, Select);
    STATE_FUNC_DECLARE(dLytMapPinIcon_c, ToUnselect);
    STATE_FUNC_DECLARE(dLytMapPinIcon_c, Remove);

private:
    void removeBeacon();

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMapPinIcon_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x190 */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x1B8 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x1BC */ UNKWORD field_0x1BC;
    /* 0x1C0 */ mVec3_c field_0x1C0;
    /* 0x1CC */ u8 field_0x1CC;
    /* 0x1D0 */ LytMap0x80520B5C *field_0x1D0;
    /* 0x1D4 */ UNKWORD field_0x1D4;
    /* 0x1D8 */ s32 mIndex;
    /* 0x1DC */ UNKWORD field_0x1DC;
};

class dLytMapPinIconAggregate_c {
public:
    dLytMapPinIconAggregate_c() : mStateMgr(*this, sStateID::null) {
        field_0x9A0 = nullptr;
        field_0x9A4 = 0;
        field_0x9A8 = 0;
        for (int i = 0; i < 5; i++) {
            mPins[i].mIndex = i;
        }
    }
    virtual ~dLytMapPinIconAggregate_c() {}

    bool remove();

    STATE_FUNC_DECLARE(dLytMapPinIconAggregate_c, Wait);
    STATE_FUNC_DECLARE(dLytMapPinIconAggregate_c, Select);
    STATE_FUNC_DECLARE(dLytMapPinIconAggregate_c, Remove);

private:
    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMapPinIconAggregate_c);
    /* 0x040 */ dLytMapPinIcon_c mPins[5];

    /* 0x9A0 */ LytMap0x80520B5C *field_0x9A0;
    /* 0x9A4 */ UNKWORD field_0x9A4;
    /* 0x9A8 */ UNKWORD field_0x9A8;
};

// Size 0x50
class dLytMapFloorBtn_c {
public:
    dLytMapFloorBtn_c()
        : mStateMgr(*this, sStateID::null),
          field_0x3C(0),
          field_0x40(0),
          field_0x44(0),
          field_0x48(0),
          field_0x4C(0),
          field_0x4D(0) {}
    ~dLytMapFloorBtn_c() {}

    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, ToSelect);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, Select);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, ToWait);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, ToDecide);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, Decide);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, ToSelectInDecide);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, SelectInDecide);
    STATE_FUNC_DECLARE(dLytMapFloorBtn_c, DecideToSelectInDecide);

private:
    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMapFloorBtn_c);
    /* 0x3C */ UNKWORD field_0x3C;
    /* 0x40 */ UNKWORD field_0x40;
    /* 0x44 */ UNKWORD field_0x44;
    /* 0x48 */ UNKWORD field_0x48;
    /* 0x4C */ u8 field_0x4C;
    /* 0x4D */ u8 field_0x4D;
};

class dLytMapFloorBtnMgr_c : public d2d::dSubPane {
public:
    dLytMapFloorBtnMgr_c(dLytMapGlobal_c *global)
        : mpGlobal(global), field_0x51C(nullptr), mStateMgr(*this, sStateID::null) {
        field_0x700 = 1;
        field_0x704 = 0;
        field_0x708 = 0;
        field_0x70C = 4;
        field_0x710 = 0;
        field_0x711 = 0;
        field_0x712 = 0;
    }
    virtual ~dLytMapFloorBtnMgr_c();

    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    STATE_FUNC_DECLARE(dLytMapFloorBtnMgr_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapFloorBtnMgr_c, Wait);

private:
    /* 0x008 */ dLytMapGlobal_c *mpGlobal;
    /* 0x00C */ UI_STATE_MGR_DECLARE(dLytMapFloorBtnMgr_c);
    /* 0x048 */ d2d::dLytSub mLyt;
    /* 0x0DC */ d2d::AnmGroup_c mAnmGroups[1];
    /* 0x11C */ LytMapFourAnimGroups mMoreGroups[4];
    /* 0x51C */ void *field_0x51C;
    /* 0x520 */ dCursorHitCheckLyt_c mCsHitChecks[4];
    /* 0x5C0 */ dLytMapFloorBtn_c mFloorBtns[4];
    /* 0x700 */ UNKWORD field_0x700;
    /* 0x704 */ UNKWORD field_0x704;
    /* 0x708 */ UNKWORD field_0x708;
    /* 0x70C */ UNKWORD field_0x70C;
    /* 0x710 */ u8 field_0x710;
    /* 0x711 */ u8 field_0x711;
    /* 0x712 */ u8 field_0x712;
};

// Size 0x4C
class dLytMapPopupInfo_c {
public:
    dLytMapPopupInfo_c() : mStateMgr(*this, sStateID::null) {}

    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, In);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, Wait);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, WaitInvalid);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, Out);

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMapPopupInfo_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnmGroups[3];
};

// Size 0x4C
class dLytMapSavePopupAction_c {
public:
    dLytMapSavePopupAction_c() : mStateMgr(*this, sStateID::null), field_0x44(0.0f), field_0x48(0) {}

    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, In);
    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, Wait);
    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, Out);

private:
    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMapSavePopupAction_c);
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ u8 field_0x48;
};

// Assumed name
class dLytMapSavePopup_c {
public:
    dLytMapSavePopup_c() {}

private:
    /* 0x000 */ d2d::LytBase_c mLyt;
    /* 0x090 */ d2d::AnmGroup_c mAnmGroups[2];
    /* 0x110 */ dLytMapSavePopupAction_c mActions[12];
};

class dLytMapSaveCaption_c {
public:
    dLytMapSaveCaption_c() : mStateMgr(*this, sStateID::null) {}

    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, In);
    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, Wait);
    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, Out);

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMapSaveCaption_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnmGroups[5];
};

// Size 0x190
class dLytMapSaveObj_c {
public:
    dLytMapSaveObj_c() : mStateMgr(*this, sStateID::null), field_0x18C(0), field_0x18D(0), field_0x18E(0) {}

    STATE_FUNC_DECLARE(dLytMapSaveObj_c, Wait);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, ToSelect);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, Select);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, ToWait);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, Decide);

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMapSaveObj_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x18C */ u8 field_0x18C;
    /* 0x18D */ u8 field_0x18D;
    /* 0x18E */ u8 field_0x18E;
};

class dLytMapDecoration_c {
public:
    dLytMapDecoration_c() {}
    virtual ~dLytMapDecoration_c() {}
    // TODO, name made up
};

class dLytMapPutIcon {
public:
    dLytMapPutIcon() {}
    virtual ~dLytMapPutIcon() {}
    // TODO, name made up

    /* 0x04 */ d2d::LytBase_c mLyt;
    /* 0x98 */ mVec2_c field_0x98;
};

class dLytMapMain_c : public m2d::Base_c {
    friend class dLytMap_c;

public:
    dLytMapMain_c();
    virtual ~dLytMapMain_c();
    virtual void draw() override;
    virtual void dLytMapMain_vt0x10();

    void build();

    bool isNotInvisible() const;
    bool isOpenMaybe() const;
    bool fn_80139EA0() const;
    void lightPillarRelated(s32, s32, s32);

    STATE_FUNC_DECLARE(dLytMapMain_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapMain_c, RenderingWait);
    STATE_FUNC_DECLARE(dLytMapMain_c, In);
    STATE_FUNC_DECLARE(dLytMapMain_c, Active);
    STATE_FUNC_DECLARE(dLytMapMain_c, Out);
    STATE_FUNC_DECLARE(dLytMapMain_c, FloorChange);
    STATE_FUNC_DECLARE(dLytMapMain_c, ChgMapUpDirection);
    STATE_FUNC_DECLARE(dLytMapMain_c, ChgDispAreaMode_Map);
    STATE_FUNC_DECLARE(dLytMapMain_c, ChgDispAreaMode_MapRot);
    STATE_FUNC_DECLARE(dLytMapMain_c, ChgDispAreaMode_WA);
    STATE_FUNC_DECLARE(dLytMapMain_c, ChgDispAreaMode_CsrRot);
    STATE_FUNC_DECLARE(dLytMapMain_c, ResetPos);
    STATE_FUNC_DECLARE(dLytMapMain_c, ResetPosWithFloorChange);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSwBankSmall_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSwBankSmall_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSwBankSmall_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventMapIntro_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventMapIntro_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventMapIntro_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventMapIntro_Step4);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventDungeonMapGet_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventDungeonMapGet_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventDungeonMapGet_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventFieldMapChange_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventFieldMapChange_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventFieldMapChange_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventFieldMapChange_Step4);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventForestMapChange_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventForestMapChange_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventForestMapChange_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSignalAdd_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSignalAdd_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSignalAdd_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventGoddessCube_Step1);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventGoddessCube_Step2);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventGoddessCube_Step3);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSaveObjMsgWindow);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSaveObjSelect);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSaveObjConfirmMsgWindow);
    STATE_FUNC_DECLARE(dLytMapMain_c, EventSaveObjDecide);

private:
    /* 0x0010 */ UI_STATE_MGR_DECLARE(dLytMapMain_c);
    /* 0x004C */ dFlowMgr_c mFlowMgr;
    /* 0x0058 */ u8 _0x0058[0x00A4 - 0x0058];
    /* 0x00A4 */ dFlow_c mFlow;
    /* 0x0108 */ u8 _0x0108[0x010C - 0x0108];
    /* 0x010C */ d2d::LytBase_c mLyt;
    /* 0x019C */ d2d::AnmGroup_c mAnmGroups[54];
    /* 0x0F1C */ LytMap0x80520B5C field_0xF1C;
    /* 0x0F24 */ dLytMapCapture_c mMapCapture;
    /* 0x0FA0 */ dLytMapFloorBtnMgr_c mFloorBtnMgr;
    /* 0x16B4 */ dLytMapPinIconAggregate_c mPinIconAggregate;

    /* 0x2060 */ dLytMapDecoration_c field_0x2060;

    /* 0x2064 */ u8 _0x2064[0x64C0 - 0x2064];

    /* 0x64C0 */ dLytMapFootPrints_c mFootPrints;

    /* 0x6664 */ dLytMapPutIcon mPutIcon;

    /* 0x6700 */ u8 _0x6700[0x6704 - 0x6700];

    /* 0x6704 */ dLytMapSaveObj_c mSaveObjs[12];
    /* 0x79C4 */ dLytMapSaveCaption_c mSaveCaption;
    /* 0x7BD0 */ dLytMapSavePopup_c mSavePopup;

    /* 0x8010 */ u8 _0x7F84[0x807C - 0x8010];

    /* 0x807C */ dLytMapPopupInfo_c mPopupInfo;

    /* 0x8208 */ u8 _0x8208[0x828C - 0x8208];

    /* 0x828C */ mVec3_c field_0x828C[12];

    /* 0x831C */ u8 _0x831C[0x832C - 0x831C];

    /* 0x832C */ dCursorHitCheckLyt_c field_0x832C[33];

    /* 0x8854 */ u8 _0x8854[0x8904 - 0x8854];

    /* 0x8904 */ mVec3_c field_0x8904;
    /* 0x8910 */ mVec3_c field_0x8910;
    /* 0x891C */ mVec3_c field_0x891C;
    /* 0x8928 */ mVec2_c field_0x8928;
    /* 0x8930 */ mVec3_c field_0x8930;
    /* 0x893C */ mVec3_c field_0x893C;

    /* 0x8948 */ u8 _0x8948[0x8C94 - 0x8948];
    
    /* 0x8C94 */ s32 field_0x8C94;

    /* 0x8C98 */ u8 _0x8C98[0x8CC4 - 0x8C98];

    /* 0x8CC4 */ mVec3_c field_0x8CC4;
    /* 0x8CD0 */ mVec3_c field_0x8CD0;
    /* 0x8CDC */ mVec3_c field_0x8CDC;
    /* 0x8CE8 */ mVec3_c field_0x8CE8;
    /* 0x8CF4 */ mVec3_c field_0x8CF4;
    /* 0x8D00 */ mVec3_c field_0x8D00;
    /* 0x8D0C */ mVec3_c field_0x8D0C;
    /* 0x8D18 */ mVec3_c field_0x8D18;
    /* 0x8D24 */ mVec3_c field_0x8D24;
    /* 0x8D30 */ mVec2_c field_0x8D30;
    /* 0x8D38 */ mVec2_c field_0x8D38;
    /* 0x8D40 */ f32 field_0x8D40;
    /* 0x8D44 */ mAng field_0x8D44;
    /* 0x8D46 */ mAng field_0x8D46;
    /* 0x8D48 */ mAng field_0x8D48;
    /* 0x8D4C */ f32 field_0x8D4C;
    /* 0x8D50 */ f32 field_0x8D50;
    /* 0x8D54 */ u8 _0x8D54[0x8D58 - 0x8D54];
    /* 0x8D58 */ UNKWORD field_0x8D58;
    /* 0x8D5C */ UNKWORD field_0x8D5C;
    /* 0x8D60 */ UNKWORD field_0x8D60;
    /* 0x8D64 */ UNKWORD field_0x8D64;
    /* 0x8D68 */ mAng field_0x8D68;
    /* 0x8D6A */ u8 field_0x8D6A;
    /* 0x8D6B */ u8 field_0x8D6B;
    /* 0x8D6C */ UNKWORD field_0x8D6C;
    /* 0x8D70 */ UNKWORD field_0x8D70;
    /* 0x8D74 */ u8 _0x8D74[0x8D94 - 0x8D74];
    /* 0x8D94 */ d2d::SubPaneList mSubpaneList;
    /* 0x8DA0 */ d2d::SubPaneListNode mSubpane;
    /* 0x8DB0 */ u8 _0x8DB0[0x8DC8 - 0x8DB0];
    /* 0x8DC8 */ dLytMapGlobal_c mGlobal;
};

// Made up name
class dLytMapFader_c : public m2d::Base_c {
public:
    dLytMapFader_c();
    virtual ~dLytMapFader_c();
    /* vt 0x0C */ virtual void draw() override;

    void calc();

private:
    /* 0x10 */ EGG::ColorFader mFader;
};

// Size 0x91FC, inline ctor at 802ccd88
class dLytMap_c {
public:
    dLytMap_c() {
        sInstance = this;
    }
    virtual ~dLytMap_c() {
        sInstance = nullptr;
    }

    static d2d::ResAccIf_c *getResAcc() {
        return &sInstance->mResAcc;
    }

    static dLytMap_c *GetInstance() {
        return sInstance;
    }

    bool isNotInvisible() const {
        return mMapMain.isNotInvisible();
    }

    bool isOpenMaybe() const {
        return mMapMain.isOpenMaybe();
    }

    bool getFn_80139EA0() const {
        return mMapMain.fn_80139EA0();
    }

    void build();

    bool isSomeMapFieldEq2Or4Or5Or6() const {
        return mMapMain.field_0x8C94 == 2 || mMapMain.field_0x8C94 == 4 || mMapMain.field_0x8C94 == 5 ||
               mMapMain.field_0x8C94 == 6;
    }

    bool isSomeMapFieldEq10() const {
        return mMapMain.field_0x8C94 == 10;
    }

    void lightPillarRelated(s32 p1, s32 p2, s32 p3) {
        mMapMain.lightPillarRelated(p1, p2, p3);
    }

private:
    /* 0x0004 */ d2d::ResAccIf_c mResAcc;
    /* 0x0374 */ dLytMapMain_c mMapMain;

    static dLytMap_c *sInstance;
};

#endif
