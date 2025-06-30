#ifndef EGG_STATE_EFB_H
#define EGG_STATE_EFB_H

#include "common.h"
#include "egg/egg_types.h"

namespace EGG {

class StateEfb {
public:
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
private:
    static Buffer spBufferSet[BUFFER_MAX];
    static f32 sWorkSpaceV[6];
    static f32 sWorkSpaceHideV[6];
    static f32 sUnkBuffer[6]; // unk name
    static s32 sWorkBuffer;
    static s32 sPushCount;
    static u32 sFlag;

    static bool isEnableDirtyBufferMode();
    static bool GetUseTfRgb565();
    
    public:
    static bool CheckFlag0x2() {
        return (sFlag & 2) != 0;
    }
    
    static void Clean();
    static TextureBuffer *captureEfb(BufferType type, bool, u32 userData);
    static bool releaseEfb(BufferType type, u32 userData);
    
    // Unknown names
    static void fn_804B4270(BufferType type, u32 userData);
    static void popWorkBuffer(bool b, u32 userData);
    static f32 *fn_804B4550();
};

} // namespace EGG

#endif
