#ifndef NW4R_UT_TAG_PROCESSOR_BASE_H
#define NW4R_UT_TAG_PROCESSOR_BASE_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_Rect.h>

namespace nw4r {
namespace ut {

enum PrintFlags {
    PRINTFLAGS_CHARSPACE = (1 << 0),
};

template <typename T> struct PrintContext {
    TextWriterBase<T>* writer; // at 0x0
    const T* str;              // at 0x4
    f32 x;                     // at 0x8
    f32 y;                     // at 0xC
    u32 flags;                 // at 0x10
};

enum Operation {
    OPERATION_DEFAULT,
    OPERATION_NO_CHAR_SPACE,
    OPERATION_CHAR_SPACE,
    OPERATION_NEXT_LINE,
    OPERATION_END_DRAW
};

template <typename T> class TagProcessorBase {
public:
    TagProcessorBase();
    virtual ~TagProcessorBase(); // at 0x8

    virtual Operation Process(u16 ch, PrintContext<T>* ctx); // at 0xC
    virtual Operation CalcRect(Rect* rect, u16 ch,
                               PrintContext<T>* ctx); // at 0x10

    void ProcessTab(PrintContext<T>* ctx);
    void ProcessLinefeed(PrintContext<T>* ctx);
};

} // namespace ut
} // namespace nw4r

#endif
