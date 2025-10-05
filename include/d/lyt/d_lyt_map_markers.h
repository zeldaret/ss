#ifndef D_LYT_MAP_MARKERS_H
#define D_LYT_MAP_MARKERS_H

#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_pane.h"
#include "sized_string.h"

class dAcBase_c;
class dAcTbox_c;
class dAcPy_c;
class dTgMapMark_c;

// Size: 0x1C
struct dLytMapPopupState {
public:
    dLytMapPopupState() : textPos(0.0f, 0.0f) {}
    ~dLytMapPopupState() {}

    /* 0x00 */ f32 inAnimFrame;
    /* 0x04 */ mVec2_c textPos;
    /* 0x0C */ mVec2_c boundingPos;
    /* 0x14 */ bool visible;
    /* 0x15 */ u8 objId;
    /* 0x18 */ const char *labelPrefix;
};

/**
 * Since a map has a dynamic number of actors, the layout
 * doesn't actually contain multiple panes for each icon -
 * instead it contains one copy per icon, and then the custom
 * draw function simply reads a bunch of draw commands to position
 * and draw the panes, potentially using a pane multiple times.
 *
 * Size: 0xC
 */
struct dLytMapIcon01DrawCommand {
public:
    dLytMapIcon01DrawCommand() {}
    ~dLytMapIcon01DrawCommand() {}

    /* 0x00 */ mVec2_c position;
    /* 0x08 */ mAng rotation;
    /* 0x0A */ u8 paneIdx;
    /* 0x0B */ u8 passIdx;
};

// Size: 0x20
struct dLytMapPopupConfig {
public:
    dLytMapPopupConfig() : textPos(0.0f, 0.0f) {}
    ~dLytMapPopupConfig() {}

    /* 0x00 */ s32 cmdIdx;
    /* 0x04 */ nw4r::lyt::Bounding *bounding;
    /* 0x08 */ mVec2_c textPos;
    /* 0x10 */ mVec2_c boundingPos;
    /* 0x18 */ bool pointedAt;
    /* 0x19 */ u8 objId;
    /* 0x1C */ const char *labelPrefix;
};

/**
 * Copy of dLytMapIcon01DrawCommand
 *
 * Size: 0xC
 */
struct dLytMapIcon00DrawCommand {
public:
    dLytMapIcon00DrawCommand() {}
    ~dLytMapIcon00DrawCommand() {}

    /* 0x00 */ mVec2_c position;
    /* 0x08 */ mAng rotation;
    /* 0x0A */ u8 paneIdx;
    /* 0x0B */ u8 passIdx;
};

/**
 * Since map icons are completely virtual, we need to separately
 * track animation state if there are special in/out animations.
 *
 * Size: 0x10
 */
struct dLytMapIconAnimState {
public:
    // no ctor/dtor

    /* 0x00 */ s32 cmdIndex;
    /* 0x04 */ f32 frame;
    /* 0x08 */ bool animIn;
    /* 0x09 */ bool animOut;
    /* 0x0A */ bool visible;
    /* 0x0B */ bool prevVisible;
    /* 0x0C */ bool render;
};

/** 2D UI - Map - Popup text that appears when pointing at certain points of interest */
class dLytMapPopup_c {
public:
    dLytMapPopup_c()
        : mLabel("MAP_00"),
          mModeCheckResult(MODE_MAX),
          field_0x178(0),
          mMaxScale(0.0f),
          field_0x180(0),
          field_0x184(0) {}
    virtual ~dLytMapPopup_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void draw();

    void setLabel(const char *label);
    void realizeText();
    void setInout(f32 value);
    f32 getTextRenderWidth() const;

    f32 getInAnimDuration() const {
        return mAnm[1].getLastFrame();
    }

    void setPosition(const mVec2_c &pos) {
        mPosition = pos;
    }

private:
    enum ModeCheck_e {
        MODE_TRANSITION_TO_ZOOM = 2,
        MODE_TRANSITION_TO_STAGE = 3,
        MODE_STABLE_STAGE = 4,
        MODE_STABLE_ZOOM = 5,
        MODE_MAX = 6,
    };

    void setInitialState();

