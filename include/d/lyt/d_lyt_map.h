#ifndef D_LYT_MAP_H
#define D_LYT_MAP_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_map_capture.h"
#include "d/lyt/d_structd.h"
#include "egg/core/eggColorFader.h"
#include "m/m2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

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
        : mStateMgr(*this, sStateID::null), mpBounding(nullptr), field_0x1BC(0), field_0x1C0(0.0f, 0.0f, 0.0f),
          field_0x1CC(0), field_0x1D0(nullptr), field_0x1D4(0), mIndex(0), field_0x1DC(0) {}
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
    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMapPinIcon_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x190 */ d2d::dLytStructD mStructD;
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
        : mStateMgr(*this, sStateID::null), field_0x3C(0), field_0x40(0), field_0x44(0), field_0x48(0), field_0x4C(0),
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
    dLytMapFloorBtnMgr_c(void *arg) : field_0x008(arg), field_0x51C(nullptr), mStateMgr(*this, sStateID::null) {
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
    virtual bool LytMeter0x14() override;
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
    /* 0x008 */ void *field_0x008;
    /* 0x00C */ UI_STATE_MGR_DECLARE(dLytMapFloorBtnMgr_c);
    /* 0x048 */ d2d::dLytSub mLyt;
    /* 0x0DC */ d2d::AnmGroup_c mAnmGroups[1];
    /* 0x11C */ LytMapFourAnimGroups mMoreGroups[4];
    /* 0x51C */ void *field_0x51C;
    /* 0x520 */ d2d::dLytStructD mStructDs[4];
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

class dLytMapMain_c : public m2d::Base_c {
public:
    dLytMapMain_c();
    virtual ~dLytMapMain_c();
    virtual void draw() override;
    virtual void dLytMapMain_vt0x10();

    void build();

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
    /* 0x004C */ u8 field_0x004C[0x00A4 - 0x004C];
    /* 0x00A4 */ u8 field_0x00A4[0x010C - 0x00A4];
    /* 0x010C */ d2d::LytBase_c mLyt;
    /* 0x019C */ d2d::AnmGroup_c mAnmGroups[54];
    /* 0x0F1C */ LytMap0x80520B5C field_0xF1C;
    /* 0x0F24 */ dLytMapCapture_c mMapCapture;
    /* 0x0FA0 */ dLytMapFloorBtnMgr_c mFloorBtnMgr;
    /* 0x16B4 */ dLytMapPinIconAggregate_c mPinIconAggregate;

    /* 0x6704 */ dLytMapSaveObj_c mSaveObjs[12];
    /* 0x79C4 */ dLytMapSaveCaption_c mSaveCaption;
    /* 0x0000 */ dLytMapSavePopup_c mSavePopup;
    /* 0x807C */ dLytMapPopupInfo_c mPopupInfo;
    /* 0x828C */ mVec3_c field_0x828C[12];
    /* 0x832C */ d2d::dLytStructD field_0x832C[33];
    /* 0x8904 */ mVec3_c field_0x8904;
    /* 0x8910 */ mVec3_c field_0x8910;
    /* 0x891C */ mVec3_c field_0x891C;
    // ???
    /* 0x8930 */ mVec3_c field_0x8930;
    /* 0x893C */ mVec3_c field_0x893C;
    // ???

    /* 0x8CC4 */ mVec3_c field_0x8CC4;
    /* 0x8CD0 */ mVec3_c field_0x8CD0;
    /* 0x8CDC */ mVec3_c field_0x8CDC;
    /* 0x8CE8 */ mVec3_c field_0x8CE8;
    /* 0x8CF4 */ mVec3_c field_0x8CF4;
    /* 0x8D00 */ mVec3_c field_0x8D00;
    /* 0x8D0C */ mVec3_c field_0x8D0C;
    /* 0x8D18 */ mVec3_c field_0x8D18;
    /* 0x8D24 */ mVec3_c field_0x8D24;

    /* 0x8DC8 */ UNKWORD field_0x8DC8;
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

    void build();

private:
    /* 0x0004 */ d2d::ResAccIf_c mResAcc;
    /* 0x0374 */ dLytMapMain_c mMapMain;

    static dLytMap_c *sInstance;
};

#endif
