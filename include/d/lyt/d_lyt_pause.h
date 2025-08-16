#ifndef D_LYT_PAUSE_H
#define D_LYT_PAUSE_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_base.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

class dLytPauseBack_c;
class dLytPauseInfo_c;
class dLytPauseText_c;
class dLytCommonArrow_c;
class dLytPauseDisp00_c;
class dLytPauseDisp01_c;

class dLytPauseMain_c {
public:
    dLytPauseMain_c();
    virtual ~dLytPauseMain_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();
    void drawDirectly();

    void loadBgTextureUnused();
    void loadBgTexture();

    void requestIn();
    void requestOut();
    void requestChange();
    void requestSelect();
    void requestRingToggle();

    bool hasSelection() const;
    void updateTitle();

    bool isChangingState() const {
        return mIsChangingState;
    }

    dLytCommonArrow_c *getArrow() const {
        return mpArrow;
    }

private:
    STATE_FUNC_DECLARE(dLytPauseMain_c, None);
    STATE_FUNC_DECLARE(dLytPauseMain_c, In);
    STATE_FUNC_DECLARE(dLytPauseMain_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseMain_c, Change);
    STATE_FUNC_DECLARE(dLytPauseMain_c, Select);
    STATE_FUNC_DECLARE(dLytPauseMain_c, Ring);
    STATE_FUNC_DECLARE(dLytPauseMain_c, GetDemo);
    STATE_FUNC_DECLARE(dLytPauseMain_c, Out);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytPauseMain_c);
    /* 0x40 */ dLytPauseBack_c *mpPauseBack;
    /* 0x44 */ dLytPauseDisp00_c *mpDisp00;
    /* 0x48 */ dLytPauseDisp01_c *mpDisp01;
    /* 0x4C */ dLytPauseInfo_c *mpPauseInfo;
    /* 0x50 */ dLytPauseText_c *mpPauseText;
    /* 0x54 */ dLytCommonArrow_c *mpArrow;
    /* 0x58 */ s32 mTimer;
    /* 0x5C */ s32 mStep;
    /* 0x60 */ u8 mPartStateChangeFlags;
    /* 0x61 */ bool mInRequest;
    /* 0x62 */ bool mOutRequest;
    /* 0x63 */ bool mIsChangingState;
    /* 0x64 */ bool mChangeRequest;
    /* 0x65 */ bool mSelectRequest;
    /* 0x66 */ bool mRingToggleRequest;
};

class dLytPauseMgr_c : public dLytBase_c {
public:
    dLytPauseMgr_c();
    ~dLytPauseMgr_c();

    enum Disp_e {
        DISP_00_INVENTORY = 0,
        DISP_01_COLLECTION = 1,

        DISP_MAX,
    };

    enum Tab_e {
        TAB_ITEM = 0,
        TAB_POUCH = 1,
        TAB_DOWSING = 2,

        TAB_MAX = 3,
    };

    /* 0x08 */ virtual int create() override;
    /* 0x14 */ virtual int doDelete() override;
    /* 0x20 */ virtual int execute() override;
    /* 0x2C */ virtual int draw() override;
    /* 0x4C */ virtual bool build() override;

    static dLytPauseMgr_c *GetInstance() {
        return sInstance;
    }

    d2d::ResAccIf_c *getResAcc1() {
        return &mResAcc1;
    }

    d2d::ResAccIf_c *getResAcc2() {
        return &mResAcc2;
    }

    EGG::CpuTexture *getBgTexture() {
        return mpBgTexture;
    }

    bool isNavLeft() const {
        return mIsNavLeft;
    }

    bool getField_0x0832() const {
        return field_0x0832;
    }

    void setField_0x0837(bool b) {
        field_0x0837 = b;
    }

    bool getField_0x0838() const {
        return field_0x0838;
    }

    bool getField_0x083B() const {
        return field_0x083B;
    }

    bool getField_0x083C() const {
        return field_0x083C;
    }

    void setField_0x083C(bool b) {
        field_0x083C = b;
    }

    bool getField_0x083D() const {
        return field_0x083D;
    }

    void setField_0x083D(bool b) {
        field_0x083D = b;
    }