    void checkMapMode();
    void updateScale();

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[2];
    /* 0x114 */ SizedString<0x40> mLabel;
    /* 0x154 */ nw4r::lyt::Pane *mpPanes[2];
    /* 0x15C */ dTextBox_c *mpTextboxes[2];
    /* 0x164 */ dWindow_c *mpWindow[1];
    /* 0x168 */ dTextBox_c *mpSizeBox[1];
    /* 0x16C */ s32 mModeCheckResult;
    /* 0x170 */ mVec2_c mPosition;
    /* 0x178 */ u8 field_0x178;
    /* 0x17C */ f32 mMaxScale;
    /* 0x180 */ UNKWORD field_0x180;
    /* 0x184 */ UNKWORD field_0x184;
};

/** 2D UI - Map - Text holding area names (on the world/province view, or exits on the stage view) */
class dLytMapPlace_c {
public:
    dLytMapPlace_c()
        : mLabel("MAP_02"), mModeCheckResult(MODE_MAX), field_0x1C0(0.0f), field_0x1C4(0.0f), mMaxScale(0.0f) {}
    virtual ~dLytMapPlace_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void draw();

    void setLabel(const char *label);
    void setInout(f32 value);
    void setColor(u32 value);

    void realize();

private:
    enum ModeCheck_e {
        MODE_TRANSITION_TO_ZOOM = 2,
        MODE_TRANSITION_TO_STAGE = 3,
        MODE_STABLE_STAGE = 4,
        MODE_STABLE_ZOOM = 5,
        MODE_MAX = 6,
    };

    void realizeText();
    void setInitialState();
    void updateScale();
    void realizeTextSize();
    void realizeBgSize();

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[3];
    /* 0x154 */ SizedString<0x40> mLabel;
    /* 0x194 */ nw4r::lyt::Pane *mpPanes[2];
    /* 0x19C */ nw4r::lyt::Pane *mpBgPanes[3];
    /* 0x1A8 */ dTextBox_c *mpTextboxes[3];
    /* 0x1B4 */ dWindow_c *mpWindow[1];
    /* 0x1B8 */ dTextBox_c *mpSizeBox[1];
    /* 0x1BC */ s32 mModeCheckResult;
    /* 0x1C0 */ f32 field_0x1C0;
    /* 0x1C4 */ f32 field_0x1C4;
    /* 0x1C8 */ mVec2_c field_0x1C8;
    /* 0x1D0 */ u8 _0x1D0[0x1D8 - 0x1D0];
    /* 0x1D8 */ f32 mMaxScale;
};

class dLytMapIcon01_c : public m2d::Base_c {
    friend class dLytMapMarkers_c;

public:
    dLytMapIcon01_c()
        : field_0x000D(0),
          mShowIslandNames(false),
          mModeCheckResult(MODE_MAX),
          field_0x0C70(0),
          field_0x0C83(0),
          field_0x0C84(0),
          field_0x1838(0.0f),
          field_0x183C(0.0f),
          field_0x1840(0.0f),
          field_0x1844(false),
          field_0x1845(0),
          field_0x1848(0),
          field_0x184C(false),
          field_0x184D(false) {}
    virtual void draw() override;
    virtual bool build(d2d::ResAccIf_c *resAcc);
    virtual bool remove();
    virtual bool execute();
    virtual void resetDrawCommands();

private:
    enum ModeCheck_e {
        MODE_TRANSITION_TO_ZOOM = 2,
        MODE_TRANSITION_TO_STAGE = 3,
        MODE_STABLE_STAGE = 4,
        MODE_STABLE_ZOOM = 5,
        MODE_MAX = 6,
    };

    void setupActorDrawCommands();
    void setupStageDrawCommands();
    void setupLinkDrawCommand();
    void setupLinkDrawCommand1(dAcPy_c *ac);
    void setupLinkDrawCommand2();
    void setupCloudBigDrawCommand();
    void setupTriforceDrawCommands();

    void sortPanes();

