#include <d/d_font_manager.h>
#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_meter.h>
#include <d/lyt/d_textbox.h>
#include <d/lyt/d_window.h>
#include <egg/gfx/eggScreen.h>
#include <nw4r/lyt/lyt_bounding.h>
#include <nw4r/lyt/lyt_group.h>
#include <nw4r/lyt/lyt_picture.h>
#include <nw4r/lyt/lyt_textBox.h>
#include <nw4r/lyt/lyt_utils.h>
#include <nw4r/lyt/lyt_window.h>
#include <sized_string.h>

using namespace nw4r::lyt;

namespace d2d {

void ResAccIf_c::detach() {
    for (u8 i = 0; i < 5; i++) {
        mAccessor.UnregistFont(&mFontRefLinks[i]);
    }
    m2d::ResAccIf_c::detach();
}

bool ResAccIf_c::attach(void *data, const char *name) {
    nw4r::lyt::FontRefLink *link;
    bool ok = m2d::ResAccIf_c::attach(data, name);
    if (ok) {
        for (u8 i = 0; i < 5; i++) {
            link = &mFontRefLinks[i];
            link->Set(dFontMng_c::getFontName(i), dFontMng_c::getFont(i));
            mAccessor.RegistFont(link);
        }
    }

    return ok;
}

// Copied from lyt, with some types replaced
nw4r::lyt::Pane *Layout_c::BuildPaneObj(s32 kind, const void *dataPtr, const ResBlockSet &resBlockSet) {
    // Oddly enough the breaks are required here to not inline the function???
    // Had them as left over from editing and somehow when removing them it just broke Build.
    // Probably some analysis depth as 3 break statements do it
    switch (kind) {
    case 'pan1': {
        const res::Pane *pResPane = (const res::Pane *)dataPtr;
        return NewObj<Pane>(pResPane);
    } break;
    case 'pic1': {
        const res::Picture *pResPic = (const res::Picture *)dataPtr;
        return NewObj<Picture>(pResPic, resBlockSet);
    } break;
    case 'txt1': {
        const res::TextBox *pBlock = (const res::TextBox *)dataPtr;
        return NewObj<dTextBox_c>(pBlock, resBlockSet);
    } break;
    case 'wnd1': {
        const res::Window *pBlock = (const res::Window *)dataPtr;
        return NewObj<dWindow_c>(pBlock, resBlockSet);
    } break;
    case 'bnd1': {
        const res::Bounding *pResBounding = (const res::Bounding *)dataPtr;
        return NewObj<Bounding>(pResBounding, resBlockSet);
    } break;
    default:
        return nullptr;
    }
}

// This is exactly copied from nw4r::lyt::Layout
bool Layout_c::Build(const void *lytResBuf, ResourceAccessor *pResAcsr) {
    const res::BinaryFileHeader *pFileHead = (const res::BinaryFileHeader *)(lytResBuf);
    if (!detail::TestFileHeader(*pFileHead, 'RLYT')) {
        return false;
    }
    if (!detail::TestFileVersion(*pFileHead)) {
        return false;
    }

    ResBlockSet resBlockSet = {nullptr, nullptr, nullptr, pResAcsr};
    Pane *pParentPane = nullptr;
    Pane *pLastPane = nullptr;
    bool bReadRootGroup = false;
    int groupNestLevel = 0;

    const void *dataPtr = ((u8 *)lytResBuf + pFileHead->headerSize);
    for (int i = 0; i < pFileHead->dataBlocks; i++) {
        const res::DataBlockHeader *pDataBlockHead = (const res::DataBlockHeader *)dataPtr;
        switch (detail::GetSignatureInt(pDataBlockHead->kind)) {
        case 'lyt1': // Main Layout
        {
            const res::Layout *pResLyt = ((const res::Layout *)dataPtr);
            mLayoutSize = pResLyt->layoutSize;
        } break;
        case 'txl1': // Texture List
            resBlockSet.pTextureList = (const res::TextureList *)dataPtr;
            break;
        case 'fnl1': // Font List
            resBlockSet.pFontList = (const res::FontList *)dataPtr;
            break;
        case 'mat1': // Material
            resBlockSet.pMaterialList = (const res::MaterialList *)dataPtr;
            break;
        case 'wnd1': // Window
        case 'pan1': // Pane
        case 'pic1': // Picture
        case 'txt1': // Text Box
        case 'bnd1': // Boundary Pane
        {
            Pane *pPane = BuildPaneObj(detail::GetSignatureInt(pDataBlockHead->kind), dataPtr, resBlockSet);
            if (pPane) {
                if (mpRootPane == nullptr) {
                    mpRootPane = pPane;
                }
                if (pParentPane) {
                    pParentPane->AppendChild(pPane);
                }
                pLastPane = pPane;
            }
        } break;
        case 'usd1': // User Data
            pLastPane->SetExtUserDataList((const res::ExtUserDataList *)dataPtr);
            break;
        case 'pas1': // PaneChildren Start
            pParentPane = pLastPane;
            break;
        case 'pae1': // PaneChildren End
            pLastPane = pParentPane;
            pParentPane = pLastPane->GetParent();
            break;
        case 'grp1': // Group
            if (!bReadRootGroup) {
                bReadRootGroup = true;
                mpGroupContainer = NewObj<GroupContainer>();
            } else {
                if (mpGroupContainer && groupNestLevel == 1) {
                    Group *pGroup = NewObj<Group>((const res::Group *)dataPtr, mpRootPane);
                    if (pGroup) {
                        mpGroupContainer->AppendGroup(pGroup);
                    }
                }
            }
            break;
        case 'grs1': // Group Children Start
            groupNestLevel++;
            break;
        case 'gre1': // Group Children End
            groupNestLevel--;
            break;
        default:
            break;
        }
        dataPtr = ((u8 *)dataPtr + pDataBlockHead->size);
    }
    return true;
}

nw4r::lyt::AnimTransform *Layout_c::CreateAnimTransform(const void *animResBuf, nw4r::lyt::ResourceAccessor *pResAcsr) {
    nw4r::lyt::AnimTransform *transform = nw4r::lyt::Layout::CreateAnimTransform(animResBuf, pResAcsr);
    if (transform != nullptr) {
        mAnimTransList.Erase(transform);
    }
    return transform;
}

Multi_c::Multi_c() : Base_c(0x80), mLayout(), mDrawInfo(), mpResAcc(nullptr), mFlags(0) {
    mDrawInfo.SetLocationAdjustScale(nw4r::math::VEC2((f32)EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_1) /
                    (f32)EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_2),
            1.0f));
    mDrawInfo.SetLocationAdjust(true);
}

