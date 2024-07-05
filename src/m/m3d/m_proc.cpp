#include <m/m3d/m3d.h>
#include <m/m3d/m_proc.h>
#include <nw4r/g3d/g3d_scnproc.h>

namespace m3d {

void proc_c_drawProc(nw4r::g3d::ScnProc *proc, bool b) {
    proc_c *p = static_cast<proc_c *>(proc->GetUserData());
    if (b) {
        p->drawOpa();
    } else {
        p->drawXlu();
    }
}

proc_c::~proc_c() {}

int proc_c::getType() const {
    return 0x2;
}

void proc_c::drawOpa() {}
void proc_c::drawXlu() {}

bool proc_c::create(mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size;
    if (pSize == nullptr) {
        pSize = &size;
    }

    mpScnLeaf = nw4r::g3d::ScnProc::Construct(alloc, pSize, proc_c_drawProc, true, true, 0);
    if (!mpScnLeaf) {
        return false;
    }

    mpScnLeaf->SetPriorityDrawOpa(0x7f);
    mpScnLeaf->SetPriorityDrawXlu(0x7f);
    nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnProc>(mpScnLeaf)->SetUserData(this);
    return true;
}

} // namespace m3d
