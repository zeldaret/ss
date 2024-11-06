#ifndef NW4R_SND_AX_VOICE_H
#define NW4R_SND_AX_VOICE_H
#include "nw4r/snd/snd_Common.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/AX.h"  // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

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

    void SetVoiceAddr(const AXPBADDR &rAddr) {
        if (IsAvailable()) {
            // AXSetVoiceAddr doesn't actually modify the object
            AXSetVoiceAddr(mVpb, const_cast<AXPBADDR *>(&rAddr));
        }
    }

    void SetVoicePriority(u32 priority) {
        if (IsAvailable()) {
            AXSetVoicePriority(mVpb, priority);
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

    void Set(AXVPB *pVpb);
    void Clear();

    void SetVoiceType(u16 type);
    void SetVoiceVe(u16 volume, u16 initVolume);
    void SetVoiceMix(const AXPBMIX &rMix, bool syncNow);
    void SetVoiceLoop(u16 loop);
    void SetVoiceLoopAddr(u32 addr);
    void SetVoiceEndAddr(u32 addr);
    void SetVoiceAdpcm(const AXPBADPCM &rAdpcm);
    void SetVoiceSrcType(u32 type);
    void SetVoiceSrc(const AXPBSRC &rSrc);
    void SetVoiceSrcRatio(f32 ratio);
    void SetVoiceAdpcmLoop(const AXPBADPCMLOOP &rLoop);
    void SetVoiceLpf(const AXPBLPF &rLpf);
    void SetVoiceLpfCoefs(u16 a0, u16 b0);
    void SetVoiceRmtOn(u16 on);
    void SetVoiceRmtMix(const AXPBRMTMIX &rMix);
    void SetVoiceRmtIIR(const AXPBRMTIIR &rIir);
    void SetVoiceRmtIIRCoefs(u16 type, ...);
    void UpdateDelta();

private:
    static const u16 DEFAULT_VOLUME = AX_MAX_VOLUME;

private:
    AXVPB *mVpb;                    // at 0x0
    u32 mSync;                      // at 0x4
    volatile AXPBVE mPrevVeSetting; // at 0x8
    bool mFirstVeUpdateFlag;        // at 0xC
    u16 mVolume;                    // at 0xE
};

class AxVoice {
    friend class AxVoiceManager;

public:
    enum AxVoiceCallbackStatus {
        CALLBACK_STATUS_CANCEL,
        CALLBACK_STATUS_DROP_DSP
    };

    typedef void (*AxVoiceCallback)(AxVoice *pDropVoice, AxVoiceCallbackStatus status, void *pArg);

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

    SampleFormat GetFormat() const {
        return mFormat;
    }

    void SetBaseAddress(const void *pBase) {
        mWaveData = pBase;
    }

    f32 GetDspRatio(f32 ratio) const {
        return (ratio * mSampleRate) / AX_SAMPLE_RATE;
    }

    void Setup(const void *pWave, SampleFormat fmt, int rate);

    bool IsPlayFinished() const;
    void SetLoopStart(const void *pBase, u32 samples);
    void SetLoopEnd(const void *pBase, u32 samples);
    void SetLoopFlag(bool loop);
    void StopAtPoint(const void *pBase, u32 samples);

    bool IsDataAddressCoverd(const void *pBegin, const void *pEnd) const;
    u32 GetCurrentPlayingSample() const;
    u32 GetCurrentPlayingDspAddress() const;
    u32 GetLoopEndDspAddress() const;

    void SetPriority(u32 priority);
    void SetVoiceType(VoiceType type);
    void EnableRemote(bool enable);
    void ResetDelta();
    void SetAddr(bool loop, const void *pWave, u32 offset, u32 loopStart, u32 loopEnd);
    void SetSrcType(SrcType type, f32 pitch);
    void SetAdpcm(const AdpcmParam *pParam);
    void SetAdpcmLoop(const AdpcmLoopParam *pParam);
    bool SetMix(const MixParam &rParam);
    void SetRmtMix(const RemoteMixParam &rParam);
    void SetSrc(f32 ratio, bool initial);
    void SetVe(f32 volume, f32 initVolume);
    void SetLpf(u16 freq);
    void SetRemoteFilter(u8 filter);

    static u32 GetDspAddressBySample(const void *pBase, u32 samples, SampleFormat fmt);
    static u32 GetSampleByDspAddress(const void *pBase, u32 addr, SampleFormat fmt);
    static u32 GetSampleByByte(u32 addr, SampleFormat fmt);

    static void CalcOffsetAdpcmParam(
        u16 *pPredScale, u16 *pYN1, u16 *pYN2, u32 offset, const void *pData, const AdpcmParam &rParam
    );

private:
    static void VoiceCallback(void *pArg);

private:
    AxVoiceParamBlock mVpb;    // at 0x0
    const void *mWaveData;     // at 0x10
    SampleFormat mFormat;            // at 0x14
    int mSampleRate;           // at 0x18
    bool mFirstMixUpdateFlag;  // at 0x1C
    bool mReserveForFreeFlag;  // at 0x1D
    MixParam mMixPrev;         // at 0x1E
    AxVoiceCallback mCallback; // at 0x38
    void *mCallbackData;       // at 0x3C

public:
    NW4R_UT_LIST_NODE_DECL(); // at 0x40
};

NW4R_UT_LIST_TYPEDEF_DECL(AxVoice);

inline int CalcAxvpbDelta(u16 init, u16 target) {
    return (target - init) / AX_SAMPLES_PER_FRAME;
}

inline u16 CalcMixVolume(f32 volume) {
    return ut::Min<u32>(0xFFFF, AX_MAX_VOLUME * volume);
}

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
