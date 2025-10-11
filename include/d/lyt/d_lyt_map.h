#ifndef D_LYT_MAP_H
#define D_LYT_MAP_H

#include "common.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_message.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_map_capture.h"
#include "d/lyt/d_lyt_map_global.h"
#include "d/lyt/d_lyt_map_markers.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "egg/core/eggColorFader.h"
#include "m/m2d.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_flow_mgr.h"

class dAcTbox_c;

/** Bird Statue Definition (StatueSelectDestination) */
struct dMapSaveObjDefinition {
    /* 0x00 */ const char *stageName;
    /* 0x04 */ u8 room;
    /* 0x05 */ u8 layer;
    /* 0x06 */ u8 entrance;
    /* 0x08 */ s32 type;
    /* 0x0C */ const char *statueLabel;
};

/** Something for making sure re-opening the map opens it in a similar state as when it was last closed */
struct dMapSavedDataEntry {
    /* 0x00 */ s32 mapMode;
    /* 0x04 */ bool mapUpDirection;
    /* 0x05 */ bool mapUpDirectionAfterZoomToDetail;
    /* 0x06 */ u8 field_0x06;
};

struct dMapSavedData {
    /* 0x00 */ dMapSavedDataEntry entries[/* dLytMapMain_c::ROOMTYPE_MAX */ 6];
    /* 0x30 */ bool showIslandNames;
};

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

    void setVisible(bool visible) {
        mVisible = visible;
    }

    void setField_0x198(f32 f) {
        field_0x198 = f;
    }

    void setField_0x19C(f32 f) {
        field_0x19C = f;
    }

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
    /* 0x1A0 */ bool mVisible;
};

struct LytMap0x80520B5C {
    LytMap0x80520B5C() : field_0x04(false), field_0x05(false) {}
    virtual ~LytMap0x80520B5C() {}

    bool field_0x04;
    bool field_0x05;
};

struct dLytMapFloorBtnAnmGroups {
    /* 0x00 */ d2d::AnmGroup_c mLoop;
    /* 0x40 */ d2d::AnmGroup_c mOnOff;
    /* 0x80 */ d2d::AnmGroup_c mDecide;
    /* 0xC0 */ d2d::AnmGroup_c mOnOffLight;
};

struct dLytMapBoundingWorldBounds {
    /* 0x00 */ mVec3_c min;
    /* 0x0C */ mVec3_c max;
};

/** 2D UI - Map - a placed down beacon */
class dLytMapPinIcon_c {
public:
    friend class dLytMapPinIconAggregate_c;

    dLytMapPinIcon_c()
        : mStateMgr(*this, sStateID::null),
          mpBounding(nullptr),
          field_0x1BC(0),
          mPosition(0.0f, 0.0f, 0.0f),
          mIsSet(false),
          field_0x1D0(nullptr),
          field_0x1D4(0),
          mIndex(0),
          mLoopFrame(0) {}
    virtual ~dLytMapPinIcon_c();

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool draw();
    bool execute();

    void updatePosition();
    void checkPointedAt();
    void setPosition(const mVec3_c &pos);
    void setScale(f32 scale);

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

    dLytMapGlobal_c *getGlobal() const;

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMapPinIcon_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x190 */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x1B8 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x1BC */ UNKWORD field_0x1BC;
    /* 0x1C0 */ mVec3_c mPosition;
    /* 0x1CC */ bool mIsSet;
    /* 0x1D0 */ LytMap0x80520B5C *field_0x1D0;
    /* 0x1D4 */ UNKWORD field_0x1D4;
    /* 0x1D8 */ s32 mIndex;
    /* 0x1DC */ UNKWORD mLoopFrame;
};