    void setupTboxDrawCommand(dAcBase_c *actor);
    void setupTboxDrawCommandGoddessClosed(dAcTbox_c *box);
    void setupTboxDrawCommandOpen(dAcTbox_c *box);
    void setupTboxDrawCommandClosed(dAcTbox_c *box);
    void setupNpcKenseiDrawCommand(dAcBase_c *actor);
    void setupObjShutterDrawCommand(dAcBase_c *actor);
    void setupObjDoorDrawCommand(dAcBase_c *actor);
    void setupObjSaveDrawCommand(dAcBase_c *actor);
    void setupObjLightLineDrawCommand(dAcBase_c *actor);
    void setupNpcBNusiDrawCommand(dAcBase_c *actor);
    void setupObjTerryShopDrawCommand(dAcBase_c *actor);
    void setupObjRoAtTargetDrawCommand(dAcBase_c *actor);
    void setupObjD3DummyDrawCommand(dAcBase_c *actor);
    void setupObjSealedDoorDrawCommand(dAcBase_c *actor);
    void setupObjHarpHintDrawCommand(dAcBase_c *actor);
    void setupNpcBeeDrawCommand(dAcBase_c *actor);

    void setupTgMapMarkDrawCommand(dAcBase_c *actor);
    void setupTgMapMarkDrawCommand1(dTgMapMark_c *actor);
    void setupTgMapMarkDrawCommand2(dTgMapMark_c *actor);
    void setupTgMapInstDrawCommand(dAcBase_c *actor);
    void setupTgForceGetFlagDrawCommand(dAcBase_c *actor);
    void setupTgInsectDrawCommand(dAcBase_c *actor);

    void setLinkTunic(s32 type);
    void setGoddessStatue(u32 present);
    void setTerry(bool present);
    void setAreaLight(u32 color, nw4r::lyt::Pane *pane);
    void setCloud(u32 alpha, nw4r::lyt::Pane *pane);
    void setSaveObj(u32 color, nw4r::lyt::Pane *pane);

    void drawWithAnimIn(u32 paneIdx, nw4r::lyt::Pane *pane, f32 frame);
    void drawWithAnimOut(u32 paneIdx, nw4r::lyt::Pane *pane, f32 frame);
    void drawFullyIn(u32 paneIdx, nw4r::lyt::Pane *pane);

    void fn_80181C40();

    void fn_80181880(nw4r::lyt::Pane *, nw4r::lyt::Pane *, mVec2_c &) const;

    void checkMapMode();
    void loadFlags();
    void updateScale();

    void recordAnimVisState();
    void transitionAnims();
    void checkBoundingPointing();
    bool isPointingAtBounding(nw4r::lyt::Pane *pane) const;

    /* 0x000D */ u8 field_0x000D;
    /* 0x000E */ bool mShowIslandNames;
    /* 0x0010 */ nw4r::lyt::Bounding *mpBoundings[59];
    /* 0x00FC */ nw4r::lyt::Pane *mpSpecialPanes[27];
    /* 0x0168 */ nw4r::lyt::Pane *mpSpecialPictures[27];
    /* 0x01D4 */ nw4r::lyt::Pane *mpTerryOnOffPane;
    /* 0x01D8 */ d2d::ResAccIf_c *mpResAcc;
    /* 0x01DC */ d2d::LytBase_c mLyt;
    /* 0x026C */ d2d::AnmGroup_c mAnm[18];
    /* 0x06EC */ nw4r::lyt::Pane *mpPanes[42];
    /* 0x0794 */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x07BC */ dLytMapIcon01DrawCommand mCommands[100];
    /* 0x0C6C */ s32 mModeCheckResult;
    /* 0x0C70 */ s32 field_0x0C70;
    /* 0x0C74 */ bool mFunFunIslandDiscovered;
    /* 0x0C75 */ bool mLumpyPumkpinDiscovered;
    /* 0x0C76 */ bool mBeedleIslandDiscovered;
    /* 0x0C77 */ bool mBambooIslandDiscovered;
    /* 0x0C78 */ bool mIsleOfSongsDiscovered;
    /* 0x0C79 */ bool mBugHavenDiscovered;
    /* 0x0C7A */ bool mBilocyteFightTriggered;
    /* 0x0C7B */ bool mFaronPillarOpened;
    /* 0x0C7C */ bool mFaronDiscovered;
    /* 0x0C7D */ bool mEldinPillarOpened;
    /* 0x0C7E */ bool mEldinDiscovered;
    /* 0x0C7F */ bool mLanayruPillarOpened;
    /* 0x0C80 */ bool mLanaryuDiscovered;
    /* 0x0C81 */ bool mThunderheadEntered;
    /* 0x0C82 */ bool mBeedleShopEntered;
    /* 0x0C83 */ u8 field_0x0C83;
    /* 0x0C84 */ u8 field_0x0C84;
    /* 0x0C85 */ u8 field_0x0C85[100];
    /* 0x0CE9 */ u8 mPassIdxes[35];
    /* 0x0D0C */ s32 mCurrentFloor;
    /* 0x0D10 */ s32 mLastFloor;
    /* 0x0D14 */ u8 mNumCommandsPerPass[35];
    /* 0x0D37 */ u8 mNumCommands;