void Multi_c::animate() {
    u32 arg = 0;
    if ((mFlags & 1) != 0) {
        arg = 1;
    }
    mLayout.Animate(arg);
}

void Multi_c::calc() {
    calcBefore();
    calcAfter();
}

void Multi_c::calcBefore() {
    u32 arg = 0;
    if ((mFlags & 1) != 0) {
        arg = 1;
    }
    mLayout.Animate(arg);
}

void Multi_c::calcAfter() {
    nw4r::ut::Rect rect = mLayout.GetLayoutRect();
    mDrawInfo.SetViewRect(rect);
    mDrawInfo.SetMultipleViewMtxOnDraw(true);
    mLayout.CalculateMtx(mDrawInfo);
}

extern "C" bool NeedsScreenAdjustment();

// Largerly copied from m2d::Simple_c::draw
void Multi_c::draw() {
    nw4r::ut::Rect r = mLayout.GetLayoutRect();
    f32 near = 0.0f;
    f32 far = 500.0f;
    EGG::Screen s;
    bool needsAdjust = NeedsScreenAdjustment();
    f32 f1 = EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_2);
    f32 f2 = EGG::Screen::GetSizeXMax(EGG::Screen::TV_MODE_1);

    f32 left = needsAdjust ? f1 * r.left / f2 : r.left;
    f32 right = needsAdjust ? f1 * r.right / f2 : r.right;
    s.SetProjectionType(EGG::Frustum::PROJ_ORTHO);
    s.ResetOrthographic(r.top, r.bottom, left, right, near, far);
    if (needsAdjust) {
        s.SetScale(nw4r::math::VEC3(f2 / f1, 1.0f, 1.0f));
    }
    s.SetProjectionGX();
    mDrawInfo.SetMultipleViewMtxOnDraw(false);
    mLayout.Draw(mDrawInfo);
}

