#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnMdl);

/******************************************************************************
 *
 * CopiedMatAccess
 *
 ******************************************************************************/
ResTexObj ScnMdl::CopiedMatAccess::GetResTexObj(bool markDirty) {
    if (mpScnMdl != NULL && mTexObj.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_TEXOBJ);
        }

        return mTexObj;
    }

    return ResTexObj(NULL);
}

ResTexSrt ScnMdl::CopiedMatAccess::GetResTexSrt(bool markDirty) {
    if (mpScnMdl != NULL && mTexSrt.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_TEXSRT);
        }

        return mTexSrt;
    }

    return ResTexSrt(NULL);
}

ResMatChan ScnMdl::CopiedMatAccess::GetResMatChan(bool markDirty) {
    if (mpScnMdl != NULL && mChan.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_MATCHAN);
        }

        return mChan;
    }

    return ResMatChan(NULL);
}

ResGenMode ScnMdl::CopiedMatAccess::GetResGenMode(bool markDirty) {
    if (mpScnMdl != NULL && mGenMode.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_GENMODE);
        }

        return mGenMode;
    }

    return ResGenMode(NULL);
}

ResMatPix ScnMdl::CopiedMatAccess::GetResMatPix(bool markDirty) {
    if (mpScnMdl != NULL && mPix.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_MATPIX);
        }

        return mPix;
    }

    return ResMatPix(NULL);
}

ResMatTevColor ScnMdl::CopiedMatAccess::GetResMatTevColor(bool markDirty) {
    if (mpScnMdl != NULL && mTevColor.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_MATTEVCOLOR);
        }

        return mTevColor;
    }

    return ResMatTevColor(NULL);
}

ResTev ScnMdl::CopiedMatAccess::GetResTev(bool markDirty) {
    if (mpScnMdl != NULL && mTev.IsValid()) {
        if (markDirty) {
            mpScnMdl->MatBufferDirty(mMatID, ScnMdl::BUFOPTION_TEV);
        }

        return mTev;
    }

    return ResTev(NULL);
}

ResTexSrt ScnMdl::CopiedMatAccess::GetResTexSrtEx() {
    if (mpScnMdl != NULL) {
        if (mTexSrt.IsValid()) {
            return mTexSrt;
        }

        return mpScnMdl->GetResMdl().GetResMat(mMatID).GetResTexSrt();
    }

    return ResTexSrt(NULL);
}

ScnMdl::CopiedMatAccess::CopiedMatAccess(ScnMdl* pScnMdl, u32 id)
    : mTexObj(NULL),
      mTlutObj(NULL),
      mTexSrt(NULL),
      mChan(NULL),
      mGenMode(NULL),
      mMatMisc(NULL),
      mPix(NULL),
      mTevColor(NULL),
      mIndMtxAndScale(NULL),
      mTexCoordGen(NULL),
      mTev(NULL) {

    if (pScnMdl != NULL && pScnMdl->GetResMdl().GetResMat(id).IsValid()) {
        mpScnMdl = pScnMdl;
        mMatID = id;

        DrawResMdlReplacement& rReplacement = pScnMdl->mReplacement;

        if (rReplacement.texObjDataArray != NULL) {
            mTexObj = ResTexObj(&rReplacement.texObjDataArray[id]);
        } else {
            mTexObj = ResTexObj(NULL);
        }

        if (rReplacement.tlutObjDataArray != NULL) {
            mTlutObj = ResTlutObj(&rReplacement.tlutObjDataArray[id]);
        } else {
            mTlutObj = ResTlutObj(NULL);
        }

        if (rReplacement.texSrtDataArray != NULL) {
            mTexSrt = ResTexSrt(&rReplacement.texSrtDataArray[id]);
        } else {
            mTexSrt = ResTexSrt(NULL);
        }

        if (rReplacement.chanDataArray != NULL) {
            mChan = ResMatChan(&rReplacement.chanDataArray[id]);
        } else {
            mChan = ResMatChan(NULL);
        }

        if (rReplacement.genModeDataArray != NULL) {
            mGenMode = ResGenMode(&rReplacement.genModeDataArray[id]);
        } else {
            mGenMode = ResGenMode(NULL);
        }

        if (rReplacement.matMiscDataArray != NULL) {
            mMatMisc = ResMatMisc(&rReplacement.matMiscDataArray[id]);
        } else {
            mMatMisc = ResMatMisc(NULL);
        }

        if (rReplacement.pixDLArray != NULL) {
            mPix = ResMatPix(&rReplacement.pixDLArray[id]);
        } else {
            mPix = ResMatPix(NULL);
        }

        if (rReplacement.tevColorDLArray != NULL) {
            mTevColor = ResMatTevColor(&rReplacement.tevColorDLArray[id]);
        } else {
            mTevColor = ResMatTevColor(NULL);
        }

        if (rReplacement.indMtxAndScaleDLArray != NULL) {
            mIndMtxAndScale =
                ResMatIndMtxAndScale(&rReplacement.indMtxAndScaleDLArray[id]);
        } else {
            mIndMtxAndScale = ResMatIndMtxAndScale(NULL);
        }

        if (rReplacement.texCoordGenDLArray != NULL) {
            mTexCoordGen =
                ResMatTexCoordGen(&rReplacement.texCoordGenDLArray[id]);
        } else {
            mTexCoordGen = ResMatTexCoordGen(NULL);
        }

        if (rReplacement.tevDataArray != NULL) {
            mTev = ResTev(&rReplacement.tevDataArray[id]);
        } else {
            mTev = ResTev(NULL);
        }

    } else {
        mpScnMdl = NULL;
        mMatID = id;

        mTexObj = ResTexObj(NULL);
        mTlutObj = ResTlutObj(NULL);
        mTexSrt = ResTexSrt(NULL);
        mChan = ResMatChan(NULL);
        mGenMode = ResGenMode(NULL);
        mMatMisc = ResMatMisc(NULL);
        mPix = ResMatPix(NULL);
        mTevColor = ResMatTevColor(NULL);
        mIndMtxAndScale = ResMatIndMtxAndScale(NULL);
        mTexCoordGen = ResMatTexCoordGen(NULL);
        mTev = ResTev(NULL);
    }
}

