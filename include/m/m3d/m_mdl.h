#ifndef M3D_M_MDL_H
#define M3D_M_MDL_H

#include <m/m3d/m_banm.h>
#include <m/m3d/m_calc_ratio.h>
#include <m/m3d/m_smdl.h>
#include <nw4r/g3d/g3d_anmchr.h>
#include <nw4r/g3d/g3d_calcworld.h>
#include <nw4r/g3d/g3d_resmdl.h>

namespace m3d {

class callback_c {
public:
    virtual ~callback_c() {}
    virtual void timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) {}
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) {}
    virtual void timingC(nw4r::math::MTX34 *, nw4r::g3d::ResMdl) {}
};

class mdl_c : public smdl_c {
public:
    class mdlCallback_c : public nw4r::g3d::ICalcWorldCallback {
    public:
        mdlCallback_c();
        virtual ~mdlCallback_c();

        bool create(nw4r::g3d::ResMdl, mAllocator_c *, u32 *);

        virtual void ExecCallbackA(nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl, nw4r::g3d::FuncObjCalcWorld *);
        virtual void ExecCallbackB(nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl, nw4r::g3d::FuncObjCalcWorld *);
        virtual void ExecCallbackC(nw4r::math::MTX34 *, nw4r::g3d::ResMdl, nw4r::g3d::FuncObjCalcWorld *);

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
        /* 0x04 */ calcRatio_c mCalcRatio;
        /* 0x20 */ int mNumNode;
        /* 0x24 */ nw4r::g3d::ChrAnmResult *mpNodes;
        /* 0x28 */ callback_c *mpBaseCallback;
        /* 0x2C */ mAllocator_c *mpAlloc;
    };

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
