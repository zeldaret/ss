#ifndef D_FONT_MANAGER_H
#define D_FONT_MANAGER_H

#include "d/d_dvd.h"
#include "nw4r/ut/ut_ResFont.h"

class dFontMng_c {
public:
    dFontMng_c();
    ~dFontMng_c();

    static nw4r::ut::ResFont *getFont(u8 type);
    void setFontFile(int idx, void *fileData);
    static dFontMng_c *getFontManager(int idx);
    static u8 getFontMgrIdx(u8 type);
    static const char *getFontPath(u8 idx);
    static const char *getFontName(u8 type);

    static bool create();

private:
    static char special_01[];
    static char special_00[];
    static char normal_01[];
    static char normal_00[];

    nw4r::ut::ResFont mFont;
    dDvd::loader_c mLoader;
    // The purpose of these two is a bit unclear because they appear
    // to be set to the same thing?
    nw4r::ut::BinaryFileHeader *mpFontFile;
    void *mpFileData;
};

#endif
