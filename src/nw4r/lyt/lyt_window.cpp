#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_types.h>
#include <nw4r/lyt/lyt_window.h>

namespace nw4r {
namespace lyt {
// TODO: Find Place
struct TextureFlipInfo {
    u8 coords[4][2]; // at 0x00
    u8 idx[2];       // at 0x8
};

namespace {
// GetTexutreFlipInfo__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FUc
// GetTexutreFlipInfo__24@unnamed@lyt_window_cpp@FUc
// look at TextureFlip enum in lyt_window.h
TextureFlipInfo &GetTextureFlipInfo(u8 textureFlip) {
    static TextureFlipInfo flipInfos[6] = {
            // TopL    TopR    BotL    BotR      idx
            {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {0, 1}}, // Normal
            {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}, {0, 1}}, // Horizontal Flip
            {{{0, 1}, {1, 1}, {0, 0}, {1, 0}}, {0, 1}}, // Vertical Flip
            {{{0, 1}, {0, 0}, {1, 1}, {1, 0}}, {1, 0}}, // Rotate 90
            {{{1, 1}, {0, 1}, {1, 0}, {0, 0}}, {0, 1}}, // Rotate 180
            {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}, {1, 0}}, // Rotate 270
    };
    return flipInfos[textureFlip];
}

/**                                                        +----> +x
 *     LT                         RT                       |
 *      v                         v                        v
 *      +-------+-----------------+-------+  ^            -y
 *      |                ^        |       |  |
 *      |                t        |       |  |   VTX: (x, y)
 *      |                v        |       |  |     base = (x, y)
 *  LB->+-------+-----------------+       +  |     LT = (         x , y         )
 *      |       |                 |       |  |     RT = ( x + w - r , y         )
 *      |       |                 |       |  |     LB = (         x , y - t     )
 *      |<- l ->|                 |<- r ->|  h     RB = (     x + l , y - h + b )
 *      |       |                 |       |  |   SIZE: (w, h)
 *      |       | RB (at +)       |       |  |     LT = ( w - r , t     )
 *      |       | V               |       |  |     RT = (     r , h - b )
 *      +       +-----------------+-------+  |     LB = (     l , h - t )
 *      |       |        ^                |  |     RB = ( w - l , b     )
 *      |       |        b                |  |
 *      |       |        v                |  |
 *      +-------+-----------------+-------+  v
 *      <--------------- w --------------->
 */

// The below functions were started with
// https://github.com/Treeki/LayoutStudio/blob/be8b56a7f0a8f6ba5456a099b61d032fd8aa2f61/layoutgl/widget.cpp#L162 Really
// helped read the awful Ghidra output xD

// GetLTFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetLTFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetLTFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {
    *pPt = basePt;
    // pPt->x = basePt.x;
    // pPt->y = basePt.y;
    pSize->width = windSize.width - frameSize.r;
    pSize->height = frameSize.t;
    // *pSize = Size(windSize.width - frameSize.r, frameSize.t);
}

// GetLTTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetLTTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetLTTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = GetTextureFlipInfo(textureFlip);

    int ix = flipInfo.idx[0];
    int iy = flipInfo.idx[1];
    math::VEC2 tSz(texSize.width, texSize.height);

    // clang-format off
    // Again no temp vars used here
    texCds[TopL][ix] = texCds[BotL][ix] = flipInfo.coords[TopL][ix];
    texCds[TopL][iy] = texCds[TopR][iy] = flipInfo.coords[TopL][iy];

    texCds[BotR][ix] = texCds[TopR][ix] = flipInfo.coords[TopL][ix] +  polSize.width / ((flipInfo.coords[TopR][ix] - flipInfo.coords[TopL][ix]) * tSz[ix]);
    texCds[BotR][iy] = texCds[BotL][iy] = flipInfo.coords[TopL][iy] + polSize.height / ((flipInfo.coords[BotL][iy] - flipInfo.coords[TopL][iy]) * tSz[iy]);
    // clang-format on
}

// GetRTFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetRTFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetRTFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {
    *pPt = math::VEC2(basePt.x + windSize.width - frameSize.r, basePt.y);
    // pPt->x = basePt.x + windSize.width - frameSize.r;
    // pPt->y = basePt.y
    // *pSize = Size(frameSize.r, windSize.height - frameSize.b);
    pSize->width = frameSize.r;
    pSize->height = windSize.height - frameSize.b;
}

// GetRTTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetRTTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetRTTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = GetTextureFlipInfo(textureFlip);
    int ix = flipInfo.idx[0];
    int iy = flipInfo.idx[1];

    math::VEC2 tSz(texSize.width, texSize.height);
    // clang-format off
    // Again no temp vars used here
    texCds[TopR][ix] = texCds[BotR][ix] = flipInfo.coords[TopR][ix];
    texCds[TopR][iy] = texCds[TopL][iy] = flipInfo.coords[TopR][iy];

    texCds[BotL][ix] = texCds[TopL][ix] = flipInfo.coords[TopR][ix] +  polSize.width / ((flipInfo.coords[TopL][ix] - flipInfo.coords[TopR][ix]) * tSz[ix]);
    texCds[BotL][iy] = texCds[BotR][iy] = flipInfo.coords[TopR][iy] + polSize.height / ((flipInfo.coords[BotR][iy] - flipInfo.coords[TopR][iy]) * tSz[iy]);
    // clang-format on
}

// GetLBFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetLBFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetLBFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {
    *pPt = math::VEC2(basePt.x, basePt.y - frameSize.t);
    // pPt->x = basePt.x;
    // pPt->y = basePt.y - frameSize.t;

    // *pSize = Size(frameSize.l, windSize.height - frameSize.t);
    pSize->width = frameSize.l;
    pSize->height = windSize.height - frameSize.t;
}

// GetLBTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetLBTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetLBTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = GetTextureFlipInfo(textureFlip);
    int ix = flipInfo.idx[0];
    int iy = flipInfo.idx[1];

    math::VEC2 tSz(texSize.width, texSize.height);
    // clang-format off
    // Again no temp vars used here
    texCds[BotL][ix] = texCds[TopL][ix] = flipInfo.coords[BotL][ix];
    texCds[BotL][iy] = texCds[BotR][iy] = flipInfo.coords[BotL][iy];

    texCds[TopR][ix] = texCds[BotR][ix] = flipInfo.coords[BotL][ix] +  polSize.width /  ((flipInfo.coords[BotR][ix] - flipInfo.coords[BotL][ix]) * tSz[ix]);
    texCds[TopR][iy] = texCds[TopL][iy] = flipInfo.coords[BotL][iy] + polSize.height /  ((flipInfo.coords[TopL][iy] - flipInfo.coords[BotL][iy]) * tSz[iy]);

    // clang-format on
}

// GetRBFrameSize__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
// GetRBFrameSize__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2PQ34nw4r3lyt4SizeRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt15WindowFrameSize
void GetRBFrameSize(math::VEC2 *pPt, Size *pSize, const math::VEC2 &basePt, const Size &windSize,
        const WindowFrameSize &frameSize) {
    *pPt = math::VEC2(basePt.x + frameSize.l, basePt.y - windSize.height + frameSize.b);
    // pPt->x = basePt.x + frameSize.l;
    // pPt->y = basePt.y - windSize.height + frameSize.b;
    // *pSize = Size(windSize.width - frameSize.l, frameSize.b);
    pSize->width = windSize.width - frameSize.l;
    pSize->height = frameSize.b;
}

// GetRBTexCoord__Q34nw4r3lyt24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
// GetRBTexCoord__24@unnamed@lyt_window_cpp@FPQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeRCQ34nw4r3lyt4SizeUc
void GetRBTexCoord(math::VEC2 *texCds, const Size &polSize, const Size &texSize, u8 textureFlip) {
    const TextureFlipInfo &flipInfo = GetTextureFlipInfo(textureFlip);
    int ix = flipInfo.idx[0];
    int iy = flipInfo.idx[1];
    math::VEC2 tSz(texSize.width, texSize.height);

    // clang-format off
    texCds[BotR][ix] = texCds[TopR][ix] = flipInfo.coords[BotR][ix];
    texCds[BotR][iy] = texCds[BotL][iy] = flipInfo.coords[BotR][iy];

    texCds[TopL][ix] = texCds[BotL][ix] = flipInfo.coords[BotR][ix] +  polSize.width / ((flipInfo.coords[BotL][ix] - flipInfo.coords[BotR][ix]) * tSz[ix]);
    texCds[TopL][iy] = texCds[TopR][iy] = flipInfo.coords[BotR][iy] + polSize.height / ((flipInfo.coords[TopR][iy] - flipInfo.coords[BotR][iy]) * tSz[iy]);
    // clang-format on
}
} // namespace

