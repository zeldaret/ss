#ifndef NW4R_SND_FX_REVERB_STD_DPL2_H
#define NW4R_SND_FX_REVERB_STD_DPL2_H

#include "nw4r/snd/snd_AxfxImpl.h"
#include "nw4r/snd/snd_FxBase.h"
#include "nw4r/types_nw4r.h"

#include "rvl/AXFX.h" // IWYU pragma: export

namespace nw4r {
namespace snd {

namespace detail {
struct FxReverbStdParam {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    int field_0x14;
    f32 field_0x18;
    int field_0x1C;
    f32 field_0x20;
    f32 field_0x24;

    FxReverbStdParam()
        : field_0x00(0.02f),
          field_0x04(3.0f),
          field_0x08(0.6f),
          field_0x0C(0.4f),
          field_0x10(1.0f),
          field_0x14(5),
          field_0x18(0.02f),
          field_0x1C(0),
          field_0x20(0.0f),
          field_0x24(1.0f) {}
};
} // namespace detail

class FxReverbStdDpl2 : public FxBase {
public:
public:
    FxReverbStdDpl2();

    virtual ~FxReverbStdDpl2() {
        Shutdown();
        ReleaseWorkBuffer();
    } // at 0x8

    virtual bool StartUp() override;  // at 0xC
    virtual void Shutdown() override; // at 0x10

    virtual void UpdateBuffer(
        int channels, void **ppBuffer, u32 size, SampleFormat format, f32 sampleRate,
        OutputMode mode
    ) override; // at 0x14

    virtual bool AssignWorkBuffer(void *pBuffer, u32 size); // at 0x18
    virtual void ReleaseWorkBuffer();                       // at 0x1C

    u32 GetRequiredMemSize();
    bool SetParam(const detail::FxReverbStdParam &rParam);

    void OnChangeOutputMode() override;

private:
    u8 mIsActive;
    int mOutputMode;                       // at 0x10
    detail::AxfxImpl mImpl;                // at 0x14
    detail::FxReverbStdParam mParam;       // at 0x1C
    AXFX_REVERBSTD_EXP mAxfxParam;         // at 0x44
    AXFX_REVERBSTD_EXP_DPL2 mAxfxParamDpl; // at 0x12C
};

} // namespace snd
} // namespace nw4r

#endif