class dLytMapPinIconAggregate_c {
    friend class dLytMapMain_c;

public:
    dLytMapPinIconAggregate_c() : mStateMgr(*this, sStateID::null) {
        field_0x9A0 = nullptr;
        mLoopFrame = 0;
        mLoopFrameMax = 0;
        for (int i = 0; i < 5; i++) {
            mPins[i].mIndex = i;
        }
    }
    virtual ~dLytMapPinIconAggregate_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    void setScale(f32 scale);
    void updatePosition();
    void setUnk(LytMap0x80520B5C *unk);
    bool setPosition(s32 index, const mVec3_c &position);
    void unsetAll();
    u32 getNumSetPins() const;

    STATE_FUNC_DECLARE(dLytMapPinIconAggregate_c, Wait);
    STATE_FUNC_DECLARE(dLytMapPinIconAggregate_c, Select);
    STATE_FUNC_DECLARE(dLytMapPinIconAggregate_c, Remove);

private:
    bool findNewBeaconId(s32 *pOutId);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMapPinIconAggregate_c);
    /* 0x040 */ dLytMapPinIcon_c mPins[5];

    /* 0x9A0 */ LytMap0x80520B5C *field_0x9A0;
    /* 0x9A4 */ s32 mLoopFrame;
    /* 0x9A8 */ s32 mLoopFrameMax;
};

// Size 0x50
class dLytMapFloorBtn_c {
    friend class dLytMapFloorBtnMgr_c;

public:
    dLytMapFloorBtn_c()
        : mStateMgr(*this, sStateID::null),
          mpOwnerLyt(nullptr),
          mpAnmGroups(nullptr),
          field_0x44(0),
          mpBounding(nullptr),
          mSelected(false),
          mDecided(false) {}
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

    void init();
    void execute();

    bool build(d2d::ResAccIf_c *resAcc);

    void directlyDecide();
    void directlyUndecide();

private:
    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMapFloorBtn_c);
    /* 0x3C */ d2d::LytBase_c *mpOwnerLyt;
    /* 0x40 */ dLytMapFloorBtnAnmGroups *mpAnmGroups;
    /* 0x44 */ UNKWORD field_0x44;
    /* 0x48 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x4C */ bool mSelected;
    /* 0x4D */ bool mDecided;
};

class dLytMapFloorBtnMgr_c : public d2d::dSubPane {
public:
    dLytMapFloorBtnMgr_c(dLytMapGlobal_c *global)
        : mpGlobal(global), mpPane(nullptr), mStateMgr(*this, sStateID::null) {
        mNumFloors = 1;
        mBaseFloorOffset = 0;
        mSelectedBtnIdx = 0;
        field_0x70C = 4;
        mPointerVisible = false;
        mPrevPointerVisible = false;
        mForceNoNav = false;
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

    void init(s32 numFloors, s32 baseFloorOffset, s32 selectedFloor);

    bool canDecideFloor() const;
    void decideSelectedFloor();
    s32 getCurrentFloor() const;
    s32 getPointedAtBtnIdx() const;

    bool hasPointedAtABtnIdx() const {
        return getPointedAtBtnIdx() >= 0;
    }

    void resetFloor(s32 newFloorBtn);
    bool isUsingPointerNav() const;

    void setForceNoNav(bool b) {
        mForceNoNav = b;
    }

    void checkPointedAtBtn();
private:

    /* 0x008 */ dLytMapGlobal_c *mpGlobal;
    /* 0x00C */ UI_STATE_MGR_DECLARE(dLytMapFloorBtnMgr_c);
    /* 0x048 */ d2d::dLytSub mLyt;
    /* 0x0DC */ d2d::AnmGroup_c mAnmGroups[1];
    /* 0x11C */ dLytMapFloorBtnAnmGroups mBtnGroups[4];
    /* 0x51C */ nw4r::lyt::Pane *mpPane;
    /* 0x520 */ dCursorHitCheckLyt_c mCsHitChecks[4];
    /* 0x5C0 */ dLytMapFloorBtn_c mFloorBtns[4];
    /* 0x700 */ s32 mNumFloors;
    /* 0x704 */ s32 mBaseFloorOffset;
    /* 0x708 */ s32 mSelectedBtnIdx;
    /* 0x70C */ UNKWORD field_0x70C;
    /* 0x710 */ bool mPointerVisible;
    /* 0x711 */ bool mPrevPointerVisible;
    /* 0x712 */ bool mForceNoNav;
};

// Size 0x4C
class dLytMapPopupInfo_c {
    friend class dLytMapMain_c;

public:
    dLytMapPopupInfo_c() : mStateMgr(*this, sStateID::null) {}

    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, In);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, Wait);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, WaitInvalid);
    STATE_FUNC_DECLARE(dLytMapPopupInfo_c, Out);

    void build(d2d::ResAccIf_c *resAcc);
    void remove();
    void execute();
    void draw();

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMapPopupInfo_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnmGroups[3];
};