/******************************************************************************
 *
 * CopiedVisAccess
 *
 ******************************************************************************/
bool ScnMdl::CopiedVisAccess::IsVisible() const {
    if (mpScnMdl != NULL) {
        if (mpVis != NULL) {
            return *mpVis != 0;
        }

        return mpScnMdl->GetResMdl().GetResNode(mNodeID).IsVisible();
    }

    return false;
}

bool ScnMdl::CopiedVisAccess::SetVisibilityEx(bool visible) {
    if (mpScnMdl != NULL) {
        if (mpVis != NULL) {
            if (visible && *mpVis == 0 || !visible && *mpVis != 0) {
                mpScnMdl->VisBufferDirty();
            }

            *mpVis = visible;
        } else {
            mpScnMdl->GetResMdl().GetResNode(mNodeID).SetVisibility(visible);
        }

        return true;
    }

    return false;
}

ScnMdl::CopiedVisAccess::CopiedVisAccess(ScnMdl* pScnMdl, u32 id) {
    if (pScnMdl != NULL && pScnMdl->GetResMdl().GetResNode(id).IsValid()) {
        mpScnMdl = pScnMdl;
        mNodeID = id;

        if (pScnMdl->mReplacement.visArray != NULL) {
            mpVis = &pScnMdl->mReplacement.visArray[id];
        } else {
            mpVis = NULL;
        }
    } else {
        mpScnMdl = NULL;
        mNodeID = id;
        mpVis = NULL;
    }
}

/******************************************************************************
 *
 * ScnMdl
 *
 ******************************************************************************/
