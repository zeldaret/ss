#ifndef D_T_GEKOTAG_H
#define D_T_GEKOTAG_H

#include "d/a/d_a_base.h"
#include "d/a/e/d_a_e_geko.h"
#include "d/t/d_tg.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgGekoTag : public dTg_c {
public:
    dTgGekoTag() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgGekoTag() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dTgGekoTag, Wait);

private:
    void doExecute();

    s32 getNumGeko() const {
        return (params >> 16) & 0xF;
    }

    s32 getNoSpawnRadiusParm() const {
        return params & 0xFF;
    }

    u8 getGekoParam() const {
        return (params >> 20) & 0x3;
    }

    u8 getParm3() const {
        return (params >> 22) & 0xFF;
    }

    f32 getNoSpawnRadius() const {
        return getNoSpawnRadiusParm() != 0xFF ? getNoSpawnRadiusParm() * 100.0f : 100.0f;
    }

    /* 0x0FC */ STATE_MGR_DECLARE(dTgGekoTag);
    /* 0x138 */ dAcRef_c<dAcEgeko_c> mRefs[15];
    /* 0x1EC */ u8 field_0x1EC[0x1F0 - 0x1EC];
    /* 0x1F0 */ f32 mNoSpawnRadius;
    /* 0x1F4 */ s32 mNumGeko;
    /* 0x1F8 */ u16 mTimer1;
    /* 0x1FA */ u16 mTimer2;
    /* 0x1FC */ u8 mGekoParam;
    // seemingly unused
    /* 0x1FD */ u8 field_0x1FD;
};

#endif
