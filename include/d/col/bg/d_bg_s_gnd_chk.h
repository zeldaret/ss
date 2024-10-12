#ifndef D_BG_S_GND_CHK_H
#define D_BG_S_GND_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_gnd_chk.h"

class dBgS_GndChk : public cBgS_GndChk, public dBgS_Chk {
public:
    dBgS_GndChk();
    virtual ~dBgS_GndChk();
};

class dBgS_LinkGndChk : public dBgS_GndChk {
public:
    dBgS_LinkGndChk() {
        // SetLink();
    }
    virtual ~dBgS_LinkGndChk() {}
};

class dBgS_ObjGndChk : public dBgS_GndChk {
public:
    dBgS_ObjGndChk();
    virtual ~dBgS_ObjGndChk();

    dBgS_ObjGndChk &GetInstance();
    static void CtInstance();

private:
    static dBgS_ObjGndChk sInstance;
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

    dBgS_ObjGndChk_All() {
        // OnAll();
    }
};

class dBgS_CamGndChk : public dBgS_GndChk {
public:
    dBgS_CamGndChk() {
        // SetCam();
    }

    virtual ~dBgS_CamGndChk();
};

class dBgS_CamGndChk_Wtr : public dBgS_CamGndChk {
public:
    dBgS_CamGndChk_Wtr();

    virtual ~dBgS_CamGndChk_Wtr();
};

#endif