ScnMdl* ScnMdl::Construct(MEMAllocator* pAllocator, u32* pSize, ResMdl mdl,
                          u32 bufferOption, int numView) {

    if (!mdl.IsValid()) {
        return NULL;
    }

    if (numView == 0) {
        numView = 1;
    } else if (numView > VIEW_MAX) {
        numView = VIEW_MAX;
    }

    ScnMdl* pScnMdl = NULL;

    u32 worldMtxNum = mdl.GetResMdlInfo().GetNumPosNrmMtx();
    u32 viewMtxNum = mdl.GetResMdlInfo().GetNumViewMtx();
    u32 matNum = mdl.GetResMatNumEntries();
    u32 nodeNum = mdl.GetResNodeNumEntries();

    u32 scnMdlSize = sizeof(ScnMdl);
    u32 worldMtxArraySize = worldMtxNum * sizeof(math::MTX34);
    u32 worldMtxAttribArraySize = worldMtxNum * sizeof(u32);

    u32 viewPosMtxArrayUnitSize = viewMtxNum * sizeof(math::MTX34);
    u32 viewPosMtxArraySize = numView * align32(viewPosMtxArrayUnitSize);

    u32 viewNrmMtxArrayUnitSize = viewMtxNum * sizeof(math::MTX33);
    u32 viewNrmMtxArraySize = mdl.GetResMdlInfo().ref().need_nrm_mtx_array
                                  ? numView * align32(viewNrmMtxArrayUnitSize)
                                  : 0;

    u32 viewTexMtxArrayUnitSize = viewPosMtxArrayUnitSize;

    // TODO: Fakematch
    u32 viewTexMtxArraySize = mdl.ref().info.need_tex_mtx_array
                                  ? numView * align32(viewTexMtxArrayUnitSize)
                                  : 0;

    u32 matBufferDirtyFlagSize = matNum * sizeof(u32);

    // clang-format off
    u32 resTexObjSize         = (bufferOption & BUFOPTION_TEXOBJ)         ? matNum  * sizeof(ResTexObjData)        : 0;
    u32 resTlutObjSize        = (bufferOption & BUFOPTION_TLUTOBJ)        ? matNum  * sizeof(ResTlutObjData)       : 0;
    u32 resTexSrtSize         = (bufferOption & BUFOPTION_TEXSRT)         ? matNum  * sizeof(ResTexSrtData)        : 0;
    u32 resChanSize           = (bufferOption & BUFOPTION_MATCHAN)        ? matNum  * sizeof(ResChanData)          : 0;
    u32 resGenModeSize        = (bufferOption & BUFOPTION_GENMODE)        ? matNum  * sizeof(ResGenModeData)       : 0;
    u32 resMatMiscSize        = (bufferOption & BUFOPTION_MATMISC)        ? matNum  * sizeof(ResMatMiscData)       : 0;
    u32 visSize               = (bufferOption & BUFOPTION_VIS)            ? nodeNum * sizeof(u8)                   : 0;
    u32 resPixSize            = (bufferOption & BUFOPTION_MATPIX)         ? matNum  * sizeof(ResPixDL)             : 0;
    u32 resTevColorSize       = (bufferOption & BUFOPTION_MATTEVCOLOR)    ? matNum  * sizeof(ResTevColorDL)        : 0;
    u32 resIndMtxAndScaleSize = (bufferOption & BUFOPTION_MATINDMTXSCALE) ? matNum  * sizeof(ResIndMtxAndScaleDL)  : 0;
    u32 resTexCoordGenSize    = (bufferOption & BUFOPTION_MATTEXCOORDGEN) ? matNum  * sizeof(ResTexCoordGenDL)     : 0;
    u32 resTevSize            = (bufferOption & BUFOPTION_TEV)            ? matNum  * sizeof(ResTevData)           : 0;
    // clang-format on

    u32 vtxPosTableSize = 0;
    u32 vtxPosBufferSize = 0;

    if (bufferOption & BUFOPTION_VTXPOS) {
        u32 vtxPosNum = mdl.GetResVtxPosNumEntries();
        u32 shpNum = mdl.GetResShpNumEntries();

        vtxPosTableSize = vtxPosNum * sizeof(ResVtxPosData*);

        for (u32 i = 0; i < vtxPosNum; i++) {
            u32 j;
            ResVtxPos pos = mdl.GetResVtxPos(i);

            for (j = 0; j < shpNum; j++) {
                ResShp shp = mdl.GetResShp(j);

                if (pos.ptr() == shp.GetResVtxPos().ptr()) {
                    break;
                }
            }

            if (j != shpNum) {
                vtxPosBufferSize += align32(pos.GetSize());
            }
        }
    }

    u32 vtxNrmTableSize = 0;
    u32 vtxNrmBufferSize = 0;

    if (bufferOption & BUFOPTION_VTXNRM) {
        u32 vtxNrmNum = mdl.GetResVtxNrmNumEntries();
        u32 shpNum = mdl.GetResShpNumEntries();

        vtxNrmTableSize = vtxNrmNum * sizeof(ResVtxNrmData*);

        for (u32 i = 0; i < vtxNrmNum; i++) {
            u32 j;
            ResVtxNrm nrm = mdl.GetResVtxNrm(i);

            for (j = 0; j < shpNum; j++) {
                ResShp shp = mdl.GetResShp(j);

                if (nrm.ptr() == shp.GetResVtxNrm().ptr()) {
                    break;
                }
            }

            if (j != shpNum) {
                vtxNrmBufferSize += align32(nrm.GetSize());
            }
        }
    }

    u32 vtxClrTableSize = 0;
    u32 vtxClrBufferSize = 0;

    if (bufferOption & BUFOPTION_VTXCLR) {
        u32 vtxClrNum = mdl.GetResVtxClrNumEntries();
        u32 shpNum = mdl.GetResShpNumEntries();

        vtxClrTableSize = vtxClrNum * sizeof(ResVtxClrData*);

        for (u32 i = 0; i < vtxClrNum; i++) {
            u32 j;
            ResVtxClr clr = mdl.GetResVtxClr(i);

            for (j = 0; j < shpNum; j++) {
                ResShp shp = mdl.GetResShp(j);

                if (clr.ptr() == shp.GetResVtxClr(0).ptr() ||
                    clr.ptr() == shp.GetResVtxClr(1).ptr()) {
                    break;
                }
            }

            if (j != shpNum) {
                vtxClrBufferSize += align32(clr.GetSize());
            }
        }
    }

    // clang-format off
    u32 worldMtxArrayOfs       = align32(scnMdlSize);
    u32 worldMtxAttribArrayOfs = align32(worldMtxArrayOfs + worldMtxArraySize);
    u32 viewPosMtxArrayOfs     = align32(worldMtxAttribArrayOfs + worldMtxAttribArraySize);
    u32 viewNrmMtxArrayOfs     = align32(viewPosMtxArrayOfs + viewPosMtxArraySize);
    u32 viewTexMtxArrayOfs     = align32(viewNrmMtxArrayOfs + viewNrmMtxArraySize);
    u32 matBufferDirtyFlagOfs  = align4(viewTexMtxArrayOfs + viewTexMtxArraySize);
    u32 resTexObjOfs           = align32(matBufferDirtyFlagOfs + matBufferDirtyFlagSize);
    u32 resTlutObjOfs          = align4(resTexObjOfs + resTexObjSize);
    u32 resTexSrtOfs           = align4(resTlutObjOfs + resTlutObjSize);
    u32 resChanOfs             = align4(resTexSrtOfs + resTexSrtSize);
    u32 resGenModeOfs          = align4(resChanOfs + resChanSize);
    u32 resMatMiscOfs          = align4(resGenModeOfs + resGenModeSize);
    u32 visOfs                 = align4(resMatMiscOfs + resMatMiscSize);
    u32 resPixOfs              = align32(visOfs + visSize);
    u32 resTevColorOfs         = align32(resPixOfs + resPixSize);
    u32 resIndMtxAndScaleOfs   = align32(resTevColorOfs + resTevColorSize);
    u32 resTexCoordGenOfs      = align32(resIndMtxAndScaleOfs + resIndMtxAndScaleSize);
    u32 resTevOfs              = align32(resTexCoordGenOfs + resTexCoordGenSize);
    // clang-format on

    u32 vtxPosTableOfs = align32(resTevOfs + resTevSize);
    u32 vtxNrmTableOfs = vtxPosTableOfs + vtxPosTableSize;
    u32 vtxClrTableOfs = vtxNrmTableOfs + vtxNrmTableSize;

    u32 vtxPosBufferOfs = align32(vtxClrTableOfs + vtxClrTableSize);
    u32 vtxNrmBufferOfs = align32(vtxPosBufferOfs + vtxPosBufferSize);
    u32 vtxClrBufferOfs = align32(vtxNrmBufferOfs + vtxNrmBufferSize);

    u32 size = align32(vtxClrBufferOfs + vtxClrBufferSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8* pBuffer = reinterpret_cast<u8*>(Alloc(pAllocator, size));
        if (pBuffer == NULL) {
            return NULL;
        }

        // clang-format off
        DrawResMdlReplacement replacement;
        
        replacement.visArray              = visSize               != 0 ? reinterpret_cast<u8*>(pBuffer + visOfs)                                : NULL;
        replacement.texObjDataArray       = resTexObjSize         != 0 ? reinterpret_cast<ResTexObjData*>(pBuffer + resTexObjOfs)               : NULL;
        replacement.tlutObjDataArray      = resTlutObjSize        != 0 ? reinterpret_cast<ResTlutObjData*>(pBuffer + resTlutObjOfs)             : NULL;
        replacement.texSrtDataArray       = resTexSrtSize         != 0 ? reinterpret_cast<ResTexSrtData*>(pBuffer + resTexSrtOfs)               : NULL;
        replacement.chanDataArray         = resChanSize           != 0 ? reinterpret_cast<ResChanData*>(pBuffer + resChanOfs)                   : NULL;
        replacement.genModeDataArray      = resGenModeSize        != 0 ? reinterpret_cast<ResGenModeData*>(pBuffer + resGenModeOfs)             : NULL;
        replacement.matMiscDataArray      = resMatMiscSize        != 0 ? reinterpret_cast<ResMatMiscData*>(pBuffer + resMatMiscOfs)             : NULL;
        replacement.pixDLArray            = resPixSize            != 0 ? reinterpret_cast<ResPixDL*>(pBuffer + resPixOfs)                       : NULL;
        replacement.tevColorDLArray       = resTevColorSize       != 0 ? reinterpret_cast<ResTevColorDL*>(pBuffer + resTevColorOfs)             : NULL;
        replacement.indMtxAndScaleDLArray = resIndMtxAndScaleSize != 0 ? reinterpret_cast<ResIndMtxAndScaleDL*>(pBuffer + resIndMtxAndScaleOfs) : NULL;
        replacement.texCoordGenDLArray    = resTexCoordGenSize    != 0 ? reinterpret_cast<ResTexCoordGenDL*>(pBuffer + resTexCoordGenOfs)       : NULL;
        replacement.tevDataArray          = resTevSize            != 0 ? reinterpret_cast<ResTevData*>(pBuffer + resTevOfs)                     : NULL;

        replacement.vtxPosTable = vtxPosTableSize != 0 ? reinterpret_cast<ResVtxPosData**>(pBuffer + vtxPosTableOfs) : NULL;
        replacement.vtxNrmTable = vtxNrmTableSize != 0 ? reinterpret_cast<ResVtxNrmData**>(pBuffer + vtxNrmTableOfs) : NULL;
        replacement.vtxClrTable = vtxClrTableSize != 0 ? reinterpret_cast<ResVtxClrData**>(pBuffer + vtxClrTableOfs) : NULL;
        // clang-format on

        if (replacement.vtxPosTable != NULL) {
            u32 vtxPosNum = mdl.GetResVtxPosNumEntries();
            u32 shpNum = mdl.GetResShpNumEntries();

            u32 ofs = vtxPosBufferOfs;

            for (u32 i = 0; i < vtxPosNum; i++) {
                u32 j;
                ResVtxPos pos = mdl.GetResVtxPos(i);

                for (j = 0; j < shpNum; j++) {
                    ResShp shp = mdl.GetResShp(j);

                    if (pos.ptr() == shp.GetResVtxPos().ptr()) {
                        break;
                    }
                }

                if (j != shpNum) {
                    replacement.vtxPosTable[i] =
                        reinterpret_cast<ResVtxPosData*>(pBuffer + ofs);

                    ofs += align32(pos.GetSize());
                    pos.CopyTo(replacement.vtxPosTable[i]);
                } else {
                    replacement.vtxPosTable[i] = pos.ptr();
                }
            }
        }

        if (replacement.vtxNrmTable != NULL) {
            u32 vtxNrmNum = mdl.GetResVtxNrmNumEntries();
            u32 shpNum = mdl.GetResShpNumEntries();

            u32 ofs = vtxNrmBufferOfs;

            for (u32 i = 0; i < vtxNrmNum; i++) {
                u32 j;
                ResVtxNrm nrm = mdl.GetResVtxNrm(i);

                for (j = 0; j < shpNum; j++) {
                    ResShp shp = mdl.GetResShp(j);

                    if (nrm.ptr() == shp.GetResVtxNrm().ptr()) {
                        break;
                    }
                }

                if (j != shpNum) {
                    replacement.vtxNrmTable[i] =
                        reinterpret_cast<ResVtxNrmData*>(pBuffer + ofs);

                    ofs += align32(nrm.GetSize());
                    nrm.CopyTo(replacement.vtxNrmTable[i]);
                } else {
                    replacement.vtxNrmTable[i] = nrm.ptr();
                }
            }
        }

        if (replacement.vtxClrTable != NULL) {
            u32 vtxClrNum = mdl.GetResVtxClrNumEntries();
            u32 shpNum = mdl.GetResShpNumEntries();

            u32 ofs = vtxClrBufferOfs;

            for (u32 i = 0; i < vtxClrNum; i++) {
                u32 j;
                ResVtxClr clr = mdl.GetResVtxClr(i);

                for (j = 0; j < shpNum; j++) {
                    ResShp shp = mdl.GetResShp(j);

                    if (clr.ptr() == shp.GetResVtxClr(0).ptr() ||
                        clr.ptr() == shp.GetResVtxClr(1).ptr()) {
                        break;
                    }
                }

                if (j != shpNum) {
                    replacement.vtxClrTable[i] =
                        reinterpret_cast<ResVtxClrData*>(pBuffer + ofs);

                    ofs += align32(clr.GetSize());
                    clr.CopyTo(replacement.vtxClrTable[i]);
                } else {
                    replacement.vtxClrTable[i] = clr.ptr();
                }
            }
        }

        // clang-format off
        pScnMdl = new (pBuffer) ScnMdl(
            pAllocator,
            mdl,
            reinterpret_cast<math::MTX34*>(pBuffer + worldMtxArrayOfs),
            reinterpret_cast<u32*>(pBuffer + worldMtxAttribArrayOfs),
            reinterpret_cast<math::MTX34*>(pBuffer + viewPosMtxArrayOfs),
            viewNrmMtxArraySize != 0 ? reinterpret_cast<math::MTX33*>(pBuffer + viewNrmMtxArrayOfs) : NULL,
            viewTexMtxArraySize != 0 ? reinterpret_cast<math::MTX34*>(pBuffer + viewTexMtxArrayOfs) : NULL,
            numView,
            viewMtxNum,
            &replacement,
            reinterpret_cast<u32*>(pBuffer + matBufferDirtyFlagOfs));
        // clang-format off

        pScnMdl->InitBuffer();
    }

    return pScnMdl;
}

