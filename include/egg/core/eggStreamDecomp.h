#ifndef EGG_STREAM_DECOMP_H
#define EGG_STREAM_DECOMP_H

#include "common.h"
#include "rvl/CX/cx.h"

// NOTE: I put StreamDecomp stuff here. im unsure of the correct place

namespace EGG {
class StreamDecomp {
public:
    /* vt 0x08 */ virtual bool init(void *pDest, u32 maxCompressedSize);
    /* vt 0x0C */ virtual bool decomp(const void *pSrc, u32 len);
    /* vt 0x10 */ virtual u32 getHeaderSize();
    /* vt 0x14 */ virtual u32 getUncompressedSize(const void *pSrc);

public:
    /* 0x04 */ void *mpDest;
    /* 0x08 */ u32 mMaxCompressedSize;
};

class StreamDecompLZ : public StreamDecomp {
public:
    virtual bool init(void *pDest, u32 maxCompressedSize) override;
    virtual bool decomp(const void *pSrc, u32 len) override;
    virtual u32 getHeaderSize() override {
        return 0x20;
    }
    virtual u32 getUncompressedSize(const void *pSrc) override {
        return CXGetUncompressedSize(pSrc);
    }

public:
    /* 0x0C */ CXUncompContextLZ mContext;
};
class StreamDecompRL : public StreamDecomp {
public:
    virtual bool init(void *pDest, u32 maxCompressedSize) override;
    virtual bool decomp(const void *pSrc, u32 len) override;
    virtual u32 getHeaderSize() override {
        return 0x20;
    }
    virtual u32 getUncompressedSize(const void *pSrc) override {
        return CXGetUncompressedSize(pSrc);
    }

public:
    /* 0x0C */ CXUncompContextRL mContext;
};

class StreamDecompLH : public StreamDecomp {
public:
    virtual bool init(void *pDest, u32 maxCompressedSize) override;
    virtual bool decomp(const void *pSrc, u32 len) override;
    virtual u32 getHeaderSize() override {
        return 0x20;
    }
    virtual u32 getUncompressedSize(const void *pSrc) override {
        return CXGetUncompressedSize(pSrc);
    }

public:
    /* 0x0C */ CXUncompContextLH mContext;
};
class StreamDecompLRC : public StreamDecomp {
public:
    virtual bool init(void *pDest, u32 maxCompressedSize) override;
    virtual bool decomp(const void *pSrc, u32 len) override;
    virtual u32 getHeaderSize() override {
        return 0x20;
    }
    virtual u32 getUncompressedSize(const void *pSrc) override {
        return CXGetUncompressedSize(pSrc);
    }

public:
    /* 0x0C */ CXUncompContextLRC mContext;
};

// NON-OFFICIAL
struct UncompContextSZS {
    static u32 getUncompressedSize(const void *pSrc);
    void initUncompContext(void *pDest);
    static s32 readHeader(u8 *pHeaderLen, s32 *pUncompSize, const u8 *src, u32 maxCompLen, s32 maxUncompSize);
    s32 readUncomp(const void *pSrca, u32 len);

    /* 0x00 */ u8 *mpDest;
    /* 0x04 */ s32 mUncompSize;
    /* 0x08 */ s32 mMaxUncompSize;
    /* 0x0C */ u8 mGroupHead;
    /* 0x0D */ u8 mSecondByte;
    /* 0x0E */ u8 mFirstByte;
    /* 0x0F */ u8 mState;
    /* 0x10 */ u16 mCopySrc;
    /* 0x12 */ u8 mHeaderLen;
};

class StreamDecompSZS : public StreamDecomp {
public:
    virtual bool init(void *pDest, u32 maxCompressedSize) override;
    virtual bool decomp(const void *pSrc, u32 len) override;
    virtual u32 getHeaderSize() override {
        return 0x20;
    }
    virtual u32 getUncompressedSize(const void *pSrc) override {
        return mContext.getUncompressedSize(pSrc);
    }

public:
    /* 0x0C */ UncompContextSZS mContext;
};
} // namespace EGG

#endif
