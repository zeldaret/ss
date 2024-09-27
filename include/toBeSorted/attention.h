#ifndef ATTENTION_H
#define ATTENTION_H

#include <common.h>

class dAcBase_c;

class dAttention_c {
public:
    static dAttention_c *sInstance;

    void addTarget(dAcBase_c *target, s32, f32, f32, f32);
    void addTarget(dAcBase_c *target, s32, f32, f32, f32, f32);
};

#endif
