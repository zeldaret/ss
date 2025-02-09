#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void CalcMaterialDirectly(ResMdl mdl, AnmObjTexPat* pAnmTexPat,
                          AnmObjTexSrt* pAnmTexSrt, AnmObjMatClr* pAnmMatClr) {

    u32 i;
    u32 numMatID = mdl.GetResMatNumEntries();

    for (i = 0; i < numMatID; i++) {
        ResMat mat = mdl.GetResMat(i);

        if (pAnmTexPat != NULL && pAnmTexPat->TestExistence(i)) {
            TexPatAnmResult result;
            const TexPatAnmResult* pResult;

            pResult = pAnmTexPat->GetResult(&result, i);

            ApplyTexPatAnmResult(mat.GetResTexObj(), mat.GetResTlutObj(),
                                 pResult);

            mat.GetResTexObj().EndEdit();
            mat.GetResTlutObj().EndEdit();
        }

        if (pAnmTexSrt != NULL && pAnmTexSrt->TestExistence(i)) {
            TexSrtAnmResult result;
            const TexSrtAnmResult* pResult;

            ResTexSrt srt = mat.GetResTexSrt();
            ResMatIndMtxAndScale ind = mat.GetResMatIndMtxAndScale();

            pResult = pAnmTexSrt->GetResult(&result, i);

            ApplyTexSrtAnmResult(srt, ind, pResult);

            ind.EndEdit();
            srt.EndEdit();
        }

        if (pAnmMatClr != NULL && pAnmMatClr->TestExistence(i)) {
            ClrAnmResult result;
            const ClrAnmResult* pResult;

            ResMatTevColor tevColor = mat.GetResMatTevColor();
            ResMatChan chan = mat.GetResMatChan();

            pResult = pAnmMatClr->GetResult(&result, i);

            ApplyClrAnmResult(chan, tevColor, pResult);

            chan.EndEdit();
            tevColor.EndEdit();
        }
    }
}

} // namespace g3d
} // namespace nw4r
