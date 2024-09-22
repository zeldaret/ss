#include <egg/core/eggDecomp.h>

namespace EGG {

// *********************************** LZ ********************************** //

bool StreamDecompLZ::init(void *pDest, u32 maxCompressedSize) {
    mpDest = pDest;
    mMaxCompressedSize = maxCompressedSize;
    CXInitUncompContextLZ(&mContext);
    return true;
}

bool StreamDecompLZ::decomp(const void *pSrc, u32 len) {
    return CXReadUncompLZ(&mContext, pSrc, len) == CX_READ_OK;
}

// *********************************** RL ********************************** //

bool StreamDecompRL::init(void *pDest, u32 maxCompressedSize) {
    mpDest = pDest;
    mMaxCompressedSize = maxCompressedSize;
    CXInitUncompContextRL(&mContext);
    return true;
}

bool StreamDecompRL::decomp(const void *pSrc, u32 len) {
    return CXReadUncompRL(&mContext, pSrc, len) == CX_READ_OK;
}

// *********************************** LH ********************************** //

bool StreamDecompLH::init(void *pDest, u32 maxCompressedSize) {
    mpDest = pDest;
    mMaxCompressedSize = maxCompressedSize;
    CXInitUncompContextLH(&mContext);
    return true;
}

bool StreamDecompLH::decomp(const void *pSrc, u32 len) {
    return CXReadUncompLH(&mContext, pSrc, len) == CX_READ_OK;
}

// ********************************** LRC ********************************** //

bool StreamDecompLRC::init(void *pDest, u32 maxCompressedSize) {
    mpDest = pDest;
    mMaxCompressedSize = maxCompressedSize;
    CXInitUncompContextLRC(&mContext);
    return true;
}

bool StreamDecompLRC::decomp(const void *pSrc, u32 len) {
    return CXReadUncompLRC(&mContext, pSrc, len) == CX_READ_OK;
}

// ********************************** SZS ********************************** //

u32 UncompContextSZS::getUncompressedSize(const void *pSrc) {
    const u8 *pData = (u8 *)pSrc;
    return (pData[4] << 24) | (pData[5] << 16) | (pData[6] << 8) | (pData[7] << 0);
}

void UncompContextSZS::initUncompContext(void *pDest) {
    mpDest = (u8 *)pDest;
    mUncompSize = 0;
    mHeaderLen = 0x10;
    mMaxUncompSize = 0;
    _x0C = 0;
    _x0D = 0;
    _x0E = 0;
    mChunkType = 0;
    _x10 = 0;
}

s32 UncompContextSZS::readHeader(u8 *pHeaderLen, s32 *pUncompSize, const u8 *src, u32 maxCompLen, s32 maxUncompSize) {
    s32 read_amount = 0;
    while (*pHeaderLen != 0) {
        (*pHeaderLen)--;
        if (*pHeaderLen == 0xF) {
            if (*src != (u8)'Y') {
                return -1;
            }
        } else if (*pHeaderLen == 0xE) {
            if (*src != (u8)'a') {
                return -1;
            }
        } else if (*pHeaderLen == 0xD) {
            if (*src != (u8)'z') {
                return -1;
            }
        } else if (*pHeaderLen == 0xC) {
            if (*src != (u8)'0') {
                return -1;
            }
        } else if (*pHeaderLen >= 0x8) {
            *pUncompSize |= *src << ((*pHeaderLen - 8) << 3);
        }

        maxCompLen--;
        src++;
        read_amount++;

        if (maxCompLen == 0 && *pHeaderLen != 0) {
            return read_amount;
        }
    }
    if (maxUncompSize > 0 && maxUncompSize < *pUncompSize) {
        *pUncompSize = maxUncompSize;
    }
    return read_amount;
}

s32 UncompContextSZS::readUncomp(const void *pSrc, u32 len) {
    // TODO
    if (mHeaderLen != 0) {
        s32 read_len = readHeader(&mHeaderLen, &mUncompSize, (u8 *)pSrc, len, mMaxUncompSize);
        pSrc = (u8 *)pSrc - read_len;
        len = len - read_len;

        if (len == 0) {
            if (mHeaderLen == 0) {
                return mUncompSize;
            }
            return CX_READ_ERR_1;
        }
    }

    do {
        u32 uncompressed_left = mUncompSize;
        if (uncompressed_left < 1) {
            break;
        }
        if (mChunkType == 2) {
            u8 byte = *(u8 *)pSrc;
            pSrc = (u8 *)pSrc + 1;
            len--;

            u16 chunkLen = byte + 0x12;
            if (uncompressed_left < chunkLen) {
                if (mMaxUncompSize == 0) {
                    return CX_READ_ERR_4;
                }
                chunkLen = uncompressed_left;
            }
            mUncompSize -= chunkLen;
            do {
                chunkLen--;
                *mpDest = mpDest[-_x10];
                mpDest++;
            } while (chunkLen != 0);
            mChunkType = 0;
        } else if (mChunkType == 1) {
        } else {
        }

    } while (len != 0);

    if (mUncompSize == 0 && mMaxUncompSize == 0 && len > 0x20) {
        return CX_READ_ERR_3;
    }
    return mUncompSize;
}

bool StreamDecompSZS::init(void *pDest, u32 maxCompressedSize) {
    mpDest = pDest;
    mMaxCompressedSize = maxCompressedSize;
    mContext.initUncompContext(pDest);
    return true;
}

bool StreamDecompSZS::decomp(const void *pSrc, u32 len) {
    return mContext.readUncomp(pSrc, len) == CX_READ_OK;
}

} // namespace EGG
