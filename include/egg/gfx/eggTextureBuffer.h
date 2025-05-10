#ifndef EGG_TEXTURE_BUFFER_H
#define EGG_TEXTURE_BUFFER_H

#include "egg/gfx/eggCapTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class TextureBuffer : public CapTexture {
public:
    enum EBufferState {
        STATE_FREE,
        STATE_ALLOCED
    };

    static void initialize(u32, Heap *);
    static TextureBuffer *getNotJoin();

    static void append(TextureBuffer *buf) {
        if (spTailNotJoin != NULL) {
            spTailNotJoin->mpNext = buf;
        }

        buf->mpPrev = spTailNotJoin;
        buf->mpNext = NULL;

        spTailNotJoin = buf;
    }

    TextureBuffer();
    virtual ~TextureBuffer() {} // at 0x8
    virtual void configure();   // at 0xC

    void alloc(u32 size);
    static TextureBuffer *alloc(u16 w, u16 h, GXTexFmt fmt);
    void free();

private:
    static bool includesHeader();
    static void setIncludesHeader(bool);

    u32 mSize;             // at 0x2C
    EBufferState mState;   // at 0x30
    TextureBuffer *mpNext; // at 0x34
    TextureBuffer *mpPrev; // at 0x38

    static const int NUM_BUFFERS = 128;
    static TextureBuffer *spHead;
    static TextureBuffer *spTailNotJoin;
    static char *spBufferAll;
    static u32 sBufferAllSize;
    static u32 sBufferSize;
    static u32 sBufferRestSizeMin;
    static bool sIncludesHeader;
    static TextureBuffer *spBufferTable[NUM_BUFFERS];
};

} // namespace EGG

#endif
