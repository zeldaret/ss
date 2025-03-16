#ifndef EGG_BINARY_H
#define EGG_BINARY_H

#include "common.h"

namespace EGG {

/**
 * @brief Common interface for objects that can be serialized to
 *        and deserializized from binary.
 *
 * @tparam T The type of the Egg class. Must have a BinData type
 *           that describes the binary type.
 */
template <class T>
class IBinary {
public:
#pragma pack(push, 1)
    struct BinHeader {
        /* 0x00 */ char mType[4];
        /* 0x04 */ size_t mSize;
        /* 0x08 */ u8 mVersion;
        /* 0x09 */ u16 field_0x09;
        /* 0x0B */ u8 field_0x0B;
        /* 0x0C */ u32 field_0x0C;
    };
    struct Bin {
        BinHeader mHeader;
        typename T::BinData mData;
    };
#pragma pack(pop)

protected:
    /** Load the object from binary */
    virtual void SetBinaryInner(const Bin &) = 0;
    /** Save the object to binary */
    virtual void GetBinaryInner(Bin *) const = 0;
    /** Get the size of the serialized data. Override if your binary has a dynamic size */
    virtual size_t GetBinarySize() const {
        return sizeof(Bin);
    }
    /** Load the object from binary, interpolating between values */
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) {}

    virtual ~IBinary() {}

    /** Get a 4-bytes long ID string corresponding to this type */
    static const char *GetBinaryType();
    /** Get the current version of this type */
    static int GetVersion();

public:
    // These functions below are automatically provided, you should not need to
    // touch them to implement de-/serialization for your type. They will parse
    // the binary header and then invoke the above virtual functions.
    void GetBinary(void *pData) const;
    void SetBinary(const void *);
    void SetBinaryBlend(const void *a, const void *b, f32 blend);
};

template <typename T>
void IBinary<T>::GetBinary(void *pData) const {
    Bin *pBin = reinterpret_cast<Bin *>(pData);
    BinHeader zeroedHeader = {0};
    pBin->mHeader = zeroedHeader;

    pBin->mHeader.mVersion = GetVersion();
    pBin->mHeader.mSize = GetBinarySize();
    for (int i = 0; i < 4; i++) {
        pBin->mHeader.mType[i] = GetBinaryType()[i];
    }
    typename T::BinData zeroedDatat = {0};
    pBin->mData = zeroedDatat;
    GetBinaryInner(pBin);
}

template <typename T>
void IBinary<T>::SetBinary(const void *a) {
    const Bin *pBinA = reinterpret_cast<const Bin *>(a);
    SetBinaryInner(*pBinA);
}

template <typename T>
void IBinary<T>::SetBinaryBlend(const void *a, const void *b, f32 blend) {
    const Bin *pBinA = reinterpret_cast<const Bin *>(a);
    const Bin *pBinB = reinterpret_cast<const Bin *>(b);
    SetBinaryInner(*pBinA, *pBinB, blend);
}

} // namespace EGG

#endif
