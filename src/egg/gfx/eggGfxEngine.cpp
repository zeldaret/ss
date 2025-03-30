#include "egg/gfx/eggGfxEngine.h"

#include "common.h"
#include "egg/core/eggDisplay.h"
#include "egg/core/eggSystem.h"
#include "egg/core/eggVideo.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

// Needed for dtor placement
void ProbablySomethingCreatingThisConfiguration() {
    GfxEngine::Configuration c;
}

GfxEngine::Configuration::Configuration()
    : efbWidth(640),
      efbHeight(528),
      clearColor((GXColor){0, 0, 0, 0}),
      field_0x08(0),
      field_0x0C(32),
      field_0x0E(256),
      bufferSize(0),
      field_0x14(608),
      field_0x16(812),
      field_0x18(0) {
    if (BaseSystem::getVideo() != nullptr) {
        efbWidth = BaseSystem::getVideo()->pRenderMode->fbWidth;
        efbHeight = BaseSystem::getVideo()->pRenderMode->efbHeight;
    }
    if (BaseSystem::getDisplay() != nullptr) {
        clearColor = BaseSystem::getDisplay()->getClearColor();
    }
    bufferSize = GXGetTexBufferSize(efbWidth, efbHeight, GX_TF_RGBA8, false, 0) * 3;
}

} // namespace EGG
