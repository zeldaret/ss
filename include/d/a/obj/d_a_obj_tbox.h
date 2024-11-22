#ifndef D_A_OBJ_TBOX_H
#define D_A_OBJ_TBOX_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/ut/ut_Color.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/stage_render_stuff.h"

// Somewhere in d_cc
class dAcTboxCcD : public dCcD_Unk {
public:
    dAcTboxCcD() : mpList(nullptr), field_0x210(this) {}
    virtual ~dAcTboxCcD() {
        if (mpList != nullptr) {
            mpList->remove(&mNode);
            mpList = nullptr;
        }
    }
    /* 0x00 */ cListNd_c mNode;
    /* 0x08 */ dCcD_Unk *field_0x210;
    /* 0x0C */ cListMg_c *mpList;
};

struct TboxAndMoreUnkCC {
    cListMg_c mList;
    virtual ~TboxAndMoreUnkCC();

    void addCc(dAcTboxCcD &ccD, const dCcD_SrcUnk &src);
    void SetStts(cCcD_Stts &stts);
    void registerColliders();
};

class dAcTbox_c : public dAcObjBase_c {
public:
    enum TboxVariant_e {
        NORMAL,
        SMALL,
        BOSS,
        GODDESS,
    };

    dAcTbox_c();
    virtual ~dAcTbox_c();

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    virtual void registerInEvent() override;
    virtual void unkVirtFunc_0x6C() override;

    virtual void *getObjectListEntry() override;

    STATE_FUNC_DECLARE(dAcTbox_c, DugOut);
    STATE_FUNC_DECLARE(dAcTbox_c, WaitAppear);
    STATE_FUNC_DECLARE(dAcTbox_c, DemoAppear);
    STATE_FUNC_DECLARE(dAcTbox_c, WaitOpen);
    STATE_FUNC_DECLARE(dAcTbox_c, GoddessWaitOff);
    STATE_FUNC_DECLARE(dAcTbox_c, GoddessWaitOn);
    STATE_FUNC_DECLARE(dAcTbox_c, DeleteArchive);
    STATE_FUNC_DECLARE(dAcTbox_c, LoadArchive);
    STATE_FUNC_DECLARE(dAcTbox_c, Open);
    STATE_FUNC_DECLARE(dAcTbox_c, PresentItem);
    STATE_FUNC_DECLARE(dAcTbox_c, Close);
    STATE_FUNC_DECLARE(dAcTbox_c, Wait);
    STATE_FUNC_DECLARE(dAcTbox_c, GoddessWait);

private:
    static f32 getSomeRate();
    bool initBgW(dBgW &bg, const char *arcName, const char *fileName);
    bool isNotSmall() const;
    void setItemId(u16 item);
    void setChestFlag();
    void setDoObstructionCheck();
    int isActualVisibleBox() const;
    void fn_8026B380(mVec3_c &outResult) const;
    const mVec3_c &fn_8026B3C0() const;

    bool noObstructionCheck() const;
    int fn_8026B370() const;
    bool fn_8026D670() const;
    bool getSomeCounter(u32 *outIndex) const;
    bool checkTboxFlag() const;
    bool fn_8026D560() const;
    bool isItemRupee() const;

    void getDowsingTargetOffset(mVec3_c *offset) const;
    void initDowsingTarget(DowsingTarget *target, DowsingTarget::DowsingSlot slot, mVec3_c *offset);
    void initDowsingTarget(DowsingTarget::DowsingSlot slot);
    void initDowsingTargetCube();

    void registerRupeeOrTreasureDowsing();
    void registerKeyPieceDowsing();
    void unregisterDowsing();
    void noRegisterDowsing();
    void noUnregisterDowsing();

    static bool fn_80268660(int arg);
    bool checkForLinkBonk();
    void fn_8026E630();
    bool fn_8026D540();
    bool fn_8026D3C0();
    void syncScaleToMdl(m3d::scnLeaf_c *lf);
    void getCCBounds(mVec3_c *out1, mVec3_c *out2) const;
    void getCylParams(mVec3_c *out1, f32 *out2, f32 *out3) const;
    void fn_8026DAD0(mVec3_c *src, mVec3_c *dest) const;
    void fn_8026DAC0(mAng& ang);

    static bool hasCollectedAllTears();
    static bool isValidVariant(int variant);

