#ifndef NW4R_LYT_RESOURCES_H
#define NW4R_LYT_RESOURCES_H

#include <common.h>
#include <nw4r/lyt/lyt_common.h>
#include <nw4r/lyt/lyt_types.h>
#include <nw4r/ut/ut_Color.h>

#define PANE_NAME_SIZE 16
#define PANE_USERDATA_SIZE 8
#define ANIMTARGET_PANE_MAX 10
#define MATERIAL_NAME_SIZE 20
#define NW4R_RES_NAME_SIZE 16
#define NW4R_MAT_NAME_SIZE 20

#define TEXCOORD_VTX_COUNT 4

namespace nw4r {

namespace lyt {

namespace res {
struct BinaryFileHeader {
    char signature[4]; // at 0x00
    u16 byteOrder;     // at 0x04;
    u16 version;       // at 0x06
    u32 fileSize;      // at 0x08
    u16 headerSize;    // at 0x0C
    u16 dataBlocks;    // at 0x0E
};

struct DataBlockHeader {
    char kind[4]; // at 0x0
    u32 size;     // at 0x4
};

struct TexSRT {
    math::VEC2 translate; // at 0x00
    f32 rotate;           // at 0x08
    math::VEC2 scale;     // at 0x0C
};

struct TexMap {
    // GetWarpModeS__Q44nw4r3lyt3res6TexMapCFv
    GXTexWrapMode GetWarpModeS() const {
        return (GXTexWrapMode)(wrapSflt & 3);
    }

    // GetWarpModeT__Q44nw4r3lyt3res6TexMapCFv
    GXTexWrapMode GetWarpModeT() const {
        return (GXTexWrapMode)(wrapTflt & 3);
    }

    // GetMinFilter__Q44nw4r3lyt3res6TexMapCFv
    GXTexFilter GetMinFilter() const {
        u8 bitData = (wrapSflt >> 2) & 7;
        return (GXTexFilter)((bitData + 1) & 7);
    }

    // GetMagFilter__Q44nw4r3lyt3res6TexMapCFv
    GXTexFilter GetMagFilter() const {
        u8 bitData = (wrapTflt >> 2) & 1;
        return (GXTexFilter)((bitData + 1) & 1);
    }
    u16 texIdx;  // at 0x0
    u8 wrapSflt; // at 0x2
    u8 wrapTflt; // at 0x3
};

struct MaterialResourceNum {
    // GetTevStageNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetTevStageNum() const {
        return (bits >> 18) & 0x1F;
    }

    // GetIndTexStageNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetIndTexStageNum() const {
        return (bits >> 15) & 0x7;
    }
    // GetIndTexSRTNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetIndTexSRTNum() const {
        return (bits >> 13) & 0x3;
    }

    // HasBlendMode__Q44nw4r3lyt3res19MaterialResourceNumCFv
    bool HasBlendMode() const {
        return (bits >> 24) & 1;
    }

    // HasAlphaCompare__Q44nw4r3lyt3res19MaterialResourceNumCFv
    bool HasAlphaCompare() const {
        return (bits >> 23) & 1;
    }

    // HasTevSwapTable__Q44nw4r3lyt3res19MaterialResourceNumCFv
    bool HasTevSwapTable() const {
        return (bits >> 12) & 1;
    }

    // GetMatColNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetMatColNum() const {
        return (bits >> 27) & 1;
    }

    // GetChanCtrlNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetChanCtrlNum() const {
        return (bits >> 25) & 1;
    }

    // GetTexCoordGenNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetTexCoordGenNum() const {
        return (bits >> 8) & 0xF;
    }

    // GetTexSRTNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetTexSRTNum() const {
        return (bits >> 4) & 0xF;
    }

    // GetTexMapNum__Q44nw4r3lyt3res19MaterialResourceNumCFv
    u8 GetTexMapNum() const {
        return (bits >> 0) & 0xF;
    }

    u32 bits;
};
struct Material {
    char name[20];              // at 0x00
    GXColorS10 tevCols[3];      // at 0x14
    GXColor tevKCols[4];        // at 0x2C
    MaterialResourceNum resNum; // at 0x3C
};

struct MaterialList {
    DataBlockHeader blockHeader; // at 0x00
    u16 materialNum;             // at 0x08
    u8 padding[2];               // at 0x0A
};

struct Group {
    DataBlockHeader blockHeader;    // at 0x00 "grp1"
    char mName[NW4R_RES_NAME_SIZE]; // at 0x08
    u16 paneNum;                    // at 0x18
    u16 padding;                    // at 0x1A
};
struct ExtUserData {
    u32 nameOffs;   // at 0x00
    u32 datOffs;    // at 0x04
    u16 numEntries; // at 0x08
    u8 type;        // at 0x0A
    u8 padding;     // at 0x0B
};

