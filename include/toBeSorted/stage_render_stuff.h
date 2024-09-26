#ifndef D_STAGE_RENDER_STUFF_H
#define D_STAGE_RENDER_STUFF_H

#include <d/a/obj/d_a_obj_base.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <m/m3d/m_scnleaf.h>

class dScnCallback_c : public nw4r::g3d::IScnObjCallback {
public:
    dScnCallback_c(dAcObjBase_c *arg): mpActor(arg) {}
    /* vt 0x10 */ virtual void ExecCallback_CALC_MAT(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

    /* 0x04 */ dAcObjBase_c *mpActor;
    /* 0x08 */ UNKWORD field_0x08;

    /* 8001a230 */ void attach(m3d::scnLeaf_c&);
};

#endif
