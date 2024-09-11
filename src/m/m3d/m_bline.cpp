#include <egg/core/eggHeap.h>
#include <m/m3d/m_bline.h>

namespace m3d {

blineMat_c::~blineMat_c() {
    remove();
}

bool blineMat_c::create(mAllocator_c *pAllocator, int numLines, u16 numLinePts, f32 width, f32 repeat, mColor &color,
        void *pTex, bool p9) {
    if (!proc_c::create(pAllocator, nullptr)) {
        return false;
    }
    nw4r::ut::List_Init(&mLines, 0);
    if (numLines) {
        mpLineArr = new (pAllocator) bline_c[numLines];

        if (!mpLineArr) {
            return false;
        }
        bline_c *pLineArr = mpLineArr;
        EGG::Heap *pHeap = pAllocator->mHeap;
        for (int i = 0; i < numLines; ++i) {
            if (!pLineArr->create(pHeap, numLinePts, width, repeat, color)) {
                remove();
                return false;
            }
            nw4r::ut::List_Append(&mLines, pLineArr);
            pLineArr++;
        }
    }

    mLineArrNum = numLines;
    mpTex = pTex;
    field_0x4A = p9;

    return true;
}

// void bline_c::bline_c() {}

void blineMat_c::remove() {
    if (mpLineArr) {
        delete[] mpLineArr;
        mpLineArr = nullptr;
    }
    scnLeaf_c::remove();
}

void blineMat_c::update() {}

bline_c *blineMat_c::getLine(u16 idx) {
    return (bline_c *)nw4r::ut::List_GetNth(&mLines, idx);
}

void blineMat_c::setupGX(bool bTransparent) {}

void blineMat_c::drawOpa() {
    update();
    setupGX(false);
    for (bline_c *line = (bline_c *)nw4r::ut::List_GetNext(&mLines, 0); line != nullptr;
            line = (bline_c *)nw4r::ut::List_GetNext(&mLines, line)) {
        if ((line->mFlags & 1) == 0 && line->mColor.a == 0xFF) {
            line->draw();
        }
    }
}

void blineMat_c::drawXlu() {
    update();
    setupGX(true);
    for (bline_c *line = (bline_c *)nw4r::ut::List_GetNext(&mLines, 0); line != nullptr;
            line = (bline_c *)nw4r::ut::List_GetNext(&mLines, line)) {
        if ((line->mFlags & 1) == 0 && line->mColor.a != 0xFF) {
            line->draw();
        }
    }
}

bline_c::~bline_c() {
    remove();
}

bool bline_c::create(EGG::Heap *pHeap, u16 numPts, f32 width, f32 repeat, ::mColor &color) {
    mpPathArr = new (pHeap) mVec3_c[numPts];
    if (!mpPathArr) {
        return false;
    }

    mpVtxPosArr = new (pHeap) VtxPos[numPts];
    if (!mpVtxPosArr) {
        remove();
        return false;
    }

    mpVtxNrmArr = new (pHeap) VtxNrm[numPts];
    if (!mpVtxNrmArr) {
        remove();
        return false;
    }

    mpVtxTexArr = new (pHeap) VtxTex[numPts];
    if (!mpVtxTexArr) {
        remove();
        return false;
    }

    // Color being weird, dont really wanna try rn xD
    mPathNum = numPts;
    mVtxNum = numPts;
    mWidth = width;
    mColor = color;
    mTexRepeat = repeat;
    field_0x2C = 0;

    return true;
}

// void bline_c::VtxPos::VtxPos() {}
// void bline_c::VtxPos::~VtxPos() {}
// void bline_c::VtxNrm::VtxNrm() {}

void bline_c::remove() {
    if (mpPathArr) {
        delete[] mpPathArr;
        mpPathArr = nullptr;
    }
    if (mpVtxPosArr) {
        delete[] mpVtxPosArr;
        mpVtxPosArr = nullptr;
    }
    if (mpVtxNrmArr) {
        delete[] mpVtxNrmArr;
        mpVtxNrmArr = nullptr;
    }
    if (mpVtxTexArr) {
        delete[] mpVtxTexArr;
        mpVtxTexArr = nullptr;
    }
}

void bline_c::update(mVec3_c *startPos) {}

void bline_c::draw() {}

} // namespace m3d
