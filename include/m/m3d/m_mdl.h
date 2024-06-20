#ifndef M3D_M_MDL_H
#define M3D_M_MDL_H

#include <m/m3d/m_banm.h>
#include <m/m3d/m_calc_ratio.h>
#include <m/m3d/m_smdl.h>
#include <nw4r/g3d/g3d_resmdl.h>
#include <nw4r/g3d/g3d_calcworld.h>

class UnkClass3 {};

namespace m3d {

class callback_c {
public:
    virtual ~callback_c() {}
    virtual void timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) {}
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) {}
    virtual void timingC(nw4r::math::MTX34*, nw4r::g3d::ResMdl) {}
};

struct UnkNode {
    UNKWORD field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    UNKWORD field_0x10;
    UNKWORD field_0x14;
    UNKWORD field_0x18;
    nw4r::math::MTX34 mtx;
};

class mdl_c : smdl_c {
    class mdlCallback_c : public nw4r::g3d::IScnObjCallback {
    public:
        mdlCallback_c();
        virtual ~mdlCallback_c();

        bool create(nw4r::g3d::ResMdl, mAllocator_c *, u32 *);

        // TODO g3d headers
        virtual void ExecCallbackA();
        virtual void ExecCallbackB();
        virtual void ExecCallbackC();

        void remove();
        void setBlendFrame(f32);
        void calcBlend();

        mAllocator_c *getAllocator() {
            return mpAlloc;
        }

        void setBaseCallback(callback_c *cb) {
            mpBaseCallback = cb;
        }

    private:
        calcRatio_c mCalcRatio;
        int mNumNode;
        UnkNode *mpNodes;
        callback_c *mpBaseCallback;
        mAllocator_c *mpAlloc;
    };

public:
    mdl_c();
    virtual ~mdl_c();

    bool create(nw4r::g3d::ResMdl, mAllocator_c *, u32, int, u32 *);
    bool create(nw4r::g3d::ResMdl, mdlCallback_c *cb, mAllocator_c *, u32, int, u32 *);

    virtual void remove();

    void setCallback(callback_c *cb);
    void play();
    void setAnm(banm_c &);
    void setAnm(banm_c &, f32);

private:
    /** If we allocated the callback ourselves, this is what we need to free */
    mdlCallback_c *mpOwnedCallback;
    /** The actual callback to use */
    mdlCallback_c *mpCallback;
};

} // namespace m3d

#endif
