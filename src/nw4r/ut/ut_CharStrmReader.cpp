#include <nw4r/ut/ut_CharStrmReader.h>

namespace nw4r {
namespace ut {
namespace {

bool IsSJISLeadByte(u8 c) {
    // Idk how to force the comparison correctly
    return ((c >= 0x81) && (c < 0xA0)) || c >= 0xe0;
}

} // namespace

u16 CharStrmReader::ReadNextCharUTF8() {
    u16 code;

    if (!(GetChar<u8>(0) & 0x80)) {
        code = GetChar<u8>(0);
        StepStrm<u8>(1);
    } else if ((GetChar<u8>(0) & 0xe0) == 0xC0) {
        code = ((GetChar<u8>(0) & 0x1F) << 6) | GetChar<u8>(1) & 0x3F;
        StepStrm<u8>(2);
    } else {
        code = ((GetChar<u8>(0) & 0x1F) << 12) | ((GetChar<u8>(1) & 0x3F) << 6) | (GetChar<u8>(2) & 0x3F);
        StepStrm<u8>(3);
    }
    return code;
}

u16 CharStrmReader::ReadNextCharUTF16() {
    u16 code = GetChar<u16>(0);
    StepStrm<u16>(1);
    return code;
}
u16 CharStrmReader::ReadNextCharCP1252() {
    u16 code = GetChar<u8>(0);
    StepStrm<u8>(1);
    return code;
}
u16 CharStrmReader::ReadNextCharSJIS() {
    u16 code = GetChar<u8>(0);

    bool isLead = false;

    if ((((u8)GetChar<u8>(0) >= 0x81) && ((u8)GetChar<u8>(0) <= 0xA0)) || (u8)GetChar<u8>(0) >= 0xe0) {
        isLead = true;
    }

    if (isLead) {
        code = GetChar<u8>(1) | (GetChar<u8>(0) << 8);
        StepStrm<u8>(2);
    } else {
        code = GetChar<u8>(0);
        StepStrm<u8>(1);
    }
    return code;
}

} // namespace ut

} // namespace nw4r
