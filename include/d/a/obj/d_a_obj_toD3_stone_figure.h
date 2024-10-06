#ifndef D_A_O_TTOD3_STONE_FIGURE_H
#define D_A_O_TTOD3_STONE_FIGURE_H

#include <c/c_math.h>
#include <d/a/obj/d_a_obj_base.h>
#include <d/col/cc/d_cc_shape_colliders.h>
#include <m/m3d/m_smdl.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOtoD3StoneFigure_c : public dAcObjBase_c {
public:
    dAcOtoD3StoneFigure_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOtoD3StoneFigure_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;
    virtual void doInteraction(s32) override;

    STATE_FUNC_DECLARE(dAcOtoD3StoneFigure_c, OneEye);
    STATE_FUNC_DECLARE(dAcOtoD3StoneFigure_c, Eyes);

private:
    f32 getYPos() {
        return position.y + sHeight;
    }

    const char *getModelName() const;
    bool isStoneOfTrialsPlaced() const;
    bool hasStoneOfTrials() const;
    static f32 sRadius;
    static f32 sHeight;
    static dCcD_SrcCyl sCcSrc;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    STATE_MGR_DECLARE(dAcOtoD3StoneFigure_c);
    dCcD_Cyl mCollision;
    s8 mExitId;
    bool mIsSkyKeepAlreadyOpen;
};

#endif