void ScnMdl::ScnMdl_G3DPROC_CALC_WORLD(u32 param, const math::MTX34* pParent) {
    ScnMdlSmpl_CalcPosture(param, pParent);

    if (IsVisBufferRefreshNeeded() && IsVisBufferDirty()) {
        CleanVisBuffer();
    }

    if (GetAnmObjVis() != NULL) {
        if (mReplacement.visArray != NULL) {
            ApplyVisAnmResult(mReplacement.visArray, GetResMdl(),
                              GetAnmObjVis());
            VisBufferDirty();
        } else {
            ApplyVisAnmResult(GetResMdl(), GetAnmObjVis());
        }
    }

    CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param,
                             const_cast<math::MTX34*>(pParent));
}

void ScnMdl::ScnMdl_G3DPROC_CALC_MAT(u32 param, void* pInfo) {
    CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);

    ResMdl mdl = GetResMdl();
    u32 matNum = mdl.GetResMatNumEntries();

    for (u32 i = 0; i < matNum; i++) {
        ResMat mat = mdl.GetResMat(i);

        if (IsMatBufferDirty(i, BUFOPTION_TEXOBJ)) {
            CleanMatBuffer(i, BUFOPTION_TEXOBJ);
        }

        if (IsMatBufferDirty(i, BUFOPTION_TLUTOBJ)) {
            CleanMatBuffer(i, BUFOPTION_TLUTOBJ);
        }

        if (IsMatBufferDirty(i, BUFOPTION_MATINDMTXSCALE)) {
            CleanMatBuffer(i, BUFOPTION_MATINDMTXSCALE);
        }

        if (IsMatBufferDirty(i, BUFOPTION_TEXSRT)) {
            CleanMatBuffer(i, BUFOPTION_TEXSRT);
        }

        if (IsMatBufferDirty(i, BUFOPTION_MATCHAN)) {
            CleanMatBuffer(i, BUFOPTION_MATCHAN);
        }

        if (IsMatBufferDirty(i, BUFOPTION_MATTEVCOLOR)) {
            CleanMatBuffer(i, BUFOPTION_MATTEVCOLOR);
        }

        if (GetAnmObjTexPat() != NULL && GetAnmObjTexPat()->TestExistence(i)) {
            TexPatAnmResult resultBuf;

            const TexPatAnmResult* pResult =
                GetAnmObjTexPat()->GetResult(&resultBuf, i);

            ResTexObj texObj = mReplacement.texObjDataArray != NULL
                                   ? ResTexObj(&mReplacement.texObjDataArray[i])
                                   : mat.GetResTexObj();

            ResTlutObj tlutObj =
                mReplacement.tlutObjDataArray != NULL
                    ? ResTlutObj(&mReplacement.tlutObjDataArray[i])
                    : mat.GetResTlutObj();

            ApplyTexPatAnmResult(texObj, tlutObj, pResult);

            texObj.EndEdit();
            tlutObj.EndEdit();

            MatBufferDirty(i, BUFOPTION_TEXOBJ | BUFOPTION_TLUTOBJ);
        }

        if (GetAnmObjTexSrt() != NULL && GetAnmObjTexSrt()->TestExistence(i)) {
            TexSrtAnmResult resultBuf;

            const TexSrtAnmResult* pResult =
                GetAnmObjTexSrt()->GetResult(&resultBuf, i);

            ResTexSrt srt = mReplacement.texSrtDataArray != NULL
                                ? ResTexSrt(&mReplacement.texSrtDataArray[i])
                                : mat.GetResTexSrt();

            ResMatIndMtxAndScale ind =
                mReplacement.indMtxAndScaleDLArray != NULL
                    ? ResMatIndMtxAndScale(
                          &mReplacement.indMtxAndScaleDLArray[i])
                    : mat.GetResMatIndMtxAndScale();

            ApplyTexSrtAnmResult(srt, ind, pResult);

            ind.EndEdit();
            srt.EndEdit();

            MatBufferDirty(i, BUFOPTION_TEXSRT | BUFOPTION_MATINDMTXSCALE);
        }

        if (GetAnmObjMatClr() != NULL && GetAnmObjMatClr()->TestExistence(i)) {
            ClrAnmResult resultBuf;

            ResMatTevColor tevColor =
                mReplacement.tevColorDLArray != NULL
                    ? ResMatTevColor(&mReplacement.tevColorDLArray[i])
                    : mat.GetResMatTevColor();

            ResMatChan chan = mReplacement.chanDataArray != NULL
                                  ? ResMatChan(&mReplacement.chanDataArray[i])
                                  : mat.GetResMatChan();

            const ClrAnmResult* pResult =
                GetAnmObjMatClr()->GetResult(&resultBuf, i);

            ApplyClrAnmResult(chan, tevColor, pResult);

            chan.EndEdit();
            tevColor.EndEdit();

            MatBufferDirty(i, BUFOPTION_MATTEVCOLOR | BUFOPTION_MATCHAN);
        }
    }

    CheckCallback_CALC_MAT(CALLBACK_TIMING_C, param, pInfo);
}

