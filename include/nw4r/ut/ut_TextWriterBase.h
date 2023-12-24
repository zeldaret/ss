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
    bool IsDrawFlagSet(u32 mask, u32 flag) const {
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

    f32 GetLineHeight() const;

    f32 CalcLineWidth(const T *str, int len);
    f32 CalcStringWidth(const T *str, int len) const;
    void CalcStringRect(Rect *rect, const T *str, int len) const;

    int VSNPrintf(T *buffer, u32 count, const T *fmt, va_list args);
    f32 VPrintf(const T *str, va_list args);
    f32 Print(const T *str, int len);

    bool CalcLineRectImpl(Rect *rect, const T **str, int len);
    void CalcStringRectImpl(Rect *rect, const T *str, int len);

    f32 PrintImpl(const T *str, int len);
    f32 AdjustCursor(f32 *x1, f32 *y1, const T *str, int len);

private:
    f32 mWidthLimit;                    // at 0x4C
    f32 mCharSpace;                     // at 0x50
    f32 mLineSpace;                     // at 0x54
    int mTabWidth;                      // at 0x58
    u32 mDrawFlag;                      // at 0x5C
    TagProcessorBase<T> *mTagProcessor; // at 0x60

    static T *mFormatBuffer;
    static u32 mFormatBufferSize;
    static TagProcessorBase<T> mDefaultTagProcessor;
};

template <>
inline int TextWriterBase<char>::VSNPrintf(char *buffer, u32 count, const char *fmt, va_list args) {
    return vsnprintf(buffer, count, fmt, args);
}

template <>
inline int TextWriterBase<wchar_t>::VSNPrintf(wchar_t *buffer, u32 count, const wchar_t *fmt, va_list args) {
    return vswprintf(buffer, count, fmt, args);
}

} // namespace ut
} // namespace nw4r

#endif
