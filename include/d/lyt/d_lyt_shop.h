#ifndef D_LYT_SHOP_H
#define D_LYT_SHOP_H

#include "d/d_cursor_hit_check.h"
#include "d/d_pad_nav.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_base.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_common_icon_material.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

class dLytShopMain_c {
public:
    enum Service_e {
        SERVICE_POTION_INFUSE,
        SERVICE_GEAR_UPGRADE,
        SERVICE_SHIELD_REPAIR,
    };

private:
    enum Phase_e {
        PHASE_SELECT_ITEM,
        PHASE_MATERIAL_CHECK,
    };

public:
    dLytShopMain_c();
    virtual ~dLytShopMain_c() {
        dPadNav::setNavEnabled(false, false);
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool execute();
    bool draw();
    bool remove();

    bool isIdle() const {
        return mIsIdle;
    }

    s32 getService() const {
        return mService;
    }

    bool getField_0x116DD() const {
        return field_0x116DD;
    }

    bool getField_0x116DE() const {
        return field_0x116DE;
    }

    void retry() {
        mIsIdle = false;
    }

    void setModeNone();
    void setModeSelectIn();
    void setModeSelectCheck();
    void setModeSelectOut();
    void setModeOutIn();
    void setModeMaterialCheck();
    void setModeOut();
    void setModeInOut();

private:
    static const s32 NUM_ITEMS = 12;

    void buildSubpanes();
    void handleNavigation();
    s32 checkNav();
    void buildMaterialCheckIcons(d2d::ResAccIf_c *resAcc, u8 variant);
    
    void addItemToDisplayed(s32 itemIdx, s32 itemKind, s32 pouchSlot);
    void updateShopStuffMaybe();
    void prepareRemodelStoreStuff(s32 service);
    void showShopLytMaybe();
    void showPriceMaybe(s32 value);
    
    s32 getItemTier(s32 item);
    s32 calcNumDigits(s32 value);

    void fn_802A5CA0(bool);
    void fn_802A5BD0();
    s32 fn_802A4490(s32);
    s32 fn_802A4500(s32);
    s32 fn_802A5760(u8);
    s32 fn_802A5780(u8);

    STATE_FUNC_DECLARE(dLytShopMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeSelectIn);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeSelectCheck);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeSelectOut);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeOutIn);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeMaterialCheck);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeOut);
    STATE_FUNC_DECLARE(dLytShopMain_c, ModeInOut);

    /* 0x00004 */ UI_STATE_MGR_DECLARE(dLytShopMain_c);
    /* 0x00040 */ d2d::LytBase_c mLyts[3];
    /* 0x001F0 */ d2d::AnmGroup_c mAnmItemSelect[9];
    /* 0x00430 */ d2d::AnmGroup_c mAnmMaterialCheck[18];
    /* 0x008B0 */ d2d::AnmGroup_c mAnmShopInfo[2];
    /* 0x00930 */ nw4r::lyt::Bounding *mpItemBoundings[13];
    /* 0x00964 */ nw4r::lyt::Bounding *mpMaterialCheckChoiceBoundings[2];
    /* 0x0096C */ dCursorHitCheckLyt_c mCsHitChecks[3];
    /* 0x009E4 */ dLytCommonIconItem_c mBWheelOrPouchItemIcons[24];
    /* 0x0CAA4 */ dLytCommonIconItem_c mUpgradePreviewIcons[4];
    /* 0x0EAC4 */ dLytCommonIconMaterial_c mUpgradeCostIcons[8];
    /* 0x11484 */ d2d::SubPaneList mItemList;
    /* 0x11490 */ d2d::SubPaneListNode mBWheelOrPouchNodes[24];
    /* 0x11610 */ d2d::SubPaneList mUpgradeList;
    /* 0x1161C */ d2d::SubPaneListNode mUpgradeNodes[12];
    /* 0x116DC */ bool mIsIdle;
    /* 0x116DD */ bool field_0x116DD;
    /* 0x116DE */ bool field_0x116DE;
    /* 0x116DF */ bool mUpgradeLoaded;
    /* 0x116E0 */ bool mPrevPointerVisible;
    /* 0x116E1 */ bool field_0x116E1;
    /* 0x116E4 */ s32 mPhase;
    /* 0x116E8 */ s32 mCurrentNavTarget;
    /* 0x116EC */ s32 mStateStep;
    /* 0x116F0 */ s32 mUpgradeIdx;
    /* 0x116F4 */ s32 mService;
    /* 0x116F8 */ s32 mItemCount;
    /* 0x116FC */ s32 mUpgradeCostCount;
    /* 0x11700 */ UNKWORD field_0x11700;
    /* 0x11704 */ s32 mItemUpgradeIdxes[NUM_ITEMS];
    /* 0x11734 */ s32 mItemPouchSlots[NUM_ITEMS];
    /* 0x11764 */ u8 mItemKinds[NUM_ITEMS];
    /* 0x11770 */ u8 field_0x11770[8];
};

class dLytShop_c : public dLytBase_c {
public:
    dLytShop_c() : mStateMgr(*this) {}
    virtual ~dLytShop_c() {
        removeLyt();
    }

    static dLytShop_c *GetInstance() {
        return sInstance;
    }

    virtual bool build() override;
    virtual int create() override;
    virtual int execute() override;
    virtual int draw() override;
    virtual int doDelete() override;

private:
    static dLytShop_c *sInstance;

    STATE_FUNC_DECLARE(dLytShop_c, None);
    STATE_FUNC_DECLARE(dLytShop_c, SelectIn);
    STATE_FUNC_DECLARE(dLytShop_c, SelectCheck);
    STATE_FUNC_DECLARE(dLytShop_c, SelectOut);
    STATE_FUNC_DECLARE(dLytShop_c, OutIn);
    STATE_FUNC_DECLARE(dLytShop_c, MaterialCheck);
    STATE_FUNC_DECLARE(dLytShop_c, Out);
    STATE_FUNC_DECLARE(dLytShop_c, InOut);
    STATE_FUNC_DECLARE(dLytShop_c, End);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dLytShop_c);

    /* 0x0008C */ UI_STATE_MGR_DECLARE(dLytShop_c);
    /* 0x000C8 */ d2d::ResAccIf_c mResAcc;
    /* 0x00438 */ dLytShopMain_c mMain;
    /* 0x11BB0 */ bool mInRequest;
    /* 0x11BB1 */ bool mIsMovingOut;
};

#endif
