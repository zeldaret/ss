#ifndef D_TAG_PROCESSOR_H
#define D_TAG_PROCESSOR_H

#include "d/lyt/d_textbox.h"
#include "nw4r/ut/ut_TagProcessorBase.h"
#include "nw4r/ut/ut_TextWriterBase.h"

struct dLytMsgWindowCharData;

struct LineData {
    f32 mLineWidths[0x32];
    s32 mNumLines;
};

// inofficial name
// size: 0xEF4
class dTagProcessor_c : public nw4r::ut::TagProcessorBase<wchar_t> {
public:
    dTagProcessor_c();
    virtual ~dTagProcessor_c();

    void format(dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, u32 destLen, u32 *pOutLen, void *unk, ...);
    void
    formatV(dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, u32 destLen, u32 *pOutLen, void *unk, va_list list);

    virtual nw4r::ut::Operation Process(u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) override;
    virtual nw4r::ut::Operation CalcRect(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) override;

    nw4r::ut::Operation ProcessTags(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx);

    u8 getAnimatedWiggleIconReplacement(u8 arg);
    u8 symbolToFontIdx(s32 s);
    u8 getAnimatedIconReplacement(u8);

    u8 getAlphaForAnimatedIcon(u8 arg);
    f32 getMarginForCenteredLine(s32);
    f32 fn_800B85C0(s32);
    void somethingWithScrapperAndMusic(wchar_t* src);

