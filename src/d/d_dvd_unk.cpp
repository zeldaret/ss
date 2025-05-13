#include "d/d_dvd_unk.h"

#include "d/d_dvd_drive_error.h"
#include "egg/core/eggHeap.h"
#include "toBeSorted/nand_result_tracker.h"


#include "d/dol_backup_error_font.inc"

namespace dDvdUnk {

FontUnk *FontUnk::sInstance;
extern "C" void fn_80059D10(EGG::Heap *heap);
void FontUnk::create(EGG::Heap *heap) {
    sInstance = new (heap, 0x04) FontUnk();
    sInstance->init();
    dDvdDriveError_c::create(heap);
    NandResultTracker::create(heap);
    fn_80059D10(heap);
    sInstance->field_0x2C = 0;
}

} // namespace dDvdUnk
