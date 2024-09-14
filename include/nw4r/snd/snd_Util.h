#ifndef NW4R_SND_UTIL_H
#define NW4R_SND_UTIL_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Common.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace Util {

enum PanCurve {
    PAN_CURVE_SQRT,
    PAN_CURVE_SINCOS,
    PAN_CURVE_LINEAR,
};

struct PanInfo {
    PanCurve curve;  // at 0x0
    bool centerZero; // at 0x4
    bool zeroClamp;  // at 0x5

    PanInfo() : curve(PAN_CURVE_SQRT), centerZero(false), zeroClamp(false) {}
};

enum RefType {
    REFTYPE_ADDRESS,
    REFTYPE_OFFSET,
};

enum DataType {
    DATATYPE_T0,
    DATATYPE_T1,
    DATATYPE_T2,
    DATATYPE_T3,
    DATATYPE_INVALID
};

template <typename T0, typename T1 = void, typename T2 = void,
          typename T3 = void>
struct DataRef {
    u8 refType;   // at 0x0
    u8 dataType;  // at 0x1
    u16 reserved; // at 0x2
    u32 value;    // at 0x4
};

template <typename T> struct Table {
    u32 count; // at 0x0
    T items[]; // at 0x4
};

template <typename T0, typename T1, typename T2, typename T3>
inline const T0* GetDataRefAddress0(const DataRef<T0, T1, T2, T3>& rRef,
                                    const void* pBase) {
    return static_cast<const T0*>(GetDataRefAddressImpl(
        static_cast<RefType>(rRef.refType), rRef.value, pBase));
}

template <typename T0, typename T1, typename T2, typename T3>
inline const T1* GetDataRefAddress1(const DataRef<T0, T1, T2, T3>& rRef,
                                    const void* pBase) {
    return static_cast<const T1*>(GetDataRefAddressImpl(
        static_cast<RefType>(rRef.refType), rRef.value, pBase));
}

template <typename T0, typename T1, typename T2, typename T3>
inline const T2* GetDataRefAddress2(const DataRef<T0, T1, T2, T3>& rRef,
                                    const void* pBase) {
    return static_cast<const T2*>(GetDataRefAddressImpl(
        static_cast<RefType>(rRef.refType), rRef.value, pBase));
}

template <typename T0, typename T1, typename T2, typename T3>
inline const T3* GetDataRefAddress3(const DataRef<T0, T1, T2, T3>& rRef,
                                    const void* pBase) {
    return static_cast<const T3*>(GetDataRefAddressImpl(
        static_cast<RefType>(rRef.refType), rRef.value, pBase));
}

#ifdef NW4R_LITLE_ENDIAN
inline u16 ReadBigEndian(u16 x) {
    return x >> 8 | x << 8;
}

inline u32 ReadBigEndian(u32 x) {
    return (x >> 24) & 0x000000FF | (x >> 8) & 0x0000FF00 |
           (x << 8) & 0x00FF0000 | (x << 24) & 0xFF000000;
}
#else
inline u16 ReadBigEndian(u16 x) {
    return x;
}

inline u32 ReadBigEndian(u32 x) {
    return x;
}
#endif

f32 CalcPitchRatio(int pitch);
f32 CalcVolumeRatio(f32 db);
f32 CalcPanRatio(f32 pan, const PanInfo& rInfo);
f32 CalcSurroundPanRatio(f32 pan, const PanInfo& rInfo);
int CalcLpfFreq(f32 scale);
void GetRemoteFilterCoefs(int filter, u16* pB0, u16* pB1, u16* pB2, u16* pA1,
                          u16* pA2);
u16 CalcRandom();
const void* GetDataRefAddressImpl(RefType type, u32 value, const void* pBase);

} // namespace Util
} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
