#include "nw4r/g3d.h" // IWYU pragma: export

#include "rvl/GX.h" // IWYU pragma: export

#include <cstring>

namespace nw4r {
namespace g3d {

ResMdl ResShp::GetParent() const {
    return ofs_to_obj<ResMdl>(ref().toResMdlData);
}

bool ResShp::GXGetVtxDescv(GXVtxDescList *pList) const {
    ResShpPrePrim prim = GetResShpPrePrim();

    if (prim.ref().dl.vtxDescv[GX_CP_CMD_SZ * 0] == 0) {
        return false;
    }

    u32 vcdLo;
    detail::ResReadCPCmd(&prim.ref().dl.vtxDescv[GX_CP_CMD_SZ * 0], &vcdLo);

    u32 vcdHi;
    detail::ResReadCPCmd(&prim.ref().dl.vtxDescv[GX_CP_CMD_SZ * 1], &vcdHi);

    for (int i = 0; i <= GX_VA_TEX7; i++) {
        pList[i].attr = static_cast<GXAttr>(i);
    }

    pList[GX_VA_TEX7 + 1].attr = GX_VA_NULL;

    // clang-format off
    // Matrix indices
    pList[GX_VA_PNMTXIDX].type   = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_POSMATIDX_SHIFT  & GX_CP_VCD_LO_POSMATIDX_LMASK);
    pList[GX_VA_TEX0MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX0MATIDX_SHIFT & GX_CP_VCD_LO_TEX0MATIDX_LMASK);
    pList[GX_VA_TEX1MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX1MATIDX_SHIFT & GX_CP_VCD_LO_TEX1MATIDX_LMASK);
    pList[GX_VA_TEX2MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX2MATIDX_SHIFT & GX_CP_VCD_LO_TEX2MATIDX_LMASK);
    pList[GX_VA_TEX3MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX3MATIDX_SHIFT & GX_CP_VCD_LO_TEX3MATIDX_LMASK);
    pList[GX_VA_TEX4MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX4MATIDX_SHIFT & GX_CP_VCD_LO_TEX4MATIDX_LMASK);
    pList[GX_VA_TEX5MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX5MATIDX_SHIFT & GX_CP_VCD_LO_TEX5MATIDX_LMASK);
    pList[GX_VA_TEX6MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX6MATIDX_SHIFT & GX_CP_VCD_LO_TEX6MATIDX_LMASK);
    pList[GX_VA_TEX7MTXIDX].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_TEX7MATIDX_SHIFT & GX_CP_VCD_LO_TEX7MATIDX_LMASK);

    // Position/normal
    pList[GX_VA_POS].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_POSITION_SHIFT & GX_CP_VCD_LO_POSITION_LMASK);
    pList[GX_VA_NRM].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_NORMAL_SHIFT   & GX_CP_VCD_LO_NORMAL_LMASK);

    // Color attributes
    pList[GX_VA_CLR0].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_COLORDIFFUSED_SHIFT & GX_CP_VCD_LO_COLORDIFFUSED_LMASK);
    pList[GX_VA_CLR1].type = static_cast<GXAttrType>(vcdLo >> GX_CP_VCD_LO_COLORSPECULAR_SHIFT & GX_CP_VCD_LO_COLORSPECULAR_LMASK);

    // Texcoord attributes
    pList[GX_VA_TEX0].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX0COORD_SHIFT & GX_CP_VCD_HI_TEX0COORD_LMASK);
    pList[GX_VA_TEX1].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX1COORD_SHIFT & GX_CP_VCD_HI_TEX1COORD_LMASK);
    pList[GX_VA_TEX2].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX2COORD_SHIFT & GX_CP_VCD_HI_TEX2COORD_LMASK);
    pList[GX_VA_TEX3].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX3COORD_SHIFT & GX_CP_VCD_HI_TEX3COORD_LMASK);
    pList[GX_VA_TEX4].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX4COORD_SHIFT & GX_CP_VCD_HI_TEX4COORD_LMASK);
    pList[GX_VA_TEX5].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX5COORD_SHIFT & GX_CP_VCD_HI_TEX5COORD_LMASK);
    pList[GX_VA_TEX6].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX6COORD_SHIFT & GX_CP_VCD_HI_TEX6COORD_LMASK);
    pList[GX_VA_TEX7].type = static_cast<GXAttrType>(vcdHi >> GX_CP_VCD_HI_TEX7COORD_SHIFT & GX_CP_VCD_HI_TEX7COORD_LMASK);
    // clang-format on

    return true;
}

