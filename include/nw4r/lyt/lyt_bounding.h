#ifndef NW4R_LYT_BOUNDING_H
#define NW4R_LYT_BOUNDING_H
#include "common.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/lyt/lyt_pane.h"

namespace nw4r {
namespace lyt {

struct ResBlockSet {
    const res::TextureList *pTextureList;   // at 0x00
    const res::FontList *pFontList;         // at 0x04
    const res::MaterialList *pMaterialList; // at 0x08
    ResourceAccessor *pResAccessor;         // at 0x0C
};

class Bounding : public Pane {
public:
    Bounding(const res::Bounding *, const ResBlockSet &);
    virtual ~Bounding();
    NW4R_UT_RTTI_DECL(Bounding);
    virtual void DrawSelf(const DrawInfo &);
};
} // namespace lyt
} // namespace nw4r

#endif
