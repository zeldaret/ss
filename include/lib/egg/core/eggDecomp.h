#pragma once

#include <types.h>
#include <rvl/CX/cx.h>

// NOTE: I put StreamDecomp stuff here. im unsure of the correct place

namespace EGG
{
class StreamDecomp {
public:
    /* vt 0x08 */ virtual bool init(void* dest, u32 maxCompressedSize);
    /* vt 0x0C */ virtual bool decomp(const void* src, u32 len);
    /* vt 0x10 */ virtual u32 getHeaderSize();
    /* vt 0x14 */ virtual u32 getUncompressedSize(const void* src);
public:
    /* 0x04 */ void* dest;
    /* 0x08 */ u32 maxCompressedSize;
};

class StreamDecompLZ : public StreamDecomp {
public:
    /* vt 0x08 | 80494c80 */ virtual bool init(void* dest, u32 maxCompressedSize);
    /* vt 0x0C | 80494cb0 */ virtual bool decomp(const void* src, u32 len);
    /* vt 0x10 | 804952c0 */ virtual u32 getHeaderSize();
    /* vt 0x14 | 804952b0 */ virtual u32 getUncompressedSize(const void* src);
public:
    /* 0x0C */ CXUncompContextLZ context;
};
class StreamDecompRL : public StreamDecomp {
public:
    /* vt 0x08 | 80494ce0 */ virtual bool init(void* dest, u32 maxCompressedSize);
    /* vt 0x0C | 80494d10 */ virtual bool decomp(const void* src, u32 len);
    /* vt 0x10 | 804952a0 */ virtual u32 getHeaderSize();
    /* vt 0x14 | 80495290 */ virtual u32 getUncompressedSize(const void* src);
public:
    /* 0x0C */ CXUncompContextRL context;
};

class StreamDecompLH : public StreamDecomp {
public:
    /* vt 0x08 | 80494d40 */ virtual bool init(void* dest, u32 maxCompressedSize);
    /* vt 0x0C | 80494d70 */ virtual bool decomp(const void* src, u32 len);
    /* vt 0x10 | 80495280 */ virtual u32 getHeaderSize();
    /* vt 0x14 | 80495270 */ virtual u32 getUncompressedSize(const void* src);
public:
    /* 0x0C */ CXUncompContextLH context;
};
class StreamDecompLRC : public StreamDecomp {
public:
    /* vt 0x08 | 80494da0 */ virtual bool init(void* dest, u32 maxCompressedSize);
    /* vt 0x0C | 80494dd0 */ virtual bool decomp(const void* src, u32 len);
    /* vt 0x10 | 80495260 */ virtual u32 getHeaderSize();
    /* vt 0x14 | 80495250 */ virtual u32 getUncompressedSize(const void* src);
public:
    /* 0x0C */ CXUncompContextLRC context;
};
class StreamDecompSZS : public StreamDecomp {
public:
    /* vt 0x08 | 804951d0 */ virtual bool init(void* dest, u32 maxCompressedSize);
    /* vt 0x0C | 80495200 */ virtual bool decomp(const void* src, u32 len);
    /* vt 0x10 | 80495240 */ virtual u32 getHeaderSize();
    /* vt 0x14 | 80495230 */ virtual u32 getUncompressedSize(const void* src);
private:
    // NON-OFFICIAL
    struct SZSCompContext {
        u8 _[0x18];
        /* 80494e00 */ static u32 getUncompressedSize(const void* src);
        /* 80494e30 */ void init(u32 maxCompSize);
        /* 80494e60 */ static int readSzsHeader(/* unk params*/);
        /* 80494f60 */ void* decomp(const void* src, u32 len);
    };
public:
    /* 0x0C */ SZSCompContext context;
};
} // namespace EGG