// Size 0x4C
class dLytMapSavePopupAction_c {
    friend class dLytMapSavePopup_c;

public:
    dLytMapSavePopupAction_c() : mStateMgr(*this, sStateID::null), mInOutFrame(0.0f), mInRequest(false) {}

    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, In);
    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, Wait);
    STATE_FUNC_DECLARE(dLytMapSavePopupAction_c, Out);

    void execute();
    void init(d2d::AnmGroup_c *pGroups);
    void hide();

private:
    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMapSavePopupAction_c);
    /* 0x3C */ d2d::AnmGroup_c *mpAnmGroupInOut;
    /* 0x40 */ d2d::AnmGroup_c *mpAnmGroupInput;
    /* 0x44 */ f32 mInOutFrame;
    /* 0x48 */ bool mInRequest;
};

// Assumed name
class dLytMapSavePopup_c {
public:
    dLytMapSavePopup_c() : mStatueNum(0), mpStatueBoundings(nullptr), mCurrentlyInStatue(-1) {}

    void build(d2d::ResAccIf_c *resAcc);
    void remove();
    void execute();
    void draw();

    void set(nw4r::lyt::Bounding **pStatueBoundings, s32 count);
    void hide(s32 statueIdx);

    void setCurrentStatue(s32 idx) {
        mCurrentlyInStatue = idx;
    }

private:
    f32 setStatueLabel(const char *label);

    /* 0x000 */ d2d::LytBase_c mLyt;
    /* 0x090 */ d2d::AnmGroup_c mAnmGroups[2];
    /* 0x110 */ dLytMapSavePopupAction_c mActions[12];
    /* 0x4A0 */ s32 mStatueNum;
    /* 0x4A4 */ nw4r::lyt::Bounding **mpStatueBoundings;
    /* 0x4A8 */ s32 mCurrentlyInStatue;
};

class dLytMapSaveCaption_c {
    friend class dLytMapMain_c;

public:
    dLytMapSaveCaption_c() : mStateMgr(*this, sStateID::null) {}

    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, Invisible);
    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, In);
    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, Wait);
    STATE_FUNC_DECLARE(dLytMapSaveCaption_c, Out);

    void build(d2d::ResAccIf_c *resAcc);
    void remove();
    void execute();
    void draw();

    void setType(s32 type);
    void setLabel(const char *label);

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMapSaveCaption_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnmGroups[5];
};

// Size 0x190
class dLytMapSaveObj_c {
    friend class dLytMapMain_c;

public:
    dLytMapSaveObj_c()
        : mStateMgr(*this, sStateID::null), mSelectRequest(false), mDecideRequest(false), mDecideFinished(false) {}

    STATE_FUNC_DECLARE(dLytMapSaveObj_c, Wait);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, ToSelect);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, Select);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, ToWait);
    STATE_FUNC_DECLARE(dLytMapSaveObj_c, Decide);

    void build(d2d::ResAccIf_c *resAcc);
    void remove();
    void execute();
    void draw();

    void setPosition(const mVec3_c &position);
    void setAlpha(u8 alpha);
    void init();

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMapSaveObj_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x18C */ bool mSelectRequest;
    /* 0x18D */ bool mDecideRequest;
    /* 0x18E */ bool mDecideFinished;
};