    void changeScale(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, bool);
    wchar_t *writeItem(wchar_t *dest, wchar_t *src, s32 *, bool isProcessingOption);
    wchar_t *writeStringArg(wchar_t *dest, wchar_t *src, s32 *, bool isProcessingOption);
    wchar_t *writeNumericArg(wchar_t *dest, wchar_t *src, s32 *, bool isProcessingOption);
    void fn_800B4FF0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void drawIcon(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void calcRectIcon(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, wchar_t *ptr);
    void fn_800B61D0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void setFramesLeftOnPause(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void fn_800B5500(u8 cmdLen, wchar_t *ptr);
    void fn_800B5540(wchar_t *ptr);
    void fn_800B60E0(u8 cmdLen, wchar_t *ptr);
    void fn_800B6110(u8 cmdLen, wchar_t *ptr);
    void fn_800B6140(u8 cmdLen, wchar_t *ptr);
    void playSound(u8 cmdLen, wchar_t *ptr);
    void fn_800B6170(u8 cmdLen, wchar_t *ptr);
    void fn_800B6190(u8 cmdLen, wchar_t *ptr);
    void fn_800B61B0(u8 cmdLen, wchar_t *ptr);
    void setColor(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void restoreColor(nw4r::ut::PrintContext<wchar_t> *ctx, u8 windowType);
    void setScale(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);

    void SetupGXCommon();
    void SetupGXWithColorMapping(nw4r::ut::Color min, nw4r::ut::Color max);
    void SetupVertexFormat();

    void setStringArg(const wchar_t *arg, s32 index);

    static s32 getMaxNumLines(s32 arg);

    static void getTextCommand(wchar_t _0xe, const wchar_t *src, u8 *outCmdLen, s32 *outCmd, wchar_t **outEndPtr);
    static void process0xFCommand(wchar_t _0xf, const wchar_t *src, s32 *outCmd);

    static f32 fn_800B8040(s8, u32);
    wchar_t *writeSingleCharacter(wchar_t, wchar_t *, s32 *);
    wchar_t *writeTextNormal(const wchar_t *src, wchar_t *dest, s32 *, u8 cmdLen, bool isProcessingOption);
    wchar_t *writeHeroname(wchar_t *dest, s32 *, bool isProcessingOption);
    wchar_t *writeSingularOrPluralWord(wchar_t *dest, wchar_t *src, s32 *, bool isProcessingOption);
    void fn_800B5520(wchar_t *src);
    void saveIconWidth(dTextBox_c *textBox, wchar_t *cmd, f32);
    void configureWriterForIcon(nw4r::ut::TextWriterBase<wchar_t> *, nw4r::ut::PrintContext<wchar_t> *ctx, u16 c, s32);
    void drawPicture(nw4r::ut::CharWriter *w, nw4r::ut::PrintContext<wchar_t> *ctx, u16, u8);

    void
    computeCharacterPlacement(dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, s32, u16 *, dLytMsgWindowCharData *charData);

    void resetSomething();
    void resetLineData();
    void setNumericArg0(s32 arg);
    void setNumericArgs(const s32 *args, s32 numArgs);
    f32 getLineWidth(s32 i);
    s32 tickPauseFrame();
    s32 tick0x830();

    void executeBtnPressTimer();
    void executeBtnWiggleTimer();
    void execute();

    void setTextbox(dTextBox_c *box) {
        mpTextBox = box;
    }

    dTextBox_c *getTextBox() const {
        return mpTextBox;
    }

    s32 getNumLinesMaybe() const {
        return mLineData.mNumLines;
    }

    u8 getMsgWindowSubtype() const {
        return mMsgWindowSubtype;
    }

    u8 getField_0x90D() const {
        return field_0x90D;
    }

    u8 getField_0x90E() const {
        return mCurrentOptionIdx;
    }

    s32 getField_0x824() const {
        return field_0x824;
    }

    s32 getCancelBtnIdx() const {
        return mCancelBtnIdx;
    }

    s32 getField_0x82C() const {
        return field_0x82C;
    }

    s32 getField_0x830() const {
        return field_0x830;
    }

    void setMsgWindowSubtype(u8 type) {
        mMsgWindowSubtype = type;
    }

    void setCancelBtnIdx(s32 val) {
        mCancelBtnIdx = val;
    }

    void setField_0x82C(s32 val) {
        field_0x82C = val;
    }

    void setFields_0x8FC_0x900(s32 v1, s32 v2) {
        field_0x8FC = v1;
        field_0x900 = v2;
    }

    s32 getField_0x8FC() const {
        return field_0x8FC;
    }

    s32 getField_0x900() const {
        return field_0x900;
    }

    void setField_0x90D(u8 val) {
        field_0x90D = val;
    }

    void setField_0x90E(u8 val) {
        mCurrentOptionIdx = val;
    }

    void setField_0xEE0(u8 val) {
        field_0xEE0 = val;
    }

    void setIsShadowText(bool val) {
        mIsShadowText = val;
    }

    void setField_0xEE2(u8 val) {
        field_0xEE2 = val;
    }

    void setField_0xEE3(u8 val) {
        field_0xEE3 = val;
    }

    wchar_t *getOptionString(s32 idx) {
        return mOptionBufs[idx];
    }

    LineData getLineData() const {
        return mLineData;
    }

    u8 getOptionSound(s32 idx) const {
        return mOptionSound[idx];
    }

    void resetSomeThings() {
        // TODO clean this up - are these arrays or not???
        for (int i = 0; i < 11; i++) {
            mNumericArgsCopy[i] = 0;
        }
        for (int i = 0; i < 11; i++) {
            (&field_0xEE5)[i] = 0;
        }
    }

private:
    enum Cmd_e {
        TAG_CMD_SCALE = 0x2,
        TAG_CMD_COLOR = 0x3,
        TAG_CMD_0x0F0F0F0E = 0x0F0F0F0E,
        TAG_CMD_0x0F0F0F0F = 0x0F0F0F0F,
        TAG_CMD_OPTION_0 = 0x10000,
        TAG_CMD_OPTION_1 = 0x10001,
        TAG_CMD_OPTION_2 = 0x10002,
        TAG_CMD_OPTION_3 = 0x10003,

        TAG_CMD_WRITE_HERONAME = 0x20000,
        TAG_CMD_WRITE_ITEM = 0x20001,
        TAG_CMD_WRITE_STRING_ARG = 0x20002,
        TAG_CMD_WRITE_NUMERIC_ARG = 0x20003,

        TAG_CMD_WRITE_WORD = 0x30004,
    };

    wchar_t *getOptionBuf() {
        return mOptionBufs[mCurrentOptionIdx - 1];
    }

    void onWriteOptionBuf() {
        if (mCurrentOptionIdx - 1 < 4) {
            mOptionLengths[mCurrentOptionIdx - 1]++;
        }
    }

    /* 0x004 */ dTextBox_c *mpTextBox;
    /* 0x008 */ wchar_t mOptionBufs[4][256];
    /* 0x808 */ u16 mOptionLengths[4];
    /* 0x810 */ f32 field_0x810;
    /* 0x814 */ f32 field_0x814;
    /* 0x818 */ f32 field_0x818;
    /* 0x81C */ s32 field_0x81C;
    /* 0x820 */ s32 field_0x820;
    /* 0x824 */ u32 field_0x824;
    /* 0x828 */ s32 mCancelBtnIdx;
    /* 0x82C */ s32 field_0x82C;
    /* 0x830 */ s32 field_0x830;
    /* 0x834 */ s32 mPauseFramesLeft;
    /* 0x838 */ s32 field_0x838;
    /* 0x83C */ s32 field_0x83C;
    /* 0x840 */ s32 field_0x840;
    /* 0x844 */ s32 field_0x844;
    /* 0x848 */ s32 field_0x848;
    /* 0x84C */ s32 field_0x84C;
    /* 0x850 */ s32 field_0x850;
    /* 0x854 */ s32 field_0x854;
    /* 0x858 */ s32 field_0x858;
    /* 0x85C */ s32 field_0x85C;
    /* 0x860 */ s32 field_0x860;
    /* 0x864 */ s32 field_0x864;
    /* 0x868 */ s32 field_0x868;
    /* 0x86C */ s32 field_0x86C;
    /* 0x870 */ s32 field_0x870;
    /* 0x874 */ s32 field_0x874;
    /* 0x878 */ s32 field_0x878;
    /* 0x87C */ s32 field_0x87C;
    /* 0x880 */ s32 field_0x880;
    /* 0x884 */ s32 field_0x884;
    /* 0x888 */ s32 field_0x888;
    /* 0x88C */ s32 field_0x88C;
    /* 0x890 */ s32 field_0x890;
    /* 0x894 */ s32 field_0x894;
    /* 0x898 */ s32 field_0x898;
    /* 0x89C */ s32 mNumericArgs[10];
    /* 0x8C4 */ s32 mFirstNumericArg; // For pluralization
    /* 0x8C8 */ s32 mButtonPressAnimTimer;
    /* 0x8CC */ s32 mButtonWiggleAnimTimer;
    /* 0x8D0 */ s32 mNumericArgsCopy[11]; // probably not an array
    /* 0x8FC */ s32 field_0x8FC;
    /* 0x900 */ s32 field_0x900;
    /* 0x904 */ f32 field_0x904;
    /* 0x908 */ f32 field_0x908;
    /* 0x90C */ u8 mMsgWindowSubtype;
    /* 0x90D */ u8 field_0x90D;
    /* 0x90E */ u8 mCurrentOptionIdx;
    /* 0x90F */ u8 mOptionSound[4];
    /* 0x914 */ LineData mLineData;
    /* 0x9E0 */ wchar_t mStringArgs[8][64];
    /* 0xDE0 */ u8 field_0xDE0[0xEE0 - 0xDE0];
    /* 0xEE0 */ u8 field_0xEE0;
    /* 0xEE1 */ bool mIsShadowText;
    /* 0xEE2 */ u8 field_0xEE2;
    /* 0xEE3 */ u8 field_0xEE3;
    /* 0xEE4 */ u8 field_0xEE4;
    /* 0xEE5 */ u8 field_0xEE5;
    /* 0xEE6 */ u8 field_0xEE6;
    /* 0xEE7 */ u8 field_0xEE7;
    /* 0xEE8 */ u8 field_0xEE8;
    /* 0xEE9 */ u8 field_0xEE9;
    /* 0xEEA */ u8 field_0xEEA;
    /* 0xEEB */ u8 field_0xEEB;
    /* 0xEEC */ u8 field_0xEEC;
    /* 0xEED */ u8 field_0xEED;
    /* 0xEEE */ u8 field_0xEEE;
    /* 0xEEF */ u8 field_0xEEF;
    /* 0xEF0 */ u8 field_0xEF0;
    /* 0xEF1 */ u8 field_0xEF1;
};

#endif
