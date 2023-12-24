#ifndef NW4R_SND_AX_VOICE_MANAGER_H
#define NW4R_SND_AX_VOICE_MANAGER_H
#include <nw4r/snd/snd_AxVoice.h>
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

class AxVoiceManager {
public:
    static AxVoiceManager& GetInstance();

    AxVoiceManager();
    ~AxVoiceManager() {}

    u32 GetRequiredMemSize();
    void Setup(void* work, u32 size);
    void Shutdown();

    AxVoice* AcquireAxVoice(u32 prio, AxVoice::Callback callback, void* arg);
    void FreeAxVoice(AxVoice* voice);
    void ReserveForFreeAxVoice(AxVoice* voice);
    void FreeAllReservedAxVoice();

private:
    AxVoice* Alloc();
    void Free(AxVoice* voice);
    void ReserveForFree(AxVoice* voice);

private:
    AxVoiceList mActiveList;       // at 0x0
    AxVoiceList mFreeList;         // at 0xC
    AxVoiceList mFreeReservedList; // at 0x18
    bool mInitialized;             // at 0x24
    int mVoiceCount;               // at 0x28
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
