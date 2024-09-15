#ifndef NW4R_SND_ENV_GENERATOR_H
#define NW4R_SND_ENV_GENERATOR_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

class EnvGenerator {
public:
    enum Status { STATUS_ATTACK, STATUS_DECAY, STATUS_SUSTAIN, STATUS_RELEASE };

public:
    EnvGenerator();

    void Init(f32 db = VOLUME_INIT);
    void Reset(f32 db = VOLUME_INIT);
    f32 GetValue() const;
    void Update(int msec);

    Status GetStatus() const {
        return mStatus;
    }
    void SetStatus(Status status) {
        mStatus = status;
    }

    void SetAttack(int attack);
    void SetDecay(int decay);
    void SetSustain(int sustain);
    void SetRelease(int release);

private:
    static const int DECIBEL_SQUARE_TABLE_SIZE = 128;

    static const volatile f32 VOLUME_INIT;
    static const int ATTACK_INIT = 127;
    static const int DECAY_INIT = 127;
    static const int SUSTAIN_INIT = 127;
    static const int RELEASE_INIT = 127;

private:
    f32 CalcRelease(int release);
    int CalcDecibelSquare(int scale);

private:
    Status mStatus; // at 0x0
    f32 mValue;     // at 0x4
    f32 mDecay;     // at 0x8
    f32 mRelease;   // at 0xC
    f32 mAttack;    // at 0x10
    u8 mSustain;    // at 0x14

    static const s16 DecibelSquareTable[DECIBEL_SQUARE_TABLE_SIZE];
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
