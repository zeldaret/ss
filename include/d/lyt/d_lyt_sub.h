#ifndef D_LYT_SUB_H
#define D_LYT_SUB_H

#include "d/lyt/d2d.h"

class dLytSub : public d2d::LytBase_c {
public:
    dLytSub() {}
    virtual bool build(const char *name, m2d::ResAccIf_c *acc) override {
        mpName = name;
        return d2d::LytBase_c::build(name, acc);
    }

    const char *getName() const {
        return mpName;
    }

private:
    /// This name is stored by `build` and usually accessed by dLytMeters' getName functions
    const char *mpName;
};

#endif