struct ExtUserDataList {
    DataBlockHeader blockHeader; // at 0x00
    u16 num;                     // at 0x08
    u16 padding;                 // at 0x0A
};
struct Pane {
    DataBlockHeader blockHeader;        // at 0x00 "pan1"
    u8 flag;                            // at 0x08
    u8 basePosition;                    // at 0x09
    u8 alpha;                           // at 0x0A
    u8 padding;                         //
    char name[PANE_NAME_SIZE];          // at 0x0C
    char mUserData[PANE_USERDATA_SIZE]; // at 0x1C
    math::VEC3 translate;               // at 0x24
    math::VEC3 rotate;                  // at 0x30
    math::VEC2 scale;                   // at 0x3C
    Size size;                          // at 0x44
};

struct Picture : public Pane {
    u32 vtxCols[TEXCOORD_VTX_COUNT]; // at 0x4C
    u16 materialIdx;                 // at 0x5C
    u8 texCoordNum;                  // at 0x5E
    u8 padding[1];                   // at 0x5F
};

struct FontList {
    DataBlockHeader blockHeader; // at 0x00
    u16 fontNum;                 // at 0x08
    u8 padding[2];               // at 0x0A
};

struct Font {
    u32 nameStrOffset; // at 0x0
    u8 type;           // at 0x4
    u8 padding[3];     // at 0x5
};

struct Texture {
    u32 nameStrOffset; // at 0x0
    u8 type;           // at 0x4
    u8 padding[3];     // at 0x5
};

struct TextureList {
    DataBlockHeader blockHeader; // at 0x00
    u16 texNum;                  // at 0x08
    u8 padding[2];               // at 0x0A
};
struct TextBox : public Pane {
    u16 textBufBytes;  // at 0x4C
    u16 textStrBytes;  // at 0x4E
    u16 materialIdx;   // at 0x50
    u16 fontIdx;       // at 0x52
    u8 textPosition;   // at 0x54
    u8 textAlignment;  // at 0x55
    u8 padding[2];     // at 0x56
    u32 textStrOffset; // at 0x58
    u32 textCols[2];   // at 0x5C
    Size fontSize;     // at 0x64
    f32 charSpace;     // at 0x6C
    f32 lineSpace;     // at 0x70
};

struct InflationLRTB {
    f32 l; // at 0x00
    f32 r; // at 0x04
    f32 t; // at 0x08
    f32 b; // at 0x0C
};

struct Bounding : Pane {};

struct Window : Pane {
    InflationLRTB inflation;    // at 0x4C
    u8 frameNum;                // at 0x5C
    u8 padding1;                // at 0x5D
    u8 padding2;                // at 0x5E
    u8 padding3;                // at 0x5F
    u32 contentOffset;          // at 0x60
    u32 frameOffsetTableOffset; // at 0x64
};

struct WindowFrame {
    u16 materialIdx; // at 0x0
    u8 textureFlip;  // at 0x2
    u8 padding1;     // at 0x3
};

struct WindowContent {
    u32 vtxCols[4];  // at 0x00
    u16 materialIdx; // at 0x10
    u8 texCoordNum;  // at 0x12
    u8 padding[1];   // at 0x13
};

struct AnimationBlock {
    DataBlockHeader blockHeader; // at 0x00
    u16 frameSize;               // at 0x08
    u8 loop;                     // at 0x0A
    u8 padding1;                 // at 0x0B
    u16 fileNum;                 // at 0x0C
    u16 animContNum;             // at 0x0E
    u32 animContOffsetsOffset;   // at 0x10
};

struct AnimationTagBlock {
    DataBlockHeader blockHeader; // at 0x00
    u16 tagOrder;                // at 0x08
    u16 groupNum;                // at 0x0A
    u32 nameOffset;              // at 0x0C
    u32 groupsOffset;            // at 0x10
    u16 startFrame;              // at 0x14
    u16 endFrame;                // at 0x16
    u8 flag;                     // at 0x18
    u8 padding[3];               // at 0x19
};

struct AnimationShareBlock {
    DataBlockHeader blockHeader; // at 0x00
    u32 animShareInfoOffset;     // at 0x04
    u16 shareNum;                // at 0x0C
    u8 padding[2];               // at 0x0E
};

struct HermiteKey {
    f32 frame; // at 0x0
    f32 value; // at 0x4
    f32 slope; // at 0x8
};

struct StepKey {
    f32 frame;   // at 0x0
    u16 value;   // at 0x4
    u16 padding; // at 0x6
};

struct AnimationInfo {
    u32 kind;      // at 0x0
    u8 num;        // at 0x4
    u8 padding[3]; // at 0x5
};

struct AnimationTarget {
    u8 id;          // at 0x0
    u8 target;      // at 0x1
    u8 curveType;   // at 0x2
    u8 padding1;    // at 0x3
    u16 keyNum;     // at 0x4
    u8 padding2[2]; // at 0x6
    u32 keyOffset;  // at 0x8
};

struct AnimationContent {
    char name[20]; // at 0x00
    u8 num;        // at 0x14
    u8 type;       // at 0x15
    u8 padding[2]; // at 0x67
};

} // namespace res
} // namespace lyt

} // namespace nw4r

#endif
