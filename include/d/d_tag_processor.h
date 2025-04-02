#ifndef D_TAG_PROCESSOR_H
#define D_TAG_PROCESSOR_H

#include "d/lyt/d_textbox.h"
#include "nw4r/ut/ut_TagProcessorBase.h"
#include "nw4r/ut/ut_TextWriterBase.h"

// inofficial name
// size: 0xEF4
class dTagProcessor_c : public nw4r::ut::TagProcessorBase<wchar_t> {
public:
    dTagProcessor_c();
    virtual ~dTagProcessor_c();

    void
    eventFlowTextProcessingRelated(dTextBox_c *textBox, const wchar_t *src, wchar_t *dest, u32 destLen, u32 *pOutLen);

    virtual nw4r::ut::Operation Process(u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) override;
    virtual nw4r::ut::Operation CalcRect(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx) override;

    nw4r::ut::Operation ProcessTags(nw4r::ut::Rect *rect, u16 ch, nw4r::ut::PrintContext<wchar_t> *ctx);

    void changeScale(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, bool);
    wchar_t *writeItem(wchar_t *dest, wchar_t *src, s32 *, s32);
    wchar_t *writeStringArg(wchar_t *dest, wchar_t *src, s32 *, s32);
    wchar_t *writeNumericArg(wchar_t *dest, wchar_t *src, s32 *, s32);
    void fn_800B4FF0(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void fn_800B6450(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, u8 cmdLen, wchar_t *ptr);
    void makeSpaceForIconMaybe(nw4r::ut::Rect *rect, nw4r::ut::PrintContext<wchar_t> *ctx, wchar_t *ptr);
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

    u8 symbolToFontIdx(s32 s);
    void setStringArg(const wchar_t *arg, s32 index);

    static s32 getMaxNumLines(s32 arg);

    static void getTextCommand(wchar_t _0xe, const wchar_t *src, u8 *outCmdLen, s32 *outCmd, wchar_t **outEndPtr);
    static void process0xFCommand(wchar_t _0xf, const wchar_t *src, s32 *outCmd);

    static f32 fn_800B8040(s8, u32);
    wchar_t *fn_800B5FD0(u32, wchar_t *, s32 *);
    wchar_t *writeTextNormal(const wchar_t *src, wchar_t *dest, s32 *, u8 cmdLen, s32);
    wchar_t *writeHeroname(wchar_t *dest, s32 *, s32);
    wchar_t *fn_800B5DD0(wchar_t *dest, wchar_t *src, s32 *, s32);
    void fn_800B5520(wchar_t *src);
    void writeIcon(dTextBox_c *textBox, wchar_t *cmd, f32);
    char fn_800B7880(u32);
    void fn_800B70D0(nw4r::ut::TextWriterBase<wchar_t> *, nw4r::ut::PrintContext<wchar_t> *ctx, u16 c, s32);

    void resetSomething();
    void resetSomeFloats();
    void setNumericArg0(s32 arg);
    void setNumericArgs(const s32 *args, s32 numArgs);
    f32 getFloat(s32 i);
    s32 tickPauseFrame();
    s32 tick0x830();

    s32 getNumLinesMaybe() const {
        return mCommandInsert;
    }

private:
    /* 0x004 */ dTextBox_c *field_0x004;
    /* 0x008 */ wchar_t field_0x008[4][256];
    /* 0x808 */ wchar_t field_0x808[4];
    /* 0x810 */ f32 field_0x810;
    /* 0x814 */ f32 field_0x814;
    /* 0x818 */ f32 field_0x818;
    /* 0x81C */ s32 field_0x81C;
    /* 0x820 */ s32 field_0x820;
    /* 0x824 */ s32 field_0x824;
    /* 0x828 */ s32 field_0x828;
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
    /* 0x8C4 */ s32 mNumericArgsCopy[10];
    /* 0x8EC */ s32 field_0x8EC;
    /* 0x8EC */ s32 field_0x8F0;
    /* 0x8F4 */ s32 field_0x8F4;
    /* 0x8F8 */ s32 field_0x8F8;
    /* 0x8FC */ s32 field_0x8FC;
    /* 0x900 */ s32 field_0x900;
    /* 0x904 */ f32 field_0x904;
    /* 0x908 */ f32 field_0x908;
    /* 0x90C */ u8 field_0x90C;
    /* 0x90D */ u8 field_0x90D;
    /* 0x90E */ u8 field_0x90E;
    /* 0x90F */ u8 field_0x90F[4];
    /* 0x914 */ f32 field_0x914[0x32]; // Maybe width for each individual line
    /* 0x9DC */ s32 mCommandInsert;  // Maybe number of lines
    /* 0x9E0 */ wchar_t mStringArgs[8][64];
    /* 0xDE0 */ u8 field_0xDE0[0xEE0 - 0xDE0];
    /* 0xEE0 */ u8 field_0xEE0;
    /* 0xEE1 */ u8 field_0xEE1;
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
