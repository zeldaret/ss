#ifndef D_A_O_TRIFORCE_H
#define D_A_O_TRIFORCE_H

#include <c/c_math.h>
#include <d/a/obj/d_a_obj_base.h>
#include <d/col/cc/d_cc_shape_colliders.h>
#include <m/m3d/m_anmtexsrt.h>
#include <m/m3d/m_smdl.h>
#include <m/m_angle.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <toBeSorted/effects_struct.h>

class dAcOtriforce_c : public dAcObjBase_c {
public:
    dAcOtriforce_c() {}
    virtual ~dAcOtriforce_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

private:
    f32 getBounceScalar() const {
        int zero = 0;
        return (23.0f + zero + -3.0f);
    }

    static dCcD_SrcSph sCcSrc;

    static const f32 sScale;
    static const f32 sAmpNeg;
    static const u32 sStartingOffsetRange;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    dCcD_Sph mCollision;
    EffectsStruct mEffects;
    m3d::anmTexSrt_c mAnm;
    mAng mStartingOffset;
};

#endif
