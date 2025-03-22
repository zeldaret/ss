#ifndef EGG_DRAW_PATH_BASE_H
#define EGG_DRAW_PATH_BASE_H

#include "egg/egg_types.h"
#include "egg/gfx/eggIScnProc.h"
#include "egg/gfx/eggScreenEffectBase.h"
#include "nw4r/g3d/g3d_scnobj.h"

namespace EGG {

class DrawPathBase : public ScreenEffectBase, public IScnProc, public nw4r::g3d::IScnObjCallback {
public:
    DrawPathBase();

    void createScnProc(MEMAllocator *allocator);

protected:
    virtual u16 getNumStep() const = 0;    // at 0x3C
    virtual void internalResetForDraw() {} // at 0x40
    virtual void internalDraw() = 0;       // at 0x44

    virtual void ExecCallback_CALC_VIEW(
        nw4r::g3d::ScnObj::Timing /* timing */, nw4r::g3d::ScnObj * /* pObj */, u32 /* param */, void * /* pInfo */
    ) override; // also at 0x48
    virtual void scnProcDraw(u16) override; // also at 0x4C
};

} // namespace EGG

#endif
