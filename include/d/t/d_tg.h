#ifndef D_TG_H
#define D_TG_H

#include "d/a/d_a_base.h"

class dTg_c : public dAcBase_c {
public:
    virtual ~dTg_c() {}
};

typedef dAcRef_c<dTg_c> dTgRef_c;

#endif
