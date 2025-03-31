#include "nw4r/g3d.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {
namespace {

NW4R_G3D_RESFILE_NAME_DEF(Models, "3DModels(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Pltts, "Palettes(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Textures, "Textures(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmChr, "AnmChr(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmVis, "AnmVis(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmClr, "AnmClr(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmTexPat, "AnmTexPat(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmTexSrt, "AnmTexSrt(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmShp, "AnmShp(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmScn, "AnmScn(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Ext, "External");

} // namespace

/******************************************************************************
 *
 * ResMdl
 *
 ******************************************************************************/
ResMdl ResFile::GetResMdl(const char *pName) const {
    void *pResMdlDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Models)];

    if (pResMdlDicData != NULL) {
        return ResMdl(ResDic(pResMdlDicData)[pName]);
    }

    return ResMdl(NULL);
}

ResMdl ResFile::GetResMdl(int idx) const {
    void *pResMdlDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Models)];

    if (pResMdlDicData != NULL) {
        return ResMdl(ResDic(pResMdlDicData)[idx]);
    }

    return ResMdl(NULL);
}

ResMdl ResFile::GetResMdl(u32 idx) const {
    return GetResMdl(static_cast<int>(idx));
}

u32 ResFile::GetResMdlNumEntries() const {
    void *pResMdlDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Models)];

    if (pResMdlDicData != NULL) {
        return ResDic(pResMdlDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResPltt
 *
 ******************************************************************************/
ResPltt ResFile::GetResPltt(const char *pName) const {
    void *pResPlttDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[pName]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(const ResName name) const {
    void *pResPlttDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[name]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(int idx) const {
    void *pResPlttDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[idx]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(u32 idx) const {
    return GetResPltt(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResTex
 *
 ******************************************************************************/
ResTex ResFile::GetResTex(const char *pName) const {
    void *pResTexDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[pName]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(const ResName name) const {
    void *pResTexDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[name]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(int idx) const {
    void *pResTexDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[idx]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(u32 idx) const {
    return GetResTex(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmChr
 *
 ******************************************************************************/
ResAnmChr ResFile::GetResAnmChr(const char *pName) const {
    void *pResAnmChrDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmChr)];

    if (pResAnmChrDicData != NULL) {
        return ResAnmChr(ResDic(pResAnmChrDicData)[pName]);
    }

    return ResAnmChr(NULL);
}

ResAnmChr ResFile::GetResAnmChr(int idx) const {
    void *pResAnmChrDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmChr)];

    if (pResAnmChrDicData != NULL) {
        return ResAnmChr(ResDic(pResAnmChrDicData)[idx]);
    }

    return ResAnmChr(NULL);
}

ResAnmChr ResFile::GetResAnmChr(u32 idx) const {
    return GetResAnmChr(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmVis
 *
 ******************************************************************************/
ResAnmVis ResFile::GetResAnmVis(const char *pName) const {
    void *pResAnmVisDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmVis)];

    if (pResAnmVisDicData != NULL) {
        return ResAnmVis(ResDic(pResAnmVisDicData)[pName]);
    }

    return ResAnmVis(NULL);
}

ResAnmVis ResFile::GetResAnmVis(int idx) const {
    void *pResAnmVisDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmVis)];

    if (pResAnmVisDicData != NULL) {
        return ResAnmVis(ResDic(pResAnmVisDicData)[idx]);
    }

    return ResAnmVis(NULL);
}

ResAnmVis ResFile::GetResAnmVis(u32 idx) const {
    return GetResAnmVis(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmClr
 *
 ******************************************************************************/
ResAnmClr ResFile::GetResAnmClr(const char *pName) const {
    void *pResAnmClrDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmClr)];

    if (pResAnmClrDicData != NULL) {
        return ResAnmClr(ResDic(pResAnmClrDicData)[pName]);
    }

    return ResAnmClr(NULL);
}

ResAnmClr ResFile::GetResAnmClr(int idx) const {
    void *pResAnmClrDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmClr)];

    if (pResAnmClrDicData != NULL) {
        return ResAnmClr(ResDic(pResAnmClrDicData)[idx]);
    }

    return ResAnmClr(NULL);
}

ResAnmClr ResFile::GetResAnmClr(u32 idx) const {
    return GetResAnmClr(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmTexPat
 *
 ******************************************************************************/
ResAnmTexPat ResFile::GetResAnmTexPat(const char *pName) const {
    void *pResAnmTexPatDicData =
        ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmTexPat)];

    if (pResAnmTexPatDicData != NULL) {
        return ResAnmTexPat(ResDic(pResAnmTexPatDicData)[pName]);
    }

    return ResAnmTexPat(NULL);
}

ResAnmTexPat ResFile::GetResAnmTexPat(int idx) const {
    void *pResAnmTexPatDicData =
        ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmTexPat)];

    if (pResAnmTexPatDicData != NULL) {
        return ResAnmTexPat(ResDic(pResAnmTexPatDicData)[idx]);
    }

    return ResAnmTexPat(NULL);
}

ResAnmTexPat ResFile::GetResAnmTexPat(u32 idx) const {
    return GetResAnmTexPat(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmTexSrt
 *
 ******************************************************************************/
ResAnmTexSrt ResFile::GetResAnmTexSrt(const char *pName) const {
    void *pResAnmTexSrtDicData =
        ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmTexSrt)];

    if (pResAnmTexSrtDicData != NULL) {
        return ResAnmTexSrt(ResDic(pResAnmTexSrtDicData)[pName]);
    }

    return ResAnmTexSrt(NULL);
}

ResAnmTexSrt ResFile::GetResAnmTexSrt(int idx) const {
    void *pResAnmTexSrtDicData =
        ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmTexSrt)];

    if (pResAnmTexSrtDicData != NULL) {
        return ResAnmTexSrt(ResDic(pResAnmTexSrtDicData)[idx]);
    }

    return ResAnmTexSrt(NULL);
}

