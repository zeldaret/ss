#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_window.h>

namespace nw4r {
namespace lyt {

// TODO: Find Place
struct TextureFlipInfo {
    u8 coords[2][4]; // at 0x00
    u8 idx[2];       // at 0x8
};

TextureFlipInfo flipInfos[6] = {};

namespace {
// GetTexutreFlipInfo__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FUc
// GetTexutreFlipInfo__24@unnamed@lyt_window_cpp@FUc
TextureFlipInfo &GetTextureFlipInfo(u8 textureFlip) {}

// GetLTFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetLTFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetLTFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {}

// GetLTTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetLTTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetLTTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = flipInfo;
    int ix;
    int iy;
    math::VEC2 tSz;
}

// GetRTFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetRTFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetRTFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {}

// GetRTTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetRTTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetRTTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = flipInfo;
    int ix;
    int iy;
    math::VEC2 tSz;
}

// GetLBFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetLBFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetLBFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {}

// GetLBTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetLBTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetLBTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = flipInfo;
    int ix;
    int iy;
    math::VEC2 tSz;
}

// GetRBFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetRBFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetRBFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {}

// GetRBTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetRBTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetRBTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = flipInfo;
    int ix;
    int iy;
    math::VEC2 tSz;
}
} // namespace

NW4R_UT_RTTI_DEF_DERIVED(Window, Pane);

// __dt__Q44nw4r3lyt6Window5FrameFv

// __ct__Q34nw4r3lyt6WindowFPCQ44nw4r3lyt3res6WindowRCQ34nw4r3lyt11ResBlockSet
Window::Window(const res::Window *pBlock, const ResBlockSet &ResBlockSet) {
    const res::WindowContent *pResContent;
    u8 texCoordNum;
    const u32 *matOffsTbl;
    {
        int i;
        const res::Material *pResMaterial;
    }

    const u32 *frameOffsetTable;
    {
        int i;
        const res::WindowFrame *pResWindowFrame;
    }
}

// InitContent__Q34nw4r3lyt6WindowFUc
void Window::InitContent(u8 texNum) {}

// InitFrame__Q34nw4r3lyt6WindowFUc
void Window::InitFrame(u8 frameNum) {}

// __dt__Q44nw4r3lyt6Window7ContentFv

// __dt__Q34nw4r3lyt6WindowFv
Window::~Window() {}

// ReserveTexCoord__Q34nw4r3lyt6WindowFUc
void Window::ReserveTexCoord(u8 num) {}

// FindMaterialByName__Q34nw4r3lyt6WindowFPCcb
Material *Window::FindMaterialByName(const char *findName, bool bRecursive) {
    int i;
    // ut::LinkList<>::Iterator it
    Material *pMat;
}

// FindAnimationLink__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransform
AnimationLink *Window::FindAnimationLink(AnimTransform *pAnimTrans) {
    // TOOD
}

// SetAnimationEnable__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransformbb
void Window::SetAnimationEnable(AnimTransform *pAnimTrans, bool, bool) {
    // TOOD
}

// GetVtxColor__Q34nw4r3lyt6WindowCFUl
ut::Color Window::GetVtxColor(u32 idx) const {}

// SetVtxColor__Q34nw4r3lyt6WindowFUlQ34nw4r2ut5Color
void Window::SetVtxColor(u32, ut::Color) {}

// GetVtxColorElement__Q34nw4r3lyt6WindowCFUl
u8 Window::GetVtxColorElement(u32 idx) const {}

// SetVtxColorElement__Q34nw4r3lyt6WindowFUlUc
void Window::SetVtxColorElement(u32 idx, u8 value) {}

// DrawSelf__Q34nw4r3lyt6WindowFRCQ34nw4r3lyt8DrawInfo
void Window::DrawSelf(const DrawInfo &drawInfo) {
    WindowFrameSize frameSize;
    math::VEC2 bastPt;
}

// AnimateSelf__Q34nw4r3lyt6WindowFUl
void Window::AnimateSelf(u32 option) {
    int i;
}

// UnbindAnimationSelf__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransform
void Window::UnbindAnimationSelf(AnimTransform *pAnimTrans) {
    int i;
}

// DrawContent__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawContent(const math::VEC2 &basePt, const WindowFrameSize &frameSize, u8 alpha) {
    bool bUseVtxCol;
}

// DrawFrame__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawFrame(const math::VEC2 &basePt, const Frame &frame, const WindowFrameSize &frameSize, u8 alpha) {
    bool bUseVtxCol;
    Size texSize;
    ut::Color vtxColors[4];
    detail::TexCoordData texCds[1];
    math::VEC2 polPt;
    Size polSize;
}

// DrawFrame4__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawFrame4(const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, u8 alpha) {
    ut::Color vtxColors[4];
    detail::TexCoordData texCds[1];
    math::VEC2 polPt;
    Size polSize;
    bool bModVtxCol;
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
}

// DrawFrame8__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawFrame8(const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, u8 alpha) {
    ut::Color vtxColors[4];
    detail::TexCoordData texCds[1];
    math::VEC2 polPt;
    Size polSize;
    bool bModVtxCol;
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
    { bool bUseVtxCol; }
}

// GetFrameSize__Q34nw4r3lyt6WindowFUcPCQ44nw4r3lyt6Window5Frame
WindowFrameSize Window::GetFrameSize(u8 frameNum, const Frame *frames) {
    { Size texSize; }
    { Size texSize; }
}

// GetMaterialNum__Q34nw4r3lyt6WindowCFv
u8 Window::GetMaterialNum() const {}

// GetMaterial__Q34nw4r3lyt6WindowCFUl
Material *Window::GetMaterial(u32 idx) const {}

// GetFrameMaterial__Q34nw4r3lyt6WindowCFUl
Material *Window::GetFrameMaterial(u32 frameIdx) const {}

// GetContentMaterial__Q34nw4r3lyt6WindowCFv
Material *Window::GetContentMaterial() const {}

} // namespace lyt

} // namespace nw4r

// HEADER STUFF
// ConvertOffsToPtr<res::WindowFrame>
// ConvertOffsToPtr<res::WindowContent>
// __as__Q34nw4r3lyt13InflationLRTBFRCQ34nw4r3lyt13InflationLRTB

// texCoordNum .sdata
// flipInfos . data
// ut::Min<uc>
