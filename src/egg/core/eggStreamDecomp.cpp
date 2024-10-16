#include "egg/core/eggStreamDecomp.h"

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
    mGroupHead = 0;
    mSecondByte = 0;
    mFirstByte = 0;
    mState = 0;
    mCopySrc = 0;
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

s32 UncompContextSZS::readUncomp(const void *pSrca, u32 len) {
    const u8 *pSrc = static_cast<const u8 *>(pSrca);
    if (mHeaderLen != 0) {
        s32 read_len = readHeader(&mHeaderLen, &mUncompSize, pSrc, len, mMaxUncompSize);
        pSrc += read_len;
        len -= read_len;

        if (len == 0) {
            if (mHeaderLen == 0) {
                return mUncompSize;
            }
            return CX_READ_ERR_1;
        }
    }

    while (mUncompSize > 0) {
        if (mState == 2) {
            u32 num_bytes = *pSrc++ + 0x12;
            len--;
            if (num_bytes > mUncompSize) {
                if (mMaxUncompSize == 0) {
                    return CX_READ_ERR_4;
                }
                num_bytes = (u16)mUncompSize;
            }
            mUncompSize -= num_bytes;
            do {
                *mpDest = *(mpDest - mCopySrc);
                mpDest++;
            } while (--num_bytes);
            mState = 0;
        } else if (mState == 1) {
            len--;
            u32 b1 = (mFirstByte << 8) | *pSrc++;
            u32 num_bytes = b1 >> 12;
            mCopySrc = (b1 & 0xFFF) + 1;

            if (num_bytes == 0) {
                mState = 2;
            } else {
                num_bytes += 2;
                if (num_bytes > mUncompSize) {
                    if (mMaxUncompSize == 0) {
                        return CX_READ_ERR_4;
                    }
                    num_bytes = (u16)mUncompSize;
                }
                mUncompSize -= num_bytes;
                do {
                    *mpDest = *(mpDest - mCopySrc);
                    mpDest++;
                } while (--num_bytes);
                mState = 0;
            }
        } else {
            if (mGroupHead == 0) {
                len--;
                mSecondByte = *pSrc++;
                mGroupHead = 0x80;
                if (len == 0) {
                    break;
                }
            }
            if (mSecondByte & mGroupHead) {
                *mpDest++ = *pSrc++;
                mUncompSize--;
            } else {
                mFirstByte = *pSrc++;
                mState = 1;
            }
            len--;
            mGroupHead >>= 1;
        }
        if (len == 0) {
            break;
        }
    }

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
    return mContext.readUncomp((const u8 *)pSrc, len) == CX_READ_OK;
}

} // namespace EGG
