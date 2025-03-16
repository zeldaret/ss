#include "egg/gfx/eggTextureBuffer.h"

#include "common.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggStateGX.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

TextureBuffer::TextureBuffer() : mSize(0), mState(STATE_FREE), mpNext(nullptr), mpPrev(nullptr) {}

void TextureBuffer::configure() {
    CapTexture::configure();
    setClearColor(StateGX::getDefaultTexColor());
    setWrap(GX_CLAMP, GX_CLAMP);
    setFilt(GX_LINEAR, GX_LINEAR);
    setPixModeSync(true);
}

// Allow buffer to be reclaimed
void TextureBuffer::free() {
    mState = STATE_FREE;
    sBufferSize += mSize;

    // Check if previous buffer is reclaimable
    if (mpPrev != nullptr && mpPrev->mState == STATE_FREE) {
        // Merge with previous buffer
        mSize += mpPrev->mSize;
        setBuffer((u8 *)getBuffer() - mpPrev->mSize);

        // Append reusable buffer
        TextureBuffer *beforeAppend = mpPrev->mpPrev;
        append(mpPrev);

        // Repair mpPrev link
        mpPrev = beforeAppend;
        if (beforeAppend != nullptr) {
            beforeAppend->mpNext = this;
        } else {
            spHead = this;
        }
    }

    // Check if next buffer is reclaimable
    if (mpNext != nullptr && mpNext->mState == STATE_FREE) {
        // Merge with next buffer
        mSize += mpNext->mSize;

        // Append reusable buffer
        TextureBuffer *afterAppend = mpNext->mpNext;
        append(mpNext);

        // Repair mpNext link
        mpNext = afterAppend;
        if (afterAppend != nullptr) {
            afterAppend->mpPrev = this;
        }
    }
}

// Alloc list buffer
void TextureBuffer::initialize(u32 size, Heap *heap_) {
    Heap *heap = heap_ == nullptr ? Heap::getCurrentHeap() : heap_;
    if (includesHeader()) {
        size += 0x20;
    }

    sBufferAllSize = size;
    spBufferAll = new (heap, 32) char[size];

    if (includesHeader()) {
        spBufferAll += 0x20;
    }

    sBufferSize = size;
    sBufferRestSizeMin = size;
    spTailNotJoin = nullptr;
    for (int i = 0; i < NUM_BUFFERS; i++) {
        spBufferTable[i] = new TextureBuffer();
        spBufferTable[i]->configure();
        append(spBufferTable[i]);
    }

    // Initialize head
    spHead = getNotJoin();
    spHead->mState = STATE_FREE;
    spHead->mSize = size;
    spHead->setBuffer(spBufferAll);
}

// Remove and return buffer at list end
TextureBuffer *TextureBuffer::getNotJoin() {
    TextureBuffer *p_buffer = spTailNotJoin;

    // Move list tail back
    spTailNotJoin = spTailNotJoin->mpPrev;
    if (spTailNotJoin != nullptr) {
        spTailNotJoin->mpNext = nullptr;
    }

    // Pop old tail
    p_buffer->mpPrev = nullptr;
    return p_buffer;
}

// Attempt to alloc buffer of specified size
void TextureBuffer::alloc(u32 size) {
    size = ROUND_UP(size, 0x20);

    sBufferSize -= size;
    if (sBufferSize < sBufferRestSizeMin) {
        sBufferRestSizeMin = sBufferSize;
    }

    // Try to find a free buffer with at least the requested size
    for (TextureBuffer *p_list = spHead; p_list != nullptr; p_list = p_list->mpNext) {
        if (p_list->mState == STATE_FREE && p_list->mSize >= size) {
            // Assign free buffer data to new buffer
            mState = STATE_ALLOCED;
            mSize = size;

            u8 *buffer = (u8 *)p_list->getBuffer();
            if (includesHeader()) {
                setBuffer(buffer);
                if (buffer != nullptr) {
                    setFlag(HAS_HEADER);
                }
            } else {
                setBuffer(buffer);
            }

            // Prepend new buffer before old buffer
            mpNext = p_list;
            TextureBuffer *oldPrev = p_list->mpPrev;
            mpPrev = oldPrev;

            // Fix this->prev link
            if (oldPrev != nullptr) {
                oldPrev->mpNext = this;
            } else {
                // nullptr prev buffer must be list head

                spHead = this;
            }

            // Remove requested size from old buffer
            p_list->mSize -= size;

            // Free old buffer if now empty
            if (p_list->mSize == 0) {
                if (p_list->mpNext != nullptr) {
                    p_list->mpNext->mpPrev = this;
                }
                mpNext = p_list->mpNext;
                append(p_list);
            }
            // Adjust old buffer by requested size
            else {
                p_list->setBuffer(buffer + size);
                p_list->mpPrev = this;
            }

            return;
        }
    }
}

TextureBuffer *TextureBuffer::alloc(u16 width, u16 height, GXTexFmt fmt) {
    if (width == 0 || height == 0) {
        return nullptr;
    }
    TextureBuffer *tex = getNotJoin();
    tex->setWidth(width);
    tex->setHeight(height);
    tex->setFormat(fmt);
    tex->configure();
    tex->alloc(GXGetTexBufferSize(tex->getWidth(), tex->getHeight(), tex->getFormat(), false, 1));
    return tex;
}

void TextureBuffer::setIncludesHeader(bool b) {
    sIncludesHeader = b;
}

bool TextureBuffer::includesHeader() {
    return sIncludesHeader;
}

TextureBuffer *TextureBuffer::spHead;
TextureBuffer *TextureBuffer::spTailNotJoin;
char *TextureBuffer::spBufferAll;
u32 TextureBuffer::sBufferAllSize;
u32 TextureBuffer::sBufferSize;
u32 TextureBuffer::sBufferRestSizeMin;
bool TextureBuffer::sIncludesHeader;

} // namespace EGG