bool ResShp::GXGetVtxAttrFmtv(GXVtxAttrFmtList *pList) const {
    ResShpPrePrim prim = GetResShpPrePrim();

    if (prim.ref().dl.vtxFmtv[GX_CP_CMD_SZ * 0] == 0) {
        return false;
    }

    u32 vat0;
    detail::ResReadCPCmd(&prim.ref().dl.vtxFmtv[GX_CP_CMD_SZ * 0], &vat0);

    u32 vat1;
    detail::ResReadCPCmd(&prim.ref().dl.vtxFmtv[GX_CP_CMD_SZ * 1], &vat1);

    u32 vat2;
    detail::ResReadCPCmd(&prim.ref().dl.vtxFmtv[GX_CP_CMD_SZ * 2], &vat2);

    int i;
    for (i = 0; i <= GX_VA_TEX7 - GX_VA_POS; i++) {
        pList[i].attr = static_cast<GXAttr>(GX_VA_POS + i);
    }

    // clang-format off
    // Position/normal
    pList[GX_VA_POS - GX_VA_POS].compType = static_cast<GXCompType>(vat0 >> GX_CP_VAT_GROUP0_POS_TYPE_SHIFT & GX_CP_VAT_GROUP0_POS_TYPE_LMASK);
    pList[GX_VA_POS - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat0 >> GX_CP_VAT_GROUP0_POS_CNT_SHIFT & GX_CP_VAT_GROUP0_POS_CNT_LMASK);
    pList[GX_VA_POS - GX_VA_POS].shift    = vat0 >> GX_CP_VAT_GROUP0_POS_SHIFT_SHIFT & GX_CP_VAT_GROUP0_POS_SHIFT_LMASK;
    
    pList[GX_VA_NRM - GX_VA_POS].compType = static_cast<GXCompType>(vat0 >> GX_CP_VAT_GROUP0_NRM_TYPE_SHIFT & GX_CP_VAT_GROUP0_NRM_TYPE_LMASK);
    
    pList[GX_VA_NRM - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat0 >> GX_CP_VAT_GROUP0_NORMALINDEX3_SHIFT & GX_CP_VAT_GROUP0_NORMALINDEX3_LMASK
                                                                    ? i - GX_VA_NRM
                                                                    : vat0 >> GX_CP_VAT_GROUP0_NRM_CNT_SHIFT & GX_CP_VAT_GROUP0_NRM_CNT_LMASK);
                                                                    
    pList[GX_VA_NRM - GX_VA_POS].shift    = pList[GX_VA_NRM - GX_VA_POS].compType == GX_S8 ? 6 : 14;

    // Color attributes
    pList[GX_VA_CLR0 - GX_VA_POS].compType = static_cast<GXCompType>(vat0 >> GX_CP_VAT_GROUP0_COLORDIFF_TYPE_SHIFT & GX_CP_VAT_GROUP0_COLORDIFF_TYPE_LMASK);
    pList[GX_VA_CLR0 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat0 >> GX_CP_VAT_GROUP0_COLORDIFF_CNT_SHIFT & GX_CP_VAT_GROUP0_COLORDIFF_CNT_LMASK);

    pList[GX_VA_CLR1 - GX_VA_POS].compType = static_cast<GXCompType>(vat0 >> GX_CP_VAT_GROUP0_COLORSPEC_TYPE_SHIFT & GX_CP_VAT_GROUP0_COLORSPEC_TYPE_LMASK);
    pList[GX_VA_CLR1 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat0 >> GX_CP_VAT_GROUP0_COLORSPEC_CNT_SHIFT & GX_CP_VAT_GROUP0_COLORSPEC_CNT_LMASK);

    // Texcoord attributes
    pList[GX_VA_TEX0 - GX_VA_POS].compType = static_cast<GXCompType>(vat0 >> GX_CP_VAT_GROUP0_TXC0_TYPE_SHIFT & GX_CP_VAT_GROUP0_TXC0_TYPE_LMASK);
    pList[GX_VA_TEX0 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat0 >> GX_CP_VAT_GROUP0_TXC0_CNT_SHIFT & GX_CP_VAT_GROUP0_TXC0_CNT_LMASK);
    pList[GX_VA_TEX0 - GX_VA_POS].shift    = vat0 >> GX_CP_VAT_GROUP0_TXC0_SHIFT_SHIFT & GX_CP_VAT_GROUP0_TXC0_SHIFT_LMASK;

    pList[GX_VA_TEX1 - GX_VA_POS].compType = static_cast<GXCompType>(vat1 >> GX_CP_VAT_GROUP1_TXC1_TYPE_SHIFT & GX_CP_VAT_GROUP1_TXC1_TYPE_LMASK);
    pList[GX_VA_TEX1 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat1 >> GX_CP_VAT_GROUP1_TXC1_CNT_SHIFT & GX_CP_VAT_GROUP1_TXC1_CNT_LMASK);
    pList[GX_VA_TEX1 - GX_VA_POS].shift    = vat1 >> GX_CP_VAT_GROUP1_TXC1_SHIFT_SHIFT & GX_CP_VAT_GROUP1_TXC1_SHIFT_LMASK;
    
    pList[GX_VA_TEX2 - GX_VA_POS].compType = static_cast<GXCompType>(vat1 >> GX_CP_VAT_GROUP1_TXC2_TYPE_SHIFT & GX_CP_VAT_GROUP1_TXC2_TYPE_LMASK);
    pList[GX_VA_TEX2 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat1 >> GX_CP_VAT_GROUP1_TXC2_CNT_SHIFT & GX_CP_VAT_GROUP1_TXC2_CNT_LMASK);
    pList[GX_VA_TEX2 - GX_VA_POS].shift    = vat1 >> GX_CP_VAT_GROUP1_TXC2_SHIFT_SHIFT & GX_CP_VAT_GROUP1_TXC2_SHIFT_LMASK;
    
    pList[GX_VA_TEX3 - GX_VA_POS].compType = static_cast<GXCompType>(vat1 >> GX_CP_VAT_GROUP1_TXC3_TYPE_SHIFT & GX_CP_VAT_GROUP1_TXC3_TYPE_LMASK);
    pList[GX_VA_TEX3 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat1 >> GX_CP_VAT_GROUP1_TXC3_CNT_SHIFT & GX_CP_VAT_GROUP1_TXC3_CNT_LMASK);
    pList[GX_VA_TEX3 - GX_VA_POS].shift    = vat1 >> GX_CP_VAT_GROUP1_TXC3_SHIFT_SHIFT & GX_CP_VAT_GROUP1_TXC3_SHIFT_LMASK;
    
    pList[GX_VA_TEX4 - GX_VA_POS].compType = static_cast<GXCompType>(vat1 >> GX_CP_VAT_GROUP1_TXC4_TYPE_SHIFT & GX_CP_VAT_GROUP1_TXC4_TYPE_LMASK);
    pList[GX_VA_TEX4 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat1 >> GX_CP_VAT_GROUP1_TXC4_CNT_SHIFT & GX_CP_VAT_GROUP1_TXC4_CNT_LMASK);
    pList[GX_VA_TEX4 - GX_VA_POS].shift    = vat2 >> GX_CP_VAT_GROUP2_TXC4_SHIFT_SHIFT & GX_CP_VAT_GROUP2_TXC4_SHIFT_LMASK;
    
    pList[GX_VA_TEX5 - GX_VA_POS].compType = static_cast<GXCompType>(vat2 >> GX_CP_VAT_GROUP2_TXC5_TYPE_SHIFT & GX_CP_VAT_GROUP2_TXC5_TYPE_LMASK);
    pList[GX_VA_TEX5 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat2 >> GX_CP_VAT_GROUP2_TXC5_CNT_SHIFT & GX_CP_VAT_GROUP2_TXC5_CNT_LMASK);
    pList[GX_VA_TEX5 - GX_VA_POS].shift    = vat2 >> GX_CP_VAT_GROUP2_TXC5_SHIFT_SHIFT & GX_CP_VAT_GROUP2_TXC5_SHIFT_LMASK;
    
    pList[GX_VA_TEX6 - GX_VA_POS].compType = static_cast<GXCompType>(vat2 >> GX_CP_VAT_GROUP2_TXC6_TYPE_SHIFT & GX_CP_VAT_GROUP2_TXC6_TYPE_LMASK);
    pList[GX_VA_TEX6 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat2 >> GX_CP_VAT_GROUP2_TXC6_CNT_SHIFT & GX_CP_VAT_GROUP2_TXC6_CNT_LMASK);
    pList[GX_VA_TEX6 - GX_VA_POS].shift    = vat2 >> GX_CP_VAT_GROUP2_TXC6_SHIFT_SHIFT & GX_CP_VAT_GROUP2_TXC6_SHIFT_LMASK;
    
    pList[GX_VA_TEX7 - GX_VA_POS].compType = static_cast<GXCompType>(vat2 >> GX_CP_VAT_GROUP2_TXC7_TYPE_SHIFT & GX_CP_VAT_GROUP2_TXC7_TYPE_LMASK);
    pList[GX_VA_TEX7 - GX_VA_POS].compCnt  = static_cast<GXCompCnt>(vat2 >> GX_CP_VAT_GROUP2_TXC7_CNT_SHIFT & GX_CP_VAT_GROUP2_TXC7_CNT_LMASK);
    pList[GX_VA_TEX7 - GX_VA_POS].shift    = vat2 >> GX_CP_VAT_GROUP2_TXC7_SHIFT_SHIFT & GX_CP_VAT_GROUP2_TXC7_SHIFT_LMASK;
    // clang-format on

    return true;
}

