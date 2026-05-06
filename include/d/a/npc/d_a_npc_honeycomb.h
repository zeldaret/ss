#ifndef D_A_NPC_HONEYCOMB_H
#define D_A_NPC_HONEYCOMB_H

#include "d/a/e/d_a_en_base.h"
#include "m/m_vec.h"

class dAcNpcHc_c : public dAcEnBase_c {
public:
    dAcNpcHc_c() {}
    virtual ~dAcNpcHc_c() {}

// private:
/* 0x378 */ u8 field_0x378[0x915-0x378];
/* 0x915 */ u8 field_0x915;
/* 0x916 */ u8 field_0x916;
/* 0x917 */ u8 field_0x917[0x928-0x917];
/* 0x928 */ mVec3_c field_0x928;
};

#endif
