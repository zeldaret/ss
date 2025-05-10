#ifndef M_M3D_PROC_H
#define M_M3D_PROC_H

#include "common.h"
#include "m/m3d/m_scnleaf.h"

namespace m3d {

class proc_c : public scnLeaf_c {
public:
    /* vt 0x08 */ virtual ~proc_c();
    /* vt 0x0C */ virtual int getType() const override;
    bool create(mAllocator_c *, u32 *);

    /* vt 0x18 */ virtual void drawOpa();
    /* vt 0x1C */ virtual void drawXlu();
};

} // namespace m3d

#endif
