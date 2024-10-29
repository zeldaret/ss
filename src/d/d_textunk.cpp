#include "d/d_textunk.h"

#include "common.h"
#include "nw4r/ut/ut_Color.h"

UnkTextThing textThing;
UnkTextThing *UnkTextThing::sInstance;

// UnkTextThing::~UnkTextThing() {}
UnkTextThing::UnkTextThing() {
    sInstance = this;

    field_0x754 = 0.0f;
    field_0x758 = 12.0f;
    field_0x75C = 0.04f;
    field_0x760 = 0.22f;
    field_0x764 = 42.0f;
    field_0x768 = 0.8f;
    field_0x76C = 0.3f;

    field_0x7B3 = false;
    field_0x798 = 0;
    field_0x79C = 0;
    field_0x7A0 = 0;

    field_0x770 = 80.0f;
    field_0x774 = 30.0f;
    field_0x796 = 3;
    field_0x778 = 10.0f;
    field_0x780 = 8.0f;
    field_0x77C = 8.0f;
    field_0x784 = 6.0f;

    field_0x7A4 = 0xB6;
    field_0x7A6 = 0x16C;

    for (int i = 0; i < 82; i++) {
        field_buf0[i] = 0;
        field_buf2[i] = 0;
    }

    for (int i = 0; i < 23; i++) {
        field_buf4[i] = 0;
        field_buf5[i] = 0;
    }

    for (int i = 0; i < 80; i++) {
        field_buf1[i] = 0;
        field_buf3[i] = 0;
    }

    field_0x74C = 0;
    field_0x750 = 0;

    field_0x7B4 = false;
    field_0x7B5 = false;

    field_0x004 = "";
    field_0x084 = "";
    field_0x104 = "";

    field_0x7B6 = 0;

    field_0x7A8 = 0;
    field_0x788 = 0.45;
    field_0x78C = -5.0f;
    field_0x7AE = 4;
    field_0x7B7 = 0;
    field_0x7AA = 5;
    field_0x7AC = 15;
    field_0x7B0 = 0;
    field_0x7B2 = false;

    // Weird init order
    field_0x7B8.a = 0;
    field_0x7B8.b = 0;
    field_0x7B8.g = 0;
    field_0x7B8.r = 0;
    field_0x7BC.a = 0;
    field_0x7BC.b = 0;
    field_0x7BC.g = 0;
    field_0x7BC.r = 0;

    field_0x790 = -1.0f;
    field_0x794 = 100.0f;
}

void UnkTextThing::destroy() {
    for (int i = 0; i < 82; i++) {
        if (field_buf0[i] != nullptr) {
            destroyUnk(field_buf0[i]);
            field_buf0[i] = nullptr;
        }

        if (field_buf2[i] != nullptr) {
            destroyMsg(field_buf2[i]);
            field_buf2[i] = nullptr;
        }
    }

    for (int i = 0; i < 23; i++) {
        if (field_buf4[i] != nullptr) {
            destroyUnk(field_buf4[i]);
            field_buf4[i] = nullptr;
        }

        if (field_buf5[i] != nullptr) {
            destroyMsg(field_buf5[i]);
            field_buf5[i] = nullptr;
        }
    }

    for (int i = 0; i < 80; i++) {
        if (field_buf1[i] != nullptr) {
            destroyUnk(field_buf1[i]);
            field_buf1[i] = nullptr;
        }

        if (field_buf3[i] != nullptr) {
            destroyFlow(field_buf3[i]);
            field_buf3[i] = nullptr;
        }
    }

    if (field_0x74C != nullptr) {
        destroyUnk(field_0x74C);
        field_0x74C = nullptr;
    }

    if (field_0x750 != nullptr) {
        destroyUnk(field_0x750);
        field_0x750 = nullptr;
    }
}

f32 UnkTextThing::fn_800B1F10() {
    return field_0x794 / 100.0f;
}

void *UnkTextThing::allocUnk(size_t size, int align) {
    return nullptr;
}
void UnkTextThing::destroyUnk(void *_thing) {}

f32 UnkTextThing::fn_800B1F70() {
    f32 v = fn_800B1FC0();
    v *= fn_800B1FE0();
    return v;
}
f32 UnkTextThing::fn_800B1FC0() {
    return 1.0f;
}
f32 UnkTextThing::fn_800B1FD0() {
    return 0.9f;
}
f32 UnkTextThing::fn_800B1FE0() {
    return 1.0f;
}
f32 UnkTextThing::fn_800B1FF0() {
    return 0.0f;
}
f32 UnkTextThing::fn_800B2000() {
    return 0.0f;
}
f32 UnkTextThing::fn_800B2010() {
    return -1.0f;
}
f32 UnkTextThing::fn_800B2020() {
    return 0.0f;
}
f32 UnkTextThing::fn_800B2030() {
    return 0.0f;
}
f32 UnkTextThing::fn_800B2040() {
    return 0.0f;
}

void UnkTextThing::destroyMsg(MsbtInfo *msg) {
    field_0x7B6 = true;
    LMS_CloseMessage(msg);
    field_0x7B6 = false;
}

void UnkTextThing::destroyFlow(MsbfInfo *flow) {
    field_0x7B6 = true;
    LMS_CloseFlow(flow);
    field_0x7B6 = false;
}
