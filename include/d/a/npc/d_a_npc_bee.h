#ifndef D_A_NPC_BEE_H
#define D_A_NPC_BEE_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "m/m3d/m_proc.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcNpcBeeSingleBee {
public:
    enum State {
        BEE_STATE_DEAD = 0,
        BEE_STATE_CHASE = 1,
        BEE_STATE_FLY_UP = 2,
        BEE_STATE_FLY_UP2 = 3,
        BEE_STATE_UNK4 = 4,
        BEE_STATE_RISE = 5,
        BEE_STATE_CRAWL = 0xA,
    };
    /* 0x000 */ u8 mIndex;
    /* 0x001 */ u8 mBeeState; // see State enum
    /* 0x002 */ u8 field_0x002;
    /* 0x003 */ u8 field_0x003;
    /* 0x004 */ u8 field_0x004;
    /* 0x005 */ u8 field_0x005;
    /* 0x006 */ u8 field_0x006;
    /* 0x007 */ u8 field_0x007;
    /* 0x008 */ u8 field_0x008;
    /* 0x009 */ u8 field_0x009[0xc-9];
    /* 0x00C */ s16 mRotY;
    /* 0x00E */ s16 field_0x00E;
    /* 0x010 */ u8 field_0x010[0x18-0x10];
    /* 0x018 */ mVec3_c mPos;
    /* 0x024 */ u8 field_0x024[0x3C-0x24];
    /* 0x03C */ mVec3_c field_0x03C;
    /* 0x048 */ mVec3_c field_0x048;
    /* 0x054 */ mVec3_c field_0x054;
    /* 0x060 */ mAng3_c field_0x060;
    /* 0x066 */ u8 field_0x066[0x6C-0x66];
    /* 0x06C */ dCcD_Linked_Sph mCollider;
};


class dAcNpcBee_c : public dAcEnBase_c {
public:
    dAcNpcBee_c(): mRes(nullptr) {}
    virtual ~dAcNpcBee_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int execute() override;
    void updateSwarmBeeColliders();
    void actuallyUpdateSwarmBees();
    void fn_14_1F40(dAcNpcBeeSingleBee* bee);
    u32 fn_14_2690(dAcNpcBeeSingleBee* bee);

private:
    /* 0x0378 */ u8 mSwarmBeeCount;
    /* 0x0379 */ u8 field_0x0379; // rumble timer
    /* 0x037A */ u8 mSceneflag;
    /* 0x037B */ u8 field_0x037B;
    /* 0x037C */ s16 field_0x037C;
    /* 0x037E */ u8 field_0x037E[0x388-0x37E];
    /* 0x0388 */ nw4r::g3d::ResFile mRes;
    /* 0x038C */ dShpProc1_c mBeeModels[4];
    /* 0x045C */ dAcNpcBeeSingleBee mBees[100];
    /* 0xB80C */ dAcRef_c<dAcObjBase_c> mHiveRef;
    /* 0xB818 */ dAcRef_c<dAcObjBase_c> mAttackRef;
    /* 0xB824 */ dColliderLinkedList mColliderList;
    /* 0xB830 */ mVec3_c field_0xB830;
    /* 0xB834 */ dAcObjBase_c* mAttackActor;
    /* 0xB838 */ u8 field_0xB838[0xB844-0xB838];
};

#endif
