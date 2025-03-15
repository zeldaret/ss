#ifndef EGG_DRAW_PATH_DOF_H
#define EGG_DRAW_PATH_DOF_H

#include "egg/prim/eggBinary.h"
#include "egg/gfx/eggDrawPathBase.h"

namespace EGG {

class DrawPathDOF : public DrawPathBase, public IBinary<DrawPathDOF> {
public:
    class BinData {
        // TODO
    };
    DrawPathDOF();
    virtual ~DrawPathDOF();

    virtual u16 getNumStep() {
        return 3;
    }
    virtual void internalResetForDraw() override;
    virtual void internalDraw() override;

    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;
private:
    void *IHaveMembers;
};

} // namespace EGG

#endif
