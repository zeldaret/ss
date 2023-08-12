#pragma once

#include <nw4r/nw4r_types.h>

namespace nw4r
{
namespace snd
{
        
class SoundSystem  {
public:
    struct SoundSystemParam {
        int soundThreadPriority;
        int soundThreadStackSize;
        int dvdThreadPriority;
        int dvdThreadStackSize;
    };
};

} // namespace snd
    
} // namespace nw4r

