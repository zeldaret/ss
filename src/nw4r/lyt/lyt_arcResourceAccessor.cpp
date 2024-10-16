
#include "nw4r/lyt/lyt_arcResourceAccessor.h"

#include "common.h"
#include "nw4r/ut/ut_Font.h"

#include <extras.h>

namespace {

// FindNameResource__37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCc
DONT_INLINE s32 FindNameResource(ARCHandle *pArcHandle, const char *resName) {
    s32 entryNum = -1;
    ARCDir dir;
    ARCDirEntry dirEntry;
    BOOL bSuccess = ARCOpenDir(pArcHandle, ".", &dir);

    while (ARCReadDir(&dir, &dirEntry)) {
        if (dirEntry.isDir) {
            ARCChangeDir(pArcHandle, dirEntry.name);
            entryNum = FindNameResource(pArcHandle, resName);
            ARCChangeDir(pArcHandle, "..");

            if (entryNum != -1) {
                break;
            }
        } else {
            if (stricmp(resName, dirEntry.name) == 0) {
                entryNum = dirEntry.entryNum;
                break;
            }
        }
    }
    bSuccess = ARCCloseDir(&dir);
    return entryNum;
}

// GetResourceSub__37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCcUlPCcPUl
void *GetResourceSub(ARCHandle *pArcHandle, const char *resRootDir, u32 resType, const char *name, u32 *pSize) {
    s32 entryNum = -1;
    if (ARCConvertPathToEntrynum(pArcHandle, resRootDir) != -1 && ARCChangeDir(pArcHandle, resRootDir)) {
        if (resType == 0) {
            entryNum = FindNameResource(pArcHandle, name);
        } else {
            char resTypeStr[5];
            resTypeStr[0] = resType >> 24;
            resTypeStr[1] = resType >> 16;
            resTypeStr[2] = resType >> 8;
            resTypeStr[3] = resType;
            resTypeStr[4] = '\0';

            if (ARCConvertPathToEntrynum(pArcHandle, resTypeStr) != -1) {
                if (ARCChangeDir(pArcHandle, resTypeStr)) {
                    entryNum = ARCConvertPathToEntrynum(pArcHandle, name);
                    ARCChangeDir(pArcHandle, "..");
                }
            }
        }
        ARCChangeDir(pArcHandle, "..");
    }
    if (entryNum != -1) {
        ARCFileInfo arcFileInfo;
        ARCFastOpen(pArcHandle, entryNum, &arcFileInfo);
        void *resPointer = ARCGetStartAddrInMem(&arcFileInfo);
        if (pSize != nullptr) {
            *pSize = ARCGetLength(&arcFileInfo);
        }
        ARCClose(&arcFileInfo);
        return resPointer;
    }
    return nullptr;
}

} // namespace

namespace nw4r {
namespace lyt {
// __ct__Q34nw4r3lyt11FontRefLinkFv
FontRefLink::FontRefLink() : mFont(nullptr) {}

// Set__Q34nw4r3lyt11FontRefLinkFPCcPQ34nw4r2ut4Font
void FontRefLink::Set(const char *name, ut::Font *pFont) {
    strcpy(this->mFontName, name);
    this->mFont = pFont;
}

// __ct__Q34nw4r3lyt19ArcResourceAccessorFv
ArcResourceAccessor::ArcResourceAccessor() : mArcBuf(nullptr) {}

// Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc
bool ArcResourceAccessor::Attach(void *archiveStart, const char *resourceRootDirectory) {
    if (!ARCInitHandle(archiveStart, &this->mArcHandle)) {
        return false;
    }
    this->mArcBuf = archiveStart;
    strncpy(this->mResRootDir, resourceRootDirectory, ROOTPATH_MAX - 1);
    this->mResRootDir[ROOTPATH_MAX - 1] = '\0';
    return true;
}

// Detach__Q34nw4r3lyt19ArcResourceAccessorFv
void *ArcResourceAccessor::Detach() {
    void *ret = this->mArcBuf;
    this->mArcBuf = nullptr;
    return ret;
}

// GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl
void *ArcResourceAccessor::GetResource(u32 resType, const char *name, u32 *pSize) {
    return GetResourceSub(&this->mArcHandle, this->mResRootDir, resType, name, pSize);
}

// Set__Q34nw4r3lyt15ArcResourceLinkFPvPCc
bool ArcResourceLink::Set(void *pFile, const char *name) {
    if (!ARCInitHandle(pFile, &this->mArcHandle)) {
        return false;
    }
    strncpy(this->mResRootDir, name, ROOTPATH_MAX - 1);
    this->mResRootDir[ROOTPATH_MAX - 1] = '\0';
    return true;
}

// RegistFont__Q34nw4r3lyt19ArcResourceAccessorFPQ34nw4r3lyt11FontRefLink
void ArcResourceAccessor::RegistFont(FontRefLink *pLink) {
    mFontList.PushBack(pLink);
}

// UnregistFont__Q34nw4r3lyt19ArcResourceAccessorFPQ34nw4r3lyt11FontRefLink
void ArcResourceAccessor::UnregistFont(FontRefLink *pLink) {
    mFontList.Erase(pLink);
}

// GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc
ut::Font *ArcResourceAccessor::GetFont(const char *name) {
    return detail::FindFont(&this->mFontList, name);
}

// __ct__Q34nw4r3lyt24MultiArcResourceAccessorFv
MultiArcResourceAccessor::MultiArcResourceAccessor() {}

// __dt__Q34nw4r3lyt24MultiArcResourceAccessorFv
MultiArcResourceAccessor::~MultiArcResourceAccessor() {
    this->mArcList.Clear();
}

// Attach__Q34nw4r3lyt24MultiArcResourceAccessorFPQ34nw4r3lyt15ArcResourceLink
void MultiArcResourceAccessor::Attach(ArcResourceLink *pArcResource) {
    this->mArcList.PushBack(pArcResource);
}

// GetResource__Q34nw4r3lyt24MultiArcResourceAccessorFUlPCcPUl
void *MultiArcResourceAccessor::GetResource(u32 resType, const char *name, u32 *pSize) {
    void *pResource;
    ut::LinkList<ArcResourceLink, 0>::Iterator it = this->mArcList.GetBeginIter();
    for (; it != mArcList.GetEndIter(); it++) {
        pResource = GetResourceSub(it->GetArcHandle(), it->GetResRootDir(), resType, name, pSize);
        if (pResource != nullptr) {
            return pResource;
        }
    }

    return NULL;
}

void MultiArcResourceAccessor::RegistFont(FontRefLink *pLink) {
    this->mFontList.PushBack(pLink);
}

// GetFont__Q34nw4r3lyt24MultiArcResourceAccessorFPCc
ut::Font *MultiArcResourceAccessor::GetFont(const char *name) {
    return detail::FindFont(&this->mFontList, name);
}

} // namespace lyt
} // namespace nw4r