NW4R_UT_RTTI_DEF_DERIVED(Window, Pane);

// __dt__Q44nw4r3lyt6Window5FrameFv

// __ct__Q34nw4r3lyt6WindowFPCQ44nw4r3lyt3res6WindowRCQ34nw4r3lyt11ResBlockSet
Window::Window(const res::Window *pBlock, const ResBlockSet &resBlockSet) : Pane(pBlock), mContent() {
    const res::WindowContent *pResContent =
            detail::ConvertOffsToPtr<const res::WindowContent>(pBlock, pBlock->contentOffset);

    u8 texCoordNum = ut::Min<u8>(pResContent->texCoordNum, 8);

    InitContent(texCoordNum);

    const u32 *matOffsTbl = detail::ConvertOffsToPtr<u32>(resBlockSet.pMaterialList, sizeof(res::MaterialList));

    mContentInflation = pBlock->inflation;
    for (int i = 0; i < TEXCOORD_VTX_COUNT; i++) {
        mContent.vtxColors[i] = pResContent->vtxCols[i];
    }

    if (texCoordNum && !mContent.texCoordAry.IsEmpty()) {
        mContent.texCoordAry.Copy(&pResContent[1], texCoordNum);
    }

    const res::Material *pResMaterial =
            detail::ConvertOffsToPtr<res::Material>(resBlockSet.pMaterialList, matOffsTbl[pResContent->materialIdx]);
    mpMaterial = Layout::NewObj<Material>(pResMaterial, resBlockSet);

    mFrameNum = 0;
    mFrames = nullptr;
    if (pBlock->frameNum) {
        InitFrame(pBlock->frameNum);
        const u32 *frameOffsetTable = detail::ConvertOffsToPtr<u32>(pBlock, pBlock->frameOffsetTableOffset);
        for (int i = 0; i < mFrameNum; i++) {
            const res::WindowFrame *pResWindowFrame =
                    detail::ConvertOffsToPtr<res::WindowFrame>(pBlock, frameOffsetTable[i]);
            mFrames[i].textureFlip = pResWindowFrame->textureFlip;
            const res::Material *pResMaterial = detail::ConvertOffsToPtr<res::Material>(resBlockSet.pMaterialList,
                    matOffsTbl[pResWindowFrame->materialIdx]);
            mFrames[i].pMaterial = Layout::NewObj<Material>(pResMaterial, resBlockSet);
        }
    }
}

// InitContent__Q34nw4r3lyt6WindowFUc
void Window::InitContent(u8 texNum) {
    // Guess based of of usual Init Patterns
    // not needed to match ¯\_(ツ)_/¯
    if (texNum) {
        ReserveTexCoord(texNum);
    }
}

// InitFrame__Q34nw4r3lyt6WindowFUc
void Window::InitFrame(u8 frameNum) {
    mFrameNum = 0;
    mFrames = Layout::NewArray<Frame>(frameNum);
    if (mFrames) {
        mFrameNum = frameNum;
    }
}

// __dt__Q44nw4r3lyt6Window7ContentFv

// __dt__Q34nw4r3lyt6WindowFv
Window::~Window() {
    Layout::DeleteArray(mFrames, mFrameNum);
    if (mpMaterial && !mpMaterial->IsUserAllocated()) {
        Layout::DeleteObj(mpMaterial);
        mpMaterial = nullptr;
    }
    mContent.texCoordAry.Free();
}

// ReserveTexCoord__Q34nw4r3lyt6WindowFUc
void Window::ReserveTexCoord(u8 num) {
    // Inline Guess
    mContent.texCoordAry.Reserve(num);
}

// FindMaterialByName__Q34nw4r3lyt6WindowFPCcb
Material *Window::FindMaterialByName(const char *findName, bool bRecursive) {
    if (mpMaterial && detail::EqualsMaterialName(mpMaterial->GetName(), findName)) {
        return mpMaterial;
    }

    for (int i = 0; i < mFrameNum; i++) {
        if (detail::EqualsMaterialName(mFrames[i].pMaterial->GetName(), findName)) {
            return mFrames[i].pMaterial;
        }
    }

    if (bRecursive) {
        for (ut::LinkList<Pane, 4>::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); it++) {
            Material *pMat = it->FindMaterialByName(findName, bRecursive);
            if (pMat) {
                return pMat;
            }
        }
    }
    return nullptr;
}

