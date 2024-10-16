#ifndef D_A_OBJ_PUMPKIN_LEAF_H
#define D_A_OBJ_PUMPKIN_LEAF_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_smdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOPumpkinLeaf_c : public dAcObjBase_c {
public:
    dAcOPumpkinLeaf_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOPumpkinLeaf_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOPumpkinLeaf_c, Wait);

private:
    nw4r::g3d::ResFile mBrres;
    m3d::smdl_c mModel;
    STATE_MGR_DECLARE(dAcOPumpkinLeaf_c);
    void dummy_executeState() {
        // Not called from the class but needed for weak function ordering
        mStateMgr.executeState();
    }
};

#endif
