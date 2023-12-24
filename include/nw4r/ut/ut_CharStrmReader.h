#ifndef NW4R_UT_CHAR_STRM_READER_H
#define NW4R_UT_CHAR_STRM_READER_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace ut {

class CharStrmReader {
public:
    typedef u16 (CharStrmReader::*ReadFunc)();

    CharStrmReader(ReadFunc func) : mCharStrm(NULL), mReadFunc(func) {}
    ~CharStrmReader() {}

    u16 ReadNextCharUTF8();
    u16 ReadNextCharUTF16();
    u16 ReadNextCharCP1252();
    u16 ReadNextCharSJIS();

    const void* GetCurrentPos() const { return mCharStrm; }

    template <typename T> T GetChar(int offset) const {
        return static_cast<const T*>(mCharStrm)[offset];
    }

    template <typename T> void StepStrm(int offset) {
        static_cast<const T*>(mCharStrm) += offset;
    }

    u16 Next() { return (this->*mReadFunc)(); }

    void Set(const char* strm) { mCharStrm = strm; }
    void Set(const wchar_t* strm) { mCharStrm = strm; }

private:
    const void* mCharStrm; // at 0x0
    ReadFunc mReadFunc;    // at 0x4
};

} // namespace ut
} // namespace nw4r

#endif
