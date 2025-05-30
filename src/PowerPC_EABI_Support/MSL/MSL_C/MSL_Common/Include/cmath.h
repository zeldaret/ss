#ifndef STD_CMATH_H_
#define STD_CMATH_H_


namespace std {

inline float fabsf(float num) {
    return ::fabsf(num);
}

inline float abs(float x) {
    return ::fabsf(x);
}
}  // namespace std

#endif
