#ifndef D_A_BOMB_H
#define D_A_BOMB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/effects_struct.h>

// TODO
class dAcBomb_c : public dAcObjBase_c {
public:

    u8 field_0x330[0x9D4 - 0x330];
    /* 0x9D4 */ EffectsStruct mEffects1;
    /* 0xA08 */ EffectsStruct mEffects2;
    u8 field_0xA3C[0xA44 - 0xA3C];
    s16 field_0xA44;
    u8 field_0xA46[0xA50 - 0xA46];
    f32 field_0xA50;
    u8 field_0xA54[0xA9C - 0xA54];
    /* 0xA9C */ STATE_MGR_DECLARE(dAcBomb_c);

    STATE_FUNC_DECLARE(dAcBomb_c, FlowerWait);

    void setTransformFromFlower(const mMtx_c &);
};

#endif