void ResShp::GXSetArray(GXAttr attr, const void *pBase, u8 stride) {
    u8 *pCmd = GetResShpPrePrim().ref().dl.array[attr - GX_VA_POS];
    u32 cpAttr = attr != GX_VA_NBT ? attr - GX_VA_POS : 1;

    detail::ResWriteCPCmd(
        &pCmd[GX_CP_CMD_SZ * 0], cpAttr + GX_CP_REG_ARRAYBASE, reinterpret_cast<u32>(OSCachedToPhysical(pBase))
    );

    detail::ResWriteCPCmd(&pCmd[GX_CP_CMD_SZ * 1], cpAttr + GX_CP_REG_ARRAYSTRIDE, stride);
}

void ResShp::DisableSetArray(GXAttr attr) {
    u8 *pCmd = GetResShpPrePrim().ref().dl.array[attr - GX_VA_POS];
    std::memset(pCmd, 0, GX_CP_CMD_SZ * 2);
}

ResVtxPos ResShp::GetResVtxPos() const {
    return GetParent().GetResVtxPos(ref().idVtxPosition);
}

ResVtxNrm ResShp::GetResVtxNrm() const {
    const ResShpData &r = ref();

    if (r.idVtxNormal != -1) {
        return GetParent().GetResVtxNrm(r.idVtxNormal);
    }

    return ResVtxNrm(NULL);
}

