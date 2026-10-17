#ifndef D_T_TIME_DOOR_BEAM_H
#define D_T_TIME_DOOR_BEAM_H

#include "d/d_area.h"
#include "d/t/d_tg.h"

class dTgTimeDoorBeam_c : public dTg_c {
public:
    dTgTimeDoorBeam_c() {}
    virtual ~dTgTimeDoorBeam_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool isPlayerInArea();

private:
    dArea_c mArea;
};

#endif
