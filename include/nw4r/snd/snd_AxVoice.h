#ifndef NW4R_SND_AX_VOICE_H
#define NW4R_SND_AX_VOICE_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut.h>
#include <rvl/AX.h>

namespace nw4r {
namespace snd {
namespace detail {

inline int CalcAxvpbDelta(u16 init, u16 target) {
    return (target - init) / AX_SAMPLES_PER_FRAME;
}

class AxVoiceParamBlock {
public:
    AxVoiceParamBlock();

    operator AXVPB *() {
        return mVpb;
    }

    bool IsAvailable() const {
        return mVpb != NULL;
    }

    bool IsRun() const {
        return IsAvailable() && mVpb->pb.state == AX_VOICE_RUN;
    }

    bool IsLpfEnable() const {
        return IsAvailable() && mVpb->pb.lpf.on == AX_PB_LPF_ON;
    }

    bool IsBiquadEnable() const {
        return IsAvailable() && mVpb->pb.biquad.on == AX_PB_BIQUAD_ON;
    }

    u32 GetCurrentAddress() const {
        if (!IsAvailable()) {
            return 0;
        }

        return (mVpb->pb.addr.currentAddressHi << 16) + mVpb->pb.addr.currentAddressLo;
    }

    u32 GetLoopAddress() const {
        if (!IsAvailable()) {
            return 0;
        }

        return (mVpb->pb.addr.loopAddressHi << 16) + mVpb->pb.addr.loopAddressLo;
    }

    u32 GetEndAddress() const {
        if (!IsAvailable()) {
            return 0;
        }

        return (mVpb->pb.addr.endAddressHi << 16) + mVpb->pb.addr.endAddressLo;
    }

    void SetVoiceAddr(const AXPBADDR &addr) {
        if (IsAvailable()) {
            // AXSetVoiceAddr doesn't actually modify the object
            AXSetVoiceAddr(mVpb, const_cast<AXPBADDR *>(&addr));
        }
    }

    void SetVoicePriority(u32 prio) {
        if (IsAvailable()) {
            AXSetVoicePriority(mVpb, prio);
        }
    }

    void SetVoiceStateRun() {
        if (IsAvailable()) {
            AXSetVoiceState(mVpb, AX_VOICE_RUN);
        }
    }

    void SetVoiceStateStop() {
        if (IsRun()) {
            AXSetVoiceState(mVpb, AX_VOICE_STOP);
        }
    }

    void Sync();
    bool IsRmtIirEnable() const;

    void Set(AXVPB *vpb);
    void Clear();

    void SetVoiceType(u16 type);
    void SetVoiceVe(u16 volume, u16 initVolume);
    void SetVoiceMix(const AXPBMIX &mix, bool syncNow);
    void SetVoiceLoop(u16 loop);
    void SetVoiceLoopAddr(u32 addr);
    void SetVoiceEndAddr(u32 addr);
    void SetVoiceAdpcm(const AXPBADPCM &adpcm);
    void SetVoiceSrcType(u32 type);
    void SetVoiceSrc(const AXPBSRC &src);
    void SetVoiceSrcRatio(f32 ratio);
    void SetVoiceAdpcmLoop(const AXPBADPCMLOOP &loop);
    void SetVoiceLpf(const AXPBLPF &lpf);
    void SetVoiceLpfCoefs(u16 a0, u16 b0);
    void SetVoiceRmtOn(u16 on);
    void SetVoiceRmtMix(const AXPBRMTMIX &mix);
    void SetVoiceRmtIIR(const AXPBRMTIIR &iir);
    void SetVoiceRmtIIRCoefs(u16 type, ...);
    void UpdateDelta();

private:
    AXVPB *mVpb;             // at 0x0
    u32 mSync;               // at 0x4
    volatile AXPBVE mVePrev; // at 0x8
    bool mFirstVeUpdate;     // at 0xC
    u16 mVolume;             // at 0xE
};

class AxVoice {
    friend class AxVoiceManager;

public:
    enum AxVoiceCallbackStatus { CALLBACK_STATUS_CANCEL, CALLBACK_STATUS_DROP_DSP };

    typedef void (*Callback)(AxVoice *voice, AxVoiceCallbackStatus status, void *arg);

    enum Format {
        FORMAT_ADPCM = 0,
        FORMAT_PCM16 = 10,
        FORMAT_PCM8 = 25,
    };

