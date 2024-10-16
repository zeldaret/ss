#ifndef NW4R_LYT_WINDOW_H
#define NW4R_LYT_WINDOW_H

#include "common.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"

namespace nw4r {

namespace lyt {

// NOT OFFICAL
// Pulled from https://wiki.tockdom.com/wiki/BRLYT_(File_Format)#Window_Frames
enum TextureFlip {
    TEXFLIP_NONE,
    TEXFLIP_HORIZONTAL,
    TEXFLIP_VERTICAL,
    TEXFLIP_ROTATE_90,
    TEXFLIP_ROTATE_180,
    TEXFLIP_ROTATE_270,
};

enum TextureCoord {
    TopL,
    TopR,
    BotL,
    BotR
};

struct WindowFrameSize {
    f32 l; // at 0x00
    f32 r; // at 0x04
    f32 t; // at 0x08
    f32 b; // at 0x0C
};

class Window : public Pane {
public:
    struct Content {
        Content() : vtxColors(), texCoordAry() {}

        ut::Color vtxColors[4];          // at 0x00
        detail::TexCoordAry texCoordAry; // at 0x10
    };
    struct Frame {
        Frame() : textureFlip(false), pMaterial(nullptr) {}
        ~Frame() {
            Layout::DeleteObj(pMaterial);
        }
        u8 textureFlip;      // at 0x00
        Material *pMaterial; // at 0x04
    };

    Window(const res::Window *pBlock, const ResBlockSet &ResBlockSet);

    void InitContent(u8 texNum);
    void InitFrame(u8 frameNum);
    void ReserveTexCoord(u8 num);

    static WindowFrameSize GetFrameSize(u8 frameNum, const Frame *frames);

    virtual ~Window();                                                                              // at 0x08
    NW4R_UT_RTTI_DECL(Window);                                                                      // at 0x0C
    virtual void DrawSelf(const DrawInfo &drawInfo);                                                // at 0x18
    virtual void AnimateSelf(u32 option);                                                           // at 0x20
    virtual ut::Color GetVtxColor(u32 idx) const;                                                   // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color value);                                             // at 0x28
    virtual u8 GetVtxColorElement(u32 idx) const;                                                   // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value);                                             // at 0x38
    virtual Material *FindMaterialByName(const char *findName, bool bRecursive);                    // at 0x40
    virtual void UnbindAnimationSelf(AnimTransform *pAnimTrans);                                    // at 0x50
    virtual u8 GetMaterialNum() const;                                                              // at 0x64
    virtual Material *GetMaterial(u32 idx) const;                                                   // at 0x6C
    virtual Material *GetContentMaterial() const;                                                   // at 0x74
    virtual Material *GetFrameMaterial(u32 frameIdx) const;                                         // at 0x78
    virtual void DrawContent(const math::VEC2 &basePt, const WindowFrameSize &frameSize, u8 alpha); // at 0x7C
    virtual void DrawFrame(
        const math::VEC2 &basePt, const Frame &frame, const WindowFrameSize &frameSize, //
        u8 alpha
    ); // at 0x80
    virtual void DrawFrame4(
        const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, //
        u8 alpha
    ); // at 0x84
    virtual void DrawFrame8(
        const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, //
        u8 alpha
    ); // at 0x88

private:
    res::InflationLRTB mContentInflation; // at 0x0D8
    Content mContent;                     // at 0x0E8
    Frame *mFrames;                       // at 0x100
    u8 mFrameNum;                         // 0x104
};

} // namespace lyt

} // namespace nw4r

#endif
