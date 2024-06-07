#include <nw4r/lyt/lyt_drawInfo.h>
#include <string.h>

namespace nw4r {
namespace lyt {

// __ct__Q34nw4r3lyt8DrawInfoFv
DrawInfo::DrawInfo() : mViewRect(), mScale(1.0f, 1.0f), mGlobalAlpha(1.0f) {
    memset(&mFlags, 0, 1);
    PSMTXIdentity(mViewMtx);
}

// __dt__Q34nw4r3lyt8DrawInfoFv
DrawInfo::~DrawInfo() {}

} // namespace lyt
} // namespace nw4r
