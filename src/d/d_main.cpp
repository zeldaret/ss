#include "d/d_main.h"

#include "d/d_sys.h"
#include "f/f_manager.h"
#include "toBeSorted/d_lib.h"
#include "toBeSorted/mpls.h"

#include "rvl/OS.h"

u32 lbl_805751D0;
OSTime g_InitialTime;

bool dMain::fn_80054F90(u32 val) {
    return lbl_805751D0 & val;
}

void dMain::fn_80054FB0(u32 val) {
    u32 flags = lbl_805751D0;

    if (!flags) {
        fManager_c::setStopProcFlags(
            fManager_c::PROC_FLAG_CONNECT | fManager_c::PROC_FLAG_CREATE | fManager_c::PROC_FLAG_EXECUTE |
            fManager_c::PROC_FLAG_DELETE
        );
    }

    lbl_805751D0 = flags | val;
}

void dMain::fn_80054FE0(u32 val) {
    lbl_805751D0 &= ~val;

    if (!lbl_805751D0) {
        fManager_c::maskStopProcFlags(fManager_c::PROC_FLAG_DRAW);
    }
}

void dMain::Create() {
    dSys_c::create();
    dSys_c::setBlack(false);
}

void dMain::Execute() {
    while (true) {
        dSys_c::execute();
    }
}

void *dMain::main01(void *arg) {
    Create();
    Execute();
    return nullptr;
}

#define STACK_SIZE 0xF000

static OSThread MAIN_THREAD;

void main(int argc, char **argv) {
    u8 pStackBase[STACK_SIZE] __attribute__((aligned(32)));

    unknownStub();
    g_InitialTime = OSGetTime();
    dSystem::fixHeaps();

    Mpls::initialize(argc, argv);
    Mpls::tryLaunchMovie();

    OSThread *curThread = OSGetCurrentThread();
    OSPriority curPrio = OSGetThreadPriority(curThread);

    OSCreateThread(&MAIN_THREAD, dMain::main01, nullptr, &pStackBase[STACK_SIZE], STACK_SIZE, curPrio, 0);
    OSResumeThread(&MAIN_THREAD);

    OSSetThreadPriority(curThread, 31);
    OSSuspendThread(curThread);
}
