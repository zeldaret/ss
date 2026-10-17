#ifndef D_T_RESTART_TIME_STONE_H
#define D_T_RESTART_TIME_STONE_H

#include "d/d_area.h"
#include "d/t/d_tg.h"
#include "m/m_vec.h"

class dTgRestartTimeStone_c : public dTg_c {
public:
    dTgRestartTimeStone_c() {}
    virtual ~dTgRestartTimeStone_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

private:
    /* 0x0FC */ dArea_c mArea;
    /* 0x12C */ mVec3_c mPathStartPos;
};

#endif
