#ifndef D_A_OBJ_JUNK_REPAIRING_H
#define D_A_OBJ_JUNK_REPAIRING_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOJunkRep_c : public dAcObjBase_c {
public:
    dAcOJunkRep_c() : mStateMgr(*this), mpModelToUse(nullptr) {}
    virtual ~dAcOJunkRep_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOJunkRep_c, Wait);

private:
    static const f32 sSomeFloat;

    inline const char *getMdlName1() {
        return "JunkRepairobject";
    }
    inline const char *getMdlName2() {
        return "JunkRepairobjectB";
    }

    bool loadMdl(m3d::smdl_c &mdl, const char *name);
    void realizeState();
    bool getState();

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c *mpModelToUse;
    m3d::smdl_c mModel1;
    m3d::smdl_c mModel2;
    dShadowCircle_c mShadow;
    STATE_MGR_DECLARE(dAcOJunkRep_c);
};

#endif
