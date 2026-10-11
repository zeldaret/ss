#ifndef D_T_PLIGHT_H
#define D_T_PLIGHT_H

#include "d/d_light_env.h"
#include "d/t/d_tg.h"
#include "m/m_color.h"

class dTgPlight_c : public dTg_c {
public:
    dTgPlight_c() : field_0x2D4(-1) {}
    virtual ~dTgPlight_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

private:
    // Unknown 0x14-byte element; only its ctor/dtor are emitted in this REL
    struct UnkEntry {
        UnkEntry() {}
        ~UnkEntry() {}

        /* 0x00 */ u8 _0x00[0x14];
    };

    /* 0x0FC */ LIGHT_INFLUENCE mLight;
    /* 0x118 */ UnkEntry mEntries[0x16];
    /* 0x2D0 */ u8 _0x2D0[0x2D4 - 0x2D0];
    /* 0x2D4 */ mColor field_0x2D4;
    /* 0x2D8 */ u8 mType;
    /* 0x2D9 */ u8 _0x2D9[0x2E0 - 0x2D9];
};

#endif
