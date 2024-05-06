#ifndef NW4R_UT_TEXT_WRITER_BASE_H
#define NW4R_UT_TEXT_WRITER_BASE_H
#include <MSL_C/stdio.h>
#include <MSL_C/wchar.h>
#include <nw4r/math.h>
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_CharWriter.h>
#include <nw4r/ut/ut_TagProcessorBase.h>

namespace nw4r {
namespace ut {

template <typename T>
class TextWriterBase : public CharWriter {
public:
    static T *GetBuffer() {
        return mFormatBuffer;
    }
    static T *SetBuffer(T *buffer, u32 size) {
        T *old = mFormatBuffer;
        mFormatBuffer = buffer;
        mFormatBufferSize = size;
        return old;
    }

    static u32 GetBufferSize() {
        return mFormatBufferSize;
    }

    TextWriterBase();
    ~TextWriterBase();

    f32 GetWidthLimit() const {
        return mWidthLimit;
    }
    void SetWidthLimit(f32 limit) {
        mWidthLimit = limit;
    }
    void ResetWidthLimit() {
        mWidthLimit = NW4R_MATH_FLT_MAX;
    }

    f32 GetCharSpace() const {
        return mCharSpace;
    }
    void SetCharSpace(f32 space) {
        mCharSpace = space;
    }

    f32 GetLineSpace() const {
        return mLineSpace;
    }
    void SetLineSpace(f32 space) {
        mLineSpace = space;
    }

    int GetTabWidth() const {
        return mTabWidth;
    }
    void SetTabWidth(int width) {
        mTabWidth = width;
    }

    void SetDrawFlag(u32 flag) {
        mDrawFlag = flag;
    }
    bool IsDrawFlagSet(unsigned long mask, u32 flag) const {
        return (mDrawFlag & mask) == flag;
    }

    TagProcessorBase<T> *GetTagProcessor() const {
        return mTagProcessor;
    }
    void SetTagProcessor(TagProcessorBase<T> *processor) {
        mTagProcessor = processor;
    }
    void ResetTagProcessor() {
        mTagProcessor = &mDefaultTagProcessor;
    }

    void SetLineHeight(f32 height);
    f32 GetLineHeight() const;

    f32 CalcFormatStringWidth(const T *str, ...) const;
    f32 CalcFormatStringHeight(const T *str, ...) const;
    void CalcFormatStringRect(Rect *rect, const T *str, ...) const;
    void CalcVStringRect(Rect *rect, const T *str, va_list args) const;

    f32 CalcStringWidth(const T *format, int len) const;
    f32 CalcStringHeight(const T *format, int len) const;
    void CalcStringRect(Rect *rect, const T *format, int len) const;

    f32 Printf(const T *format, ...);
    f32 VPrintf(const T *str, va_list args);
    f32 Print(const T *str, int len);
    f32 PrintfMutable(const T *format, ...);
    f32 VPrintfMutable(const T *format, va_list args);
    f32 PrintMutable(const T *str, int n);

    // static int VSNPrintf(T *buffer, unsigned long count, const T *fmt, va_list args);

    static int VSNPrintf(char *buffer, unsigned long count, const char *fmt, va_list args) {
        return vsnprintf(buffer, count, fmt, args);
    }

    static int VSNPrintf(wchar_t *buffer, unsigned long count, const wchar_t *fmt, va_list args) {
        return vswprintf(buffer, count, fmt, args);
    }

    f32 CalcLineWidth(const T *format, int len);
    bool CalcLineRectImpl(Rect *rect, const T **str, int len);
    void CalcStringRectImpl(Rect *rect, const T *str, int len);
    f32 PrintImpl(const T *str, int len, bool m);
    f32 AdjustCursor(f32 *x1, f32 *y1, const T *str, int len);

private:
    f32 mWidthLimit;                    // at 0x4C
    f32 mCharSpace;                     // at 0x50
    f32 mLineSpace;                     // at 0x54
    int mTabWidth;                      // at 0x58
    u32 mDrawFlag;                      // at 0x5C
    TagProcessorBase<T> *mTagProcessor; // at 0x60

    static T *mFormatBuffer;
    static int mFormatBufferSize;
    static TagProcessorBase<T> mDefaultTagProcessor;
};

// template <>
// int TextWriterBase<char>::VSNPrintf(char *buffer, unsigned long count, const char *fmt, va_list args) {
//     return vsnprintf(buffer, count, fmt, args);
// }

// template <>
// int TextWriterBase<wchar_t>::VSNPrintf(wchar_t *buffer, unsigned long count, const wchar_t *fmt, va_list args) {
//     return vswprintf(buffer, count, fmt, args);
// }

} // namespace ut
} // namespace nw4r

#endif
