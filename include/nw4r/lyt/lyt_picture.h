#ifndef NW4R_LYT_PICTURE_H
#define NW4R_LYT_PICTURE_H

#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_resources.h"

namespace nw4r {

namespace lyt {

class Picture : public Pane {
public:
    Picture(u8);
    Picture(const TexMap &texMap);
    Picture(const res::Picture *pResPic, const ResBlockSet &resDataSet);

    virtual ~Picture();                                 // at 0x08
    NW4R_UT_RTTI_DECL(Picture);                         // at 0x0C
    virtual void DrawSelf(const DrawInfo &drawInfo);    // at 0x18
    virtual ut::Color GetVtxColor(u32 idx) const;       // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color value); // at 0x28
    virtual u8 GetVtxColorElement(u32 idx) const;       // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value); // at 0x38
    virtual void Append(const TexMap &);                // at 0x78

    void Init(u8 texNum);

    void ReserveTexCoord(u8 texNum);

    void SetTexCoordNum(u8 texNum);

protected:
    ut::Color mVtxColors[TEXCOORD_VTX_COUNT]; // at 0xD8
    detail::TexCoordAry mTexCoordAry;         // at 0xE8
};

} // namespace lyt

} // namespace nw4r

#endif