/** 2D UI - Map - beacon preview icon following the cursor */
class dLytMapPutIcon_c {
public:
    dLytMapPutIcon_c() : mVisible(false), field_0x98(0.0f, 0.0f) {}
    virtual ~dLytMapPutIcon_c() {}

    void build(d2d::ResAccIf_c *resAcc);
    void remove();
    void execute();
    void draw();

    void setPosition(const mVec2_c &pos);
    void setScale(f32 scale);

    void setVisible(bool visible) {
        mVisible = visible;
    }

private:
    /* 0x04 */ d2d::LytBase_c mLyt;
    /* 0x94 */ bool mVisible;
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
    void remove();
    void execute();

    bool isNotInvisible() const;
    bool isOpenMaybe() const;
    bool fn_80139EA0() const;
    void lightPillarRelated(s32, s32, s32);

    const dMapSaveObjDefinition *getSaveObjDefinition(s32 province, s32 statueIdx) const;

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
    // TODO - need to come up with better names for all of these enums and concepts

    enum SurfaceProvince_e {
        SURFACE_PROVINCE_FARON = 0,
        SURFACE_PROVINCE_ELDIN = 1,
        SURFACE_PROVINCE_LANAYRU = 2,
    };

    enum AreaGroup_e {
        AREAGROUP_SKY = 1,
        AREAGROUP_FARON = 2,
        AREAGROUP_ELDIN = 3,
        AREAGROUP_LANAYRU = 4,
        AREAGROUP_SEALED_GROUNDS = 5,
        AREAGROUP_MAX = 6,
    };

    enum RoomType_e {
        ROOMTYPE_FIELD = 0,
        ROOMTYPE_DUNGEON = 1,
        ROOMTYPE_SKYLOFT = 2,
        ROOMTYPE_BOSS_HOUSE = 3,
        ROOMTYPE_SKYFIELD = 4,
        ROOMTYPE_SKYFIELD_INTERIOR = 5,
        ROOMTYPE_MAX = 6,
    };

    enum MapEvent_e {
        MAP_EVENT_NONE = 0,
        MAP_EVENT_1 = 1,
        // ???
        MAP_EVENT_SW_BANK_SMALL = 2,
        MAP_EVENT_MAP_INTRO = 3,
        MAP_EVENT_DUNGEON_MAP_GET = 4,
        MAP_EVENT_FIELD_MAP_CHANGE_5 = 5,
        MAP_EVENT_FOREST_MAP_CHANGE = 6,
        MAP_EVENT_SIGNAL_ADD = 7,
        MAP_EVENT_FIELD_MAP_CHANGE_8 = 8,
        MAP_EVENT_GODDESS_CUBE = 9,
        MAP_EVENT_SAVE_OBJ_MSG_WINDOW = 10,
        MAP_EVENT_11 = 11,
    };

    dLytMapGlobal_c *getGlobal();
    void checkScroll();
    bool needsNav(s32 mapMode) const;
    bool canZoomOut(s32 mapMode) const;
    bool canZoomIn(s32 mapMode) const;
    bool canResetPosition(s32 mapMode, bool) const;
    bool canChangeUpDirection(s32 mapMode, bool) const;
    bool canCenterCursor(s32 mapMode) const;
    bool canCenterCursor1(s32 mapMode) const;
    bool canPlaceBeacons(s32 mapMode) const;
    bool canChangeFloor(s32 mapMode) const;
    bool isPointingAtMainMap() const;

    void loadStageProperties();
    s32 getAreaGroup(s32 stifArea) const;
    s32 getRoomType() const;

    s32 getSelectedSaveObjIdx() const;

