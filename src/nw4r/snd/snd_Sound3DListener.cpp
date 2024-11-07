#include "nw4r/snd/snd_Sound3DListener.h"
#include "nw4r/math/math_types.h"


namespace nw4r {
namespace snd {

Sound3DListener::Sound3DListener()
    : mInteriorSize(1.0f), mMaxVolumeDistance(1.0f), mUnitDistance(1.0f) {
        math::MTX34Zero(&mMtx);
    }

} // namespace snd
} // namespace nw4r