ResVtxClr ResShp::GetResVtxClr(u32 idx) const {
    const ResShpData &r = ref();

    if (r.idVtxColor[idx] != -1) {
        return GetParent().GetResVtxClr(r.idVtxColor[idx]);
    }

    return ResVtxClr(NULL);
}

ResVtxTexCoord ResShp::GetResVtxTexCoord(u32 idx) const {
    const ResShpData &r = ref();

    if (r.idVtxTexCoord[idx] != -1) {
        return GetParent().GetResVtxTexCoord(r.idVtxTexCoord[idx]);
    }

    return ResVtxTexCoord(NULL);
}

ResVtxFurPos ResShp::GetResVtxFurPos() const {
    const ResShpData &r = ref();

    if (r.idVtxFurPos != -1) {
        return GetParent().GetResVtxFurPos(r.idVtxFurPos);
    }

    return ResVtxFurPos(NULL);
}

void ResShp::Init() {
    const void *pBase;
    u8 stride;

    GetResVtxPos().GetArray(&pBase, &stride);
    GXSetArray(GX_VA_POS, pBase, stride);

    ResVtxNrm nrm = GetResVtxNrm();
    if (nrm.IsValid()) {
        nrm.GetArray(&pBase, &stride);
        GXSetArray(GX_VA_NRM, pBase, stride);
    }

    u32 i;

    for (i = 0; i < GX_VA_TEX0 - GX_VA_CLR0; i++) {
        ResVtxClr clr = GetResVtxClr(i);

        if (clr.IsValid()) {
            clr.GetArray(&pBase, &stride);
            GXSetArray(static_cast<GXAttr>(GX_VA_CLR0 + i), pBase, stride);
        }
    }

    for (i = 0; i < GX_POS_MTX_ARRAY - GX_VA_TEX0; i++) {
        ResVtxTexCoord txc = GetResVtxTexCoord(i);

        if (txc.IsValid()) {
            txc.GetArray(&pBase, &stride);
            GXSetArray(static_cast<GXAttr>(GX_VA_TEX0 + i), pBase, stride);
        }
    }

    GetResShpPrePrim().DCStore(false);

    // TODO: Fakematch
    ResShpData &r = ref();
    DC::StoreRangeNoSync(GetPrimDLTag().GetDL(), r.tagPrimDL.bufSize);
}

