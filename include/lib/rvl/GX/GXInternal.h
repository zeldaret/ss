#ifndef RVL_SDK_GX_INTERNAL_H
#define RVL_SDK_GX_INTERNAL_H
#include <lib/rvl/GX/GXTypes.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * GX internal structures.
 *
 * Wouldn't be necessary if the public ones didn't include padding; but they do,
 * so there has to be different structure definitions.
 *
 * These internal structures are implemented like the RFL ones since we don't
 * have DWARF info for most GX structures.
 */

/**
 * Declare a public structure from the corresponding internal structure.
 * (Implementation size is included to require that such a structure already
 * exists.)
 */
#define GX_DECL_PUBLIC_STRUCT(name, size)                                      \
    typedef struct _##name {                                                   \
        u8 dummy[(size) - sizeof(name##Impl) + sizeof(name##Impl)];            \
    } name;

typedef struct _GXFifoObjImpl {
    void* base;        // at 0x0
    void* end;         // at 0x4
    u32 size;          // at 0x8
    void* hiWatermark; // at 0xC
    void* loWatermark; // at 0x10
    void* readPtr;     // at 0x14
    void* writePtr;    // at 0x18
    u32 count;         // at 0x1C
    u8 wrap;           // at 0x20
} GXFifoObjImpl;

typedef struct _GXLightObjImpl {
    char UNK_0x0[0xC];
    GXColor color; // at 0xC
    f32 aa;        // at 0x10
    f32 ab;        // at 0x14
    f32 ac;        // at 0x18
    f32 ka;        // at 0x1C
    f32 kb;        // at 0x20
    f32 kc;        // at 0x24
    f32 posX;      // at 0x28
    f32 posY;      // at 0x2C
    f32 posZ;      // at 0x30
    f32 dirX;      // at 0x34
    f32 dirY;      // at 0x38
    f32 dirZ;      // at 0x3C
} GXLightObjImpl;

typedef struct _GXTexObjImpl {
    u8 todo;
} GXTexObjImpl;

#ifdef __cplusplus
}
#endif
#endif