    void setActionState();

    void setFlags(u32 flags);

    bool fn_8026D120() const;
    void fn_8026D130();
    void fn_8026D140();
    void fn_8026D950();

    bool checkIsClear() const;
    static bool fn_802686F0(const mVec3_c &vec, f32 y);
    const InteractionTargetDef &getInteractionTargetDef() const;

    /* 0x0330 */ m3d::mdlAnmChr mMdl1;
    /* 0x0398 */ m3d::smdl_c mOpenFxMdl;
    /* 0x03B4 */ m3d::anmMatClr_c mAnmMatClr1;
    /* 0x03E0 */ m3d::anmMatClr_c mAnmMatClr2;
    /* 0x040C */ m3d::anmChr_c mAnmChr;
    /* 0x0444 */ m3d::anmTexSrt_c mAnmTexSrt1;
    /* 0x0470 */ m3d::anmTexSrt_c mAnmGoddessTexSrt;
    /* 0x049C */ m3d::anmTexPat_c mAnmGoddessPat;
    /* 0x04C8 */ dScnCallback_c mScnCallback;
    /* 0x04D4 */ dShadowCircle_c mShadowCircle;

    // Could be part of an aggregate structure
    mVec3_c field_0x4DC;
    nw4r::ut::Color field_0x4E8;
    f32 field_0x4EC;
    s32 field_0x4F0;
    s32 field_0x4F4;

    /* 0x04F8 */ dBgW mBgWs[2];
    /* 0x0918 */ dAcTboxCcD mCcD1;
    /* 0x0B30 */ dAcTboxCcD mCcD2;
    /* 0x0D48 */ TboxAndMoreUnkCC field_0x0D48;
    /* 0x0D54 */ dCcD_Unk mCcD3;
    /* 0x0F5C */ dCcD_Cyl mCcD4;
    /* 0x10AC */ STATE_MGR_DECLARE(dAcTbox_c);
    /* 0x10E8 */ fLiNdBa_c mTboxListNode;
    /* 0x10F4 */ ActorEventRelated mEvent;
    /* 0x1144 */ dAcRef_c<dAcItem_c> mItemRef;
    /* 0x1150 */ DowsingTarget mDowsingTarget;
    /* 0x1170 */ DowsingTarget mGoddessDowsingTarget;
    /* 0x1190 */ void (dAcTbox_c::*mRegisterDowsingTarget)();
    /* 0x119C */ void (dAcTbox_c::*mUnregisterDowsingTarget)();

    /* 0x11A8 */ u8 field_0x11A8[0x11C0 - 0x11A8];

    /* 0x11C0 */ mVec3_c field_0x11C0;
    /* 0x11CC */ mVec3_c field_0x11CC;

    /* 0x11D8 */ mVec3_c field_0x11D8;

    /* 0x11E4 */ u8 field_0x11E4[0x11E8 - 0x11E4];
    /* 0x11E8 */ f32 field_0x11E8;
    
    /* 0x11EC */ f32 field_0x11EC;
    /* 0x11F0 */ UNKWORD field_0x11F0;
    /* 0x11F4 */ UNKWORD field_0x11F4;
    
    /* 0x11F8 */ UNKWORD field_0x11F8;
    
    /* 0x11FC */ UNKWORD field_0x11FC;

    /* 0x1200 */ u16 mItemId;

    /* 0x1202 */ u16 mItemModelIdx;
    
    /* 0x1204 */ bool mHasBeenOpened;
    /* 0x1205 */ u8 mSpawnSceneFlag;
    /* 0x1206 */ u8 mSetSceneFlag; // set when?

    /* 0x1207 */ u8 field_0x1207;

    /* 0x1208 */ u8 field_0x1208;
    /* 0x1209 */ u8 mVariant;
    /* 0x120A */ u8 field_0x120A;

    /* 0x120B */ u8 field_0x120B;
    /* 0x120C */ u8 field_0x120C;

    /* 0x120D */ u8 field_0x120D;

    /* 0x120E */ u8 field_0x120E;

    /* 0x120F */ bool field_0x120F;
    /* 0x1210 */ bool field_0x1210;
    /* 0x1211 */ bool mDoObstructedCheck;

    static const cCcD_SrcGObj sColSrc;
    static fLiMgBa_c sTboxActorList;
};

#endif