// FindAnimationLink__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransform
// AnimationLink *Window::FindAnimationLink(AnimTransform *pAnimTrans) {
//     // Not In SS - Part of Vtable in Pane
// }

// SetAnimationEnable__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransformbb
// void Window::SetAnimationEnable(AnimTransform *pAnimTrans, bool, bool) {
//     // Not In SS - Part of Vtable in Pane
// }

// GetVtxColor__Q34nw4r3lyt6WindowCFUl
ut::Color Window::GetVtxColor(u32 idx) const {
    return mContent.vtxColors[idx];
}

// SetVtxColor__Q34nw4r3lyt6WindowFUlQ34nw4r2ut5Color
void Window::SetVtxColor(u32 idx, ut::Color value) {
    mContent.vtxColors[idx] = value;
}

// GetVtxColorElement__Q34nw4r3lyt6WindowCFUl
u8 Window::GetVtxColorElement(u32 idx) const {
    return detail::GetVtxColorElement(mContent.vtxColors, idx);
}

// SetVtxColorElement__Q34nw4r3lyt6WindowFUlUc
void Window::SetVtxColorElement(u32 idx, u8 value) {
    detail::SetVtxColElement(mContent.vtxColors, idx, value);
}

// DrawSelf__Q34nw4r3lyt6WindowFRCQ34nw4r3lyt8DrawInfo
void Window::DrawSelf(const DrawInfo &drawInfo) {
    LoadMtx(drawInfo);
    WindowFrameSize frameSize = GetFrameSize(mFrameNum, mFrames);
    math::VEC2 basePt = GetVtxPos();
    DrawContent(basePt, frameSize, mGlbAlpha);
    switch (mFrameNum) {
    case 1:
        DrawFrame(basePt, mFrames[0], frameSize, mGlbAlpha);
        break;
    case 4:
        DrawFrame4(basePt, mFrames, frameSize, mGlbAlpha);
        break;
    case 8:
        DrawFrame8(basePt, mFrames, frameSize, mGlbAlpha);
        break;
    }
}

// AnimateSelf__Q34nw4r3lyt6WindowFUl
void Window::AnimateSelf(u32 option) {
    Pane::AnimateSelf(option);
    if (IsVisible() || !(option & 1)) {
        for (int i = 0; i < mFrameNum; i++) {
            mFrames[i].pMaterial->Animate();
        }
    }
}

// UnbindAnimationSelf__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransform
void Window::UnbindAnimationSelf(AnimTransform *pAnimTrans) {
    for (int i = 0; i < mFrameNum; i++) {
        mFrames[i].pMaterial->UnbindAnimation(pAnimTrans);
    }
    Pane::UnbindAnimationSelf(pAnimTrans);
}

// DrawContent__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawContent(const math::VEC2 &basePt, const WindowFrameSize &frameSize, u8 alpha) {
    bool bUseVtxCol = detail::IsModulateVertexColor(mContent.vtxColors, alpha);
    bUseVtxCol = mpMaterial->SetupGX(bUseVtxCol, alpha);
    detail::SetVertexFormat(bUseVtxCol, mContent.texCoordAry.GetSize());

    // Why not use temps wtf. only bUseVtxCol is defined in DWARF and I cant find evidence of an inline
    detail::DrawQuad(
            math::VEC2(basePt.x + frameSize.l - mContentInflation.l, basePt.y - frameSize.t + mContentInflation.t),
            Size(mSize.width - frameSize.l + mContentInflation.l - frameSize.r + mContentInflation.r,
                    mSize.height - frameSize.t + mContentInflation.t - frameSize.b + mContentInflation.b),
            mContent.texCoordAry.GetSize(), mContent.texCoordAry.GetArray(), bUseVtxCol ? mContent.vtxColors : nullptr,
            alpha);
}

