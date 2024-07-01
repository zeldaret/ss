#ifndef D_TG_H
#define D_TG_H

#include <d/a/d_a_base.h>

class dTg_c : public dAcBase_c {
public:
    virtual ~dTg_c() {}
    virtual int draw() override {
        return 1;
    }
    virtual int actorExecute() override {
        return 1;
    }
    virtual int doDelete() override {
        return 1;
    }
    virtual int create() override {
        return SUCCEEDED;
    }
};

#endif
