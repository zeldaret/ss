#ifndef D_T_TACKLE_H
#define D_T_TACKLE_H

#include "d/t/d_tg.h"

class dTgTackle_c : public dTg_c {
public:
    dTgTackle_c() {}
    virtual ~dTgTackle_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;
};

#endif
