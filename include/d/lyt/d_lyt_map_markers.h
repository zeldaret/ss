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

// Size: 0x1C
struct MapUnkStruct1 {
public:
    MapUnkStruct1() {}
    ~MapUnkStruct1() {}

    /* 0x00 */ u8 _0x00[0x1C];
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
struct MapUnkStruct3 {
public:
    MapUnkStruct3() {}
    ~MapUnkStruct3() {}

    /* 0x00 */ UNKWORD field_0x00;
    /* 0x04 */ nw4r::lyt::Bounding *bounding;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ mVec2_c field_0x10;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1C */ const char *field_0x1C;
};

// Size: 0xC
struct MapUnkStruct4 {
public:
    MapUnkStruct4() {}
    ~MapUnkStruct4() {}

    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ u16 field_0x08;
    /* 0x0A */ u8 field_0x0A;
    /* 0x0B */ u8 field_0x0B;
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

    /* 0x00 */ UNKWORD cmdIndex;
    /* 0x04 */ f32 frame;
    /* 0x08 */ u8 animIn;
    /* 0x09 */ u8 animOut;
    /* 0x0A */ u8 idleVisible;
    /* 0x0B */ u8 field_0x0B;
    /* 0x0C */ bool visible;
};

/** 2D UI - Map - Popup text that appears when pointing at certain points of interest */
class dLytMapPopup_c {
public:
    dLytMapPopup_c() : mLabel("MAP_00") {}
    virtual ~dLytMapPopup_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void draw();

    void setLabel(const char *label);
    void setInout(f32 value);
    f32 getTextRenderWidth() const;

private:
    enum ModeCheck_e {
        MODE_TRANSITION_TO_ZOOM = 2,
        MODE_TRANSITION_TO_STAGE = 3,
        MODE_STABLE_STAGE = 4,
        MODE_STABLE_ZOOM = 5,
    };

    void realizeText();
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
};

/** 2D UI - Map - Text holding area names (on the world/province view, or exits on the stage view) */
class dLytMapPlace_c {
public:
    dLytMapPlace_c() {}
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

class dLytMapIcon00_c : public m2d::Base_c {
public:
    dLytMapIcon00_c() {}

private:
    /* 0x0104 */ d2d::LytBase_c mLyt;
    /* 0x0194 */ d2d::AnmGroup_c mAnm[29];
    /* 0x0970 */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x0998 */ MapUnkStruct4 mUnk4[100];
    /* 0x0E5B */ u8 field_0x0E5B[100];
    /* 0x0F20 */ u8 field_0x0F20[100];
    /* 0x0F84 */ mVec3_c field_0x0F84[77];
    /* 0x1342 */ u8 field_0x1342[100];
    /* 0x13A8 */ u8 field_0x13A8[100];
    /* 0x1410 */ MapUnkStruct3 mUnk3[30];
    /* 0x1D70 */ dLytMapPlace_c mPlace;
};

class dLytMapIcon01_c : public m2d::Base_c {
public:
    dLytMapIcon01_c() {}
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
    };

    void setupActorDrawCommands();
    void setupStageDrawCommands();
    void setupLinkDrawCommand();
    void setupTriforceDrawCommands();
    void setupUnkDrawCommand();

    void sortDrawCommands();

    void setupTboxDrawCommand(dAcBase_c *actor);
    void setupTboxDrawCommandGoddessClosed(dAcTbox_c *box);
    void setupTboxDrawCommandOpen(dAcTbox_c *box);
    void setupTboxDrawCommandClosed(dAcTbox_c *box);

    void setLinkTunic(s32 type);
    void setGoddessStatue(bool present);
    void setTerry(bool present);
    void drawAreaLight(s32 color, nw4r::lyt::Pane *pane);
    void drawSaveObj(s32 color, nw4r::lyt::Pane *pane);
    void drawCloud(s32 alpha, nw4r::lyt::Pane *pane);

    void drawWithAnimIn(u32 paneIdx, nw4r::lyt::Pane *pane, f32 frame);
    void drawWithAnimOut(u32 paneIdx, nw4r::lyt::Pane *pane, f32 frame);
    void drawFullyIn(u32 paneIdx, nw4r::lyt::Pane *pane);
    void fn_80181C40();

    void checkMapMode();
    void loadFlags();

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

    /* 0x0C74 */ u8 field_0x0C74;
    /* 0x0C75 */ u8 field_0x0C75;
    /* 0x0C76 */ u8 field_0x0C76;
    /* 0x0C77 */ u8 field_0x0C77;
    /* 0x0C78 */ u8 field_0x0C78;
    /* 0x0C79 */ u8 field_0x0C79;
    /* 0x0C7A */ u8 field_0x0C7A;
    /* 0x0C7B */ u8 field_0x0C7B;
    /* 0x0C7C */ u8 field_0x0C7C;
    /* 0x0C7D */ u8 field_0x0C7D;
    /* 0x0C7E */ u8 field_0x0C7E;
    /* 0x0C7F */ u8 field_0x0C7F;
    /* 0x0C80 */ u8 field_0x0C80;
    /* 0x0C81 */ u8 field_0x0C81;
    /* 0x0C82 */ u8 field_0x0C82;
    /* 0x0C83 */ u8 field_0x0C83;
    /* 0x0C84 */ u8 field_0x0C84;
    /* 0x0C85 */ u8 field_0x0C85[100];
    /* 0x0CE9 */ u8 mPassIdxes[35];
    /* 0x0D0C */ s32 field_0xD0C;
    /* 0x0D10 */ s32 field_0xD10;
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
    /* 0x0E14 */ MapUnkStruct3 mUnk3[56];
    /* 0x1514 */ dLytMapIconAnimState mIconAnims[50];
    /* 0x1834 */ s32 mNumAnims;
    /* 0x1838 */ f32 field_0x1838;
    /* 0x183C */ f32 field_0x183C;

    /* 0x1840 */ u8 _0x183C[0x1845 - 0x1840];

    /* 0x1845 */ u8 field_0x1845;

    /* 0x1846 */ u8 _0x1846[0x184C - 0x1846];

    /* 0x184C */ bool field_0x184C;
};

// TODO, name made up
class dLytMapMarkers_c : public m2d::Base_c {
public:
    dLytMapMarkers_c() : field_0x6F0(0), field_0x0702(0), field_0x0703(0), field_0x0704(0.0f) {}
    virtual ~dLytMapMarkers_c() {}
    virtual void draw() override;
    virtual void build(d2d::ResAccIf_c *resAcc);

    void setIslandNamesOn(bool on);
    void fn_80189B90();

    void setField_0x6F4(const mVec2_c &v) {
        field_0x06F4 = v;
    }

private:
    /* 0x0010 */ d2d::ResAccIf_c mResAcc1;
    /* 0x0380 */ d2d::ResAccIf_c mResAcc2;

    /* 0x06F0 */ u8 field_0x6F0;
    /* 0x06F4 */ mVec2_c field_0x06F4;

    /* 0x06FC */ u8 _0x6FC[0x0702 - 0x06FC];

    /* 0x0702 */ u8 field_0x0702;
    /* 0x0702 */ u8 field_0x0703;
    /* 0x0704 */ f32 field_0x0704;
    /* 0x0708 */ dLytMapPopup_c mMapPopup;
    /* 0x088C */ u8 _0x088C[0x0894 - 0x088C];
    /* 0x0894 */ MapUnkStruct1 mUnk1[60];

    /* 0x0F30 */ dLytMapIcon01_c mIcon01;
    /* 0x2780 */ dLytMapIcon00_c mIcon00;
};

#endif