bool Multi_c::build(const char *name, m2d::ResAccIf_c *acc) {
    if (mLayout.GetRootPane() != nullptr) {
        return true;
    }

    if (acc == nullptr) {
        acc = mpResAcc;
        if (acc == nullptr) {
            return false;
        }
    }

    void *res = acc->getAccessor()->GetResource(0, name, nullptr);
    if (res == nullptr) {
        return false;
    }

    bool ok = mLayout.Build(res, acc->getAccessor());
    if (ok) {
        calc();
    }
    return ok;
}

nw4r::lyt::Pane *Multi_c::findPane(const char *name) const {
    return mLayout.GetRootPane()->FindPaneByName(name, true);
}

nw4r::lyt::Picture *Multi_c::findPicture(const char *name) const {
    nw4r::lyt::Pane *p = findPane(name);
    if (p != nullptr) {
        return nw4r::ut::DynamicCast<nw4r::lyt::Picture *>(p);
    }
    return nullptr;
}

nw4r::lyt::Bounding *Multi_c::findBounding(const char *name) const {
    nw4r::lyt::Pane *p = findPane(name);
    if (p != nullptr) {
        return nw4r::ut::DynamicCast<nw4r::lyt::Bounding *>(p);
    }
    return nullptr;
}

void Multi_c::unbindAnims() {
    mLayout.UnbindAllAnimation();
}

LytBase_c::LytBase_c() : mpMsbtInfo(nullptr) {}

LytBase_c::~LytBase_c() {
    if (mpMsbtInfo != nullptr) {
        LMS_CloseMessage(mpMsbtInfo);
    }
}

extern "C" const char *getUsedLanguageString();
extern "C" nw4r::ut::TagProcessorBase<wchar_t> *GLOBAL_TEXT_MANAGER;

bool LytBase_c::build(const char *name, m2d::ResAccIf_c *acc) {
    if (mLayout.GetRootPane() != nullptr) {
        return true;
    }

    mFlags |= 0x1;

    if (acc == nullptr) {
        acc = mpResAcc;
        if (acc == nullptr) {
            return false;
        }
    }

    void *resource = acc->getAccessor()->GetResource(0, name, nullptr);
    if (resource == nullptr) {
        return false;
    }

    // Copy the name, and once we reach a '.' append 'msbt'
    char fileName[0x32];
    int suffixIdx = 0;
    for (int i = 0; i < 0x32; i++) {
        if (suffixIdx == 0) {
            if (name[i] == '.') {
                suffixIdx = 1;
            }
            fileName[i] = name[i];
        } else if (suffixIdx == 1) {
            fileName[i] = 'm';
            suffixIdx = 2;
        } else if (suffixIdx == 2) {
            fileName[i] = 's';
            suffixIdx = 3;
        } else if (suffixIdx == 3) {
            fileName[i] = 'b';
            suffixIdx = 4;
        } else if (suffixIdx == 4) {
            fileName[i] = 't';
            suffixIdx = 5;
        } else {
            fileName[i] = '\0';
            suffixIdx = 6;
        }
    }

    if (suffixIdx < 6) {
        return false;
    }

    SizedString<0x80> localizedName;
    localizedName.sprintf("%s_%s", getUsedLanguageString(), fileName);

    void *data = acc->getAccessor()->GetResource(0, &localizedName, nullptr);
    if (data == nullptr) {
        mpMsbtInfo = nullptr;
    } else {
        mpMsbtInfo = LMS_InitMessage(data);
    }

    bool ok = mLayout.Build(resource, acc->getAccessor());
    if (ok) {
        mLayout.SetTagProcessor(GLOBAL_TEXT_MANAGER);
        calc();
        setPropertiesRecursive(mLayout.GetRootPane(), -9999.0f, -9999.0f, -9999.0f, -9999.0f, -9999.0f);
    }

    return ok;
}

dTextBox_c *LytBase_c::getTextBox(const char *name) {
    nw4r::lyt::Pane *pane = findPane(name);
    if (pane != nullptr) {
        nw4r::ut::DynamicCast<dTextBox_c *>(pane)->setLytBase(this);
        return nw4r::ut::DynamicCast<dTextBox_c *>(pane);
    }

    return nullptr;
}

