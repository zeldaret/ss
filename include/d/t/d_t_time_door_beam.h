#ifndef D_T_TIME_DOOR_BEAM_H
#define D_T_TIME_DOOR_BEAM_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"

class dTgTimeDoorBeam_c : public dTg_c {
public:
    dTgTimeDoorBeam_c() {}
    virtual ~dTgTimeDoorBeam_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    bool checkPlayerPosInMtx();
    mMtx_c matrix;
};

#endif
