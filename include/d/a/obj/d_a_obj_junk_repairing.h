#ifndef D_A_OBJ_JUNK_REPAIRING_H
#define D_A_OBJ_JUNK_REPAIRING_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_shadow.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "s/s_State.hpp"

class dMyShadowCircle_c : public m3d::mShadowCircle_c {};

class dAcOJunkRep_c : public dAcObjBase_c {
public:
    dAcOJunkRep_c() : mStateMgr(*this, sStateID::null), mpModelToUse(nullptr) {}
    virtual ~dAcOJunkRep_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOJunkRep_c, Wait);

private:
    static const f32 sSomeFloat;

    static char sMdlName1[];
    static char sMdlName2[];

    bool loadMdl(m3d::smdl_c &mdl, const char *name);
    void realizeState();
    bool getState();

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c *mpModelToUse;
    m3d::smdl_c mModel1;
    m3d::smdl_c mModel2;
    dMyShadowCircle_c mShadow;
    STATE_MGR_DECLARE(dAcOJunkRep_c);
};

#endif
