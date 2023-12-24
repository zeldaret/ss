#ifndef NW4R_EF_DRAWORDERBASE
#define NW4R_EF_DRAWORDERBASE
#include "ef_particlemanager.h"
#include "ut_list.h"
#include <common.h>


namespace nw4r {
namespace ef {
struct DrawOrderBase {
    virtual void Add(Effect *, ParticleManager *) = 0;
    virtual void Remove(Effect *, ParticleManager *) = 0;
    virtual void Draw(Effect *, const DrawInfo &) = 0;
};
} // namespace ef
} // namespace nw4r

#endif
