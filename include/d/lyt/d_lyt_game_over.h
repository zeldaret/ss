#ifndef D_LYT_GAME_OVER_H
#define D_LYT_GAME_OVER_H

#include "d/d_base.h"

class dLytGameOver_c : public dBase_c {
public:
    dLytGameOver_c();
    virtual ~dLytGameOver_c();

    static dLytGameOver_c *GetInstance() {
        return sInstance;
    }

private:
    static dLytGameOver_c *sInstance;
};

#endif
