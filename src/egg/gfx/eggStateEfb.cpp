#include "egg/gfx/eggStateEfb.h"

#include "common.h"
#include "egg/gfx/eggTextureBuffer.h"

namespace EGG {

StateEfb::Buffer StateEfb::spBufferSet[BUFFER_MAX];
f32 StateEfb::sWorkSpaceV[6];
f32 StateEfb::sWorkSpaceHideV[6];
f32 StateEfb::sUnkBuffer[6];

u32 StateEfb::sFlag = 4;
s32 StateEfb::sWorkBuffer = -1;

s32 StateEfb::sPushCount;

void StateEfb::Clean() {
    sFlag &= ~(1 | 2);
    sWorkBuffer = -1;
    sPushCount = 0;
    
    for (u32 i = 0; i < ARRAY_LENGTH(spBufferSet); i++) {
        spBufferSet[i].field_0x08 = 0;
    }
    
    for (u32 i = 0; i < ARRAY_LENGTH(sWorkSpaceV); i++) {
        sWorkSpaceV[i] = 0.0f;
        sWorkSpaceHideV[i] = 0.0f;
    }
}

bool StateEfb::releaseEfb(BufferType type, u32 userData) {
    if (spBufferSet[type].buf != nullptr && spBufferSet[type].userData == userData) {
        spBufferSet[type].buf->free();
        spBufferSet[type].buf = nullptr;
        spBufferSet[type].userData = 0;
        spBufferSet[type].field_0x08 = 0;
        return true;
    } else {
        return false;
    }
}

} // namespace EGG
