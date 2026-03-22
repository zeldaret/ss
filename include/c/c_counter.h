#ifndef C_COUNTER_H
#define C_COUNTER_H

#include "common.h"

class cCounter_c {
public:
    static s32 GetGameFrame() {
        return m_gameFrame;
    }

    static void IncrementGameFrame() {
        m_gameFrame++;
    }

    static void IncrementUnkCounter() {
        m_unkCounter++;
    }

    static void clear();

private:
    static s32 m_gameFrame;
    static s32 m_unkCounter;
};

#endif
