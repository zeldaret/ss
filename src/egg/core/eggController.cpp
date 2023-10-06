#include "egg/core/eggController.h"


namespace EGG
{
    
void CoreControllerMgr::beginFrame() {
    for (int i = 0; i < mControllers.getSize(); ++i) {
        mControllers(i)->beginFrame(NULL);
    }
} 


} // namespace EGG