dWindow_c *LytBase_c::getWindow(const char *name) {
    nw4r::lyt::Pane *pane = findPane(name);
    if (pane != nullptr) {
        return nw4r::ut::DynamicCast<dWindow_c *>(pane);
    }

    return nullptr;
}

dTextBox_c *LytBase_c::getSizeBoxInWindow(const char *windowName) {
    nw4r::lyt::Pane *pane = findPane(windowName);
    if (pane != nullptr) {
        dWindow_c *window = nw4r::ut::DynamicCast<dWindow_c *>(pane);
        return getTextBoxViaUserData(window, "size");
    }
    return nullptr;
}

dTextBox_c *LytBase_c::getTextBoxViaUserData(nw4r::lyt::Pane *pane, const char *name) {
    u16 num = pane->GetExtUserDataNum();
    const nw4r::lyt::res::ExtUserData *dat;
    dTextBox_c *textBox;
    if (num == 0) {
        goto fail;
    }

    dat = pane->FindExtUserDataByName(name);
    if (dat == nullptr) {
        goto fail;
    }

    if (dat->GetType() != nw4r::lyt::res::TYPE_STRING) {
        goto fail;
    }

    textBox = getTextBox(dat->GetString());
    if (textBox == nullptr) {
        goto fail;
    }

    return textBox;

fail:
    return nullptr;
}

void LytBase_c::setPropertiesRecursive(nw4r::lyt::Pane *pane, f32 posX, f32 posY, f32 scale, f32 spaceX,
        f32 spaceY) {
    u16 num = pane->GetExtUserDataNum();
    if (num != 0) {
        const nw4r::lyt::res::ExtUserData *list = pane->GetExtUserData();
        for (int i = 0; i < num; i++) {
            SizedString<0x40> userDatName;
            userDatName = list->GetName();
            if (userDatName == "c11n-posX-1" && list->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
                posX = list->GetFloat();
            }
            if (userDatName == "c11n-posY-1" && list->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
                posY = list->GetFloat();
            }
            if (userDatName == "c11n-scale-1" && list->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
                scale = list->GetFloat();
            }
            if (userDatName == "c11n-spaceX-1" && list->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
                spaceX = list->GetFloat();
            }
            if (userDatName == "c11n-spaceY-1" && list->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
                spaceY = list->GetFloat();
            }

            list++;
        }
    }

    setProperties(pane, posX, posY, scale, spaceX, spaceY);
    for (nw4r::ut::LinkList<Pane, 4>::Iterator it = pane->GetChildList()->GetBeginIter();
            it != pane->GetChildList()->GetEndIter(); ++it) {
        setPropertiesRecursive(&*it, posX, posY, scale, spaceX, spaceY);
    }
}

extern "C" const char *fn_801B2600(const char *);
extern "C" void fn_800AF930(dTextBox_c *, const char *);
extern "C" void fn_800B0F40(dTextBox_c *);

// NOTE: This function uses `textBox->GetTranslateX1()` as a general placeholder to figure
// out the details first. In reality these are all different functions!
void LytBase_c::setProperties(nw4r::lyt::Pane *pane, f32 posX, f32 posY, f32 scale, f32 spaceX, f32 spaceY) {
    if (pane->GetName()[0] != 'T') {
        return;
    }

    dTextBox_c *textBox = nw4r::ut::DynamicCast<dTextBox_c *>(pane);
    if (textBox == nullptr) {
        return;
    }

    textBox->AllocStringBuffer(0x200);
    textBox->setLytBase(this);
    fn_800B0F40(textBox);

    if (posX != -9999.0f || posY != -9999.0f) {
        if (posX == -9999.0f) {
            posX = 0.0f;
        }
        if (posY == -9999.0f) {
            posY = 0.0f;
        }
        nw4r::math::VEC3 t1 = textBox->GetTranslate();
        t1.x += posX;
        t1.y += posY;
        textBox->SetTranslate(t1);
    }

    nw4r::math::VEC3 t2 = textBox->GetTranslate();
    t2.x += textBox->GetTranslateX1();
    t2.y += textBox->GetTranslateX1();
    textBox->SetTranslate(t2);


    if (scale != -9999.0f) {
        const nw4r::ut::Font *f = textBox->GetFont();
        if (f != nullptr) {
            // VEC2 internally copied via GPRs here, should be FPRs
            textBox->SetScale(scale * 0.01f);
        }
    } else {
        const nw4r::ut::Font *f = textBox->GetFont();
        if (f != nullptr) {
            // VEC2 internally copied via GPRs here, should be FPRs
            textBox->SetScale(textBox->GetTranslateX1());
        }
    }


    f32 f4 = 0.0f;
    f32 f6 = 0.0f;

    if (spaceX != -9999.0f) {
        textBox->SetCharSpace(spaceX);
    } else {
        f6 = textBox->GetTranslateX1();
    }

    if (spaceY != -9999.0f) {
        textBox->SetLineSpace(spaceY);
    } else {
        f4 = textBox->GetTranslateX1();
    }

    textBox->SetCharSpace(f6 + textBox->GetCharSpace() + textBox->GetTranslateX1());
    textBox->SetLineSpace(f4 + textBox->GetLineSpace() + textBox->GetTranslateX1());
    fn_800AB930(textBox);
}


