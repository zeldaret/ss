#ifndef D_A_OBJ_GIRAHIMU_FLOOR_H
#define D_A_OBJ_GIRAHIMU_FLOOR_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcGirahimu3First_c;
class dAcOGirahimuFloor_c : public dAcObjBase_c {
public:
    dAcOGirahimuFloor_c() : mStateMgr(*this) {}
    virtual ~dAcOGirahimuFloor_c() {}

    enum Type_e {
        TYPE_Normal,
        TYPE_Generate,
        TYPE_Max
    };

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Wait);
    STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Return);
    STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Generate);
    STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Broken);
    STATE_FUNC_DECLARE(dAcOGirahimuFloor_c, Move);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOGirahimuFloor_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOGirahimuFloor_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOGirahimuFloor_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcOGirahimuFloor_c);

    // Used to request deletion upon both Player and Boss off the platform
    bool isPlayerAndGhirahimOffFloor();

    // If it is the generate type and there is no linked child
    bool isGenerateNoChild();

    // Checks if Link is in a final blow state and has connected
    // with something other than the platform (like ghirahim)
    bool isSuccessfulFinalBlow();

    // Moves the position to the target destinaton
    void movePosition();

    // Breaks the platform, releases mBg and requests deletion
    void doShatter();

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBg[3];
    /* 0x980 */ STATE_MGR_DECLARE(dAcOGirahimuFloor_c);
    /* 0x9BC */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x9E8 */ m3d::anmMatClr_c mAnmMatClr[3];
    /* 0xA6C */ dAcRef_c<dAcGirahimu3First_c> mGhirahimRef;
    /* 0xA78 */ dAcRef_c<dAcOGirahimuFloor_c> mChild;
    /* 0xA64 */ mVec3_c mHomePos;
    /* 0xA90 */ mVec3_c mTargetPos;
    /* 0xA9C */ mVec3_c mScaleBg0;
    /* 0xAA8 */ mVec3_c mScaleBg1;
    /* 0xAB4 */ f32 mMoveRate;
    /* 0xAB4 */ f32 mGndHeight;
    /* 0xABC */ u8 mType;
    /* 0xABD */ u8 field_0xABD;
    /* 0xABE */ u8 field_0xABE;
    /* 0xAC0 */ s32 field_0xAC0;
    /* 0xAC4 */ bool mbIsHome;
    /* 0xAC5 */ bool mbMediumSize;
    /* 0xAC6 */ bool mbRequestShatter;
    /* 0xAC7 */ bool mbRequestMove;
};

#endif