void ResShp::Terminate() {
    DisableSetArray(GX_VA_POS);
    DisableSetArray(GX_VA_NRM);

    DisableSetArray(GX_VA_CLR0);
    DisableSetArray(GX_VA_CLR1);

    DisableSetArray(GX_VA_TEX0);
    DisableSetArray(GX_VA_TEX1);
    DisableSetArray(GX_VA_TEX2);
    DisableSetArray(GX_VA_TEX3);
    DisableSetArray(GX_VA_TEX4);
    DisableSetArray(GX_VA_TEX5);
    DisableSetArray(GX_VA_TEX6);
    DisableSetArray(GX_VA_TEX7);
}

void ResShp::CallPrePrimitiveDisplayList(bool sync, bool cacheIsSame) const {
    // TODO: Should be non-const, and initialized by value
    const ResTagDL &rTag = GetPrePrimDLTag();

    if (cacheIsSame) {
        if (sync) {
            GXCallDisplayList(const_cast<u8 *>(rTag.GetDL() + 32), rTag.GetCmdSize() - 32);
        } else {
            GXFastCallDisplayList(const_cast<u8 *>(rTag.GetDL() + 32), rTag.GetCmdSize() - 32);
        }

        return;
    }

    if (sync) {
        GXCallDisplayList(const_cast<u8 *>(rTag.GetDL()), rTag.GetCmdSize());
    } else {
        GXFastCallDisplayList(const_cast<u8 *>(rTag.GetDL()), rTag.GetCmdSize());
    }
}

void ResShp::CallPrimitiveDisplayList(bool sync) const {
    // TODO: Should be non-const, and initialized by value
    const ResTagDL &rTag = GetPrimDLTag();

    if (sync) {
        GXCallDisplayList(const_cast<u8 *>(rTag.GetDL()), rTag.GetCmdSize());
    } else {
        GXFastCallDisplayList(const_cast<u8 *>(rTag.GetDL()), rTag.GetCmdSize());
    }
}

void ResShpPrePrim::DCStore(bool sync) {
    ResPrePrimDL &r = ref();
    u32 size = sizeof(ResPrePrimDL);

    if (sync) {
        DC::StoreRange(&r, size);
    } else {
        DC::StoreRangeNoSync(&r, size);
    }
}

void ResShp::DCStore(bool sync) {
    ResShpData &r = ref();
    u32 size = r.size;

    if (sync) {
        DC::StoreRange(&r, size);
    } else {
        DC::StoreRangeNoSync(&r, size);
    }
}
} // namespace g3d
} // namespace nw4r