    /* 0x0D38 */ u8 _0x0D38[0x0D3A - 0x0D38];

    /* 0x0D3A */ mAng field_0x0D3A;

    /* 0x0D3C */ u8 _0x0D3C[0x0D40 - 0x0D3C];

    /* 0x0D40 */ mVec2_c field_0x0D40;
    /* 0x0D48 */ u8 field_0x0D48;
    /* 0x0D49 */ u8 field_0x0D49;
    /* 0x0D4A */ u8 field_0x0D4A;
    /* 0x0D4B */ u8 field_0x0D4B[100]; // ???

    /* 0x0DAF */ u8 _0x0DAF[0x0E13 - 0x0DAF];

    /* 0x0E13 */ u8 field_0x0E13;
    /* 0x0E14 */ dLytMapPopupConfig mPopupConfigs[56];
    /* 0x1514 */ dLytMapIconAnimState mIconAnims[50];
    /* 0x1834 */ s32 mNumAnims;
    /* 0x1838 */ f32 field_0x1838;
    /* 0x183C */ f32 field_0x183C;
    /* 0x1840 */ f32 field_0x1840;
    /* 0x1844 */ bool field_0x1844;
    /* 0x1845 */ u8 field_0x1845;

    /* 0x1848 */ UNKWORD field_0x1848;

    /* 0x184C */ bool field_0x184C;
    /* 0x184D */ bool field_0x184D;
};

class dLytMapIcon00_c : public m2d::Base_c {
public:
    dLytMapIcon00_c()
        : mModeCheckResult(MODE_MAX),
          field_0x0E4C(0),
          field_0x0E59(0),
          field_0x0E5A(0),
          field_0x13A6(0),
          field_0x13A7(0),
          field_0x1CD0(0.0f, 0.0f, 0.0f),
          field_0x1CDC(0),
          field_0x1CDD(0),
          field_0x1CDE(0),
          field_0x1CE0(0),
          field_0x1CE4(0),
          field_0x1CE5(0),
          field_0x1CE6(0),
          field_0x1CE7(0) {}

    virtual void draw() override;
    virtual bool build(d2d::ResAccIf_c *resAcc);
    virtual bool remove();
    virtual bool execute();
    virtual void resetDrawCommands();

private:
    enum ModeCheck_e {
        MODE_TRANSITION_TO_ZOOM = 2,
        MODE_TRANSITION_TO_STAGE = 3,
        MODE_STABLE_STAGE = 4,
        MODE_STABLE_ZOOM = 5,
        MODE_MAX = 6,
    };

    void fn_80189750();

    /* 0x0010 */ u8 _0x0010[0x0104 - 0x0010];

    /* 0x0104 */ d2d::LytBase_c mLyt;
    /* 0x0194 */ d2d::AnmGroup_c mAnm[29];

    /* 0x08D4 */ u8 _0x08D4[0x0970 - 0x08D4];