ResAnmTexSrt ResFile::GetResAnmTexSrt(u32 idx) const {
    return GetResAnmTexSrt(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmShp
 *
 ******************************************************************************/
ResAnmShp ResFile::GetResAnmShp(const char *pName) const {
    void *pResAnmShpDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmShp)];

    if (pResAnmShpDicData != NULL) {
        return ResAnmShp(ResDic(pResAnmShpDicData)[pName]);
    }

    return ResAnmShp(NULL);
}

ResAnmShp ResFile::GetResAnmShp(int idx) const {
    void *pResAnmShpDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmShp)];

    if (pResAnmShpDicData != NULL) {
        return ResAnmShp(ResDic(pResAnmShpDicData)[idx]);
    }

    return ResAnmShp(NULL);
}

ResAnmShp ResFile::GetResAnmShp(u32 idx) const {
    return GetResAnmShp(static_cast<int>(idx));
}

/******************************************************************************
 *
 * ResAnmScn
 *
 ******************************************************************************/
ResAnmScn ResFile::GetResAnmScn(const char *pName) const {
    void *pResAnmScnDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmScn)];

    if (pResAnmScnDicData != NULL) {
        return ResAnmScn(ResDic(pResAnmScnDicData)[pName]);
    }

    return ResAnmScn(NULL);
}

ResAnmScn ResFile::GetResAnmScn(int idx) const {
    void *pResAnmScnDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmScn)];

    if (pResAnmScnDicData != NULL) {
        return ResAnmScn(ResDic(pResAnmScnDicData)[idx]);
    }

    return ResAnmScn(NULL);
}

ResAnmScn ResFile::GetResAnmScn(u32 idx) const {
    return GetResAnmScn(static_cast<int>(idx));
}

/******************************************************************************
 *
 * Miscellaneous
 *
 ******************************************************************************/