    f32 fn_80142D90(s32);
    void fn_80142F00(mVec3_c &, s32 mapMode, u8, const mVec3_c &, const mAng &);
    void fn_8013FB70(const mVec3_c &, f32);
    bool fn_80141530() const;
    bool fn_80142D10(s32, bool, mAng &);
    void fn_80143060(mVec3_c &, const mVec3_c &, const mVec3_c &, const mAng &);
    void fn_80140B90();
    void fn_80143300();
    void fn_80143120(s32);
    void fn_801431E0();
    void fn_8013AD50();
    void fn_80143360();
    bool shouldDrawFootprints() const;

    void zoomIn();
    void zoomOut();

    void clearButtonMessages();
    void setButtonMessages(s32 currentMapMode, bool currentUpDirection, s32 nextMapMode, bool nextUpDirection);
    void setCursorType();

    void setupFlags();
    void setupStage();

    void loadTextboxes();

    void saveUnkMapData();
    void initUnkMapData();
    void loadUnkMapData();

    void checkCursorPointedAtMap();

    void calculateBoundingWorldHitboxes();

    dAcTbox_c *findGoddessChestForStoryflag(s32 flag) const;
    bool checkStoryflag(s32 flag) const;
    void displaySaveObjs();
    bool isPaneVisible(nw4r::lyt::Pane *pane) const;
    void setSaveObjsVisible(bool visible);
    void setSaveObjPanePtrs();
    void initSaveObjs();

    static dMapSavedData sSavedMapData;
    static const dMapSavedData sDefaultMapData;

    bool isSomeFieldEq0Or1Or7Or9Or11() const {
        return mMapEvent == MAP_EVENT_NONE || mMapEvent == MAP_EVENT_1 || mMapEvent == MAP_EVENT_SIGNAL_ADD ||
               mMapEvent == MAP_EVENT_GODDESS_CUBE || mMapEvent == MAP_EVENT_11;
    }

    /* 0x0010 */ UI_STATE_MGR_DECLARE(dLytMapMain_c);
    /* 0x004C */ dFlowMgrBase_c mFlowMgr;
    /* 0x00A4 */ dFlow_c mFlow;
    /* 0x0108 */ UNKWORD field_0x0108;
    /* 0x010C */ d2d::LytBase_c mLyt;
    /* 0x019C */ d2d::AnmGroup_c mAnmGroups[54];
    /* 0x0F1C */ LytMap0x80520B5C field_0xF1C;
    /* 0x0F24 */ dLytMapCapture_c mMapCapture;
    /* 0x0FA0 */ dLytMapFloorBtnMgr_c mFloorBtnMgr;
    /* 0x16B4 */ dLytMapPinIconAggregate_c mPinIconAggregate;
    /* 0x2060 */ dLytMapMarkers_c mMarkers;
    /* 0x64C0 */ dLytMapFootPrints_c mFootPrints;
    /* 0x6664 */ dLytMapPutIcon_c mPutIcon;
    /* 0x6704 */ dLytMapSaveObj_c mSaveObjs[12];
    /* 0x79C4 */ dLytMapSaveCaption_c mSaveCaption;
    /* 0x7BD0 */ dLytMapSavePopup_c mSavePopup;
    /* 0x807C */ dLytMapPopupInfo_c mPopupInfo;
    /* 0x8208 */ dTextBox_c *mpNumberTextBox;
    /* 0x820C */ dTextBox_c *mpNumberTextBoxS;
    /* 0x8210 */ nw4r::lyt::Pane *mpNoroshiPane;
    /* 0x8214 */ nw4r::lyt::Pane *mpScaleFramePane;
    /* 0x8218 */ dWindow_c *mpWakuWindow;
    /* 0x821C */ nw4r::lyt::Bounding *mpSaveObjBoundingsFaron[10];
    /* 0x8244 */ nw4r::lyt::Bounding *mpSaveObjBoundingsEldin[6];
    /* 0x825C */ nw4r::lyt::Bounding *mpSaveObjBoundingsLanayru[12];
    /* 0x828C */ mVec3_c field_0x828C[12];
    /* 0x831C */ nw4r::lyt::Bounding **mpCurrentSaveObjBoundings;
    /* 0x8320 */ u32 mNumSaveObjs;

