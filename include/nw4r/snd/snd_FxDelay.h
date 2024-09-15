#ifndef NW4R_SND_FX_DELAY_H
#define NW4R_SND_FX_DELAY_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_AxfxImpl.h>
#include <nw4r/snd/snd_FxBase.h>

#include <rvl/AXFX.h>

namespace nw4r {
namespace snd {

class FxDelay : public FxBase {
public:
    struct DelayParam {
        f32 delay;    // at 0x0
        f32 feedback; // at 0x4
        f32 outGain;  // at 0x8
    };

public:
    FxDelay();

    virtual ~FxDelay() {
        Shutdown();
        ReleaseWorkBuffer();
    } // at 0x8

    virtual bool StartUp();  // at 0xC
    virtual void Shutdown(); // at 0x10

    virtual void UpdateBuffer(int channels, void** ppBuffer, u32 size,
                              SampleFormat format, f32 sampleRate,
                              OutputMode mode); // at 0x14

    virtual bool AssignWorkBuffer(void* pBuffer, u32 size); // at 0x18
    virtual void ReleaseWorkBuffer();                       // at 0x1C

    u32 GetRequiredMemSize();
    bool SetParam(const DelayParam& rParam);

private:
    detail::AxfxImpl mImpl; // at 0xC
    DelayParam mParam;      // at 0x18
    AXFX_DELAY mAxfxParam;  // at 0x24
};

} // namespace snd
} // namespace nw4r

#endif
