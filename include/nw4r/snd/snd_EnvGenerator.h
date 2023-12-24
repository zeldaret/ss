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

    void Init(f32 db);
    void Reset(f32 db);
    f32 GetValue() const;
    void Update(int msec);

    Status GetStatus() const { return mStatus; }
    void SetStatus(Status status) { mStatus = status; }

    void SetAttack(int attack);
    void SetDecay(int decay);
    void SetSustain(int sustain);
    void SetRelease(int release);

    f32 CalcRelease(int release);
    int CalcDecibelSquare(int scale);

private:
    static const volatile f32 VOLUME_INIT;
    static const s16 DecibelSquareTable[128];

    Status mStatus; // at 0x0
    f32 mValue;     // at 0x4
    f32 mDecay;     // at 0x8
    f32 mRelease;   // at 0xC
    f32 mAttack;    // at 0x10
    u8 mSustain;    // at 0x14
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
