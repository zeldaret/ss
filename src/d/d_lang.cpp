#include "d/d_lang.h"

#include "rvl/SC/scapi.h"

u8 getCurrentLanguage1() {
    return getCurrentLanguage2();
}

u8 getCurrentLanguage2() {
    return getCurrentLanguage();
}

u8 getCurrentLanguage() {
    u8 lang = SCGetLanguage();
    if (lang != SC_LANG_FR && lang != SC_LANG_SP) {
        lang = SC_LANG_EN;
    }
    return lang;
}
