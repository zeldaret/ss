#ifndef D_MESSAGE_H
#define D_MESSAGE_H

#include "common.h"
#include "d/d_base.h"
#include "d/d_tag_processor.h"
#include "libms/flowfile.h"
#include "libms/msgfile.h"
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

    void init();
    void reset();

    bool getField_0x328() const {
        return field_0x328;
    }

    bool getField_0x329() const {
        return field_0x329;
    }

    s32 getField_0x2FC() const {
        return field_0x2FC;
    }

    void setField_0x2FC(s32 val) {
        field_0x2FC = val;
    }

    s32 getField_0x32C() const {
        return field_0x32C;
    }

    void setField_0x329(bool v) {
        field_0x329 = v;
    }

    bool getField_0x32A() const {
        return field_0x32A;
    }

    static bool isValidTextLabel(const char *name);
    static void loadTextByLabel(const char *label, dTagProcessor_c *tagProcessor, bool, u32, u32);

    static s32 getMsbtIndexForLabel(const char *name);
    static const char *getMsbtFileName(s32 index);

    static const wchar_t *getTextMessageByLabel(const char *label, bool global, wchar_t *dstBuf, u32 maxLen);
    static const wchar_t *
    getTextMessageByLabel(const char *label, dTagProcessor_c *pTagProcessor, bool global, wchar_t *dstBuf, u32 maxLen);

    static const wchar_t *formatText(const wchar_t *text);

private:
    static void *sZev0;
    static void *sZevStage;

    static void *getZev0Internal();
    static void *getZevStageInternal();

    void executeMinigame();

    const wchar_t *formatTextInternal(const wchar_t *text);
    const wchar_t *getTextMessageByLabelInternal(
        const char *label, dTagProcessor_c *pTagProcessor, bool global, wchar_t *dstBuf, u32 maxLen
    );

    static s32 getArcIndexForFile(const char *fileName);
    static const char *getArcNameByIndex(s32 idx, bool);

    bool checkIsValidTextLabel(const char *name);

    static void setZevFromMsbArc();
    static void setStageZevFromMsbArc();
    static s32 getMsbtNumberByIndex(s32 index);
    static const char *getMsbfFileName(s32 index);
    static s32 getMsbfNumberByIndex(s32 index);
    static void *getDataFromMsbArc(s32 number, const char *fileName, bool);

    s32 getTextIndexForLabel(const char *label);
    s32 getMsbtIndexForLabelInternal(const char *label);
    static MsbtInfo *getMsbtInfoForIndex(s32 index);
    MsbtInfo *getMsbtInfoForIndexInternal(s32 index);

    static MsbfInfo *getMsbfInfoForIndex(s32 index);
    MsbfInfo *getMsbfInfoForIndexInternal(s32 index);

    static const char *getArcNameByIndexInternal(s32 idx, bool global);

    static dMessage_c *sInstance;
    static dTagProcessor_c *sTagProcessor;

    /* 0x068 */ MsbtInfo *mpMsgs[82];
    /* 0x1B0 */ MsbfInfo *mpFlows[80];
    /* 0x2F0 */ SizedString<8> mLanguage;
    /* 0x2F8 */ u32 mCurrentTextFileNumber;

    /* 0x2FC */ s32 field_0x2FC;

    /* 0x300 */ u8 _0x300[0x328 - 0x300];

    /* 0x328 */ bool field_0x328;
    /* 0x329 */ bool field_0x329;
    /* 0x32A */ bool field_0x32A;

    /* 0x32B */ u8 field_0x32B;
    /* 0x32C */ s32 field_0x32C;
    /* 0x330 */ u8 _0x330[0x334 - 0x330];

    /* 0x334 */ u32 mMinigameResultPoints;
    /* 0x338 */ u32 mMinigameTime;
    /* 0x33C */ u8 _0x33C[0x344 - 0x33C];
    /* 0x344 */ s32 field_0x344;
};

#endif