void ScnMdl::ScnMdl_G3DPROC_DRAW_OPA(u32 param, void* pInfo) {
    CheckCallback_DRAW_OPA(CALLBACK_TIMING_A, param, pInfo);

    u32 drawMode = pInfo != NULL ? *static_cast<u32*>(pInfo) : GetDrawMode();

    DrawResMdlDirectly(GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
                       GetViewTexMtxArray(), GetByteCodeDrawOpa(), NULL,
                       &mReplacement, drawMode);

    CheckCallback_DRAW_OPA(CALLBACK_TIMING_C, param, pInfo);
}

void ScnMdl::ScnMdl_G3DPROC_DRAW_XLU(u32 param, void* pInfo) {
    CheckCallback_DRAW_XLU(CALLBACK_TIMING_A, param, pInfo);

    u32 drawMode = pInfo != NULL ? *static_cast<u32*>(pInfo) : GetDrawMode();

    DrawResMdlDirectly(GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
                       GetViewTexMtxArray(), NULL, GetByteCodeDrawXlu(),
                       &mReplacement, drawMode);

    CheckCallback_DRAW_XLU(CALLBACK_TIMING_C, param, pInfo);
}

void ScnMdl::ScnMdl_G3DPROC_CALC_VTX(u32 param, void* pInfo) {
#pragma unused(param)
#pragma unused(pInfo)

    if (GetAnmObjShp() == NULL) {
        return;
    }

    CalcVtx(GetResMdl(), GetAnmObjShp(), mReplacement.vtxPosTable,
            mReplacement.vtxNrmTable, mReplacement.vtxClrTable);
}

