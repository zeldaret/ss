#ifndef NW4R_LYT_TYPES_H
#define NW4R_LYT_TYPES_H

#include <nw4r/lyt/lyt_common.h>
#include <nw4r/ut/ut_algorithm.h>

namespace nw4r {
namespace lyt {

struct Size {
    inline Size() : width(), height() {}

    inline Size &operator=(const Size &rhs) {
        width = rhs.width;
        height = rhs.height;
        return *this;
    }

    f32 width, height;
};

namespace res {
// struct AnimationBlock {}; // in lyt_animation.h
// struct AnimationTagBlock {}; // in lyt_animation.h
// struct AnimationShareBlock {}; // in lyt_animation.h
// struct Bounding {}; // in lyt_bounding.h
// struct Group {}; // in lyt_group.h
// struct Material {}; // in lyt_material.h
// struct Pane {}; // in lyt_pane.h
// struct TextBox {}; // in lyt_textbox.h
// struct Window {}; // in lyt_window.h
// struct BinaryFileHeader {}; // in lyt_common.h

// IDK Proper place for this one
struct FontList {
    DataBlockHeader blockHeader; // at 0x00
    u16 fontNum;                 // at 0x08
    u8 padding[2];               // at 0x0A
};

struct TextureList {
    DataBlockHeader blockHeader; // at 0x00
    u16 texNum;                  // at 0x08
    u8 padding[2];               // at 0x0A
};

// belong in Animation?
struct HermiteKey {};
struct StepKey {};

} // namespace res

namespace detail {
template <typename T>
inline bool TestBit(T bits, int index) {
    T mask = 1 << index;
    return bits & mask;
}
template <typename T>
inline void SetBit(T *bits, int pos, bool val) {
    T mask = T(1 << pos);
    *bits = T((*bits & ~mask)) | (val << pos);
}

template <typename T>
T *ConvertOffsetToPtr(const void *baseAddress, unsigned int offset) {
    return (T *)((u32)baseAddress + offset);
}

} // namespace detail

} // namespace lyt

} // namespace nw4r

#endif
