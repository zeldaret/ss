
#include <Runtime.PPCEABI.H/__va_arg.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

template <typename T>
T *TextWriterBase<T>::mFormatBuffer;
template <typename T>
int TextWriterBase<T>::mFormatBufferSize = 0x100;

template <typename T>
TagProcessorBase<T> TextWriterBase<T>::mDefaultTagProcessor;

template <typename T>
TextWriterBase<T>::TextWriterBase()
    : mCharSpace(0.0f), mWidthLimit(NW4R_MATH_FLT_MAX), mLineSpace(0.0f), mTabWidth(4), mDrawFlag(0),
      mTagProcessor(&mDefaultTagProcessor) {}

template <typename T>
TextWriterBase<T>::~TextWriterBase() {}

template <typename T>
void TextWriterBase<T>::SetLineHeight(f32 height) {
    const Font *font = GetFont();
    int lf = font != NULL ? font->GetLineFeed() : 0;
    mLineSpace = height - lf * GetScaleV();
}

template <typename T>
f32 TextWriterBase<T>::GetLineHeight() const {
    const Font *font = GetFont();
    int lf = font != NULL ? font->GetLineFeed() : 0;
    return mLineSpace + GetScaleV() * lf;
}

template <typename T>
f32 TextWriterBase<T>::CalcFormatStringWidth(const T *format, ...) const {
    Rect rect;
    _va_list_struct args;

    va_start(args, format);
    CalcVStringRect(&rect, format, &args);
    va_end(args);

    return rect.GetWidth();
}

template <typename T>
f32 TextWriterBase<T>::CalcFormatStringHeight(const T *format, ...) const {
    Rect rect;
    _va_list_struct args;

    va_start(args, format);
    CalcVStringRect(&rect, format, &args);
    va_end(args);

    return rect.GetHeight();
}

template <typename T>
void TextWriterBase<T>::CalcFormatStringRect(Rect *rect, const T *format, ...) const {
    _va_list_struct args;

    va_start(args, format);
    CalcVStringRect(rect, format, &args);
    va_end(args);
}

template <typename T>
void TextWriterBase<T>::CalcVStringRect(Rect *rect, const T *format, va_list args) const {
    T *pBuffer;

    if (mFormatBuffer != NULL) {
        pBuffer = mFormatBuffer;
    } else {
        pBuffer = static_cast<T *>(__alloca(mFormatBufferSize));
    }

    int len = VSNPrintf(pBuffer, mFormatBufferSize, format, args);
    len = Min(len, mFormatBufferSize - 1);

    CalcStringRect(rect, pBuffer, len);
}

template <typename T>
f32 TextWriterBase<T>::CalcStringWidth(const T *str, int len) const {
    Rect rect;
    CalcStringRect(&rect, str, len);
    return rect.GetWidth();
}

template <typename T>
f32 TextWriterBase<T>::CalcStringHeight(const T *str, int len) const {
    Rect rect;
    CalcStringRect(&rect, str, len);
    return rect.GetHeight();
}

template <typename T>
void TextWriterBase<T>::CalcStringRect(Rect *rect, const T *str, int len) const {
    TextWriterBase<T> clone(*this);
    clone.CalcStringRectImpl(rect, str, len);
}

template <typename T>
f32 TextWriterBase<T>::Printf(const T *format, ...) {
    va_list args;
    va_start(args[0], format);
    f32 width = VPrintf(format, args);
    va_end(args[0]);
    return width;
}

template <typename T>
f32 TextWriterBase<T>::VPrintf(const T *str, va_list args) {
    T *pBuffer;

    if (mFormatBuffer != NULL) {
        pBuffer = mFormatBuffer;
    } else {
        pBuffer = static_cast<T *>(__alloca(mFormatBufferSize));
    }

    int len = VSNPrintf(pBuffer, mFormatBufferSize, str, args);
    len = Min(len, mFormatBufferSize - 1);
    return Print(pBuffer, len);
}

template <typename T>
f32 TextWriterBase<T>::Print(const T *str, int len) {
    TextWriterBase<T> clone(*this);

    f32 width = clone.PrintImpl(str, len, false);
    SetCursor(clone.GetCursorX(), clone.GetCursorY());

    return width;
}

template <typename T>
f32 TextWriterBase<T>::PrintfMutable(const T *format, ...) {
    va_list args;
    va_start(args[0], format);
    f32 width = VPrintfMutable(format, args);
    va_end(args[0]);
    return width;
}

template <typename T>
f32 TextWriterBase<T>::VPrintfMutable(const T *format, va_list args) {
    T *pBuffer;

    if (mFormatBuffer != NULL) {
        pBuffer = mFormatBuffer;
    } else {
        pBuffer = static_cast<T *>(__alloca(mFormatBufferSize));
    }

    int len = VSNPrintf(pBuffer, mFormatBufferSize, format, args);
    len = Min(len, mFormatBufferSize - 1);
    return PrintMutable(pBuffer, len);
}

template <typename T>
f32 TextWriterBase<T>::PrintMutable(const T *str, int len) {
    return PrintImpl(str, len, true);
}

