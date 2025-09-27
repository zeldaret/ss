#ifndef D_LANG_H
#define D_LANG_H

#include "rvl/SC.h"

// all of this is made up

enum dLang_e {
    D_LANG_JP = SC_LANG_JP,
    D_LANG_EN = SC_LANG_EN,
    D_LANG_FR = SC_LANG_FR,
    D_LANG_ES = SC_LANG_SP,
};

// Yes there are three...

u8 getCurrentLanguage1();
u8 getCurrentLanguage2();
u8 getCurrentLanguage();

#endif
