#ifndef M3D_M_ANMTEXSRT_H
#define M3D_M_ANMTEXSRT_H

#include <m/m3d/m_bmdl.h>
#include <m/m3d/m_fanm.h>
#include <nw4r/g3d/g3d_anmclr.h>

namespace m3d {

class anmMatClr_c : public banm_c {
    class child_c : public fanm_c {
    public:
        child_c() {}
        virtual ~child_c();
        virtual int getType() override;

        static u32 heapCost(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmClr, bool);
        bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmClr, mAllocator_c *, u32 *);
        void setAnm(bmdl_c &, nw4r::g3d::ResAnmClr, playMode_e);
        void releaseAnm();
        void setFrmCtrlDefault(nw4r::g3d::ResAnmClr &, playMode_e);
    };

public:
    anmMatClr_c() : mpChildren(nullptr) {}
    virtual ~anmMatClr_c();

    virtual int getType() override;
    virtual void remove() override;
    virtual void play() override;

    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmClr, mAllocator_c *, u32 *, s32);
    static u32 heapCost(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmClr, s32, bool);

    void setAnm(bmdl_c &, nw4r::g3d::ResAnmClr, s32, playMode_e);

    void play(s32);
    f32 getFrame(s32) const;
    void setFrame(f32, s32);
    f32 getRate(s32) const;
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
