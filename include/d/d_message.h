#ifndef D_MESSAGE_H
#define D_MESSAGE_H

#include "common.h"
#include "d/d_base.h"
#include "d/d_tag_processor.h"
#include "libms/flowfile.h"
#include "sized_string.h"

class dMessage_c : public dBase_c {
public:
    dMessage_c();
    virtual ~dMessage_c() {
        sInstance = nullptr;
    }

    int create() override;
    int doDelete() override;
    int execute() override;
    int draw() override;

    static const char *getLanguageIdentifier();

    static dMessage_c *getInstance() {
        return sInstance;
    }

    static dTagProcessor_c *getGlobalTagProcessor() {
        return sTagProcessor;
    }

    void reset();

    bool getField_0x329() const {
        return field_0x329;
    }

    bool getField_0x32A() const {
        return field_0x32A;
    }

    static bool isValidTextLabel(const char *name);
    static void loadTextByLabel(const char *label, dTagProcessor_c *tagProcessor, bool, u32, u32);

private:
    static s32 getArcIndexForFile(const char *fileName);
    static const char *getArcNameByIndex(s32 idx, bool);

    static void setZevFromMsbArc();
    static const char *getMsbtFileName(s32 index);
    static s32 getMsbtNumberByIndex(s32 index);
    static const char *getMsbfFileName(s32 index);
    static s32 getMsbfNumberByIndex(s32 index);
    static void *getDataFromMsbArc(s32 number, const char *fileName, bool);

    static dMessage_c *sInstance;
    static dTagProcessor_c *sTagProcessor;

    /* 0x068 */ MsbtInfo *mpMsgs[82];
    /* 0x1B0 */ MsbfInfo *mpFlows[80];
    /* 0x2F0 */ SizedString<8> mLanguage;
    /* 0x2F8 */ s32 mCurrentTextFileNumber;

    /* 0x2FC */ s32 field_0x2FC;

    /* 0x329 */ bool field_0x329;
    /* 0x32A */ bool field_0x32A;

    /* 0x334 */ u32 mMinigameResultPoints;
    /* 0x338 */ u32 mMinigameTime;
    /* 0x344 */ s32 field_0x344;
};

#endif