bool LytBase_c::fn_800AB930(dTextBox_c *box) {
    return fn_800AB9A0(box, -1);
}

bool LytBase_c::fn_800AB940(const char *name, int arg) {
    dTextBox_c *box = getTextBox(name);
    if (box == nullptr) {
        return false;
    }

    return fn_800AB9A0(box, arg);
}

bool LytBase_c::fn_800AB9A0(dTextBox_c *textbox, int arg) {
    if (getMsbtInfo() == nullptr) {
        return false;
    }
    u16 num = textbox->GetExtUserDataNum();
    if (num == 0) {
        return false;
    }
    u32 found = 0;
    s32 ty = 2;
    const nw4r::lyt::res::ExtUserData *list = textbox->GetExtUserData();
    for (int i = 0; i < num; i++) {
        SizedString<0x40> userDatName;
        userDatName = list->GetName();
        if (userDatName == "embed" && list->GetType() == nw4r::lyt::res::TYPE_INT) {
            found = 1;
            ty = 0;
            break;
        }

        if (userDatName == "copy" && list->GetType() == nw4r::lyt::res::TYPE_STRING) {
            found = 1;
            ty = 1;
            break;
        }
        list++;
    }

    if (found != 1) {
        return false;
    }

    if (ty == 1) {
        dTextBox_c *otherBox = getTextBox(list->GetString());
        if (otherBox != nullptr) {
            return fn_800ABB80(textbox, otherBox, arg);
        }
    } else {
        return fn_800ABCE0(list, textbox, textbox, arg);
    }
    return false;
}

bool LytBase_c::fn_800ABB80(dTextBox_c *textbox1, dTextBox_c *textbox2, int arg) {
    u16 num = textbox2->GetExtUserDataNum();
    if (num == 0) {
        return false;
    }
    u32 found = 0;
    const nw4r::lyt::res::ExtUserData *list = textbox2->GetExtUserData();
    for (int i = 0; i < num; i++) {
        SizedString<0x40> userDatName;
        userDatName = list->GetName();
        if (userDatName == "embed" && list->GetType() == nw4r::lyt::res::TYPE_INT) {
            found = 1;
            break;
        }
        list++;
    }

    if (found != 1) {
        return false;
    }
    return fn_800ABCE0(list, textbox1, textbox2, arg);
}

bool LytBase_c::fn_800ABCE0(const nw4r::lyt::res::ExtUserData *userDatum, dTextBox_c *textbox1, dTextBox_c *textbox2,
        int arg) {
    int userDatInt = userDatum->GetInt();
    SizedString<0x40> str1;
    SizedString<0x40> str2;

    str1 = textbox2->GetName();
    if (userDatInt != 0) {
        userDatInt = 0;
        if (arg != -1) {
            userDatInt = arg;
        }
        str2.sprintf(":%02d", userDatInt);
        // TODO this operator is not behaving correctly here - there's
        // an additional null check, and the source string address is
        // computed twice via stack instead of once.
        str1 += &str2;
    }

    const char *text = LMS_GetTextByLabel(getMsbtInfo(), &str1);
    if (text == nullptr) {
        return false;
    }

    fn_800AF930(textbox1, fn_801B2600(text));
    return true;
}

