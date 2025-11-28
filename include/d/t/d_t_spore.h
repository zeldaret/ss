#ifndef D_T_SPORE_H
#define D_T_SPORE_H

#include "d/t/d_tg.h"
#include "egg/egg_types.h"
#include "m/m3d/m_proc.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dSporeProc_c : public m3d::proc_c {
public:
    dSporeProc_c() : mpTexture(nullptr), field_0x39EC(false), field_0x39ED(false), field_0x39EE(0), field_0x39F0(0) {}
    ~dSporeProc_c() {};

    virtual void drawXlu() override;

    void execute();

private:
    /* 0x0018 */ mVec3_c mSporesPos[400];
    /* 0x12D8 */ f32 field_0x12D8[400];
    /* 0x1918 */ f32 field_0x1918[400];
    /* 0x1F58 */ f32 field_0x1F58[400];
    /* 0x2598 */ f32 field_0x2598[400];
    /* 0x2BD8 */ f32 field_0x2BD8[400];
    /* 0x3218 */ mAng field_0x3218[400];
    /* 0x3538 */ mAng field_0x3538[400];
    /* 0x3858 */ s8 field_0x3858[400];
    /* 0x39E8 */ EGG::ResTIMG *mpTexture;
    /* 0x39EC */ bool field_0x39EC;
    /* 0x39ED */ u8 field_0x39ED;
    /* 0x39EE */ s16 field_0x39EE;
    /* 0x39F0 */ u8 field_0x39F0;
    /* 0x39F1 */ u8 _0x39F1[0x3A02 - 0x39F1];
    /* 0x3A02 */ s16 field_0x3A02;
    /* 0x3A04 */ s16 field_0x3A04;
    /* 0x3A06 */ s16 field_0x3A06;

    friend class dSporeTag_c;
};

class dSporeTag_c : public dTg_c {
public:
    dSporeTag_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dSporeTag_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    STATE_FUNC_DECLARE(dSporeTag_c, Wait);

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dSporeTag_c);
    /* 0x138 */ dSporeProc_c mProc;
};

#endif
