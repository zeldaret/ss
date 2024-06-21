#ifndef M3D_M_SCNLEAF_H
#define M3D_M_SCNLEAF_H

#include <egg/core/eggDisposer.h>
#include <m/m_allocator.h>
#include <nw4r/g3d/g3d_scnobj.h>

namespace m3d {

class UnkClass {
public:
    UnkClass() {}
};

class scnLeaf_c : UnkClass, EGG::Disposer {
public:
    scnLeaf_c();
    virtual ~scnLeaf_c();
    virtual int getType() const = 0;
    virtual void remove();
    virtual int entry();

    void setOption(u32 flag, u32 set);
    void setScale(f32, f32, f32);
    void setScale(const nw4r::math::VEC3 &);

    void setLocalMtx(const nw4r::math::MTX34 *);
    void getLocalMtx(nw4r::math::MTX34 *) const;

    void getViewMtx(nw4r::math::MTX34 *) const;

    void calc(bool);
    void calcVtx(bool);

    void setPriorityDraw(int, int);

protected:
    nw4r::g3d::ScnLeaf *mpScnLeaf;
};

} // namespace m3d

#endif
