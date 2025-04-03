#ifndef D_TEXTWINDOW_UNK_H
#define D_TEXTWINDOW_UNK_H

#include "d/d_tag_processor.h"
#include "d/lyt/d_textbox.h"

// Size 0x1554
class TextWindowUnk {
public:
    TextWindowUnk(dTagProcessor_c *tagProcessor);
    virtual ~TextWindowUnk() {}

    void fn_800B2130(const char *, dTextBox_c *, void*, bool);
    void textAdvancingRelated(bool, bool);

    const wchar_t *getProcessedText() const {
        return mProcessedTextBuffer;
    }
    
private:
    /* 0x0004 */ dTagProcessor_c *mpTagProcessor;
    /* 0x0008 */ wchar_t mRawTextBuffer[1023];
    /* 0x0806 */ wchar_t mProcessedTextBuffer[1023];
    /* 0x1004 */ wchar_t mUnkBuffer[679]; // ???
    /* 0x1550 */ void* field_0x1559;
};

#endif
