#ifndef D_BG_S_LIN_CHK_H
#define D_BG_S_LIN_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_lin_chk.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "m/m_angle.h"

class dAcObjBase_c;

class dBgS_LinChk : public cBgS_LinChk, public dBgS_Chk {
public:
    dBgS_LinChk();
    void Set(mVec3_c const *pStart, mVec3_c const *pEnd, dAcObjBase_c const *pObj);

    virtual ~dBgS_LinChk();

    /* 0x00 cBgS_LinChk */;
    /* 0x58 dBgS_Chk */;
};

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

class dBgS_ObjLinChk : public dBgS_LinChk {
public:
    dBgS_ObjLinChk();

    virtual ~dBgS_ObjLinChk();
    virtual void SetEnd(const mVec3_c *) override;

    static dBgS_ObjLinChk &GetInstance() {
        return sInstance;
    }

    static void ClearInstance();
    static bool LineCross(mVec3_c const *pStart, mVec3_c const *pEnd, dAcObjBase_c const *pObj);
    static mAng GetAngle();
    static bool GetTriPla(cM3dGPla *);
    static int GetMaterial();
    static bool ChkMoveBG();
    static bool ChkGround();
    static bool ChkWall();
    static bool ChkRoof();

private:
    static dBgS_ObjLinChk sInstance;
};

class dBgS_LinkLinChk : public dBgS_LinChk {
public:
    dBgS_LinkLinChk();

    virtual ~dBgS_LinkLinChk();
};

class dBgS_BombLinChk : public dBgS_LinChk {
public:
    dBgS_BombLinChk();

    virtual ~dBgS_BombLinChk();
};

class dBgS_ArrowLinChk : public dBgS_LinChk {
public:
    dBgS_ArrowLinChk();

    virtual ~dBgS_ArrowLinChk();
};

class dBgS_BeetleLinChk : public dBgS_LinChk {
public:
    dBgS_BeetleLinChk();

    virtual ~dBgS_BeetleLinChk();
};

class dBgS_ClawshotLinChk : public dBgS_LinChk {
public:
    dBgS_ClawshotLinChk();

    virtual ~dBgS_ClawshotLinChk();
};

class dBgS_GustBellowsLinChk : public dBgS_LinChk {
public:
    dBgS_GustBellowsLinChk();

    virtual ~dBgS_GustBellowsLinChk();
};

class dBgS_WhipLinChk : public dBgS_LinChk {
public:
    dBgS_WhipLinChk();

    virtual ~dBgS_WhipLinChk();
};

class dBgS_WtrLinChk : public dBgS_LinChk {
public:
    dBgS_WtrLinChk();
    virtual ~dBgS_WtrLinChk();
    virtual void SetEnd(const mVec3_c *) override;

    static dBgS_WtrLinChk &GetInstance() {
        return sInstance;
    }
    static void ClearInstance();
    static bool SetIsWater(mVec3_c const *pStart, mVec3_c const *pEnd, dAcObjBase_c const *pObj);

private:
    static dBgS_WtrLinChk sInstance;
};

#endif
