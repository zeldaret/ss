#ifndef EGG_DRAW_PATH_BLOOM_H
#define EGG_DRAW_PATH_BLOOM_H

#include "egg/gfx/eggDrawPathBase.h"
namespace EGG {

class DrawPathBloom : public DrawPathBase {
public:
    virtual ~DrawPathBloom();

    virtual u16 getNumStep() const override {
        return 3;
    }

private:
};

} // namespace EGG

#endif