void ScnMdl::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3DPROC_GATHER_SCNOBJ: {
        ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(param,
                                         static_cast<IScnObjGather*>(pInfo));
        break;
    }

    case G3DPROC_CALC_WORLD: {
        ScnMdl_G3DPROC_CALC_WORLD(param, static_cast<math::MTX34*>(pInfo));
        break;
    }

    case G3DPROC_CALC_MAT: {
        ScnMdl_G3DPROC_CALC_MAT(param, pInfo);
        break;
    }

    case G3DPROC_CALC_VIEW: {
        ScnMdlSmpl_G3DPROC_CALC_VIEW(param, static_cast<math::MTX34*>(pInfo));
        break;
    }

    case G3DPROC_DRAW_OPA: {
        ScnMdl_G3DPROC_DRAW_OPA(param, pInfo);
        break;
    }

    case G3DPROC_DRAW_XLU: {
        ScnMdl_G3DPROC_DRAW_XLU(param, pInfo);
        break;
    }

    case G3DPROC_UPDATEFRAME: {
        ScnMdlSmpl_G3DPROC_UPDATEFRAME(param, pInfo);

        if (mpAnmObjShp != NULL) {
            mpAnmObjShp->UpdateFrame();
        }
        break;
    }

    case G3DPROC_CHILD_DETACHED: {
        RemoveAnmObj(static_cast<AnmObj*>(pInfo));
        break;
    }

    case G3DPROC_CALC_VTX: {
        ScnMdl_G3DPROC_CALC_VTX(param, pInfo);
        break;
    }

    default: {
        DefG3dProcScnLeaf(task, param, pInfo);
        break;
    }
    }
}

bool ScnMdl::SetScnObjOption(u32 option, u32 value) {
    switch (option) {
    case OPTION_VISBUFFER_REFRESH_NEEDED: {
        if (value) {
            mFlagVisBuffer &= ~VISBUFFER_NOT_REFRESH_NEEDED;
        } else {
            mFlagVisBuffer |= VISBUFFER_NOT_REFRESH_NEEDED;
        }
        break;
    }

    default: {
        return ScnMdlSimple::SetScnObjOption(option, value);
    }
    }

    return true;
}

