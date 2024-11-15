#ifndef D_A_OBJ_TBOX_H
#define D_A_OBJ_TBOX_H

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
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
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
};

class dAcTbox_c : public dAcObjBase_c {
public:
    dAcTbox_c();
    virtual ~dAcTbox_c();

    virtual bool createHeap() override;

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
    bool initBgW(dBgW &bg, const char *arcName, const char *ccName);
    bool isNotSmall() const;
    void fn_8026B380(mVec3_c &outResult) const;
    const mVec3_c &fn_8026B3C0() const;

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
    s32 field_0x4DC;
    s32 field_0x4E0;
    s32 field_0x4E4;
    s32 field_0x4E8;
    s32 field_0x4EC;
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
    /* 0x1150 */ DowsingTarget mDowsingTarget1;
    /* 0x1170 */ DowsingTarget mDowsingTarget2;

    /* 0x1204 */ bool mHasBeenOpened;
    /* 0x1209 */ u8 mVariant;

    static fLiMgBa_c sTboxActorList;
};

#endif
