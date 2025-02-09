#ifndef NW4R_G3D_RES_RES_FILE_H
#define NW4R_G3D_RES_RES_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/g3d/res/g3d_resanmclr.h>
#include <nw4r/g3d/res/g3d_resanmscn.h>
#include <nw4r/g3d/res/g3d_resanmshp.h>
#include <nw4r/g3d/res/g3d_resanmtexpat.h>
#include <nw4r/g3d/res/g3d_resanmtexsrt.h>
#include <nw4r/g3d/res/g3d_resanmvis.h>
#include <nw4r/g3d/res/g3d_rescommon.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_respltt.h>
#include <nw4r/g3d/res/g3d_restex.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace g3d {

struct ResTopLevelDictData {
    ut::BinaryBlockHeader header; // at 0x0
    ResDicData topLevel;          // at 0x8
};

struct ResFileData {
    ut::BinaryFileHeader fileHeader; // at 0x0
    ResTopLevelDictData dict;        // at 0x10
};

class ResFile : public ResCommon<ResFileData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResFile);

    void Init();
    void Terminate();
    bool CheckRevision() const;

    bool Bind(const ResFile file);
    void Release();

    bool Bind() {
        return Bind(*this);
    }

    ResMdl GetResMdl(const char *pName) const;
    ResMdl GetResMdl(int idx) const;
    ResMdl GetResMdl(u32 idx) const;

    ResPltt GetResPltt(const char *pName) const;
    ResPltt GetResPltt(const ResName name) const;
    ResPltt GetResPltt(int idx) const;
    ResPltt GetResPltt(u32 idx) const;

    ResTex GetResTex(const char *pName) const;
    ResTex GetResTex(const ResName name) const;
    ResTex GetResTex(int idx) const;
    ResTex GetResTex(u32 idx) const;

    ResAnmChr GetResAnmChr(const char *pName) const;
    ResAnmChr GetResAnmChr(int idx) const;
    ResAnmChr GetResAnmChr(u32 idx) const;

    ResAnmVis GetResAnmVis(const char *pName) const;
    ResAnmVis GetResAnmVis(int idx) const;
    ResAnmVis GetResAnmVis(u32 idx) const;

    ResAnmClr GetResAnmClr(const char *pName) const;
    ResAnmClr GetResAnmClr(int idx) const;
    ResAnmClr GetResAnmClr(u32 idx) const;

    ResAnmTexPat GetResAnmTexPat(const char *pName) const;
    ResAnmTexPat GetResAnmTexPat(int idx) const;
    ResAnmTexPat GetResAnmTexPat(u32 idx) const;

    ResAnmTexSrt GetResAnmTexSrt(const char *pName) const;
    ResAnmTexSrt GetResAnmTexSrt(int idx) const;
    ResAnmTexSrt GetResAnmTexSrt(u32 idx) const;

    ResAnmShp GetResAnmShp(const char *pName) const;
    ResAnmShp GetResAnmShp(int idx) const;
    ResAnmShp GetResAnmShp(u32 idx) const;

    ResAnmScn GetResAnmScn(const char *pName) const;
    ResAnmScn GetResAnmScn(int idx) const;
    ResAnmScn GetResAnmScn(u32 idx) const;

    u32 GetResMdlNumEntries() const;
    u32 GetResPlttNumEntries() const;
    u32 GetResTexNumEntries() const;
    u32 GetResAnmChrNumEntries() const;
    u32 GetResAnmVisNumEntries() const;
    u32 GetResAnmClrNumEntries() const;
    u32 GetResAnmTexPatNumEntries() const;
    u32 GetResAnmTexSrtNumEntries() const;
    u32 GetResAnmShpNumEntries() const;
    u32 GetResAnmScnNumEntries() const;
};

} // namespace g3d
} // namespace nw4r

#endif
