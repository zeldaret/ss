#pragma once

#include "types.h"

namespace EGG
{
    
class File {
public:
    File() : mIsOpen(false) {}
public:
    /* vt 0x08 */ virtual ~File() {}
    /* vt 0x0C */ virtual bool open(const char* path) = 0;
    /* vt 0x10 */ virtual void close() = 0;
    /* vt 0x14 */ virtual s32 readData(void* buffer, s32 length, s32 offset) = 0;
    /* vt 0x18 */ virtual s32 writeData(const void* buffer, s32 length, s32 offset) = 0;
    /* vt 0x1C */ virtual u32 getFileSize() const = 0;

    /* 0x4 */ bool mIsOpen;
    
    private: u8 pad[3];
};

} // namespace EGG
