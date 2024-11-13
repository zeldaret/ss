#ifndef D_BG_S_GND_CHK_H
#define D_BG_S_GND_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_gnd_chk.h"
#include "d/col/c/c_m3d_g_pla.h"

class dBgS_GndChk : public cBgS_GndChk, public dBgS_Chk {
public:
    dBgS_GndChk();
    virtual ~dBgS_GndChk();

    void Set(const mVec3_c *pos, u32 *id);
};

class dBgS_LinkGndChk : public dBgS_GndChk {
public:
    dBgS_LinkGndChk();
    virtual ~dBgS_LinkGndChk();
};

class dBgS_ObjGndChk : public dBgS_GndChk {
public:
    dBgS_ObjGndChk();
    virtual ~dBgS_ObjGndChk();

    static dBgS_ObjGndChk &GetInstance() {
        return sInstance;
    }
    static f32 GetGroundHeight() {
        return sGroundHeight;
    }

    static void ClearInstance();
    static bool CheckPos(const mVec3_c &);
    static int GetMaterial();
    static int GetPolyAtt1();
    static bool GetTriPlane(cM3dGPla *);
    static s32 GetRoomID();
    static bool ChkMoveBG();
    static int GetLightingCode();
    static int GetSpecialCode();

private:
    static dBgS_ObjGndChk sInstance;
    static f32 sGroundHeight;
};

class dBgS_ObjGndChk_Wtr : public dBgS_ObjGndChk {
public:
    dBgS_ObjGndChk_Wtr();

    virtual ~dBgS_ObjGndChk_Wtr();
};

class dBgS_ObjGndChk_Spl : public dBgS_ObjGndChk {
public:
    dBgS_ObjGndChk_Spl();

    virtual ~dBgS_ObjGndChk_Spl();
};

class dBgS_ObjGndChk_All : public dBgS_ObjGndChk {
public:
    virtual ~dBgS_ObjGndChk_All();

    dBgS_ObjGndChk_All();
};

class dBgS_CamGndChk : public dBgS_GndChk {
public:
    dBgS_CamGndChk();

    virtual ~dBgS_CamGndChk();
};

class dBgS_CamGndChk_Wtr : public dBgS_CamGndChk {
public:
    dBgS_CamGndChk_Wtr();

    virtual ~dBgS_CamGndChk_Wtr();
};

#endif
