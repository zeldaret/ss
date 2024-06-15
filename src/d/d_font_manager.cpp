#include <d/d_font_manager.h>
#include <d/d_heap.h>
#include <sized_string.h>

static dFontMng_c l_dFontMng_obj[3];

static char *systemFonts[] = {"normal_00.brfnt", "special_00.brfnt", "picture_00.brfnt"};
static char *fonts[] = {"normal_00.brfnt", "normal_01.brfnt", "special_00.brfnt", "special_01.brfnt",
        "picture_00.brfnt"};

// TODO What's up with these?
char dFontMng_c::normal_01[] = "normal_01.brfnt";
char dFontMng_c::normal_00[] = "normal_00.brfnt";
char dFontMng_c::special_01[] = "special_01.brfnt";
char dFontMng_c::special_00[] = "special_00.brfnt";

const u32 fontMgrIndex[] = {0, 0, 1, 1, 2};

dFontMng_c::dFontMng_c() {
    l_dFontMng_obj[0].mpFileData = nullptr;
    l_dFontMng_obj[1].mpFileData = nullptr;
    l_dFontMng_obj[2].mpFileData = nullptr;
}

dFontMng_c::~dFontMng_c() {}

nw4r::ut::ResFont *dFontMng_c::getFont(u8 type) {
    u8 index = getFontMgrIdx(type);
    return &l_dFontMng_obj[index].mFont;
}

char *getFontName(u8 type) {
    return fonts[type];
}

dFontMng_c *dFontMng_c::getFontManager(int idx) {
    return &l_dFontMng_obj[idx];
}

u8 dFontMng_c::getFontMgrIdx(u8 type) {
    return fontMgrIndex[type];
}

void dFontMng_c::setFontFile(int idx, void *fileData) {
    l_dFontMng_obj[idx].mpFileData = fileData;
}

// getUsedLanguageString
extern "C" const char *fn_801B2DB0();

const char *dFontMng_c::getFontPath(u8 idx) {
    static SizedString<128> TEMP_FONT_NAME;
    if (idx == 2) {
        TEMP_FONT_NAME.sprintf("/Font/%s", systemFonts[idx]);
    } else {
        TEMP_FONT_NAME.sprintf("/US/Font/%s/%s", fn_801B2DB0(), systemFonts[idx]);
    }
    return &TEMP_FONT_NAME;
}

bool dFontMng_c::create() {
    for (u8 i = 0; i < 3; i++) {
        if (l_dFontMng_obj[i].mpFontFile == nullptr) {
            const char *path = getFontPath(i);
            nw4r::ut::BinaryFileHeader *file = static_cast<nw4r::ut::BinaryFileHeader *>(
                    l_dFontMng_obj[i].mLoader.request(path, 0, dHeap::fontHeap.heap));
            if (file == nullptr) {
                return false;
            }
            l_dFontMng_obj[i].mpFontFile = file;
            getFontManager(i)->setFontFile(i, l_dFontMng_obj[i].mpFontFile);
            l_dFontMng_obj[i].mFont.SetResource(l_dFontMng_obj[i].mpFontFile);
        }
    }

    return true;
}
