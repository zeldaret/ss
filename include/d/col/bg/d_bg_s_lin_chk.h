#ifndef D_BG_S_LIN_CHK_H
#define D_BG_S_LIN_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_lin_chk.h"

class fBase_c;

class dBgS_LinChk : public cBgS_LinChk, public dBgS_Chk {
public:
    dBgS_LinChk();
    void Set(mVec3_c const *pi_start, mVec3_c const *pi_end, fBase_c const *);

    virtual ~dBgS_LinChk();

    /* 0x00 cBgS_LinChk */;
    /* 0x58 dBgS_Chk */;
};

class dBgS_LinkLinChk : public dBgS_LinChk {
public:
    dBgS_LinkLinChk();

    virtual ~dBgS_LinkLinChk();
};

class dBgS_ArrowLinChk : public dBgS_LinChk {
public:
    dBgS_ArrowLinChk();

    virtual ~dBgS_ArrowLinChk();
};

class dBgS_ObjLinChk : public dBgS_LinChk {
public:
    dBgS_ObjLinChk();

    virtual ~dBgS_ObjLinChk();

    dBgS_ObjLinChk &GetInstance();
    static void CtInstance();

    static dBgS_ObjLinChk sInstance;
};

class dBgS_WtrLinChk : public dBgS_LinChk {
public:
    dBgS_WtrLinChk();
    virtual ~dBgS_WtrLinChk();

    dBgS_WtrLinChk &GetInstance();
    static void CtInstance();

    static dBgS_WtrLinChk sInstance;
};

class dBgS_BombLinChk : public dBgS_LinChk {
public:
    dBgS_BombLinChk();

    virtual ~dBgS_BombLinChk();
};

// class dBgS_HorseLinChk : public dBgS_LinChk {
// public:
//     dBgS_HorseLinChk();

// virtual ~dBgS_HorseLinChk();
// };

class dBgS_CamLinChk : public dBgS_LinChk {
public:
    dBgS_CamLinChk();

    virtual ~dBgS_CamLinChk();
};

class dBgS_CamLinChk_NorWtr : public dBgS_CamLinChk {
public:
    dBgS_CamLinChk_NorWtr();

    virtual ~dBgS_CamLinChk_NorWtr();
};

#endif