template <typename T>
f32 TextWriterBase<T>::CalcLineWidth(const T *str, int len) {
    Rect rect;
    TextWriterBase<T> clone(*this);

    clone.SetCursor(0.0f, 0.0f);
    clone.CalcLineRectImpl(&rect, &str, len);

    return rect.GetWidth();
}

template <typename T>
int TextWriterBase<T>::CalcLineRectImpl(Rect *pRect, const T **pStr, int length) {
    const T *strBegin = *pStr;
    const T *strEnd = strBegin + length;
    const bool useLimit = mWidthLimit < NW4R_MATH_FLT_MAX;

    PrintContext<T> context = {this, strBegin};
    f32 x = 0.0f;
    bool charSpace = false;
    bool overLimit = false;

    const T *prevStream = NULL;

    Rect prevRect;

    CharStrmReader reader = GetFont()->GetCharStrmReader();

    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = Min(0.0f, GetLineHeight());
    pRect->bottom = Max(0.0f, GetLineHeight());
    prevRect = *pRect;

    reader.Set(strBegin);
    prevStream = NULL;

    u16 code = reader.Next();

    while (static_cast<const T *>(reader.GetCurrentPos()) <= strEnd) {
        if (code < ' ') {
            Rect r(x, 0.0f, 0.0f, 0.0f);
            context.str = static_cast<const T *>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : PRINTFLAGS_CHARSPACE;
            SetCursorX(x);

            if (useLimit && code != '\n' && prevStream != NULL) {
                PrintContext<T> context2 = context;
                TextWriterBase<T> clone(*this);

                Rect r;
                context2.writer = &clone;
                mTagProcessor->CalcRect(&r, code, &context2);

                if (r.GetWidth() > 0.0f) {
                    if (clone.GetCursorX() - context.x > mWidthLimit) {
                        overLimit = true;
                        code = '\n';
                        reader.Set(prevStream);
                        continue;
                    }
                }
            }

            Operation oper = mTagProcessor->CalcRect(&r, code, &context);
            reader.Set(context.str);

            pRect->left = Min(pRect->left, r.left);
            pRect->top = Min(pRect->top, r.top);
            pRect->right = Max(pRect->right, r.right);
            pRect->bottom = Max(pRect->bottom, r.bottom);

            x = GetCursorX();

            if (oper == OPERATION_END_DRAW) {
                *pStr += length;
                return false;
            }

            if (oper == OPERATION_NO_CHAR_SPACE) {
                charSpace = false;
            } else if (oper == OPERATION_CHAR_SPACE) {
                charSpace = true;
            } else if (oper == OPERATION_NEXT_LINE) {
                break;
            }
        } else {
            f32 dx = 0.0f;

            if (charSpace) {
                dx += GetCharSpace();
            }

            if (IsWidthFixed()) {
                dx += GetFixedWidth();
            } else {
                dx += GetFont()->GetCharWidth(code) * GetScaleH();
            }
            if (useLimit && prevStream != NULL) {
                if (x + dx > mWidthLimit) {
                    overLimit = true;
                    code = '\n';
                    reader.Set(prevStream);
                    continue;
                }
            }

            x += dx;
            pRect->left = Min(pRect->left, x);
            pRect->right = Max(pRect->right, x);

            charSpace = true;
        }

        if (useLimit) {
            prevStream = static_cast<const T *>(reader.GetCurrentPos());
        }

        code = reader.Next();
    }

    *pStr = static_cast<const T *>(reader.GetCurrentPos());
    return overLimit;
}

template <typename T>
void TextWriterBase<T>::CalcStringRectImpl(Rect *rect, const T *str, int len) {
    const T *end = str + len;
    int remain = len;

    rect->left = 0.0f;
    rect->right = 0.0f;
    rect->top = 0.0f;
    rect->bottom = 0.0f;

    SetCursor(0.0f, 0.0f);

    do {
        Rect r;
        CalcLineRectImpl(&r, &str, remain);
        remain = end - str;

        rect->left = Min(rect->left, r.left);
        rect->top = Min(rect->top, r.top);
        rect->right = Max(rect->right, r.right);
        rect->bottom = Max(rect->bottom, r.bottom);
    } while (remain > 0);
}

