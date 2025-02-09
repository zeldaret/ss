#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace {

inline void SetBind(u16* pId, u32 id) {
    *pId = id;
}

inline bool IsBound(const u16* pId) {
    return *pId != ResLightSetData::INVALID_ID;
}

} // namespace

bool ResLightSet::Bind(const ResAnmScn scene) {
    ResLightSetData& r = ref();

    int numAllLight = 0;
    int numBound = 0;

    if (HasAmbLight() && scene.HasResAnmAmbLight()) {
        numAllLight++;

        if (!IsAmbLightBound()) {
            ResName name = GetAmbLightResName();
            ResAnmAmbLight light = scene.GetResAnmAmbLight(name);

            if (light.IsValid()) {
                SetBind(&r.ambLightId, light.GetID());
                numBound++;
            }
        } else {
            numBound++;
        }
    }

    if (scene.HasResAnmLight()) {
        u32 numLight = GetNumLight();
        numAllLight += numLight;

        for (u32 i = 0; i < numLight; i++) {
            u16* pId = &r.lightId[i];

            if (IsBound(pId)) {
                numBound++;
                continue;
            }

            ResName name = GetLightResName(i);
            ResAnmLight light = scene.GetResAnmLight(name);

            if (light.IsValid()) {
                SetBind(pId, light.GetID());
                numBound++;
            }
        }
    }

    return numBound == numAllLight;
}

} // namespace g3d
} // namespace nw4r
