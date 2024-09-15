#ifndef NW4R_SND_AX_VOICE_MANAGER_H
#define NW4R_SND_AX_VOICE_MANAGER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_AxVoice.h>

namespace nw4r {
namespace snd {
namespace detail {

class AxVoiceManager {
public:
    static const int VOICE_MARGIN = 16;
    static const int VOICE_MAX = AX_VOICE_MAX + VOICE_MARGIN;
    static const int WORK_SIZE_MAX = VOICE_MAX * sizeof(AxVoice);

public:
    static AxVoiceManager& GetInstance();

    u32 GetRequiredMemSize();
    void Setup(void* pBuffer, u32 size);
    void Shutdown();

    AxVoice* AcquireAxVoice(u32 priority, AxVoice::AxVoiceCallback pCallback,
                            void* pArg);
    void FreeAxVoice(AxVoice* pVoice);

    void ReserveForFreeAxVoice(AxVoice* pVoice);
    void FreeAllReservedAxVoice();

private:
    AxVoiceManager();

    AxVoice* Alloc();
    void Free(AxVoice* pVoice);
    void ReserveForFree(AxVoice* pVoice);

private:
    AxVoiceList mActiveVoiceList;       // at 0x0
    AxVoiceList mFreeVoiceList;         // at 0xC
    AxVoiceList mFreeReservedVoiceList; // at 0x18
    bool mInitialized;                  // at 0x24
    int mVoiceCount;                    // at 0x28
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
