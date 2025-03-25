#ifndef EGG_STATE_EFB_H
#define EGG_STATE_EFB_H

#include "common.h"
#include "egg/egg_types.h"

namespace EGG {

class StateEfb {
    enum BufferType {
        BUFFER_0,
        BUFFER_1,
        BUFFER_2,
        BUFFER_3,

        BUFFER_MAX,
    };

    struct Buffer {
        TextureBuffer *buf;
        u32 userData;
        u32 field_0x08;
    };

    static Buffer spBufferSet[BUFFER_MAX];
    static f32 sWorkSpaceV[6];
    static f32 sWorkSpaceHideV[6];
    static f32 sUnkBuffer[6]; // unk name
    static s32 sWorkBuffer;
    static s32 sPushCount;
    static u32 sFlag;

    static f32 *fn_804B4550();
    static bool GetShouldCapture();
    static bool GetUseTfRgb565();

public:
    static void Clean();
    static TextureBuffer *captureEfb(BufferType type, bool, u32 userData);
    static bool releaseEfb(BufferType type, u32 userData);

    // Unknown names
    static void fn_804B4270(BufferType type, u32 userData);
    static void fn_804B4310(BufferType type, u32 userData);
};

} // namespace EGG

#endif
