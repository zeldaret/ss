#include "egg/gfx/eggG3DUtility.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resanmamblight.h"
#include "nw4r/g3d/res/g3d_resanmlight.h"
#include "nw4r/g3d/res/g3d_resanmscn.h"
#include "nw4r/g3d/res/g3d_reslightset.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/g3d/res/g3d_restev.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"

#include <cstring>

namespace EGG {

void G3DUtility::create(u32 size, Heap *pHeap) {
    if (pHeap == nullptr) {
        pHeap = Heap::getCurrentHeap();
    }

    if (size != 0) {
        sBuf = operator new[](size, pHeap, 0x20);
        sSize = size;
    } else {
        sBuf = nullptr;
        sSize = 0;
    }

    sOffset = 0;

    nw4r::g3d::G3DState::ScnDependentTexMtxFuncPtr pFunc;
    u32 id = 5;
    while (true) {
        if (nw4r::g3d::G3DState::GetScnDependentTexMtxFunc(id, &pFunc, nullptr) && pFunc == defaultTexMtxFunc) {
            break;
        }
        id++;
        if (id >= 256) {
            id = 5;
            nw4r::g3d::G3DState::ScnDependentTexMtxFuncPtr pFunc2;
            while (true) {
                if (nw4r::g3d::G3DState::GetScnDependentTexMtxFunc(id, &pFunc2, nullptr) &&
                    pFunc2 == nw4r::g3d::detail::ScnDependentMtxFunc::DefaultMapping) {
                    sId = id;
                    nw4r::g3d::G3DState::SetScnDependentTexMtxFunc(
                        id, defaultTexMtxFunc, nw4r::g3d::G3DState::SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_POS
                    );
                    break;
                }
                id++;
                if (id >= 256) {
                    break;
                }
            }
            return;
        }
    }
}

void G3DUtility::defaultTexMtxFunc(nw4r::math::MTX34 *pMtx, s8 camRef, s8 lightRef) {
    const nw4r::math::MTX34 *mtxPtr = nw4r::g3d::G3DState::GetInvCameraMtxPtr();
    PSMTXCopy(mtxPtr->m, pMtx->m);
}

bool G3DUtility::setUpLightSet(nw4r::g3d::LightSetting &lightSet, nw4r::g3d::ResAnmScn scn, int refNumber) {
    if (!scn.IsValid()) {
        return false;
    }

    int end = scn.GetResLightSetMaxRefNumber() < lightSet.GetNumLightSet() ? scn.GetResLightSetMaxRefNumber() :
                                                                             lightSet.GetNumLightSet();
    int start = 0;
    if (refNumber >= 0) {
        start = refNumber;
        end = refNumber + 1;
    }

    for (; start < end; start++) {
        nw4r::g3d::LightSet set = lightSet.GetLightSet(start);
        nw4r::g3d::ResLightSet resSet = scn.GetResLightSetByRefNumber(start);

        if (!set.IsValid()) {
            return false;
        }

        if (resSet.IsValid()) {
            u32 counter = 7;
            u32 j = 0;
            u32 numLight = resSet.GetNumLight();
            if (numLight != 0) {
                for (; j < numLight; j++) {
                    if (resSet.GetLightID(j) != 0xFFFF) {
                        nw4r::g3d::ResAnmLight anmLight = scn.GetResAnmLight(resSet.GetLightID(j));
                        set.SelectLightObj(j, anmLight.GetRefNumber());
                        if (anmLight.HasSpecularLight()) {
                            set.SelectLightObj(counter--, anmLight.GetSpecularLightIdx());
                        }
                    } else {
                        set.SelectLightObj(j, -1);
                    }
                }
            }

            for (; numLight <= counter; numLight++) {
                set.SelectLightObj(numLight, -1);
            }

            if (resSet.HasAmbLight()) {
                nw4r::g3d::ResAnmAmbLight anmAmbLight = scn.GetResAnmAmbLight(resSet.GetAmbLightID());
                set.SelectAmbLightObj(anmAmbLight.GetRefNumber());
            } else {
                set.SelectAmbLightObj(-1);
            }
        }
    }

    return true;
}

inline bool searchInner(
    const char *name, const char *candidate, bool exactMatch, G3DUtility::SearchResult *pResults, int i, int resultIdx,
    int maxNumResults
) {
    if (name != nullptr) {
        bool found;
        if (exactMatch) {
            found = !std::strcmp(candidate, name);
        } else {
            found = std::strstr(candidate, name);
        }
        if (!found) {
            goto err;
        }
    }

    if (resultIdx < maxNumResults) {
        pResults[resultIdx].nodeIdx = i;
        pResults[resultIdx].name = candidate;
    }
    return true;

err:
    return false;
}

int G3DUtility::searchStringResNode(
    nw4r::g3d::ResMdl mdl, const char *name, bool exactMatch, SearchResult *pResults, int maxNumResults
) {
    u16 resultIdx = 0;
    for (int i = 0; i < mdl.GetResNodeNumEntries(); i++) {
        nw4r::g3d::ResNode nd = mdl.GetResNode(i);
        const char *nodeName = nd.GetName();

        if (searchInner(name, nodeName, exactMatch, pResults, i, resultIdx, maxNumResults)) {
            resultIdx++;
        }
    }

    return resultIdx;
}

int G3DUtility::searchStringResMat(
    nw4r::g3d::ResMdl mdl, const char *name, bool exactMatch, SearchResult *pResults, int maxNumResults
) {
    u16 resultIdx = 0;
    for (int i = 0; i < mdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat = mdl.GetResMat(i);
        const char *nodeName = mat.GetName();

        if (searchInner(name, nodeName, exactMatch, pResults, i, resultIdx, maxNumResults)) {
            resultIdx++;
        }
    }

    return resultIdx;
}

int G3DUtility::searchStringResTexPlttInfo(
    nw4r::g3d::ResMdl mdl, const char *name, bool exactMatch, SearchResult *pResults, int maxNumResults
) {
    u16 resultIdx = 0;
    for (u32 i = 0; i < mdl.GetResTexPlttInfoOffsetFromTexNameNumEntries(); i++) {
        nw4r::g3d::ResTexPlttInfoOffset info = mdl.GetResTexPlttInfoOffsetFromTexName(i);
        for (u16 j = 0; j < info.GetNumData(); j++) {
            const char *nodeName = info.GetPllt(j).GetTexName();
            if (searchInner(name, nodeName, exactMatch, pResults, i, resultIdx, maxNumResults)) {
                resultIdx++;
            }
        }
    }

    return resultIdx;
}

u16 G3DUtility::SetTexture(
    nw4r::g3d::ResMat resMat, nw4r::g3d::ScnMdl::CopiedMatAccess *matAccess, const char *name, const GXTexObj *texObj,
    bool copy, SetTextureResult *pResult, int maxNumResults, int param_8
) {
    if (!resMat.IsValid()) {
        return 0;
    }

    u16 resultIdx = 0;
    for (u16 i = 0; i < resMat.GetNumResTexPlttInfo(); i++) {
        nw4r::g3d::ResTexPlttInfo plltInfo = resMat.GetResTexPlttInfo(i);
        const char *plltName = plltInfo.GetTexName();
        if (std::strcmp(name, plltName)) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            nw4r::g3d::ResTexObj resTexObj(nullptr);
            if (j == 0) {
                if ((matAccess == nullptr || param_8 == 1) ||
                    (param_8 == 0 && !matAccess->GetResTexObj(false).IsValid())) {
                    resTexObj = resMat.GetResTexObj();
                }
            } else if (matAccess != nullptr) {
                resTexObj = param_8 == 0 ? matAccess->GetResTexObjEx() : matAccess->GetResTexObj(param_8 == 3);
            }

            if (resTexObj.IsValid()) {
                GXTexObj *obj = resTexObj.GetTexObj(plltInfo.ref().mapID);
                if (copy) {
                    void *pImage;
                    u16 width;
                    u16 height;
                    GXTexFmt format;
                    GXTexWrapMode wrapS;
                    GXTexWrapMode wrapT;
                    u8 mipmap;
                    GXGetTexObjAll(obj, &pImage, &width, &height, &format, &wrapS, &wrapT, &mipmap);

                    GXTexFilter minFilter;
                    GXTexFilter magFilter;
                    f32 minLOD;
                    f32 maxLOD;
                    f32 LODBias;
                    u8 biasClampEnable;
                    u8 edgeLODEnable;
                    GXAnisotropy anisotropy;
                    GXGetTexObjLODAll(
                        obj, &minFilter, &magFilter, &minLOD, &maxLOD, &LODBias, &biasClampEnable, &edgeLODEnable,
                        &anisotropy
                    );

                    *obj = *texObj;
                    GXInitTexObjFilter(obj, minFilter, magFilter);
                    GXInitTexObjWrapMode(obj, wrapS, wrapT);
                } else {
                    *obj = *texObj;
                }
            }
        }

        if (pResult != nullptr && resultIdx < maxNumResults) {
            pResult[resultIdx].texCoordId = -1;
            pResult[resultIdx].texMapId = -1;

            nw4r::g3d::ResTev resTev = resMat.GetResTev();
            for (u8 stage = 0; stage < resTev.GetNumTevStages(); stage++) {
                GXTexCoordID coord;
                GXTexMapID map;
                if (resTev.GXGetTevOrder(static_cast<GXTevStageID>(stage), &coord, &map, nullptr) &&
                    map == plltInfo.ref().mapID) {
                    pResult[resultIdx].texMapId = map;
                    pResult[resultIdx].texCoordId = coord;
                }
            }
        }
        resultIdx++;
    }

