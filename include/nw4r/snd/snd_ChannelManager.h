#ifndef NW4R_SND_CHANNEL_MANAGER_H
#define NW4R_SND_CHANNEL_MANAGER_H
#include "common.h"

namespace nw4r {
namespace snd {
namespace detail {
struct ChannelManager {
    static ChannelManager *GetInstance();

    void UpdateAllChannel();
};
} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
