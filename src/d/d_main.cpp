#include "d/d_main.h"

#include "d/d_sys.h"
#include "toBeSorted/d_lib.h"
#include "toBeSorted/mpls.h"

#include "rvl/OS.h"

OSThread MAIN_THREAD;

#define STACK_SIZE 0xF000

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

void main(int argc, char **argv) {
    u8 pStackBase[STACK_SIZE] __attribute__((aligned(32)));

    fn_80006C20();
    dMain::g_InitialTime = OSGetTime();
    dSystem::fixHeaps();
    fn_80006CE0(argc, argv);
    fn_80006D60();

    OSThread *curThread = OSGetCurrentThread();
    OSPriority curPrio = OSGetThreadPriority(curThread);

    OSCreateThread(&MAIN_THREAD, dMain::main01, nullptr, &pStackBase[STACK_SIZE], STACK_SIZE, curPrio, 0);

    OSResumeThread(&MAIN_THREAD);
    OSSetThreadPriority(curThread, 31);
    OSSuspendThread(curThread);
}