    return resultIdx;
}

int G3DUtility::ApplyLightMat(nw4r::g3d::ResMdl mdl, const char *prefix) {
    GXTexCoordID matchingLayerCoords[8];

    int prefixLen = std::strlen(prefix);
    int result = 0;

    u32 i = 0;
    u32 numMats = mdl.GetResMatNumEntries();
    if (numMats != 0) {
        for (; i < numMats; i++) {
            int numMatchingLayers = 0;
            nw4r::g3d::ResMat mat = mdl.GetResMat(i);
            if (mat.GetResGenMode().GXGetNumTexGens() >= mat.GetNumResTexPlttInfo()) {
                for (u32 j = 0; j < mat.GetNumResTexPlttInfo(); j++) {
                    const char *texName = mat.GetResTexPlttInfo(j).GetTexName();
                    bool prefixMatch = true;
                    int rem = prefixLen;
                    for (int i = 0; i < prefixLen; i++) {
                        if (prefix[i] != texName[i]) {
                            prefixMatch = false;
                            break;
                        }
                    }
                    if (prefixMatch) {
                        matchingLayerCoords[numMatchingLayers++] = static_cast<GXTexCoordID>(j);
                    }
                }

                if (numMatchingLayers > 1) {
                    nw4r::g3d::ResGenMode genMode = mat.GetResGenMode();
                    int numTexGens = genMode.GXGetNumTexGens();
                    nw4r::g3d::ResMatTexCoordGen resMatTexCoordGen = mat.GetResMatTexCoordGen();
                    nw4r::g3d::ResTexSrt resTexSrt = mat.GetResTexSrt();
                    nw4r::g3d::ResTev resTev = mat.GetResTev();
                    GXTexGenType texGenType;
                    GXTexGenSrc param;
                    GXBool normalize;
                    u32 postMtx;
                    resMatTexCoordGen.GXGetTexCoordGen2(
                        matchingLayerCoords[0], &texGenType, &param, &normalize, &postMtx
                    );
                    numMatchingLayers--;
                    for (; numMatchingLayers > 0; numMatchingLayers--) {
                        GXTexCoordID idx = matchingLayerCoords[numMatchingLayers];
                        if (idx == numTexGens - 1) {
                            u32 numStages = resTev.GetNumTevStages();
                            for (u32 stage = 0; stage < numStages; stage++) {
                                GXTexCoordID coord;
                                GXTexMapID map;
                                GXChannelID channel;
                                if (resTev.GXGetTevOrder(static_cast<GXTevStageID>(stage), &coord, &map, &channel) &&
                                    coord == idx) {
                                    resTev.GXSetTevOrder(
                                        static_cast<GXTevStageID>(stage),
                                        matchingLayerCoords[0], map, channel
                                    );
                                }
                            }

                            int stage = 0;
                            int nInds = genMode.GXGetNumIndStages();
                            for (; stage < nInds; stage++) {
                                GXTexCoordID coord;
                                GXTexMapID map;
                                if (resTev.GXGetIndTexOrder(static_cast<GXIndTexStageID>(stage), &coord, &map) &&
                                    coord == idx) {
                                    resTev.GXSetIndTexOrder(
                                        static_cast<GXIndTexStageID>(stage),
                                        static_cast<GXTexCoordID>(matchingLayerCoords[0]), map
                                    );
                                }
                            }
                            resMatTexCoordGen.Disable(idx);
                            result++;
                            resTexSrt.Disable(idx);
                            numTexGens--;
                        }
                    }
                    resMatTexCoordGen.DCStore(false);
                    resTev.DCStore(false);
                    genMode.GXSetNumTexGens(numTexGens);
                }
            }
        }
    }

    return result;
}

void *G3DUtility::BumpAlloc(u32 size, u32 align) {
    u32 next = sOffset % align != 0 ? sOffset + (align - sOffset % align) : sOffset;
    sOffset = next + size;
    // REGSWAP
    return reinterpret_cast<u8 *>(sBuf) + next;
}

} // namespace EGG