// Now largely the same functions again, but with an additional argument
bool LytBase_c::fn_800ABE50(dTextBox_c *textbox, int arg, void *unk) {
    if (getMsbtInfo() == nullptr) {
        return false;
    }
    u16 num = textbox->GetExtUserDataNum();
    if (num == 0) {
        return false;
    }
    u32 found = 0;
    s32 ty = 2;
    const nw4r::lyt::res::ExtUserData *list = textbox->GetExtUserData();
    for (int i = 0; i < num; i++) {
        SizedString<0x40> userDatName;
        userDatName = list->GetName();
        if (userDatName == "embed" && list->GetType() == nw4r::lyt::res::TYPE_INT) {
            found = 1;
            ty = 0;
            break;
        }

        if (userDatName == "copy" && list->GetType() == nw4r::lyt::res::TYPE_STRING) {
            found = 1;
            ty = 1;
            break;
        }
        list++;
    }

    if (found != 1) {
        return false;
    }

    if (ty == 1) {
        dTextBox_c *otherBox = getTextBox(list->GetString());
        if (otherBox != nullptr) {
            return fn_800AC040(textbox, otherBox, arg, unk);
        }
    } else {
        return fn_800AC1AC(list, textbox, textbox, arg, unk);
    }
    return false;
}

bool LytBase_c::fn_800AC040(dTextBox_c *textbox1, dTextBox_c *textbox2, int arg, void *unk) {
    u16 num = textbox2->GetExtUserDataNum();
    if (num == 0) {
        return false;
    }
    u32 found = 0;
    const nw4r::lyt::res::ExtUserData *list = textbox2->GetExtUserData();
    for (int i = 0; i < num; i++) {
        SizedString<0x40> userDatName;
        userDatName = list->GetName();
        if (userDatName == "embed" && list->GetType() == nw4r::lyt::res::TYPE_INT) {
            found = 1;
            break;
        }
        list++;
    }

    if (found != 1) {
        return false;
    }
    return fn_800AC1AC(list, textbox1, textbox2, arg, unk);
}

extern "C" void fn_800AF840(dTextBox_c *textbox1, MsbtInfo *, const char *, int arg, void *unk);

bool LytBase_c::fn_800AC1AC(const nw4r::lyt::res::ExtUserData *userDatum, dTextBox_c *textbox1, dTextBox_c *textbox2, int arg, void *unk) {
    int userDatInt = userDatum->GetInt();
    SizedString<0x40> str1;
    SizedString<0x40> str2;

    str1 = textbox2->GetName();
    if (userDatInt != 0) {
        str2.sprintf(":%02d", 0);
        // TODO this operator is not behaving correctly here - there's
        // an additional null check, and the source string address is
        // computed twice via stack instead of once.
        str1 += &str2;
    }

    fn_800AF840(textbox1, getMsbtInfo(), &str1, arg, unk);
    return true;
}

MsbtInfo *LytBase_c::getMsbtInfo() const {
    return mpMsbtInfo;
}

nw4r::lyt::Group *LytBase_c::findGroupByName(const char *name) {
    nw4r::lyt::Group *ret = nullptr;
    if (mLayout.GetGroupContainer() != nullptr) {
        ret = mLayout.GetGroupContainer()->FindGroupByName(name);
    }
    return ret;
}

bool hasSameBaseName(const char *left, const char *right) {
    // A for loop turns this into a bdnz
    bool same = true;
    int i = 0;
    while (true) {
        if (i >= 0x1E) {
            same = false;
            break;
        }
        char a = *left;
        char b = *right;
        if (a == '\0' && b == '\0') {
            break;
        }
        if (a != b) {
            if (b != '.') {
                same = false;
                break;
            }
            break;
        }
        left++;
        right++;
        i++;
    }

    return same;
}

char *sRef = "ref";


