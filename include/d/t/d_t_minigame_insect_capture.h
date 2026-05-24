#ifndef D_T_MINIGAME_INSECT_CAPTURE_H
#define D_T_MINIGAME_INSECT_CAPTURE_H

#include "d/a/d_a_base.h"
#include "d/t/d_tg.h"

class dTgMinigameInsectCapture_c : public dTg_c {
public:
    dTgMinigameInsectCapture_c();
    virtual ~dTgMinigameInsectCapture_c();

    static dTgMinigameInsectCapture_c *GetInstance() {
        return sInstance.get();
    }

    void recordCollectedInsect(s32 id, s32 count);

private:
    static dAcRef_c<dTgMinigameInsectCapture_c> sInstance;
};

#endif
