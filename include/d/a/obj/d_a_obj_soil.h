#ifndef D_A_OBJ_SOIL_H
#define D_A_OBJ_SOIL_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/dowsing_target.h"

class dAcTbox_c;
class dTgInsect_c;

class dAcOsoil_c : public dAcObjBase_c {
public:
    dAcOsoil_c() : mStateMgr(*this, sStateID::null), mResFile(nullptr), mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dAcOsoil_c() {}

    STATE_FUNC_DECLARE(dAcOsoil_c, Soil);
    STATE_FUNC_DECLARE(dAcOsoil_c, Hole);
    STATE_FUNC_DECLARE(dAcOsoil_c, Ready);

    enum Subtype {
        SUBTYPE_VENT, // used by instances spawned by dAcOWind_c
        SUBTYPE_TBOX, // unused?
        SUBTYPE_DROP, // you can dig up items from this one
        SUBTYPE_MOGMA, // when dug up, a mogma appears
        SUBTYPE_KEY_PIECE,
    };

    enum Itemtype {
        ITEMTYPE_KEY_PIECE,
        ITEMTYPE_DIRECT_GIVE, // used for the small key in Skyview 2
        ITEMTYPE_DROP,
    };

    enum ModelIndex {
        MODEL_COVERED,
        MODEL_DUG_UP,
    };

    bool isStateHole() {
        return mStateMgr.isState(StateID_Hole);
    }

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOsoil_c);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModels[2];
    /* 0x36C */ dBgW mActorInteraction;
    /* 0x57C */ dCcD_Cyl mCollider;
    /* 0x6CC */ STATE_MGR_DECLARE(dAcOsoil_c);
    /* 0x708 */ dAcRef_c<dAcTbox_c> mTboxLink;
    /* 0x714 */ dAcRef_c<dTgInsect_c> mInsectLink;
    /* 0x720 */ DowsingTarget mDowsingTarget;
    /* 0x740 */ GXColor mColors[2];
    /* 0x748 */ u8 field_0x748[0x778 - 0x748]; // unused
    /* 0x778 */ mVec3_c mItemAndInsectSpawnPos;
    /* 0x784 */ u16 mFinalDigTimer;
    /* 0x786 */ u8 mModelIndex;
    /* 0x787 */ u8 mSubtype;
    /* 0x788 */ u8 mSceneflag;
    /* 0x789 */ u8 mSpawnSceneflag;
    /* 0x78A */ u8 mCountInParams;
    /* 0x78B */ u8 mDigsLeft;
    /* 0x78C */ u8 mItemSubtype;
    /* 0x78D */ u8 mSomeCounter1;
    /* 0x78E */ u8 mDowsingStateRelated;
    /* 0x78F */ bool mHasTriedTboxLink;
    /* 0x790 */ bool mInteractionDisabled;
    /* 0x791 */ bool mTboxPrepared;
    /* 0x792 */ bool mForInsects;
    /* 0x793 */ bool field_0x793;
    /* 0x794 */ bool mAlwaysLoaded;
    /* 0x795 */ bool mIsHidden;
    /* 0x796 */ bool mHideCover;
    /* 0x797 */ bool field_0x797; // unused
    /* 0x798 */ bool mShouldSetFlagInEvent;
    /* 0x799 */ bool mHideWhenCameraNear;

    static dCcD_SrcCyl sCylSrc;

    bool isSubtypeVentOrDrop() const {
        return mSubtype == SUBTYPE_VENT || mSubtype == SUBTYPE_DROP;
    }

    s32 getSubtype() const {
        return mSubtype;
    }

    virtual int doDelete() override;
    virtual int actorCreate() override;
    virtual bool createHeap() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    bool checkCoHit(bool*);
    void giveItemRepeated();
    void calcItemAndInsectSpawnPos();
    void tryLinkTbox();
    void giveItemFinal();
    void fn_255_1A50();
    void setModelColorFromParams(GXColor* pOutColor, m3d::smdl_c& mdl);
    void fn_255_1D80();
    void setModelAlphaToGlobalAlpha();
    void handleModelsOpacities();
    void test();
};

#endif