void LytBase_c::linkMeters(nw4r::lyt::Group *group, LytMeterGroup *meterGroup) {
    // single regswap
    nw4r::ut::LinkList<LytMeterListNode, 0>::Iterator beginIt = meterGroup->list.GetBeginIter();
    nw4r::ut::LinkList<LytMeterListNode, 0>::Iterator endIt = meterGroup->list.GetEndIter();

    for (nw4r::lyt::PaneList::Iterator paneIt = group->GetPaneList()->GetBeginIter();
            paneIt != group->GetPaneList()->GetEndIter(); ++paneIt) {
        nw4r::lyt::Pane *pane = paneIt->mTarget;
        int num = pane->GetExtUserDataNum();
        if (num != 0) {
            const nw4r::lyt::res::ExtUserData *dat = pane->FindExtUserDataByName(sRef);
            if (dat != nullptr && dat->GetType() == nw4r::lyt::res::TYPE_STRING) {
                for (nw4r::ut::LinkList<LytMeterListNode, 0>::Iterator it = beginIt; it != endIt; ++it) {
                    dLytMeterBase *meter = it->mpMeter;
                    if (!meter->LytMeter0x24()) {
                        if (hasSameBaseName(dat->GetString(), meter->LytMeter0x20())) {
                            it->mpPane = pane;
                            pane->AppendChild(meter->LytMeter0x18());
                            meter->LytMeter0x28(true);
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool AnmGroup_c::init(const char *fileName, m2d::ResAccIf_c *resAcc, d2d::Layout_c *layout, const char *groupName) {
    if (layout->GetGroupContainer() == nullptr) {
        return false;
    }

    nw4r::lyt::Group *group = layout->GetGroupContainer()->FindGroupByName(groupName);
    if (group == nullptr) {
        return nullptr;
    }
    nw4r::lyt::AnimTransform *transform = layout->CreateAnimTransform();
    return init(transform, fileName, resAcc, group);
}

bool AnmGroup_c::init(nw4r::lyt::AnimTransform *transform, const char *name, m2d::ResAccIf_c *acc,
        nw4r::lyt::Group *group) {
    if (transform == nullptr) {
        return false;
    }

    if (group == nullptr) {
        return false;
    }

    void *res = acc->getAccessor()->GetResource(0, name, nullptr);
    if (res == nullptr) {
        return false;
    }

    mAnmResource.Set(res);
    u16 animNum = mAnmResource.CalcAnimationNum(group, mAnmResource.IsDescendingBind());
    if (animNum != 0) {
        transform->SetResource(mAnmResource.GetResourceBlock(), acc->getAccessor(), animNum);
        mpGroup = group;
        mpAnmTransform = transform;
    }
    return true;
}

bool AnmGroup_c::fn_800AC6D0(bool b) {
    nw4r::lyt::AnimTransform *anmTransform = mpAnmTransform;
    if (anmTransform == nullptr) {
        return false;
    }

    if (mpGroup == nullptr) {
        return false;
    }

    nw4r::lyt::BindAnimation(mpGroup, anmTransform, false, b);
    mFlags |= 1;
    if (b) {
        mFlags = (mFlags & ~2);
    } else {
        mFlags |= 2;
    }

    u32 flags = 1;
    if (mpAnmTransform->IsLoopData()) {
        flags = 0;
    }

    mpFrameCtrl->set(anmTransform->GetFrameSize(), flags, 1.0f, -1.0f);
    syncAnmFrame();
    return true;
}

bool AnmGroup_c::fn_800AC7D0() {
    nw4r::lyt::AnimTransform *anmTransform = mpAnmTransform;
    if (anmTransform == nullptr) {
        return false;
    }

    nw4r::lyt::Group *group = mpGroup;
    if (group == nullptr) {
        return false;
    }

    nw4r::lyt::UnbindAnimation(group, anmTransform, mAnmResource.IsDescendingBind());
    mFlags = (mFlags & ~1);
    return true;
}

bool AnmGroup_c::fn_800AC860() {
    return true;
}

void AnmGroup_c::fn_800AC870(bool b) {
    nw4r::lyt::Group *group = mpGroup;
    nw4r::lyt::AnimTransform *anmTransform = mpAnmTransform;
    nw4r::lyt::SetAnimationEnable(group, anmTransform, b, mAnmResource.IsDescendingBind());
    if (b) {
        mFlags |= 2;
    } else {
        mFlags = mFlags & ~2;
    }
}

void AnmGroup_c::setAnmFrame(f32 arg) {
    mpAnmTransform->SetFrame(arg);
}

void AnmGroup_c::syncAnmFrame() {
    setAnmFrame(mpFrameCtrl->getFrame());
}

void AnmGroup_c::setForward() {
    mpFrameCtrl->setForward();
}

void AnmGroup_c::setBackward() {
    mpFrameCtrl->setBackward();
}

} // namespace d2d
