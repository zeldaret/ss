#ifndef D_T_SHUTTER_FENCE_FORBIDDANCE_H
#define D_T_SHUTTER_FENCE_FORBIDDANCE_H

#include "d/t/d_tg.h"

class dTgShutterFenceForbiddance_c : public dTg_c {
public:
    dTgShutterFenceForbiddance_c();
    virtual ~dTgShutterFenceForbiddance_c();

    virtual int create() override;
    virtual int actorExecute() override;

private:
};

#endif
