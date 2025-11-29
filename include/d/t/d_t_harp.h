#ifndef D_T_HARP_H
#define D_T_HARP_H

#include "common.h"
#include "d/snd/d_snd_source_harp_related.h"
#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgHarp_c : public dTg_c {
public:
    dTgHarp_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgHarp_c() {}

    STATE_FUNC_DECLARE(dTgHarp_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgHarp_c);
    /* 0x138 */ byte_t field_0x138;
    /* 0x139 */ byte_t field_0x139;
    /* 0x13A */ s16 field_0x13a;
    /* 0x13C */ u16 mTimer_0x13c;
    /* 0x13E */ byte_t sceneflag_0x13e;

    virtual bool createHeap() override;  // fn_536_420
    virtual int create() override;       // fn_536_430
    virtual int doDelete() override;     // fn_536_530
    virtual int actorExecute() override; // fn_536_540
    virtual int draw() override;         // fn_536_580
    void fn_536_6C0();                   // fn_536_6C0
    void fn_536_720(dSndSourceHarpRelated_c *soundSource);
};

#endif
