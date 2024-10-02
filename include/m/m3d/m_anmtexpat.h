#ifndef M3D_M_ANMTEXPAT_H
#define M3D_M_ANMTEXPAT_H

#include <m/m3d/m_bmdl.h>
#include <m/m3d/m_fanm.h>
#include <nw4r/g3d/g3d_anmtexpat.h>

namespace m3d {

class anmTexPat_c : public banm_c {
    class child_c : public fanm_c {
    public:
        child_c() {}
        virtual ~child_c();
        virtual int getType() const override;

        static u32 heapCost(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexPat, bool);
        bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexPat, mAllocator_c *, u32 *);
        void setAnm(m3d::bmdl_c &, nw4r::g3d::ResAnmTexPat, m3d::playMode_e);
        void releaseAnm();
        void setFrmCtrlDefault(nw4r::g3d::ResAnmTexPat &, playMode_e);
    };

public:
    anmTexPat_c() : mpChildren(nullptr) {}
    virtual ~anmTexPat_c();

    virtual int getType() const override;
    virtual void remove() override;
    virtual void play() override;

    bool create(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexPat, mAllocator_c *, u32 *, s32);
    static u32 heapCost(nw4r::g3d::ResMdl, nw4r::g3d::ResAnmTexPat, s32, bool);

    void setAnm(bmdl_c &, nw4r::g3d::ResAnmTexPat, s32, playMode_e);

    void play(s32);
    f32 getFrame(s32) const;
    void setFrame(f32, s32);
    void setRate(f32, s32);
    bool isStop(s32) const;

    f32 getFrameMax(s32) const;
    void setFrameStart(f32, s32);

private:
    child_c *mpChildren;
};

} // namespace m3d

#endif
