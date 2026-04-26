#ifndef TOBESORTED_MDL_BASE_H
#define TOBESORTED_MDL_BASE_H

#include "egg/core/eggDisposer.h"

class dMdl_c : public EGG::Disposer {
protected:
    dMdl_c();
    virtual ~dMdl_c();
};

#endif