    /* 0x8324 */ u8 _0x8324[0x8328 - 0x8324];

    /* 0x8328 */ d2d::AnmGroup_c *mpOutAnmGroup;

    // TODO - it appears the map abuses these hit check things
    // to calculate Lyt bounding boxes, and it stores the
    // results in 0x8948...
    /* 0x832C */ dCursorHitCheckLyt_c mHitChecks[33];

    /* 0x8854 */ nw4r::lyt::Pane *mpPanes[11];
    /* 0x8880 */ d2d::AnmGroup_c *field_0x8880[11]; // ???
    /* 0x88AC */ UNKWORD field_0x88AC;
    /* 0x88B0 */ nw4r::lyt::Pane *mpDungeonPanes[7];
    /* 0x88CC */ nw4r::lyt::Pane *mpDungeonPics[7];

    /* 0x88E8 */ nw4r::lyt::Pane *mpPaneBgAll01;
    /* 0x88EC */ nw4r::lyt::Pane *mpPaneBgAll02;
    /* 0x88F0 */ nw4r::lyt::Pane *mpPaneAll01;
    /* 0x88F4 */ nw4r::lyt::Pane *mpPaneAll02;
    /* 0x88F8 */ nw4r::lyt::Pane *mpPaneRotate00;
    /* 0x88FC */ nw4r::lyt::Pane *mpPaneRotate01;
    /* 0x8900 */ nw4r::lyt::Pane *mpAllPane;
    /* 0x8904 */ mVec3_c field_0x8904;
    /* 0x8910 */ mVec3_c field_0x8910;
    /* 0x891C */ mVec3_c field_0x891C;
    /* 0x8928 */ mVec2_c field_0x8928;
    /* 0x8930 */ mVec3_c field_0x8930;
    /* 0x893C */ mVec3_c field_0x893C;

    /* 0x8948 */ dLytMapBoundingWorldBounds mBoundingWorldBounds[33];

    /* 0x8C60 */ s32 mMaxBeaconCount;
    /* 0x8C64 */ s32 field_0x8C64;
    /* 0x8C68 */ s32 mRoomType;
    /* 0x8C6C */ s32 mAreaGroup;
    /* 0x8C70 */ u32 field_0x8C70;

    /* 0x8C74 */ u8 _0x8C74[0x8C7C - 0x8C74];
    /* 0x8C7C */ f32 field_0x8C7C;
    /* 0x8C80 */ f32 field_0x8C80;

    /* 0x8C84 */ u8 _0x8C84[0x8C88 - 0x8C84];

    /* 0x8C88 */ f32 field_0x8C88;
    /* 0x8C8C */ f32 field_0x8C8C;
    /* 0x8C90 */ bool mMapUpDirection;
    /* 0x8C91 */ bool mNextMapUpDirection;
    /* 0x8C92 */ bool mMapUpDirectionAfterZoomToDetail;
    /* 0x8C93 */ u8 field_0x8C93;
    /* 0x8C94 */ s32 mMapEvent;
    /* 0x8C98 */ s32 mNextMapEvent;
    /* 0x8C9C */ s32 mSurfaceProvince;

    /* 0x8CA0 */ u8 _0x8CA0[0x8CA4 - 0x8CA0];

    /* 0x8CA4 */ s32 mCurrentMapMode;
    /* 0x8CA8 */ s32 mNextMapMode;

    /* 0x8CAC */ u8 field_0x8CAC;
    /* 0x8CAD */ u8 field_0x8CAD;

    /* 0x8CAE */ u8 _0x8CAE[0x8CB0 - 0x8CAE];