bool ScnMdl::GetScnObjOption(u32 option, u32* pValue) const {
    if (pValue == NULL) {
        return false;
    }

    switch (option) {
    case OPTION_VISBUFFER_REFRESH_NEEDED: {
        *pValue = !(mFlagVisBuffer & VISBUFFER_NOT_REFRESH_NEEDED);
        break;
    }

    default: {
        return ScnMdlSimple::GetScnObjOption(option, pValue);
    }
    }

    return true;
}

void ScnMdl::InitBuffer() {
    ResMdl mdl = GetResMdl();

    u32 matNum = mdl.GetResMatNumEntries();
    u32 nodeNum = mdl.GetResNodeNumEntries();

    u32 i;

    if (mReplacement.visArray != NULL) {
        for (i = 0; i < nodeNum; i++) {
            if (mdl.GetResNode(i).IsVisible()) {
                mReplacement.visArray[i] = true;
            } else {
                mReplacement.visArray[i] = false;
            }
        }
    }

    for (i = 0; i < matNum; i++) {
        ResMat mat = mdl.GetResMat(i);
        mpMatBufferDirtyFlag[i] = 0;

        if (mReplacement.texObjDataArray != NULL) {
            mat.GetResTexObj()
                .CopyTo(&mReplacement.texObjDataArray[i])
                .EndEdit();
        }

        if (mReplacement.tlutObjDataArray != NULL) {
            mat.GetResTlutObj()
                .CopyTo(&mReplacement.tlutObjDataArray[i])
                .EndEdit();
        }

        if (mReplacement.texSrtDataArray != NULL) {
            mat.GetResTexSrt()
                .CopyTo(&mReplacement.texSrtDataArray[i])
                .EndEdit();
        }

        if (mReplacement.chanDataArray != NULL) {
            mat.GetResMatChan()
                .CopyTo(&mReplacement.chanDataArray[i])
                .EndEdit();
        }

        if (mReplacement.genModeDataArray != NULL) {
            mat.GetResGenMode()
                .CopyTo(&mReplacement.genModeDataArray[i])
                .EndEdit();
        }

        if (mReplacement.matMiscDataArray != NULL) {
            mat.GetResMatMisc()
                .CopyTo(&mReplacement.matMiscDataArray[i])
                .EndEdit();
        }

        if (mReplacement.pixDLArray != NULL) {
            mat.GetResMatPix().CopyTo(&mReplacement.pixDLArray[i]).EndEdit();
        }

        if (mReplacement.tevColorDLArray != NULL) {
            mat.GetResMatTevColor()
                .CopyTo(&mReplacement.tevColorDLArray[i])
                .EndEdit();
        }

        if (mReplacement.indMtxAndScaleDLArray != NULL) {
            mat.GetResMatIndMtxAndScale()
                .CopyTo(&mReplacement.indMtxAndScaleDLArray[i])
                .EndEdit();
        }

        if (mReplacement.texCoordGenDLArray != NULL) {
            mat.GetResMatTexCoordGen()
                .CopyTo(&mReplacement.texCoordGenDLArray[i])
                .EndEdit();
        }

        if (mReplacement.tevDataArray != NULL) {
            mat.GetResTev().CopyTo(&mReplacement.tevDataArray[i]).EndEdit();
        }
    }
}

void ScnMdl::CleanMatBuffer(u32 idx, u32 option) {
    ResMat mat = GetResMdl().GetResMat(idx);

    if ((option & BUFOPTION_TEXOBJ) && mReplacement.texObjDataArray != NULL) {
        mat.GetResTexObj().CopyTo(&mReplacement.texObjDataArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_TLUTOBJ) && mReplacement.tlutObjDataArray != NULL) {
        mat.GetResTlutObj().CopyTo(&mReplacement.tlutObjDataArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_TEXSRT) && mReplacement.texSrtDataArray != NULL) {
        mat.GetResTexSrt().CopyTo(&mReplacement.texSrtDataArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_MATCHAN) && mReplacement.chanDataArray != NULL) {
        mat.GetResMatChan().CopyTo(&mReplacement.chanDataArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_GENMODE) && mReplacement.genModeDataArray != NULL) {
        mat.GetResGenMode().CopyTo(&mReplacement.genModeDataArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_MATMISC) && mReplacement.matMiscDataArray != NULL) {
        mat.GetResMatMisc().CopyTo(&mReplacement.matMiscDataArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_MATPIX) && mReplacement.pixDLArray != NULL) {
        mat.GetResMatPix().CopyTo(&mReplacement.pixDLArray[idx]).EndEdit();
    }

    if ((option & BUFOPTION_MATTEVCOLOR) &&
        mReplacement.tevColorDLArray != NULL) {

        mat.GetResMatTevColor()
            .CopyTo(&mReplacement.tevColorDLArray[idx])
            .EndEdit();
    }

    if ((option & BUFOPTION_MATINDMTXSCALE) &&
        mReplacement.indMtxAndScaleDLArray != NULL) {

        mat.GetResMatIndMtxAndScale()
            .CopyTo(&mReplacement.indMtxAndScaleDLArray[idx])
            .EndEdit();
    }

    if ((option & BUFOPTION_MATTEXCOORDGEN) &&
        mReplacement.texCoordGenDLArray != NULL) {

        mat.GetResMatTexCoordGen()
            .CopyTo(&mReplacement.texCoordGenDLArray[idx])
            .EndEdit();
    }

    if ((option & BUFOPTION_TEV) && mReplacement.tevDataArray != NULL) {
        mat.GetResTev().CopyTo(&mReplacement.tevDataArray[idx]).EndEdit();
    }

    mpMatBufferDirtyFlag[idx] = mpMatBufferDirtyFlag[idx] & ~option;
}

