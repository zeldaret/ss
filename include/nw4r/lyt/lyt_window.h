#ifndef NW4R_LYT_WINDOW_H
#define NW4R_LYT_WINDOW_H

#include <common.h>
#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_types.h>

namespace nw4r {

namespace lyt {

struct WindowFrameSize {
    f32 l; // at 0x00
    f32 r; // at 0x04
    f32 t; // at 0x08
    f32 b; // at 0x0C
};

class Window : public Pane {
public:
    struct Content {
        Content() {}

        ut::Color vtxColors[4];          // at 0x00
        detail::TexCoordAry texCoordAry; // at 0x10
    };
    struct Frame {
        bool textureFlip;    // at 0x00
        Material *pMaterial; // at 0x04
    };

    Window(const res::Window *pBlock, const ResBlockSet &ResBlockSet);

    void InitContent(u8 texNum);
    void InitFrame(u8 frameNum);
    void ReserveTexCoord(u8 num);
    AnimationLink *FindAnimationLink(AnimTransform *pAnimTrans);
    void SetAnimationEnable(AnimTransform *pAnimTrans, bool, bool);

    static WindowFrameSize GetFrameSize(u8 frameNum, const Frame *frames);
    Material *GetFrameMaterial(u32 frameIdx) const;
    Material *GetContentMaterial() const;

    virtual ~Window();                                                                                      // at 0x08
    NW4R_UT_RTTI_DECL(Window);                                                                              // at 0x0C
    virtual void DrawSelf(const DrawInfo &drawInfo);                                                        // at 0x18
    virtual void AnimateSelf(u32 option);                                                                   // at 0x20
    virtual ut::Color GetVtxColor(u32 idx) const;                                                           // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color value);                                                     // at 0x28
    virtual u8 GetVtxColorElement(u32 idx) const;                                                           // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value);                                                     // at 0x38
    virtual Material *FindMaterialByName(const char *findName, bool bRecursive);                            // at 0x40
    virtual void UnbindAnimationSelf(AnimTransform *pAnimTrans);                                            // at 0x50
    virtual u8 GetMaterialNum() const;                                                                      // at 0x64
    virtual Material *GetMaterial(u32 idx) const;                                                           // at 0x6C
    virtual void DrawContent(const math::VEC2 &basePt, const WindowFrameSize &frameSize, u8 alpha);         // at 0x74
    virtual void DrawFrame(const math::VEC2 &basePt, const Frame &frame, const WindowFrameSize &frameSize,  //
            u8 alpha);                                                                                      // at 0x78
    virtual void DrawFrame4(const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, //
            u8 alpha);                                                                                      // at 0x7C
    virtual void DrawFrame8(const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, //
            u8 alpha);                                                                                      // at 0x80

private:
    res::InflationLRTB mContentInflation; // at 0x0D8
    Content mContent;                     // at 0x0E8
    Frame *mFrams;                        // at 0x100
    u8 mFrameNum;                         // 0x104
};

} // namespace lyt

} // namespace nw4r

#endif