    enum VoiceType {
        VOICE_TYPE_NORMAL,
        VOICE_TYPE_STREAM,
    };

    enum SrcType {
        SRC_NONE,
        SRC_LINEAR,
        SRC_4TAP_8K,
        SRC_4TAP_12K,
        SRC_4TAP_16K,
        SRC_4TAP_AUTO,
    };

    struct MixParam {
        u16 vL;     // at 0x0
        u16 vR;     // at 0x2
        u16 vS;     // at 0x4
        u16 vAuxAL; // at 0x6
        u16 vAuxAR; // at 0x8
        u16 vAuxAS; // at 0xA
        u16 vAuxBL; // at 0xC
        u16 vAuxBR; // at 0xE
        u16 vAuxBS; // at 0x10
        u16 vAuxCL; // at 0x12
        u16 vAuxCR; // at 0x14
        u16 vAuxCS; // at 0x16
    };

    struct RemoteMixParam {
        u16 vMain0; // at 0x0
        u16 vAux0;  // at 0x2
        u16 vMain1; // at 0x4
        u16 vAux1;  // at 0x6
        u16 vMain2; // at 0x8
        u16 vAux2;  // at 0xA
        u16 vMain3; // at 0xC
        u16 vAux3;  // at 0xE
    };

public:
    static u32 GetDspAddressBySample(const void *base, u32 samples, Format fmt);
    static u32 GetSampleByDspAddress(const void *base, u32 addr, Format fmt);
    static u32 GetSampleByByte(u32 addr, Format fmt);
    static void CalcOffsetAdpcmParam(u16 *outPredScale, u16 *outYn1, u16 *outYn2, u32 offset, const void *data,
            const AdpcmParam &param);

    AxVoice();
    ~AxVoice();

    bool IsRun() const {
        return mVpb.IsRun();
    }
    void Run() {
        mVpb.SetVoiceStateRun();
    }
    void Stop() {
        mVpb.SetVoiceStateStop();
    }
    void Sync() {
        mVpb.Sync();
    }

    Format GetFormat() const {
        return mFormat;
    }

    void SetBaseAddress(const void *base) {
        mWaveData = base;
    }

    f32 GetDspRatio(f32 ratio) const {
        return (ratio * mSampleRate) / AX_SAMPLE_RATE;
    }

    void Setup(const void *wave, Format fmt, int rate);

    bool IsPlayFinished() const;
    void SetLoopStart(const void *base, u32 samples);
    void SetLoopEnd(const void *base, u32 samples);
    void SetLoopFlag(bool loop);
    void StopAtPoint(const void *base, u32 samples);

    bool IsDataAddressCoverd(const void *begin, const void *end) const;
    u32 GetCurrentPlayingSample() const;
    u32 GetCurrentPlayingDspAddress() const;
    u32 GetLoopEndDspAddress() const;

    void SetPriority(u32 prio);
    void SetVoiceType(VoiceType type);
    void EnableRemote(bool enable);
    void ResetDelta();
    void SetAddr(bool loop, const void *wave, u32 offset, u32 loopStart, u32 loopEnd);
    void SetSrcType(SrcType type, f32 pitch);
    void SetAdpcm(const AdpcmParam *param);
    void SetAdpcmLoop(const AdpcmLoopParam *param);
    bool SetMix(const MixParam &param);
    void SetRmtMix(const RemoteMixParam &param);
    void SetSrc(f32 ratio, bool initial);
    void SetVe(f32 volume, f32 initVolume);
    void SetLpf(u16 freq);
    void SetRemoteFilter(u8 filter);

private:
    static void VoiceCallback(void *arg);

private:
    AxVoiceParamBlock mVpb; // at 0x0
    const void *mWaveData;  // at 0x10
    Format mFormat;         // at 0x14
    int mSampleRate;        // at 0x18
    bool mFirstMixUpdate;   // at 0x1C
    bool mReserveForFree;   // at 0x1D
    MixParam mMixPrev;      // at 0x1E
    Callback mCallback;     // at 0x38
    void *mCallbackArg;     // at 0x3C

public:
    NW4R_UT_LIST_NODE_DECL(); // at 0x40
};

NW4R_UT_LIST_TYPEDEF_DECL(AxVoice);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
