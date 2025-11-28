#ifndef D_LYT_MINI_GAME_H
#define D_LYT_MINI_GAME_H

#include "common.h"

class dLytMiniGame_c {
public:
    static dLytMiniGame_c *GetInstance() {
        return sInstance;
    }

    /** Updates some window  */
    void fn_80295940();
    /** ??? */
    void fn_80295980();
    /** Checks to see if in BAMBOO_CUTTING .. BOSS_RUSH and returns some boolean (start?) */
    bool fn_80295AB0();
    /** Checks to see if in BAMBOO_CUTTING .. BOSS_RUSH and returns some boolean (end?) */
    bool fn_80295AD0();

    void setHighscore(s32 time);

    /** GUESS: sets the start for the high score? */
    void fn_80295CB0(bool);

    void timeRelatedExecute();
    void scoreRelatedExecute();

    void setDisplayedTime(s32 time);
    void setDisplayedPoints(s32 time);

    void timeRelated();
    void scoreRelated();

    /** GUESS: Assumption based on function caller */
    void setComplete() {
        field_0x3866 = true;
    }

private:
    u8 _0x0000[0x3866 - 0x0000];
    bool field_0x3866;

    static dLytMiniGame_c *sInstance;
};

#endif
