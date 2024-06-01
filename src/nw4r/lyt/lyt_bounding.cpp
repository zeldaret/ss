#include <nw4r/lyt/lyt_bounding.h>

namespace nw4r {

namespace lyt {

NW4R_UT_RTTI_DEF_DERIVED(Bounding, Pane);

// __ct__Q34nw4r3lyt8BoundingFPCQ44nw4r3lyt3res8BoundingRCQ34nw4r3lyt11ResBlockSet
Bounding::Bounding(const res::Bounding *pBlock, const ResBlockSet &) : Pane(pBlock) {}

// __dt__Q34nw4r3lyt8BoundingFv
Bounding::~Bounding() {}

// DrawSelf__Q34nw4r3lyt8BoundingFRCQ34nw4r3lyt8DrawInfo
void Bounding::DrawSelf(const DrawInfo &drawInfo) {
    return;
}

} // namespace lyt

} // namespace nw4r
