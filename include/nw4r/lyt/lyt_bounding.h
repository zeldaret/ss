#ifndef NW4R_LYT_BOUNDING_H
#define NW4R_LYT_BOUNDING_H
#include "common.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/lyt/lyt_pane.h"

namespace nw4r {
namespace lyt {

class Bounding : public Pane {
public:
    Bounding(const res::Bounding *pBlock, const ResBlockSet &);
    virtual ~Bounding();
    NW4R_UT_RTTI_DECL(Bounding);
    virtual void DrawSelf(const DrawInfo &drawInfo);
};
} // namespace lyt
} // namespace nw4r

#endif
