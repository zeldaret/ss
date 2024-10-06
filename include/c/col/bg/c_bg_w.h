#ifndef C_BG_W_H
#define C_BG_W_H

#include "common.h"

enum {
    BG_ID_MAX = 600
};

class cBgW_BgId {
private:
    /* 0x0 */ u16 m_id;
    /* 0x4 vtable */

public:
    void Release();
    bool ChkUsed() const;
    void Regist(int);

    virtual ~cBgW_BgId();

    cBgW_BgId();
    void Ct();

    u16 GetId() const {
        return m_id;
    }
};

bool cBgW_CheckBGround(f32 a1);
bool cBgW_CheckBRoof(f32 a1);
bool cBgW_CheckBWall(f32 a1);

#endif
