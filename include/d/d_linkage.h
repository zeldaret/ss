#ifndef D_LINKAGE_H
#define D_LINKAGE_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/d_jnt_col.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"

class dAcObjBase_c;
class dBgS_Acch;

// Previously Known as `ActorCarryStruct`
// Size: 0xA8
class dLinkage_c {
public:
    dLinkage_c();

public:
    enum ConnectionType_e {
        CONNECTION_0 = 0,
        CONNECTION_1 = 1,
        CONNECTION_2 = 2,
        CONNECTION_3 = 3,
        CONNECTION_4 = 4,
        CONNECTION_5 = 5,
        CONNECTION_6 = 6,
        CONNECTION_7 = 7,
        CONNECTION_8 = 8,
        CONNECTION_9 = 9,
    };
    enum State_e {
        STATE_0 = 0,
        STATE_ACTIVE = 1,
        STATE_2 = 2, // Hold?
        STATE_3 = 3, // Roll?
        STATE_DELETE = 4,
    };
    enum Flag_e {
    };

public:
    void onFlag(u32 flag) {
        carryFlags |= flag;
    }
    void offFlag(u32 flag) {
        carryFlags &= ~flag;
    }
    bool checkFlag(u32 flag) const {
        return (carryFlags & flag) != 0;
    }

    // Int Required on return -> will use enum when possible
    int getState() const {
        return mState;
    }

    bool checkState(State_e state) const {
        return mState == state;
    }

    bool checkType_1_3_5_6_9() const {
        return mType == CONNECTION_1 || mType == CONNECTION_3 || mType == CONNECTION_5 || mType == CONNECTION_6 ||
               mType == CONNECTION_9;
    }

    bool checkConnection(ConnectionType_e type) const {
        return (checkState(STATE_ACTIVE) && mType == type);
    }

public:
    void set(u32 flags, f32, f32, f32, void *unk);
    bool tryAttach(dAcObjBase_c *, dAcObjBase_c *, dAcRefBase_c *, ConnectionType_e, bool);
    void fn_80050DC0(dAcObjBase_c *, f32, f32, mAng);
    void fn_80050E00(dAcObjBase_c *, f32, f32, mAng);
    void fn_80050E40(dAcObjBase_c *, f32, f32, mAng);
    void fn_80050EA0(dAcObjBase_c *);
    void fn_800511E0(dAcObjBase_c *);
    void bushTpFunc(dBgS_Acch &);
    void fn_80051190(dAcObjBase_c *);
    void fn_80050EB0(dAcObjBase_c *);
    bool fn_800513C0() const;

public:
    /* 0x00 */ dAcRefBase_c mControllingActor;
    /* 0x0C */ dAcRefBase_c *pLinkedActor;
    /* 0x10 */ u32 carryFlags;
    /* 0x14 */ ConnectionType_e mType;
    /* 0x18 */ u16 field_0x18;
    /* 0x1A */ u16 field_0x1A;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ mMtx_c carryTransMtx;
    /* 0x58 */ mMtx_c field_0x58;
    /* 0x88 */ State_e mState;
    /* 0x8C */ f32 field_0x8C;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C vt*/ virtual ~dLinkage_c(); // usually i would like functions sepearte, but this is the placement.
    /* 0xA0 */ dJntCol_c *field_0xA0;
    /* 0xA4 */ u32 field_0xA4;
};

#endif
