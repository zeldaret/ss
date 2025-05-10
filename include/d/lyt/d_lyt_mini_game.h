#ifndef D_LYT_MINI_GAME_H
#define D_LYT_MINI_GAME_H

#include "common.h"

class dLytMiniGame_c {
public:
    static dLytMiniGame_c *GetInstance() {
        return sInstance;
    }

    void timeRelatedExecute();
    void scoreRelatedExecute();

    void setDisplayedTime(s32 time);
    void setDisplayedPoints(s32 time);

    void timeRelated();
    void scoreRelated();

private:
    static dLytMiniGame_c *sInstance;
};

#endif
