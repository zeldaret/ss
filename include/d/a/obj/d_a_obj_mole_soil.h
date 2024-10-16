#ifndef D_A_OBJ_MOLE_SOIL_H
#define D_A_OBJ_MOLE_SOIL_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOmoleSoil_c : public dAcObjBase_c {
public:
    dAcOmoleSoil_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOmoleSoil_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOmoleSoil_c, Wait);
    STATE_FUNC_DECLARE(dAcOmoleSoil_c, Appear);
    STATE_FUNC_DECLARE(dAcOmoleSoil_c, DisAppear);

private:
    const static f32 sHalfScale;
    const static f32 sFullScale;

    u8 getNextState() {
        return mDesiredNextState;
    }

    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModel;
    STATE_MGR_DECLARE(dAcOmoleSoil_c);
    f32 mScale;
    u8 mDesiredNextState;
    bool field_0x391;
};

#endif