    bool getField_0x083E() const {
        return field_0x083E;
    }

    void setField_0x083E(bool b) {
        field_0x083E = b;
    }

    bool getField_0x0840() const {
        return field_0x0840;
    }

    bool isStateWait() const;

    s32 getCurrentDisp00Tab() const {
        return mCurrentDisp00Tab;
    }

    enum SelectionType_e {
        SELECT_BWHEEL = 0,
        SELECT_POUCH = 1,
        SELECT_DOWSING = 2,
        SELECT_INSECT = 4,
        SELECT_QUEST_MATERIAL = 5,
        SELECT_ITEM = 6,
        SELECT_CATEGORY = 7,
        SELECT_FIRE = 8,
        SELECT_RING = 9,
        SELECT_NONE = 10,
    };

    void setSelection(SelectionType_e type, u16 id, bool restricted);

    bool isCurrentSelectionRestricted() const {
        return mCurrentSelectionIsRestricted;
    }
    SelectionType_e getCurrentSelectionType() const {
        return mCurrentSelectionType;
    }

    s32 getCurrentSelectionId() const {
        return mCurrentSelectionId;
    }

    u8 getCurrentSelectionTab() const {
        return mCurrentSelectionTab;
    }

    void setCurrentSelectionTab(u8 tab) {
        mCurrentSelectionTab = tab;
    }

    nw4r::lyt::Bounding *getArrowBounding(int idx) const;
    void setSelectedArrowBounding(int idx);

    static void setSelectHand(f32 rotation, f32 length);

    void preDrawStage();
    void postDrawStage();

    void inRequest();

    static f32 sDisp00ArrowRotation;
    static f32 sDisp00ArrowLength;

private:
    bool checkSelectRing();
    bool checkChangeDisp();
    bool checkRing();
    void changeSavedDisp();
    void saveTabFlag();

    STATE_FUNC_DECLARE(dLytPauseMgr_c, None);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, In);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, Change);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, Select);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, Ring);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, GetDemo);
    STATE_FUNC_DECLARE(dLytPauseMgr_c, Out);

    static dLytPauseMgr_c *sInstance;

    /* 0x008C */ UI_STATE_MGR_DECLARE(dLytPauseMgr_c);
    /* 0x00C8 */ d2d::ResAccIf_c mResAcc1;
    /* 0x0438 */ d2d::ResAccIf_c mResAcc2;
    /* 0x07A8 */ dLytPauseMain_c mMain;

    /* 0x0810 */ EGG::TextureBuffer *mpTexture0x0810;
    /* 0x0814 */ EGG::TextureBuffer *mpBgTexture;
    /* 0x0818 */ SelectionType_e mCurrentSelectionType;
    /* 0x081C */ SelectionType_e mPrevSelectionType;
    /* 0x0820 */ s32 mCurrentDisp00Tab;
    /* 0x0824 */ s32 mCurrentDisp00HeldRing;
    /* 0x0828 */ s32 mSavedPauseDisp;
    /* 0x082C */ u16 mCurrentSelectionId;
    /* 0x082E */ u16 mPrevSelectionId;
    /* 0x0830 */ bool mInRequest;
    /* 0x0831 */ bool mIsNavLeft;
    /* 0x0832 */ bool field_0x0832;
    /* 0x0833 */ u8 mCurrentSelectionTab;
    /* 0x0834 */ u8 mTimer;
    /* 0x0835 */ u8 field_0x0835;
    /* 0x0836 */ bool field_0x0836;
    /* 0x0837 */ bool field_0x0837;
    /* 0x0838 */ bool field_0x0838;
    /* 0x0839 */ bool field_0x0839;
    /* 0x083A */ bool field_0x083A;
    /* 0x083B */ bool field_0x083B;
    /* 0x083C */ bool field_0x083C;
    /* 0x083D */ bool field_0x083D;
    /* 0x083E */ bool field_0x083E;
    /* 0x083F */ bool field_0x083F;
    /* 0x0840 */ bool field_0x0840;
    /* 0x0841 */ bool mCurrentSelectionIsRestricted;
};

#endif
