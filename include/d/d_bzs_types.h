#ifndef D_STAGE_TYPES_H
#define D_STAGE_TYPES_H

#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

// https://github.com/lepelog/skywardsword-tools/blob/master/bzs.py

// Size 0x2
struct ARCN {
    /* 0x00 */ u16 offset;
};

// Size 0x20
struct AREA {
    /* 0x00 */ mVec3_c mPos;
    /* 0x0C */ mVec3_c mScale;
    /* 0x18 */ mAng mAngleY;
    /* 0x1A */ mAng mAngleLink;
    /* 0x1C */ u8 field_0x1C;
};

// TODO: double check, copied from ss-tools
// Size 0x28
struct BPNT {
    /* 0x00 */ mVec3_c position1;
    /* 0x0C */ mVec3_c position2;
    /* 0x18 */ mVec3_c position3;
    /* 0x24 */ u8 _0x24[4];
};

// TODO: double check, copied from ss-tools
// Size 0x2C
struct CAM {
    /* 0x00 */ u8 _0x00[4];
    /* 0x04 */ mVec3_c position;
    /* 0x10 */ f32 angle;
    /* 0x14 */ u8 _0x14[8];
    /* 0x1C */ char name[16];
};

// Size 0x38
struct EVNT {
    /* 0x00 */ u8 _0x00[2];
    /* 0x02 */ u16 storyFlag1;
    /* 0x04 */ u16 storyFlag2;
    /* 0x06 */ u8 _0x06[3];
    /* 0x09 */ u8 exitId;
    /* 0x0A */ u8 _0x0A[3];
    /* 0x0D */ u8 skipEvent;
    /* 0x0E */ u8 _0x0E[1];
    /* 0x0F */ u8 sceneFlag1;
    /* 0x10 */ u8 sceneFlag2;
    /* 0x11 */ u8 skipFlag;
    /* 0x12 */ u8 _0x12[2];
    /* 0x14 */ u16 item;
    /* 0x16 */ u16 _0x16[1];
    /* 0x18 */ char name[32];
};

// Size 0x???
struct bzsFILE {
    // ???
};

struct LAY {
    /* 0x00 */ u16 mCount;
    /* 0x04 */ u32 mOffset;
};

// Size 0x04
struct LYLT {
    /* 0x0 */ u8 layer;
    /* 0x1 */ s8 demoHigh;
    /* 0x2 */ s8 demoLow;
    /* 0x3 */ u8 dummy;
};

// Size 0x04
struct LYSE {
    /* 0x0 */ u16 storyFlag;
    /* 0x2 */ u8 night;
    /* 0x3 */ u8 layer;
};

// Size 0x24
struct OBJ {
    /* 0x00 */ u32 params1;
    /* 0x04 */ u32 params2;
    /* 0x08 */ mVec3_c position;
    /* 0x14 */ mAng3_c angle;
    /* 0x1A */ u16 id;
    /* 0x1C */ char name[8];
};

// Size 0x2
struct OBJN {
    /* 0x00 */ u16 offset;
};

// Size 0xC
struct PATH {
    /* 0x00 */ u8 field_0x00;
    /* 0x01 */ u8 field_0x01;
    /* 0x02 */ u16 pointStartIndex;
    /* 0x04 */ u16 pointCount;
    /* 0x06 */ u8 _0x06[4];
    /* 0x0A */ u8 firstBitIsWrapAround;
    /* 0x0B */ u8 _0x0B[1];
};

// Size 0x24
struct PCAM {
    /* 0x00 */ mVec3_c position;
    /* 0x0C */ mVec3_c target;
    /* 0x18 */ f32 twist;
    /* 0x1C */ f32 radius;
    /* 0x20 */ u8 id;
    /* 0x21 */ u8 _0x21[3];
};

// Size 0x18
struct PLY {
    // The Ghidra struct seems to overambitiously encode
    // the params themselves, this is just an u32 link actor params
    // /* 0x00 */ u16 storyFlag;
    // /* 0x02 */ u8 playCutscene;
    // /* 0x03 */ u8 _0x03[1];
    /* 0x00 */ u32 linkParams;
    /* 0x04 */ mVec3_c position;
    /* 0x10 */ mAng3_c angle;
    /* 0x16 */ u16 entranceId;
};

// Size 0x10
struct PNT {
    /* 0x00 */ mVec3_c position;
    /* 0x0C */ u8 _0x0C[4];
};

// Size 0x4
struct RMPL {
    /* 0x0 */ u8 roomId;
    /* 0x1 */ u8 count;
    /* 0x2 */ u16 offset;
};

// Size 0x28
struct SCEN {
    /* 0x00 */ char name[32];
    /* 0x20 */ u8 room;
    /* 0x21 */ u8 layer;
    /* 0x22 */ u8 entrance;
    /* 0x23 */ u8 night;
    /* 0x24 */ u8 transitionType;
    /* 0x25 */ u8 transitionFadeFrames;
    /* 0x26 */ u8 zero;
    /* 0x27 */ u8 savePrompt;
};

// Size 0x30
struct SOBJ {
    /* 0x00 */ u32 params1;
    /* 0x04 */ u32 params2;
    /* 0x08 */ mVec3_c position;
    /* 0x14 */ mVec3_c scale;
    /* 0x20 */ mAng3_c angle;
    /* 0x26 */ u16 id;
    /* 0x28 */ char name[8];
};

// Size 0x???
struct SBPT {
    // ???
};

// Parsed the same way in ss-tools
typedef PNT SPNT;
typedef PATH SPTH;

// Size 0x14
struct STIF {
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 defaultCullingDistance;
    /* 0x0C */ u16 flagIndex;
    /* 0x0E */ u8 areaType;
    /* 0x0F */ u8 field_0x0F;
    /* 0x10 */ u8 field_0x10;
    /* 0x11 */ u8 field_0x11;
    /* 0x12 */ s8 mapNameId;
    /* 0x13 */ u8 field_0x13;
};

#endif
