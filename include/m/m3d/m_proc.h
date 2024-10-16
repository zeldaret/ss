#ifndef M_M3D_PROC_H
#define M_M3D_PROC_H

#include "common.h"
#include "m/m3d/m_scnleaf.h"

namespace m3d {

class proc_c : public scnLeaf_c {
public:
    virtual ~proc_c();
    virtual int getType() const override;
    bool create(mAllocator_c *, u32 *);

    virtual void drawOpa();
    virtual void drawXlu();
};

} // namespace m3d

#endif
