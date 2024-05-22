#include <egg/core/eggHeap.h>
#include <d/d_dvd_unk.h>
#include <nw4r/ut/ut_ResFont.h>

namespace dDvdUnk {

/** 805750d0 */
static unkstruct_c *singleton;

static nw4r::ut::ResFont *fontPtr;

/** 80052300 */
unkstruct_c *unkstruct_c::create(EGG::Heap *heap) {
    singleton = new (heap, 0x04) unkstruct_c();
    singleton->init();
}

/** 80052340 */
char unkstruct_c::getUnk() {
    return field_0x4;
}

/** 80052350 */
void unkstruct_c::draw() {
    // EGG::
}

/** 800526b0 */
void unkstruct_c::execute() {

}

/** 80052750 */
void unkstruct_c::init() {
    field_0x0 = 0;
    field_0x4 = 0;
}


void unkstruct_c::createFont(EGG::Heap *heap) {
    fontPtr = new (heap, 0x04) nw4r::ut::ResFont();
}

} // namespace dDvdUnk