// DrawFrame__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawFrame(const math::VEC2 &basePt, const Frame &frame, const WindowFrameSize &frameSize, u8 alpha) {
    if (frame.pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = detail::IsModulateVertexColor(nullptr, alpha);
        bUseVtxCol = frame.pMaterial->SetupGX(bUseVtxCol, alpha);

        detail::SetVertexFormat(bUseVtxCol, 1);
        Size texSize = detail::GetTextureSize(frame.pMaterial, 0);

        ut::Color vtxColors[4];
        detail::TexCoordData texCds[1];
        math::VEC2 polPt;
        Size polSize;

        // Splits Frame into 4 Corners (repeats frame for each part (texture is quartered))
        // Not sure if just edges or Quadrants
        // (Top Left Quadrant)
        GetLTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetLTTexCoord(texCds[0], polSize, texSize, TEXFLIP_NONE);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);

        // (Top Right Quadrant)
        GetRTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetRTTexCoord(texCds[0], polSize, texSize, TEXFLIP_HORIZONTAL);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);

        // (Bottom Right Quadrant)
        GetRBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetRBTexCoord(texCds[0], polSize, texSize, TEXFLIP_ROTATE_180);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);

        // (Bottom Left Quadrant)
        GetLBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetLBTexCoord(texCds[0], polSize, texSize, TEXFLIP_VERTICAL);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }
}

// DrawFrame4__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawFrame4(const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, u8 alpha) {
    ut::Color vtxColors[4];
    detail::TexCoordData texCds[1];
    math::VEC2 polPt;
    Size polSize;

    bool bModVtxCol = detail::IsModulateVertexColor(nullptr, alpha);
    if (frame[0].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[0].pMaterial->SetupGX(bModVtxCol, alpha);
        GetLTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetLTTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[0].pMaterial, 0), frame[0].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }

    if (frame[1].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[1].pMaterial->SetupGX(bModVtxCol, alpha);
        GetRTFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetRTTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[1].pMaterial, 0), frame[1].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }

    if (frame[3].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[3].pMaterial->SetupGX(bModVtxCol, alpha);
        GetRBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetRBTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[3].pMaterial, 0), frame[3].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }

    if (frame[2].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[2].pMaterial->SetupGX(bModVtxCol, alpha);
        GetLBFrameSize(&polPt, &polSize, basePt, mSize, frameSize);
        GetLBTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[2].pMaterial, 0), frame[2].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }
}

/**                                                        +----> +x
 *     LT       T                 RT                       |
 *      v       v                 v                        v
 *      +-------+-----------------+-------+  ^            -y
 *      |       |        ^        |       |  |   VTX: (x, y)
 *      |       |        t        |       |  |     base = (x, y)
 *      |       |        v        |       |  |     LT = (         x , y         ) | L = (         x , y - t     )
 *   L->+-------+-----------------+-------+  |     RT = ( x + w - r , y         ) | R = ( x + w - r , y - t     )
 *      |       |                ^|       |  |     LB = ( y - h + b , y - h + b ) | T = (     x + l , y         )
 *      |       |                R|       |  |     RB = ( x + w - r , y - h + b ) | B = (     x + l , y - h + b )
 *      |<- l ->|                 |<- r ->|  h
 *      |       |                 |       |  |   SIZE: (w, h)
 *      |       |B              RB|       |  |     LT = ( l , t ) | L = (         l , h - t - b )
 *      |       |V               V|       |  |     RT = ( r , t ) | R = (         r , h - t - b )
 *  LB->+-------+-----------------+-------+  |     LB = ( l , b ) | T = ( w - l - r , t         )
 *      |       |        ^        |       |  |     RB = ( r , b ) | B = ( w - l - r , b         )
 *      |       |        b        |       |  |
 *      |       |        v        |       |  |
 *      +-------+-----------------+-------+  v
 *      <--------------- w --------------->
 */

