#ifndef NW4R_LYT_BOUNDING_H
#define NW4R_LYT_BOUNDING_H
#include "common.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_pane.h"

namespace nw4r {
namespace lyt {
namespace res {
struct Bounding : Pane {};
} // namespace res

struct ResBlockSet {};

struct Bounding : Pane {
    Bounding(const res::Bounding *, const ResBlockSet &);
    virtual ~Bounding();
    virtual const ut::detail::RuntimeTypeInfo *GetRuntimeTypeInfo() const;
    virtual void DrawSelf(const DrawInfo &);

    static ut::detail::RuntimeTypeInfo typeInfo;
};
} // namespace lyt
} // namespace nw4r

#endif
