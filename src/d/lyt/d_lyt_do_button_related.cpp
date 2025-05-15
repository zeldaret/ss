#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_unknowns.h"

LytDoButtonRelated::LytDoButtonRelated() {
    sInstance = this;
}

bool LytDoButtonRelated::build(d2d::ResAccIf_c *resAcc) {
    for (int i = 0; i < 12; i++) {
        field_0x04[i] = ACT_IE_NONE;
        field_0x34[i] = 0;
    }

    return true;
}

bool LytDoButtonRelated::remove() {
    return true;
}
