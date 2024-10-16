#ifndef C_BG_W_H
#define C_BG_W_H

#include "common.h"

enum {
    BG_ID_MAX = 600
};

class cBgW_BgId {
private:
    /* 0x0 */ s16 m_id;
    /* 0x4 vtable */

public:
    cBgW_BgId();
    void Ct() {
        m_id = BG_ID_MAX;
    };

    void Regist(int);
    void Release();
    bool ChkUsed() const;

    virtual ~cBgW_BgId();

    s16 GetId() const {
        return m_id;
    }
};

// Defines are required for these checks to not inline some float comp bool things
#if 1

#define cBgW_CheckBGround(f) (f > 0.5f)
#define cBgW_CheckBRoof(f) (f < (-4.0f / 5.0f))

inline bool cBgW_CheckBWall(f32 a1) {
    if (!cBgW_CheckBGround(a1) && !cBgW_CheckBRoof(a1)) {
        return true;
    }

    return false;
}

#else
inline bool cBgW_CheckBGround(f32 f) {
    return f > 0.5f;
}
inline bool cBgW_CheckBRoof(f32 a1) {
    return a1 <= (-4.0f / 5.0f);
}
inline bool cBgW_CheckBWall(f32 a1) {
    if (!cBgW_CheckBGround(a1) && !cBgW_CheckBRoof(a1)) {
        return true;
    }

    return false;
}
#endif

#endif
