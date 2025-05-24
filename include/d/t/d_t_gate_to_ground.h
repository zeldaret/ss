#ifndef D_T_GATE_TO_GROUND_H
#define D_T_GATE_TO_GROUND_H

#include "d/a/d_a_base.h"

class dTgGateToGround_c : public dAcBase_c {
public:
    dTgGateToGround_c() {}
    virtual ~dTgGateToGround_c() {}

    virtual int create() override;
    virtual int doDelete() override; // fn_205_150
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override; // fn_205_2F0

private:
};

#endif
