#ifndef M3D_M_MDL_H
#define M3D_M_MDL_H

#include <m/m3d/m_banm.h>
#include <m/m3d/m_calc_ratio.h>
#include <m/m3d/m_smdl.h>

class UnkClass3 {};

namespace m3d {

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

    private:
        calcRatio_c mCalcRatio;
        u32 mNumNode;
        void *mpNodes;
        u32 mCallbackNum;
        mAllocator_c *mpAlloc;
    };

public:
    mdl_c();
    virtual ~mdl_c();

    bool create(nw4r::g3d::ResMdl, mAllocator_c *, u32, int, u32 *);
    bool create(nw4r::g3d::ResMdl, mdlCallback_c *cb, mAllocator_c *, u32, int, u32 *);

    virtual void remove();

    void setCallback(mdlCallback_c *cb);
    void play();
    void setAnm(banm_c &);
    void setAnm(banm_c &, f32);

private:
    mdlCallback_c *mpOwnedCallback;
    mdlCallback_c *mpCallback;
};

} // namespace m3d

#endif