template <typename T>
f32 TextWriterBase<T>::PrintImpl(const T *str, int len, bool m) {
    f32 cursorX = GetCursorX();
    f32 cursorY = GetCursorY();

    bool useLimit = mWidthLimit < NW4R_MATH_FLT_MAX;

    f32 orgCursorX = cursorX;
    f32 orgCursorY = cursorY;

    f32 cursorXAdj = 0.0f;
    f32 cursorYAdj = 0.0f;

    bool charSpace = false;

    const T *prevStream = str;
    const T *prevNewline = str;

    f32 textWidth = AdjustCursor(&cursorX, &cursorY, str, len);

    cursorXAdj = orgCursorX - GetCursorX();
    cursorYAdj = orgCursorY - GetCursorY();

    PrintContext<T> context = {this, str, cursorX, cursorY};

    CharStrmReader reader = GetFont()->GetCharStrmReader();
    reader.Set(str);

    Operation oper;
    u16 ch = reader.Next();

    while (static_cast<const T *>(reader.GetCurrentPos()) - str <= len) {
        if (ch < ' ') {
            context.str = static_cast<const T *>(reader.GetCurrentPos());
            context.flags = charSpace ? 0 : PRINTFLAGS_CHARSPACE;

            if (useLimit && ch != '\n' && prevStream != prevNewline) {
                PrintContext<T> context2 = context;
                TextWriterBase<T> clone(*this);
                Rect rect;

                context2.writer = &clone;
                oper = mTagProcessor->CalcRect(&rect, ch, &context2);

                if (rect.GetWidth() > 0.0f && clone.GetCursorX() - context.x > mWidthLimit) {
                    ch = '\n';
                    reader.Set(prevStream);
                    continue;
                }
            }

            oper = mTagProcessor->Process(ch, &context);
            if (oper == OPERATION_NEXT_LINE) {
                if (IsDrawFlagSet(0x3, 0x1)) {
                    int remain = len - (context.str - str);
                    f32 width = CalcLineWidth(context.str, remain);
                    f32 offset = (textWidth - width) / 2.0f;
                    SetCursorX(context.x + offset);
                } else if (IsDrawFlagSet(0x3, 0x2)) {
                    int remain = len - (context.str - str);
                    f32 width = CalcLineWidth(context.str, remain);
                    f32 offset = textWidth - width;
                    SetCursorX(context.x + offset);
                } else {
                    f32 width = GetCursorX() - context.x;
                    textWidth = Max(textWidth, width);
                    SetCursorX(context.x);
                }

                if (useLimit) {
                    prevNewline = static_cast<const T *>(reader.GetCurrentPos());
                }

                charSpace = false;
            } else if (oper == OPERATION_NO_CHAR_SPACE) {
                charSpace = false;
            } else if (oper == OPERATION_CHAR_SPACE) {
                charSpace = true;
            } else if (oper == OPERATION_END_DRAW) {
                break;
            }

            reader.Set(context.str);
        } else {
            f32 baseY = GetCursorY();
            if (useLimit && prevStream != prevNewline) {
                f32 baseX = GetCursorX();
                f32 space = charSpace ? GetCharSpace() : 0.0f;
                f32 width = IsWidthFixed() ? GetFixedWidth() : GetFont()->GetCharWidth(ch) * GetScaleH();
                if (baseX - cursorX + space + width > mWidthLimit) {
                    ch = '\n';
                    reader.Set(prevStream);
                    continue;
                }
            }

            if (charSpace) {
                MoveCursorX(GetCharSpace());
            }

            charSpace = true;

            const Font *font = GetFont();
            f32 adj = -font->GetBaselinePos() * GetScaleV();
            MoveCursorY(adj);
            CharWriter::Print(ch);
            SetCursorY(baseY);
        }

        if (useLimit) {
            prevStream = static_cast<const T *>(reader.GetCurrentPos());
        }

        ch = reader.Next();
    }

    f32 width = GetCursorX() - context.x;
    textWidth = Max(textWidth, width);

    if (IsDrawFlagSet(0x300, 0x100) || IsDrawFlagSet(0x300, 0x200)) {
        SetCursorY(orgCursorY);
    } else if (m) {
        if (IsDrawFlagSet(0x300, 0)) {
            SetCursorY(GetCursorY() - GetFontAscent());
        }
    } else {
        MoveCursorY(cursorYAdj);
    }

    return textWidth;
}

template <typename T>
f32 TextWriterBase<T>::AdjustCursor(f32 *x1, f32 *y1, const T *str, int len) {
    f32 textWidth = 0.0f;
    f32 textHeight = 0.0f;

    if (!IsDrawFlagSet(0x333, 0x300) && !IsDrawFlagSet(0x333, 0)) {
        Rect rect;
        CalcStringRect(&rect, str, len);

        textWidth = rect.left + rect.right;
        textHeight = rect.top + rect.bottom;
    }

    if (IsDrawFlagSet(0x30, 0x10)) {
        *x1 -= textWidth / 2;
    } else if (IsDrawFlagSet(0x30, 0x20)) {
        *x1 -= textWidth;
    }

    if (IsDrawFlagSet(0x300, 0x100)) {
        *y1 -= textHeight / 2;
    } else if (IsDrawFlagSet(0x300, 0x200)) {
        *y1 -= textHeight;
    }

    if (IsDrawFlagSet(0x3, 0x1)) {
        SetCursorX(*x1 + (textWidth - CalcLineWidth(str, len)) / 2);
    } else if (IsDrawFlagSet(0x3, 0x2)) {
        SetCursorX(*x1 + (textWidth - CalcLineWidth(str, len)));
    } else {
        SetCursorX(*x1);
    }

    if (IsDrawFlagSet(0x300, 0x300)) {
        SetCursorY(*y1);
    } else {
        SetCursorY(*y1 + GetFontAscent());
    }

    return textWidth;
}

template struct TextWriterBase<char>;
template struct TextWriterBase<wchar_t>;

} // namespace ut
} // namespace nw4r
