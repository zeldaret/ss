#ifndef D_T_MASS_OBJ_H
#define D_T_MASS_OBJ_H

#include "d/t/d_tg.h"

class dTgMassObj_c : public dTg_c {
public:
    dTgMassObj_c() {}
    virtual ~dTgMassObj_c() {}

    static dTgMassObj_c *GetInstance() {
        return sInstance;
    }

    void unloadRoom(u16 roomid);

private:
    static dTgMassObj_c *sInstance;
};

#endif
