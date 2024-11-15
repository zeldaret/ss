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
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/stage_render_stuff.h"

class dAcTboxCcD : public dCcD_Unk {
public:
    dAcTboxCcD() : mpList(nullptr), field_0x210(nullptr) {}
    virtual ~dAcTboxCcD() {
        if (mpList != nullptr) {
            mpList->remove(&mNode);
            mpList = nullptr;
        }
    }
    cListNd_c mNode;
    void *field_0x210;
    cListMg_c *mpList;
};

class dAcTbox_c : public dAcObjBase_c {
public:
    dAcTbox_c();
    virtual ~dAcTbox_c();

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
    /* 0x0330 */ m3d::mdlAnmChr mMdl1;
    /* 0x0398 */ m3d::smdl_c mMdl2;
    /* 0x03B4 */ m3d::anmMatClr_c mAnmMatClr1;
    /* 0x03E0 */ m3d::anmMatClr_c mAnmMatClr2;
    /* 0x040C */ m3d::anmChr_c mAnmChr;
    /* 0x0444 */ m3d::anmTexSrt_c mAnmTexSrt1;
    /* 0x0470 */ m3d::anmTexSrt_c mAnmTexSrt2;
    /* 0x049C */ m3d::anmTexPat_c mAnmTextPat;
    /* 0x04C8 */ dScnCallback_c mScnCallback;
    /* 0x04D4 */ dShadowCircle_c mShadowCircle;
    u8 f1[0x1C];
    /* 0x04F8 */ dBgW mBgWs[2];
    /* 0x0918 */ dAcTboxCcD mCcD1;
    /* 0x0B30 */ dAcTboxCcD mCcD2;
    u8 f2[0xC];
    /* 0x0D54 */ dCcD_Unk mCcD3;
    /* 0x0F5C */ dCcD_Cyl mCcD4;
    /* 0x10AC */ STATE_MGR_DECLARE(dAcTbox_c);
    /* 0x10E8 */ fLiNdBa_c mTboxListNode;
    /* 0x10F4 */ ActorEventRelated mEvent;
    dAcRef_c<dAcItem_c> mItemRef;
    /* 0x1150 */ DowsingTarget mDowsingTarget1;
    /* 0x1170 */ DowsingTarget mDowsingTarget2;

    static fLiMgBa_c sTboxActorList;
};

#endif
