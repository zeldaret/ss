#include <nw4r/g3d.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

void CalcVtx(ResMdl mdl, AnmObjShp* pAnmShp, ResVtxPosData** ppVtxPosTable,
             ResVtxNrmData** ppVtxNrmTable, ResVtxClrData** ppVtxClrTable) {

    // Allows struct offsets inside assembly
    using nw4r::math::VEC3;
    using nw4r::ut::Color;

    u32 numVtxPos = mdl.GetResVtxPosNumEntries();

    for (u32 id = 0; id < numVtxPos; id++) {
        if (!pAnmShp->TestExistence(id)) {
            continue;
        }

        ShpAnmResult result;
        const ShpAnmResult* pResult = pAnmShp->GetResult(&result, id);

        if (!(pResult->flags & ShpAnmResult::FLAG_ANM_EXISTS)) {
            continue;
        }

        if (pResult->flags & ShpAnmResult::FLAG_ANM_VTXPOS) {
            struct KeyShape {
                const math::VEC3* pVtx; // at 0x0
                f32 weight;             // at 0x4
            };

            ResVtxPos basePos = pResult->baseShapeVtxSet.resVtxPos;

            KeyShape keyShape[ShpAnmResult::MAX_KEY_SHAPE + 1];
            int numKeyShape = 0;

            ResVtxPos vtxPos(ppVtxPosTable[id]);
            math::VEC3* pVtxPosBuf = static_cast<math::VEC3*>(vtxPos.GetData());

            if (pResult->baseShapeWeight != 0.0f) {
                const math::VEC3* pBase;
                u8 stride;

                basePos.GetArray(reinterpret_cast<const void**>(&pBase),
                                 &stride);

                keyShape[numKeyShape].pVtx = pBase;
                keyShape[numKeyShape].weight = pResult->baseShapeWeight;
                numKeyShape++;
            }

            for (int i = 0; i < static_cast<int>(pResult->numKeyShape); i++) {
                if (pResult->keyShape[i].weight == 0.0f) {
                    continue;
                }

                ResVtxPos key(pResult->keyShape[i].vtxSet.resVtxPos);
                const void* pData = key.GetData();

                keyShape[numKeyShape].pVtx =
                    static_cast<const math::VEC3*>(pData);
                keyShape[numKeyShape].weight = pResult->keyShape[i].weight;
                numKeyShape++;
            }

            u32 numVtx = basePos.GetNumVtxPos();

            // clang-format off
            const math::VEC3* const pVtxPosBufEnd = reinterpret_cast<const math::VEC3*>(
                reinterpret_cast<const u8*>(pVtxPosBuf) + numVtx * sizeof(math::VEC3));
            // clang-format on

            const KeyShape* const pKeyEnd = keyShape + numKeyShape;

            for (; pVtxPosBuf < pVtxPosBufEnd; pVtxPosBuf++) {
                register f32 xy, z_;

                KeyShape& rFirst = keyShape[0];
                register f32 firstWeight = rFirst.weight;
                register const void* pVtx = rFirst.pVtx;

                // clang-format off
                asm {
                    psq_l  xy, VEC3.x(pVtx), 0, 0
                    psq_l  z_, VEC3.z(pVtx), 1, 0
                    ps_mul xy, xy, firstWeight
                    ps_mul z_, z_, firstWeight
                }
                // clang-format on

                // clang-format off
                rFirst.pVtx = reinterpret_cast<const math::VEC3*>(
                    reinterpret_cast<const u8*>(rFirst.pVtx) + sizeof(math::VEC3));
                // clang-format on

                for (KeyShape* pKey = &keyShape[1]; pKey < pKeyEnd; pKey++) {
                    register f32 keyWeight = pKey->weight;
                    register const void* pKeyVtx = pKey->pVtx;
                    register f32 key_xy, key_z_;

                    // clang-format off
                    asm {
                        psq_l   key_xy, VEC3.x(pKeyVtx), 0, 0
                        psq_l   key_z_, VEC3.z(pKeyVtx), 1, 0
                        ps_madd xy, key_xy, keyWeight, xy
                        ps_madd z_, key_z_, keyWeight, z_
                    }
                    // clang-format on

                    // clang-format off
                    pKey->pVtx = reinterpret_cast<const math::VEC3*>(
                        reinterpret_cast<const u8*>(pKey->pVtx) + sizeof(math::VEC3));
                    // clang-format on
                }

                register void* pDst = pVtxPosBuf;

                // clang-format off
                asm {
                    psq_st xy, VEC3.x(pDst), 0, 0
                    psq_st z_, VEC3.z(pDst), 1, 0
                }
                // clang-format on
            }

            DC::StoreRange(vtxPos.GetData(), numVtx * sizeof(math::VEC3));
        }

        if ((pResult->flags & ShpAnmResult::FLAG_ANM_VTXNRM) &&
            ppVtxNrmTable != NULL) {

            struct KeyShape {
                const math::VEC3* pVtx; // at 0x0
                f32 weight;             // at 0x4
            };

            ResVtxNrm baseNrm = pResult->baseShapeVtxSet.resVtxNrm;

            KeyShape keyShape[ShpAnmResult::MAX_KEY_SHAPE + 1];
            int numKeyShape = 0;

            ResVtxNrm vtxNrm(ppVtxNrmTable[baseNrm.GetID()]);
            math::VEC3* pVtxNrmBuf = static_cast<math::VEC3*>(vtxNrm.GetData());

            if (pResult->baseShapeWeight != 0.0f) {
                const math::VEC3* pBase;
                u8 stride;

                baseNrm.GetArray(reinterpret_cast<const void**>(&pBase),
                                 &stride);

                keyShape[numKeyShape].pVtx = pBase;
                keyShape[numKeyShape].weight = pResult->baseShapeWeight;
                numKeyShape++;
            }

            for (int i = 0; i < static_cast<int>(pResult->numKeyShape); i++) {
                if (pResult->keyShape[i].weight == 0.0f) {
                    continue;
                }

                ResVtxNrm key(pResult->keyShape[i].vtxSet.resVtxNrm);
                const void* pData = key.GetData();

                keyShape[numKeyShape].pVtx =
                    static_cast<const math::VEC3*>(pData);
                keyShape[numKeyShape].weight = pResult->keyShape[i].weight;
                numKeyShape++;
            }

            u32 numVtx = baseNrm.GetNumVtxNrm();

            // clang-format off
            const math::VEC3* const pVtxNrmBufEnd = reinterpret_cast<const math::VEC3*>(
                reinterpret_cast<const u8*>(pVtxNrmBuf) + numVtx * sizeof(math::VEC3));
            // clang-format on

            const KeyShape* const pKeyEnd = keyShape + numKeyShape;

            for (; pVtxNrmBuf < pVtxNrmBufEnd; pVtxNrmBuf++) {
                register f32 xy, z_;

                KeyShape& rFirst = keyShape[0];
                register f32 firstWeight = rFirst.weight;
                register const void* pVtx = rFirst.pVtx;

                // clang-format off
                asm {
                    psq_l  xy, VEC3.x(pVtx), 0, 0
                    psq_l  z_, VEC3.z(pVtx), 1, 0
                    ps_mul xy, xy, firstWeight
                    ps_mul z_, z_, firstWeight
                }
                // clang-format on

                // clang-format off
                rFirst.pVtx = reinterpret_cast<const math::VEC3*>(
                    reinterpret_cast<const u8*>(rFirst.pVtx) + sizeof(math::VEC3));
                // clang-format on

                for (KeyShape* pKey = &keyShape[1]; pKey < pKeyEnd; pKey++) {
                    register f32 keyWeight = pKey->weight;
                    register const void* pKeyVtx = pKey->pVtx;
                    register f32 key_xy, key_z_;

                    // clang-format off
                    asm {
                        psq_l   key_xy, VEC3.x(pKeyVtx), 0, 0
                        psq_l   key_z_, VEC3.z(pKeyVtx), 1, 0
                        ps_madd xy, key_xy, keyWeight, xy
                        ps_madd z_, key_z_, keyWeight, z_
                    }
                    // clang-format on

                    // clang-format off
                    pKey->pVtx = reinterpret_cast<const math::VEC3*>(
                        reinterpret_cast<const u8*>(pKey->pVtx) + sizeof(math::VEC3));
                    // clang-format on
                }

                register void* pDst = pVtxNrmBuf;

                // clang-format off
                asm {
                    psq_st xy, VEC3.x(pDst), 0, 0
                    psq_st z_, VEC3.z(pDst), 1, 0
                }
                // clang-format on
            }

            DC::StoreRange(vtxNrm.GetData(), numVtx * sizeof(math::VEC3));
        }

        if ((pResult->flags & ShpAnmResult::FLAG_ANM_VTXCLR) &&
            ppVtxClrTable != NULL) {

            struct KeyShape {
                const ut::Color* pVtx; // at 0x0
                f32 weight;            // at 0x4
            };

            ResVtxClr baseClr = pResult->baseShapeVtxSet.resVtxClr;

            KeyShape keyShape[ShpAnmResult::MAX_KEY_SHAPE + 1];
            int numKeyShape = 0;

            ResVtxClr vtxClr(ppVtxClrTable[baseClr.GetID()]);
            ut::Color* pVtxClrBuf = static_cast<ut::Color*>(vtxClr.GetData());

            if (pResult->baseShapeWeight != 0.0f) {
                const ut::Color* pBase;
                u8 stride;

                baseClr.GetArray(reinterpret_cast<const void**>(&pBase),
                                 &stride);

                keyShape[numKeyShape].pVtx = pBase;
                keyShape[numKeyShape].weight = pResult->baseShapeWeight;
                numKeyShape++;
            }

            for (int i = 0; i < static_cast<int>(pResult->numKeyShape); i++) {
                if (pResult->keyShape[i].weight == 0.0f) {
                    continue;
                }

                ResVtxClr key(pResult->keyShape[i].vtxSet.resVtxClr);
                const void* pData = key.GetData();

                keyShape[numKeyShape].pVtx =
                    static_cast<const ut::Color*>(pData);
                keyShape[numKeyShape].weight = pResult->keyShape[i].weight;
                numKeyShape++;
            }

            u32 numVtx = baseClr.GetNumVtxClr();

            // clang-format off
            const ut::Color* const pVtxClrBufEnd = reinterpret_cast<const ut::Color*>(
                reinterpret_cast<const u8*>(pVtxClrBuf) + numVtx * sizeof(ut::Color));
            // clang-format on

            const KeyShape* const pKeyEnd = keyShape + numKeyShape;

            for (; pVtxClrBuf < pVtxClrBufEnd; pVtxClrBuf++) {
                register f32 rg, ba;

                KeyShape& rFirst = keyShape[0];
                register f32 firstWeight = rFirst.weight;
                register const void* pVtx = rFirst.pVtx;

                // clang-format off
                asm {
                    psq_l  rg, Color.r(pVtx), 0, 2
                    psq_l  ba, Color.b(pVtx), 0, 2
                    ps_mul rg, rg, firstWeight
                    ps_mul ba, ba, firstWeight
                }
                // clang-format on

                // clang-format off
                rFirst.pVtx = reinterpret_cast<const ut::Color*>(
                    reinterpret_cast<const u8*>(rFirst.pVtx) + sizeof(ut::Color));
                // clang-format on

                for (KeyShape* pKey = &keyShape[1]; pKey < pKeyEnd; pKey++) {
                    register f32 keyWeight = pKey->weight;
                    register const void* pKeyVtx = pKey->pVtx;
                    register f32 key_rg, key_ba;

                    // clang-format off
                    asm {
                        psq_l   key_rg, Color.r(pKeyVtx), 0, 2
                        psq_l   key_ba, Color.b(pKeyVtx), 0, 2
                        ps_madd rg, key_rg, keyWeight, rg
                        ps_madd ba, key_ba, keyWeight, ba
                    }
                    // clang-format on

                    // clang-format off
                    pKey->pVtx = reinterpret_cast<const ut::Color*>(
                        reinterpret_cast<const u8*>(pKey->pVtx) + sizeof(ut::Color));
                    // clang-format on
                }

                register void* pDst = pVtxClrBuf;

                // clang-format off
                asm {
                    psq_st rg, Color.r(pDst), 0, 2
                    psq_st ba, Color.b(pDst), 0, 2
                }
                // clang-format on
            }

            DC::StoreRange(vtxClr.GetData(), numVtx * sizeof(ut::Color));
        }
    }
}

} // namespace g3d
} // namespace nw4r