    /* 0x0970 */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x0998 */ dLytMapIcon00DrawCommand mCommands[100];
    /* 0x0E48 */ s32 mModeCheckResult;
    /* 0x0E4C */ UNKWORD field_0x0E4C;
    /* 0x0E50 */ u8 field_0x0E50;
    /* 0x0E51 */ u8 field_0x0E51;
    /* 0x0E52 */ u8 field_0x0E52;
    /* 0x0E53 */ u8 field_0x0E53;
    /* 0x0E54 */ u8 field_0x0E54;
    /* 0x0E55 */ u8 field_0x0E55;
    /* 0x0E56 */ u8 field_0x0E56;
    /* 0x0E57 */ u8 field_0x0E57;
    /* 0x0E58 */ u8 field_0x0E58;
    /* 0x0E59 */ u8 field_0x0E59;
    /* 0x0E59 */ u8 field_0x0E5A;
    /* 0x0E5B */ u8 field_0x0E5B[100];

    /* 0x0EC0 */ u8 _0x0EC0[0x0EE4 - 0x0EC0];

    /* 0x0EE4 */ s32 mNumCommands;
    /* 0x0EE8 */ s32 field_0x0EE8;
    /* 0x0EEC */ u8 field_0x0EEC[36];

    /* 0x0F10 */ u8 _0x0F10[0x0F20 - 0x0F10];

    /* 0x0F20 */ u8 field_0x0F20[100];

    /* 0x0F84 */ mVec3_c field_0x0F84[77];

    /* 0x1320 */ u8 _0x1320[0x1331 - 0x1320];

    /* 0x1331 */ u8 mNumCommandsPerPass[17];
    /* 0x1342 */ u8 field_0x1342[100];
    /* 0x13A6 */ u8 field_0x13A6;
    /* 0x13A6 */ u8 field_0x13A7;
    /* 0x13A8 */ u8 field_0x13A8[100];
    /* 0x140C */ u8 field_0x140C;
    /* 0x1410 */ dLytMapPopupConfig mPopupConfigs[30];
    /* 0x17D0 */ dLytMapPlace_c mPlace;
    /* 0x19B8 */ dLytMapIconAnimState mIconAnims[50];
    /* 0x1CCC */ s32 mNumAnims;
    /* 0x1CD0 */ mVec3_c field_0x1CD0;
    /* 0x1CDC */ u8 field_0x1CDC;
    /* 0x1CDD */ u8 field_0x1CDD;
    /* 0x1CDE */ u8 field_0x1CDE;
    /* 0x1CE0 */ UNKWORD field_0x1CE0;
    /* 0x1CE4 */ u8 field_0x1CE4;
    /* 0x1CE5 */ u8 field_0x1CE5;
    /* 0x1CE6 */ u8 field_0x1CE6;
    /* 0x1CE7 */ u8 field_0x1CE7;
};

class dLytMapMarkers_c : public m2d::Base_c {
public:
    dLytMapMarkers_c()
        : mShowIslandNames(false), field_0x0702(0), field_0x0703(0), field_0x0704(0.0f), mNumPopups(0) {}
    virtual ~dLytMapMarkers_c() {}
    virtual void draw() override;
    virtual bool build(d2d::ResAccIf_c *resAcc);
    virtual bool remove();
    virtual bool execute();

    void setIslandNamesOn(bool on);
    void drawPopups();

    void setField_0x6F4(const mVec2_c &v) {
        field_0x06F4 = v;
    }

    void setField_0x0703(u8 v) {
        field_0x0703 = v;
    }

    f32 getField_0x0704() const {
        return field_0x0704;
    }

private:
    void resetPopups();
    void loadPopups();

    /* 0x0010 */ d2d::ResAccIf_c mResAcc00;
    /* 0x0380 */ d2d::ResAccIf_c mResAcc01;

    /* 0x06F0 */ bool mShowIslandNames;
    /* 0x06F4 */ mVec2_c field_0x06F4;

    /* 0x06FC */ u8 _0x6FC[0x0702 - 0x06FC];

    /* 0x0702 */ u8 field_0x0702;
    /* 0x0702 */ u8 field_0x0703;
    /* 0x0704 */ f32 field_0x0704;
    /* 0x0708 */ dLytMapPopup_c mMapPopup;
    /* 0x0890 */ s32 mNumPopups;
    /* 0x0894 */ dLytMapPopupState mPopups[60];
    /* 0x0F24 */ const dLytMapPopupConfig *mpPopupConfigs;
    /* 0x0F28 */ dLytMapIcon01_c mIcon01;
    /* 0x2778 */ dLytMapIcon00_c mIcon00;
};

#endif
