#pragma once

#include <f/f_base.h>
// #include <c/c_owner_set.h>

// Ghidra: dBase_c
//   size: 0x68
// non-official but likely name
class dBase_c : public fBase_c {
public:
    /* 0x64 */ u32 baseProperties; // field from profile init
public:
    /* 80050800 */ dBase_c();
    /* 80050890 */ virtual int preExecute();
    /* 800508f0 */ virtual void postExecute(MAIN_STATE_e state);
    /* 80050920 */ virtual int preDraw();
    /* 80050860 */ virtual void postDraw(MAIN_STATE_e state);
    /* 8002c530 */ virtual ~dBase_c();


public:
    /* 80050980 */ static void resetFlags();
    /* 800509a0 */ static bool isActorPlayer(dBase_c&);
    /* 800509e0 */ static void initLoader();
    /* 80050a00 */ static dBase_c* createBase(ProfileName, dBase_c*, unsigned long, u8);
    /* 80050a10 */ static dBase_c* createRoot(ProfileName, unsigned long, u8);

private:
    /* 800509c0 */ static int loadAsyncCallback();
    /* 800509d0 */ static void unloadCallback();

inline bool isProcControlFlag(u32 flag) const { return (baseProperties & flag) != 0; }

public:
    /* 805750c0 */ static u32 s_ExecuteControlFlags;
    /* 805750c0 */ static u32 s_DrawControlFlags;
    /* 805750c0 */ static u32 s_NextExecuteControlFlags;
    // /* 80575bc0 */ static fProfile::fBaseProfile_c** DAT_ACTOR_ALLOCATION_FUNCTIONS;

    friend class fBase_c;
};