    /* 0x8CB0 */ u32 mEventTimer;
    /* 0x8CB4 */ UNKWORD field_0x8CB4;
    /* 0x8CB8 */ u32 field_0x8CB8;
    /* 0x8CBC */ nw4r::lyt::Bounding *mpMapBounding;
    /* 0x8CC0 */ bool mPointerOnMap;
    /* 0x8CC1 */ bool mPointerCanPlaceBeacon;
    /* 0x8CC4 */ mVec3_c field_0x8CC4;
    /* 0x8CD0 */ mVec3_c field_0x8CD0;
    /* 0x8CDC */ mVec3_c field_0x8CDC;
    /* 0x8CE8 */ mVec3_c field_0x8CE8;
    /* 0x8CF4 */ mVec3_c field_0x8CF4;
    /* 0x8D00 */ mVec3_c mMapScroll;
    /* 0x8D0C */ mVec3_c mPlayerPos;
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
    /* 0x8D54 */ s32 field_0x8D54;
    /* 0x8D58 */ s32 field_0x8D58;
    /* 0x8D5C */ UNKWORD field_0x8D5C;
    /* 0x8D60 */ s32 field_0x8D60;
    /* 0x8D64 */ UNKWORD field_0x8D64;
    /* 0x8D68 */ mAng field_0x8D68;
    /* 0x8D6A */ u8 field_0x8D6A; // set at 0x8009e2d4
    /* 0x8D6B */ u8 field_0x8D6B;
    /* 0x8D6C */ nw4r::lyt::Pane *mpRegionPane1;
    /* 0x8D70 */ nw4r::lyt::Pane *mpRegionPane2;
    /* 0x8D74 */ nw4r::lyt::Pane *mpZoomInOutPane;
    /* 0x8D78 */ f32 field_0x8D78;
    /* 0x8D7C */ f32 field_0x8D7C;
    /* 0x8D80 */ f32 field_0x8D80;
    /* 0x8D84 */ f32 field_0x8D84;
    /* 0x8D88 */ f32 field_0x8D88;
    /* 0x8D8C */ nw4r::lyt::Group *mpPriorityGroups[2];
    /* 0x8D94 */ d2d::SubPaneList mSubpaneList;
    /* 0x8DA0 */ d2d::SubPaneListNode mSubpane;
    /* 0x8DB0 */ UNKWORD field_0x8DB0;
    /* 0x8DB4 */ bool mNavEnabled;
    /* 0x8DB5 */ bool mDrawScaleFrame;
    /* 0x8DB8 */ s32 mDisplayedBeaconCount;
    /* 0x8DBC */ bool mShowIslandNames;
    /* 0x8DBD */ bool field_0x8DBD;
    /* 0x8DBE */ u8 field_0x8DBE;
    /* 0x8DBF */ u8 field_0x8DBF;
    /* 0x8DC0 */ UNKWORD field_0x8DC0;
    /* 0x8DC4 */ UNKWORD field_0x8DC4;
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
        return mMapMain.mMapEvent == dLytMapMain_c::MAP_EVENT_SW_BANK_SMALL ||
               mMapMain.mMapEvent == dLytMapMain_c::MAP_EVENT_DUNGEON_MAP_GET ||
               mMapMain.mMapEvent == dLytMapMain_c::MAP_EVENT_FIELD_MAP_CHANGE_5 ||
               mMapMain.mMapEvent == dLytMapMain_c::MAP_EVENT_FOREST_MAP_CHANGE;
    }

    bool isSomeMapFieldEq10() const {
        return mMapMain.mMapEvent == dLytMapMain_c::MAP_EVENT_SAVE_OBJ_MSG_WINDOW;
    }

    void lightPillarRelated(s32 p1, s32 p2, s32 p3) {
        mMapMain.lightPillarRelated(p1, p2, p3);
    }

    const dMapSaveObjDefinition *getSaveObjDefinition(s32 statueIdx) const {
        return mMapMain.getSaveObjDefinition(mMapMain.mSurfaceProvince, statueIdx);
    }

    void fn_80143A30();

private:
    /* 0x0004 */ d2d::ResAccIf_c mResAcc;
    /* 0x0374 */ dLytMapMain_c mMapMain;

    static dLytMap_c *sInstance;
};

#endif
