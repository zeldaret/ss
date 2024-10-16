#ifndef NW4R_LYT_ARC_RESOURCE_ACCESSOR_H
#define NW4R_LYT_ARC_RESOURCE_ACCESSOR_H

#include "common.h"
#include "nw4r/lyt/lyt_resourceAccessor.h"
#include "nw4r/ut/ut_Font.h"
#include "nw4r/ut/ut_LinkList.h"
#include "rvl/ARC.h" // IWYU pragma: export

#include <string.h>

#define FONTNAMEBUF_MAX 128
#define ROOTPATH_MAX 128

namespace nw4r {
namespace lyt {
class FontRefLink {
public:
    FontRefLink();
    ~FontRefLink() {}

    void Set(const char *name, ut::Font *pFont);

    ut::Font *GetFont() const {
        return mFont;
    }

    const char *GetFontName() const {
        return mFontName;
    }

private:
    ut::LinkListNode mNode;          // at 0x0
    char mFontName[FONTNAMEBUF_MAX]; // at 0x8
    ut::Font *mFont;                 // at 0x88
};

class ArcResourceLink {
public:
    bool Set(void *pFile, const char *name);

    const char *GetResRootDir() const {
        return mResRootDir;
    }

    ARCHandle *GetArcHandle() {
        return &mArcHandle;
    }

private:
    ut::LinkListNode mLink;         // at 0x00
    ARCHandle mArcHandle;           // at 0x08
    char mResRootDir[ROOTPATH_MAX]; // at 0x24
};

class ArcResourceAccessor : public ResourceAccessor {
public:
    ArcResourceAccessor();
    virtual ~ArcResourceAccessor() {}                    // at 0x8
    virtual void *GetResource(u32, const char *, u32 *); // at 0xC
    virtual ut::Font *GetFont(const char *);             // at 0x10

    bool Attach(void *pFile, const char *name);
    void *Detach();

    bool IsAttached() const {
        return (mArcBuf != NULL);
    }

    void RegistFont(FontRefLink *pLink);

    void UnregistFont(FontRefLink *pLink);

private:
    ARCHandle mArcHandle;                   // at 0x4
    void *mArcBuf;                          // at 0x8
    ut::LinkList<FontRefLink, 0> mFontList; // at 0x24
    char mResRootDir[ROOTPATH_MAX];         // at 0x30
};

class MultiArcResourceAccessor : ResourceAccessor {
public:
    MultiArcResourceAccessor();
    virtual ~MultiArcResourceAccessor();                                  // at 0x8
    virtual void *GetResource(u32 resType, const char *name, u32 *pSize); // at 0xC
    virtual ut::Font *GetFont(const char *);                              // at 0x10

    void Attach(ArcResourceLink *pArcResource);
    void RegistFont(FontRefLink *pLink);

    ut::LinkList<ArcResourceLink, 0x0> mArcList; // at 0x4
    ut::LinkList<FontRefLink, 0x0> mFontList;    // at 0x10
};

namespace detail {
static ut::Font *FindFont(ut::LinkList<FontRefLink, 0> *pList, const char *name) {
    ut::LinkList<FontRefLink, 0>::Iterator it = pList->GetBeginIter();
    for (; it != pList->GetEndIter(); it++) {
        if (strcmp(name, it->GetFontName()) == 0) {
            return it->GetFont();
        }
    }

    return NULL;
}

} // namespace detail
} // namespace lyt
} // namespace nw4r

#endif
