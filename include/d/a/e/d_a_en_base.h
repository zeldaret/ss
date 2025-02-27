#ifndef D_A_EN_BASE_H
#define D_A_EN_BASE_H

#include "d/a/obj/d_a_obj_base.h"

struct dAcEnData {
    // Unknown Size yet
};

// Ghidra: ActorEnemyBase
//   size:
// non-official name
class dAcEnBase_c : public dAcObjBase_c {
public:
    /* 0x330 */ dAcRef_c<dAcEnBase_c> mEnemyLink;
    /* 0x33C */ u32 mInteractionFlags;
    /* 0x340 */ u16 field_0x338;
    /* 0x342 */ u16 mHealth;
    /* 0x344 */ u8 _344[0x374 - 0x344];
    /* 0x374 */ dAcEnData *mpEnemyData;

protected:
    // TODO: Make accessors for this list to auto-convert to dAcEnBase?
    static fLiMgBa_c sEnemyList;
};

#endif