u32 ResFile::GetResPlttNumEntries() const {
    void *pResPlttDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResDic(pResPlttDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResTexNumEntries() const {
    void *pResTexDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResDic(pResTexDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmChrNumEntries() const {
    void *pResAnmChrDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmChr)];

    if (pResAnmChrDicData != NULL) {
        return ResDic(pResAnmChrDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmVisNumEntries() const {
    void *pResAnmVisDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmVis)];

    if (pResAnmVisDicData != NULL) {
        return ResDic(pResAnmVisDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmClrNumEntries() const {
    void *pResAnmClrDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmClr)];

    if (pResAnmClrDicData != NULL) {
        return ResDic(pResAnmClrDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmTexPatNumEntries() const {
    void *pResAnmTexPatDicData =
        ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmTexPat)];

    if (pResAnmTexPatDicData != NULL) {
        return ResDic(pResAnmTexPatDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmTexSrtNumEntries() const {
    void *pResAnmTexSrtDicData =
        ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmTexSrt)];

    if (pResAnmTexSrtDicData != NULL) {
        return ResDic(pResAnmTexSrtDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmShpNumEntries() const {
    void *pResAnmShpDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmShp)];

    if (pResAnmShpDicData != NULL) {
        return ResDic(pResAnmShpDicData).GetNumData();
    }

    return 0;
}

u32 ResFile::GetResAnmScnNumEntries() const {
    void *pResAnmScnDicData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_AnmScn)];

    if (pResAnmScnDicData != NULL) {
        return ResDic(pResAnmScnDicData).GetNumData();
    }

    return 0;
}

void *ResFile::GetExternalData(const char *pName) const {
    void *pExternalData = ResDic(const_cast<ResDicData *>(&ref().dict.topLevel))[ResName(&ResNameData_Ext)];

    if (pExternalData != NULL) {
        return ResDic(pExternalData)[pName];
    }

    return NULL;
}

bool ResFile::Bind(const ResFile file) {
    u32 i;
    bool success = true;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        success = GetResMdl(i).Bind(file) && success;
    }

    u32 anmTexPatNum = GetResAnmTexPatNumEntries();
    for (i = 0; i < anmTexPatNum; i++) {
        // @bug Success value clobbered by most recent result
        success = GetResAnmTexPat(i).Bind(file);
    }

    return success;
}

void ResFile::Release() {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        GetResMdl(i).Release();
    }

    u32 anmTexPatNum = GetResAnmTexPatNumEntries();
    for (i = 0; i < anmTexPatNum; i++) {
        GetResAnmTexPat(i).Release();
    }
}

void ResFile::Init() {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        GetResMdl(i).Init();
    }

    u32 texNum = GetResTexNumEntries();
    for (i = 0; i < texNum; i++) {
        GetResTex(i).Init();
    }

    u32 plttNum = GetResPlttNumEntries();
    for (i = 0; i < plttNum; i++) {
        GetResPltt(i).Init();
    }
}

void ResFile::Terminate() {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        GetResMdl(i).Terminate();
    }
}

bool ResFile::CheckRevision() const {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        if (!GetResMdl(i).CheckRevision()) {
            return false;
        }
    }

    u32 texNum = GetResTexNumEntries();
    for (i = 0; i < texNum; i++) {
        if (!GetResTex(i).CheckRevision()) {
            return false;
        }
    }

    u32 plttNum = GetResPlttNumEntries();
    for (i = 0; i < plttNum; i++) {
        if (!GetResPltt(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmChrNum = GetResAnmChrNumEntries();
    for (i = 0; i < anmChrNum; i++) {
        if (!GetResAnmChr(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmVisNum = GetResAnmVisNumEntries();
    for (i = 0; i < anmVisNum; i++) {
        if (!GetResAnmVis(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmClrNum = GetResAnmClrNumEntries();
    for (i = 0; i < anmClrNum; i++) {
        if (!GetResAnmClr(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmTexPatNum = GetResAnmTexPatNumEntries();
    for (i = 0; i < anmTexPatNum; i++) {
        if (!GetResAnmTexPat(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmTexSrtNum = GetResAnmTexSrtNumEntries();
    for (i = 0; i < anmTexSrtNum; i++) {
        if (!GetResAnmTexSrt(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmShpNum = GetResAnmShpNumEntries();
    for (i = 0; i < anmShpNum; i++) {
        if (!GetResAnmShp(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmScnNum = GetResAnmScnNumEntries();
    for (i = 0; i < anmScnNum; i++) {
        if (!GetResAnmScn(i).CheckRevision()) {
            return false;
        }
    }

    return true;
}

} // namespace g3d
} // namespace nw4r
