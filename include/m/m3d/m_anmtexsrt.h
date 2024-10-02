#ifndef M3D_M_ANMTEXSRT_H
#define M3D_M_ANMTEXSRT_H

#include <m/m3d/m_bmdl.h>
#include <m/m3d/m_fanm.h>
#include <nw4r/g3d/g3d_anmtexsrt.h>

namespace m3d {

class anmTexSrt_c : public banm_c {
    class child_c : public fanm_c {
    public:
        child_c() {}
        virtual ~child_c();
        virtual int getType() const override;

        static u32 heapCost(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexSrt, bool);
        bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexSrt, mAllocator_c *, u32 *);
        void setAnm(m3d::bmdl_c &, nw4r::g3d::ResAnmTexSrt, m3d::playMode_e);
        void releaseAnm();
        void setFrmCtrlDefault(nw4r::g3d::ResAnmTexSrt &, playMode_e);
    };

public:
    anmTexSrt_c() : mpChildren(nullptr) {}
    virtual ~anmTexSrt_c();

    virtual int getType() const override;
    virtual void remove() override;
    virtual void play() override;

    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexSrt, mAllocator_c *, u32 *, s32);
    static u32 heapCost(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexSrt, s32, bool);

    void setAnm(bmdl_c &, nw4r::g3d::ResAnmTexSrt, s32, playMode_e);

    void play(s32);
    f32 getFrame(s32) const;
    void setFrame(f32, s32);
    void setRate(f32, s32);
    bool isStop(s32) const;

    bool checkFrame(f32, s32) const;

    void setPlayMode(playMode_e, s32);
    f32 getFrameMax(s32) const;
    void setFrameStart(f32, s32);
    f32 getFrameStart(s32) const;

private:
    child_c *mpChildren;
};

} // namespace m3d

#endif
