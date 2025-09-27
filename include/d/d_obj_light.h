#ifndef D_OBJ_LIGHT_H
#define D_OBJ_LIGHT_H

#include "d/d_base.h"
#include "f/f_base.h"
#include "m/m_color.h"
#include "toBeSorted/d_emitter.h"

struct UnkLightThing /* : Inheritance Chain TODO */ {
    UnkLightThing();
    virtual ~UnkLightThing();

    /* 0x04 */ u8 _0x04[0x40 - 0x04];
};
class dObjLight_c : public dBase_c {
public:
    dObjLight_c();

    virtual ~dObjLight_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

public:
    static bool CreateInstance();
    static dObjLight_c *GetInstance();

    mColor &GetColor0x68();
    mColor &GetColor0x6C();
    mColor &GetColor0x100();
    mColor &GetColor0x104();

private:
    /* 0x068 */ mColor field_0x68[1][3];
    /* 0x074 */ dEmitter_c mEff;
    /* 0x0A8 */ u8 _0xA8[0xB4 - 0xA8];
    /* 0x0B4 */ mColor field_0xB4;
    /* 0x0B8 */ u8 _0xB8[0xD0 - 0xB8];
    /* 0x0D0 */ mColor field_0xD0;
    /* 0x0D4 */ u8 _0xD4[0x100 - 0xD4];
    /* 0x100 */ mColor field_0x100;
    /* 0x104 */ mColor field_0x104;
    /* 0x108 */ u8 _0x108[0x10C - 0x108];
    /* 0x10C */ UnkLightThing field_0x10C[8];

private:
    static dObjLight_c sInstance;
};

#endif
