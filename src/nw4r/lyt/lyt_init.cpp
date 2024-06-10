#include <rvl/OS.h>
namespace {
const char *NW4R_LYT_Version_ = "<< NW4R    - LYT \tfinal   build: Mar 15 2010 02:17:33 (0x4302_158) >>";
}

namespace nw4r {
namespace lyt {
void LytInit() {
    OSRegisterVersion(NW4R_LYT_Version_);
    OSInitFastCast();
}
} // namespace lyt

} // namespace nw4r
