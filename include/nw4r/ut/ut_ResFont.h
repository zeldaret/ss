#ifndef NW4R_UT_RES_FONT_H
#define NW4R_UT_RES_FONT_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_ResFontBase.h>

namespace nw4r {
namespace ut {

class ResFont : public detail::ResFontBase {
public:
    static FontInformation* Rebuild(BinaryFileHeader* header);

    ResFont();
    ~ResFont();

    bool SetResource(void* buffer);
};

} // namespace ut
} // namespace nw4r

#endif
