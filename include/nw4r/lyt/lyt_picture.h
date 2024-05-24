#ifndef NW4R_LYT_PICTURE_H
#define NW4R_LYT_PICTURE_H

#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_pane.h>

namespace nw4r {

namespace lyt {

namespace res {

struct Picture : public Pane {};

} // namespace res

class Picture : public Pane {
public:
    Picture(const res::Picture *, const ResBlockSet &resDataSet);
    virtual ~Picture();
    NW4R_UT_RTTI_DECL(Picture);

private:
    ut::Color mVtxColors[4];          // at 0xD8
    detail::TexCoordAry mTexCoordAry; // at 0xE8
};

} // namespace lyt

} // namespace nw4r

#endif
