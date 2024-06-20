#ifndef D_A_OBJ_RING
#define D_A_OBJ_RING

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_smdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOring_c : public dAcObjBase_c {
public:
    dAcOring_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOring_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOring_c, Move);

    u8 getArgFromParams() {
        u32 p = (params & 3);
        return p != 3 ? p : 0;
    }

private:

    static u32 ROT_PER_TICK;

    m3d::smdl_c mModel;
    STATE_MGR_DECLARE(dAcOring_c);
    u8 field_0x388;
    f32 field_0x38C;
    

};

#endif