// DrawFrame8__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc
void Window::DrawFrame8(const math::VEC2 &basePt, const Frame *frame, const WindowFrameSize &frameSize, u8 alpha) {
    ut::Color vtxColors[4];
    detail::TexCoordData texCds[1];
    Size polSize;

    bool bModVtxCol = detail::IsModulateVertexColor(nullptr, alpha);
    // TOP LEFT
    if (frame[0].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[0].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(frameSize.l, frameSize.t);
        GetLTTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[0].pMaterial, 0), frame[0].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(basePt, polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // TOP
    if (frame[6].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[6].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(mSize.width - frameSize.l - frameSize.r, frameSize.t);
        GetLTTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[6].pMaterial, 0), frame[6].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x + frameSize.l, basePt.y), polSize, 1, texCds,
                bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // TOP RIGHT
    if (frame[1].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[1].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(frameSize.r, frameSize.t);
        GetRTTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[1].pMaterial, 0), frame[1].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x + mSize.width - frameSize.r, basePt.y), polSize, 1, texCds,
                bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // RIGHT
    if (frame[5].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[5].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(frameSize.r, mSize.height - frameSize.t - frameSize.b);
        GetRTTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[5].pMaterial, 0), frame[5].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x + mSize.width - frameSize.r, basePt.y - frameSize.t), polSize, 1, texCds,
                bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // BOTTOM RIGHT
    if (frame[3].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[3].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(frameSize.r, frameSize.b);
        GetRBTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[3].pMaterial, 0), frame[3].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x + mSize.width - frameSize.r, basePt.y - mSize.height + frameSize.b),
                polSize, 1, texCds, bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // BOTTOM
    if (frame[7].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[7].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(mSize.width - frameSize.l - frameSize.r, frameSize.b);
        GetRBTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[7].pMaterial, 0), frame[7].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x + frameSize.l, basePt.y - mSize.height + frameSize.b), polSize, 1, texCds,
                bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // BOTTOM LEFT
    if (frame[2].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[2].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(frameSize.l, frameSize.b);
        GetLBTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[2].pMaterial, 0), frame[2].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x, basePt.y - mSize.height + frameSize.b), polSize, 1, texCds,
                bUseVtxCol ? vtxColors : nullptr, alpha);
    }
    // LEFT
    if (frame[4].pMaterial->GetTextureNum() != 0) {
        bool bUseVtxCol = frame[4].pMaterial->SetupGX(bModVtxCol, alpha);
        polSize = Size(frameSize.l, mSize.height - frameSize.t - frameSize.b);
        GetLBTexCoord(texCds[0], polSize, detail::GetTextureSize(frame[4].pMaterial, 0), frame[4].textureFlip);
        detail::SetVertexFormat(bUseVtxCol, 1);
        detail::DrawQuad(math::VEC2(basePt.x, basePt.y - frameSize.t), polSize, 1, texCds,
                bUseVtxCol ? vtxColors : nullptr, alpha);
    }
}

// GetFrameSize__Q34nw4r3lyt6WindowFUcPCQ44nw4r3lyt6Window5Frame
WindowFrameSize Window::GetFrameSize(u8 frameNum, const Frame *frames) {
    WindowFrameSize ret = {0.0f, 0.0f, 0.0f, 0.0f};
    switch (frameNum) {
    case 1: {
        Size texSize = detail::GetTextureSize(frames[0].pMaterial, 0);
        ret.l = texSize.width;
        ret.t = texSize.height;
        ret.r = texSize.width;
        ret.b = texSize.height;
    } break;
    case 4:
    case 8: {
        Size texSize = detail::GetTextureSize(frames[0].pMaterial, 0);
        ret.l = texSize.width;
        ret.t = texSize.height;
        texSize = detail::GetTextureSize(frames[3].pMaterial, 0);
        ret.r = texSize.width;
        ret.b = texSize.height;
    } break;
    }
    return ret;
}

// GetMaterialNum__Q34nw4r3lyt6WindowCFv
u8 Window::GetMaterialNum() const {
    return mFrameNum + 1;
}

// GetMaterial__Q34nw4r3lyt6WindowCFUl
Material *Window::GetMaterial(u32 idx) const {
    if (idx == 0) {
        return GetContentMaterial();
    } else {
        return GetFrameMaterial(idx - 1);
    }
}

// GetFrameMaterial__Q34nw4r3lyt6WindowCFUl
Material *Window::GetFrameMaterial(u32 frameIdx) const {
    if (frameIdx >= mFrameNum) {
        return nullptr;
    }
    return mFrames[frameIdx].pMaterial;
}

// GetContentMaterial__Q34nw4r3lyt6WindowCFv
Material *Window::GetContentMaterial() const {
    return reinterpret_cast<const Pane *>(this)->GetMaterial();
}

} // namespace lyt

} // namespace nw4r

// HEADER STUFF
// ConvertOffsToPtr<res::WindowFrame>
// ConvertOffsToPtr<res::WindowContent>
// __as__Q34nw4r3lyt13InflationLRTBFRCQ34nw4r3lyt13InflationLRTB

// texCoordNum .sdata
// flipInfos . data
// ut::Min<uc>
