#ifndef D_LYT_CONTROL_GAME_H
#define D_LYT_CONTROL_GAME_H

#include "d/d_base.h"

class dLytControlGame_c : public dBase_c {
public:
    dLytControlGame_c();
    virtual ~dLytControlGame_c();

    bool isInSomeMapState() const;
    bool isNotInStateMap() const;

    void fn_802CCD40(bool);

    static dLytControlGame_c *getInstance() {
        return sInstance;
    }

private:

    static dLytControlGame_c *sInstance;
};

#endif
