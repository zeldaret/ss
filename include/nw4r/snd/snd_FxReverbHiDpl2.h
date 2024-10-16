#ifndef NW4R_SND_FX_REVERB_HI_DPL2_H
#define NW4R_SND_FX_REVERB_HI_DPL2_H
#include "nw4r/snd/snd_AxfxImpl.h"
#include "nw4r/snd/snd_FxBase.h"
#include "nw4r/types_nw4r.h"
#include "rvl/AXFX.h" // IWYU pragma: export

namespace nw4r {
namespace snd {

class FxReverbHiDpl2 : public FxBase {
public:
    struct ReverbHiDpl2Param {
        f32 preDelayTime; // at 0x0
        f32 fusedTime;    // at 0x4
        f32 coloration;   // at 0x8
        f32 damping;      // at 0xC
        f32 crossTalk;    // at 0x10
        f32 outGain;      // at 0x14
    };

public:
    FxReverbHiDpl2();

    virtual ~FxReverbHiDpl2() {
        Shutdown();
        ReleaseWorkBuffer();
    } // at 0x8

    virtual bool StartUp();  // at 0xC
    virtual void Shutdown(); // at 0x10

    virtual void UpdateBuffer(
        int channels, void **ppBuffer, u32 size, SampleFormat format, f32 sampleRate,
        OutputMode mode
    ); // at 0x14

    virtual bool AssignWorkBuffer(void *pBuffer, u32 size); // at 0x18
    virtual void ReleaseWorkBuffer();                       // at 0x1C

    u32 GetRequiredMemSize();
    bool SetParam(const ReverbHiDpl2Param &rParam);

private:
    detail::AxfxImpl mImpl;        // at 0xC
    ReverbHiDpl2Param mParam;      // at 0x18
    AXFX_REVERBHI_DPL2 mAxfxParam; // at 0x30
};

} // namespace snd
} // namespace nw4r

#endif