void ScnMdl::CleanVisBuffer() {
    ResMdl mdl = GetResMdl();
    u32 nodeNum = mdl.GetResNodeNumEntries();

    if (mReplacement.visArray != NULL) {
        for (u32 i = 0; i < nodeNum; i++) {
            if (mdl.GetResNode(i).IsVisible()) {
                mReplacement.visArray[i] = true;
            } else {
                mReplacement.visArray[i] = false;
            }
        }
    }

    mFlagVisBuffer &= ~VISBUFFER_DIRTY;
}

bool ScnMdl::SetAnmObj(AnmObj* pObj, AnmObjType type) {
    if (pObj != NULL && pObj->GetParent() == NULL) {
        if (type == ANMOBJTYPE_SHP || type == ANMOBJTYPE_NOT_SPECIFIED) {
            AnmObjShp* pShp = DynamicCast<AnmObjShp>(pObj);

            if (pShp != NULL) {
                if (!pShp->IsBound()) {
                    return false;
                }

                if (mpAnmObjShp != NULL) {
                    RemoveAnmObj(mpAnmObjShp);
                }

                mpAnmObjShp = pShp;
                pShp->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);

                return true;
            } else {
                if (type == ANMOBJTYPE_NOT_SPECIFIED) {
                    return ScnMdlSimple::SetAnmObj(pObj, type);
                }

                return false;
            }
        } else {
            return ScnMdlSimple::SetAnmObj(pObj, type);
        }
    }

    return false;
}

bool ScnMdl::RemoveAnmObj(AnmObj* pObj) {
    if (pObj == NULL) {
        return NULL;
    }

    if (pObj == mpAnmObjShp) {
        mpAnmObjShp->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjShp = NULL;

        if (mReplacement.vtxPosTable != NULL) {
            u32 vtxPosNum = GetResMdl().GetResVtxPosNumEntries();

            for (u32 i = 0; i < vtxPosNum; i++) {
                ResVtxPos pos = GetResMdl().GetResVtxPos(i);

                if (pos.ptr() != mReplacement.vtxPosTable[i]) {
                    pos.CopyTo(mReplacement.vtxPosTable[i]);
                }
            }
        }

        if (mReplacement.vtxNrmTable != NULL) {
            u32 vtxNrmNum = GetResMdl().GetResVtxNrmNumEntries();

            for (u32 i = 0; i < vtxNrmNum; i++) {
                ResVtxNrm nrm = GetResMdl().GetResVtxNrm(i);

                if (nrm.ptr() != mReplacement.vtxNrmTable[i]) {
                    nrm.CopyTo(mReplacement.vtxNrmTable[i]);
                }
            }
        }

        if (mReplacement.vtxClrTable != NULL) {
            u32 vtxClrNum = GetResMdl().GetResVtxClrNumEntries();

            for (u32 i = 0; i < vtxClrNum; i++) {
                ResVtxClr clr = GetResMdl().GetResVtxClr(i);

                if (clr.ptr() != mReplacement.vtxClrTable[i]) {
                    clr.CopyTo(mReplacement.vtxClrTable[i]);
                }
            }
        }

        return true;
    }

    return ScnMdlSimple::RemoveAnmObj(pObj);
}

AnmObj* ScnMdl::RemoveAnmObj(AnmObjType type) {
    if (type == ANMOBJTYPE_SHP) {
        AnmObj* pOld = mpAnmObjShp;
        RemoveAnmObj(mpAnmObjShp);
        return pOld;
    }

    return ScnMdlSimple::RemoveAnmObj(type);
}

AnmObj* ScnMdl::GetAnmObj(AnmObjType type) {
    if (type == ANMOBJTYPE_SHP) {
        return mpAnmObjShp;
    }

    return ScnMdlSimple::GetAnmObj(type);
}

const AnmObj* ScnMdl::GetAnmObj(AnmObjType type) const {
    if (type == ANMOBJTYPE_SHP) {
        return mpAnmObjShp;
    }

    return ScnMdlSimple::GetAnmObj(type);
}

ScnMdl::ScnMdl(MEMAllocator* pAllocator, ResMdl mdl,
               math::MTX34* pWorldMtxArray, u32* pWorldMtxAttribArray,
               math::MTX34* pViewPosMtxArray, math::MTX33* pViewNrmMtxArray,
               math::MTX34* pViewTexMtxArray, int numView, int numViewMtx,
               DrawResMdlReplacement* pReplacement, u32* pMatBufferDirtyFlag)
    : ScnMdlSimple(pAllocator, mdl, pWorldMtxArray, pWorldMtxAttribArray,
                   pViewPosMtxArray, pViewNrmMtxArray, pViewTexMtxArray,
                   numView, numViewMtx),
      mpAnmObjShp(NULL),
      mFlagVisBuffer(NULL),
      mpMatBufferDirtyFlag(pMatBufferDirtyFlag),
      mReplacement(*pReplacement) {}

ScnMdl::~ScnMdl() {
    if (mpAnmObjShp != NULL) {
        RemoveAnmObj(mpAnmObjShp);
    }
}

} // namespace g3d
} // namespace nw4r
