#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

template <typename T>
TagProcessorBase<T>::TagProcessorBase() {}

template <typename T>
TagProcessorBase<T>::~TagProcessorBase() {}

template <typename T>
Operation TagProcessorBase<T>::Process(u16 ch, PrintContext<T> *ctx) {
    switch (ch) {
    case '\n':
        ProcessLinefeed(ctx);
        return OPERATION_NEXT_LINE;
    case '\t':
        ProcessTab(ctx);
        return OPERATION_NO_CHAR_SPACE;
    }

    return OPERATION_DEFAULT;
}

template <typename T>
Operation TagProcessorBase<T>::CalcRect(Rect *rect, u16 ch, PrintContext<T> *ctx) {
    switch (ch) {
    case '\n': {
        const TextWriterBase<T> &writer = *ctx->writer;
        rect->right = writer.GetCursorX();
        rect->top = writer.GetCursorY();
        ProcessLinefeed(ctx);
        rect->left = writer.GetCursorX();
        rect->bottom = writer.GetCursorY() + ctx->writer->GetFontHeight();
        rect->Normalize();
        return OPERATION_NEXT_LINE;
    }
    case '\t': {
        const TextWriterBase<T> &writer = *ctx->writer;
        rect->left = writer.GetCursorX();
        ProcessTab(ctx);
        rect->right = writer.GetCursorX();
        rect->top = writer.GetCursorY();
        rect->bottom = rect->top + writer.GetFontHeight();
        rect->Normalize();
        return OPERATION_NO_CHAR_SPACE;
    }
    }

    return OPERATION_DEFAULT;
}

template <typename T>
void TagProcessorBase<T>::ProcessTab(PrintContext<T> *ctx) {
    TextWriterBase<T> &writer = *ctx->writer;

    int tabWidth = writer.GetTabWidth();
    if (tabWidth <= 0) {
        return;
    }

    f32 charWidth = writer.IsWidthFixed() ? writer.GetFixedWidth() : writer.GetFontWidth();

    f32 dx = writer.GetCursorX() - ctx->x;
    f32 tabPixel = tabWidth * charWidth;
    int numTab = static_cast<int>(dx / tabPixel) + 1;
    f32 x = ctx->x + (tabPixel * numTab);

    writer.SetCursorX(x);
}

template <typename T>
void TagProcessorBase<T>::ProcessLinefeed(PrintContext<T> *ctx) {
    TextWriterBase<T> &writer = *ctx->writer;

    f32 x = ctx->x;
    f32 y = writer.GetCursorY() + writer.GetLineHeight();

    writer.SetCursorX(x);
    writer.SetCursorY(y);
}

template class TagProcessorBase<char>;
template class TagProcessorBase<wchar_t>;

} // namespace ut
} // namespace nw4r
