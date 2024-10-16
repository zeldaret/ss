#include "__init_cpp_exceptions.h"

#include "NMWException.h"
#include "__ppc_eabi_linker.h"


#if __MWERKS__
#pragma exceptions off
#pragma internal on
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void __init_cpp_exceptions(void);
extern void __fini_cpp_exceptions(void);

#ifdef __cplusplus
}
#endif

static int fragmentID = -2;

void __init_cpp_exceptions(void) {
    register char *R2;
    if (fragmentID == -2) {
        __asm {
            mr R2, r2
        }
        fragmentID = __register_fragment(_eti_init_info, R2);
    }
}

void __fini_cpp_exceptions(void) {
    if (fragmentID != -2) {
        __unregister_fragment(fragmentID);
        fragmentID = -2;
    }
}

#pragma push
// HACK: When DTK sets all objects to force_active, the linker decides to strip
// these, for some reason. Set them to force_active as well.
#pragma force_active on

#pragma section ".ctors$10"
__declspec(section ".ctors$10") extern void *const __init_cpp_exceptions_reference = __init_cpp_exceptions;
#pragma section ".dtors$10"
__declspec(section ".dtors$10") extern void *const __destroy_global_chain_reference = __destroy_global_chain;
#pragma section ".dtors$15"
__declspec(section ".dtors$15") extern void *const __fini_cpp_exceptions_reference = __fini_cpp_exceptions;

#pragma pop
