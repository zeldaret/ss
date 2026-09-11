#include "JSystem/JGadget/std-vector.h"

u32 JGadget::vector::extend_default(u32 param_0, u32 param_1, u32 param_2) {
    return param_1 << 1;
}

JGadget::TVector_pointer_void::TVector_pointer_void(JGadget::TAllocator<void*> const& param_0) : TVector<void*>(param_0) {
}

JGadget::TVector_pointer_void::TVector_pointer_void(u32 size, void* const& val,
                        const JGadget::TAllocator<void*>& allocator): TVector<void*>(allocator) {
    // TODO: NONMATCHING - it has the same pattern as insert - I cant be asked to figure out more rn
    if (size != 0) {     
        TVector<void*>::insert(pEnd_, size, val);
    }
}

void dummy_weak_order(JGadget::TVector<void*> vec) {
    vec.insert(NULL, 0, NULL);
}

JGadget::TVector_pointer_void::~TVector_pointer_void() {
}

void** JGadget::TVector_pointer_void::insert(void** param_0, void* const& param_1) {
    return TVector<void*>::insert(param_0,param_1);
}

void** JGadget::TVector_pointer_void::erase(void** param_0, void** param_1) {
    return TVector<void*>::erase(param_0, param_1);
}
