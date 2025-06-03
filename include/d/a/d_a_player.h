#ifndef D_A_PLAYER_H
#define D_A_PLAYER_H

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/d_player_mdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/minigame_mgr.h"

// See Below for some info
// https://github.com/lepelog/skywardsword-tools/wiki/Player-Actor

class dAcPy_c : public daPlayerModelBase_c {
public:
    dAcPy_c();
    virtual ~dAcPy_c();

    // argument is always false in existing code, true doesn't seem to make a difference
    void bonk(bool unk = false);
    bool fn_80202D90(bool);
    void fn_80202E00(cBgS_PolyInfo *, f32, f32);

    dAcItem_c *getWhippedItem();

    /* overrides omitted */
    /* vt 0x310 */ virtual void vt_0x310();
    /* vt 0x314 */ virtual void vt_0x314();
    /* vt 0x318 */ virtual void vt_0x318();

protected:
    /* 0x137C */ u8 _0x137C[0x16F0 - 0x137C];
    /* 0x16F0 */ nw4r::g3d::ResFile mHeldResFile;
    /* 0x16F4 */ u8 _0x16F4[0x4564 - 0x16F4];
    /* 0x4564 */ f32 field_0x4564;

public:
    // Beetle Functions [0x8021AA70 - 0x8021BE20]
    f32 getBeetleRadius();

public:
    f32 getField_0x4564() const {
        return field_0x4564;
    }

    f32 getSwordAnmFrame() const {
        return mSwordAnmMatClr.getFrame(0);
    }

    nw4r::g3d::ResFile getSwordResFile() const {
        return mSwordRes;
    }
    nw4r::g3d::ResFile getHeldResFile() const {
        return mHeldResFile;
    }

    inline bool hasvt_0x1C0() const {
        return vt_0x1C0() != nullptr;
    }

    inline bool isAffectedByStaminaPotion() const {
        return FileManager::GetInstance()->hasStaminaPotionNormal() && getRidingActorType() != RIDING_BOAT;
    }

    inline bool isAffectedByStaminaPotionPlus() const {
        return FileManager::GetInstance()->hasStaminaPotionPlus() && getRidingActorType() != RIDING_BOAT;
    }

    static const dAcPy_c *GetLink() {
        return LINK;
    }

    static dAcPy_c *GetLink2() {
        return LINK2;
    }

    void onShieldUpdate();

    static dAcPy_c *LINK;
    static dAcPy_c *LINK2;

    static s32 getCurrentBowType();
    static s32 getCurrentSlingshotType();
    static s32 getCurrentBeetleType();
    static s32 getCurrentBugNetType();

    static bool isInBambooCuttingMinigame() {
        return MinigameManager::isInMinigameState(MinigameManager::BAMBOO_CUTTING);
    }
    static bool isItemRestrictedByBokoBase(ITEM_ID item);

    static u32 getCurrentHealthCapacity();

    bool canDowseProbably() const;

    mAng fn_8023B520() const;
    mAng fn_8023B590() const;
};

#endif
