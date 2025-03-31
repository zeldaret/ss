#ifndef NW4R_G3D_RES_RES_USER_H
#define NW4R_G3D_RES_RES_USER_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_rescommon.h"

namespace nw4r {
namespace g3d {

struct ResUserDataData {};

class ResUserData : public ResCommon<ResUserDataData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResUserData);
    // TODO
};

} // namespace g3d
} // namespace nw4r

#endif
