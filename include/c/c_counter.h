#ifndef C_COUNTER_H
#define C_COUNTER_H

#include "common.h"

class cCounter_c {
public:
    static s32 GetGameFrame() {
        return m_gameFrame;
    }
private:
    static s32 m_gameFrame;
};

#endif
