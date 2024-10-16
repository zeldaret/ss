#ifndef D_BG_PC_H
#define D_BG_PC_H

#include <common.h>

struct sBgPc { // Size: 0x14
    /* 0x00 */ u32 code0;
    /* 0x04 */ u32 code1;
    /* 0x08 */ u32 code2; // Always 0xFFFF_FF00
    /* 0x0C */ u32 code3; // Only 0x0000_03FF used
    /* 0x10 */ u32 code4; // Always 0x0000_0000
};

enum dBgPc_ECode {
    /* 0x00004000 */ CODE_OBJ_THRU = 0x4000,
    /* 0x00008000 */ CODE_CAM_THRU = 0x8000,
    /* 0x00010000 */ CODE_LINK_THRU = 0x10000,
    /* 0x00020000 */ CODE_ARROW_THRU = 0x20000,
    /* 0x00040000 */ CODE_SLINGSHOT_THRU = 0x40000,
    /* 0x00080000 */ CODE_BEETLE_THRU = 0x80000,
    /* 0x00100000 */ CODE_CLAWSHOT_THRU = 0x100000,
    /* 0x00200000 */ CODE_TARGET_THRU = 0x200000,
    /* 0x00400000 */ CODE_SHDW_THRU = 0x400000,
    /* 0x00800000 */ CODE_BOMB_THRU = 0x800000,
    /* 0x01000000 */ CODE_WHIP_THRU = 0x1000000,
    /* 0x40000000 */ CODE_UNDERWATER_ROOF = 0x40000000,
    /* 0x80000000 */ CODE_UNUSED_8000_0000 = 0x80000000,
};

class dBgPc {
public:
    void setCode(sBgPc &);
    bool getWtr() const;

    u32 getObjThrough() {
        return m_code.code0 & CODE_OBJ_THRU;
    }
    u32 getCamThrough() {
        return m_code.code0 & CODE_CAM_THRU;
    }
    u32 getLinkThrough() {
        return m_code.code0 & CODE_LINK_THRU;
    }
    u32 getObjThrough() const {
        return m_code.code0 & CODE_OBJ_THRU;
    }
    u32 getCamThrough() const {
        return m_code.code0 & CODE_CAM_THRU;
    }
    u32 getLinkThrough() const {
        return m_code.code0 & CODE_LINK_THRU;
    }
    u32 getArrowThrough() const {
        return m_code.code0 & CODE_ARROW_THRU;
    }
    u32 getClawshotThrough() const {
        return m_code.code0 & CODE_CLAWSHOT_THRU;
    }
    u32 getSlingshotThrough() const {
        return m_code.code0 & CODE_SLINGSHOT_THRU;
    }
    u32 getBeetleThrough() const {
        return m_code.code0 & CODE_BEETLE_THRU;
    }
    bool getTargetThrough() const {
        return m_code.code0 & CODE_TARGET_THRU;
    }
    bool getShdwThrough() const {
        return m_code.code0 & CODE_SHDW_THRU;
    }
    u32 getBombThrough() const {
        return m_code.code0 & CODE_BOMB_THRU;
    }
    u32 getWhipThrough() const {
        return m_code.code0 & CODE_WHIP_THRU;
    }
    u32 getUnderwaterRoof() const {
        return m_code.code0 & CODE_UNDERWATER_ROOF;
    }
    bool getCode0_0x80000000() const {
        return m_code.code0 & CODE_UNUSED_8000_0000;
    }

    s32 getWallCode() const {
        return m_code.code1 >> 0x8 & 0xF;
    }
    u32 getCode1_0x02000000() const {
        return m_code.code1 & 0x02000000;
    }
    u32 getCode1_0x04000000() const {
        return m_code.code1 & 0x04000000;
    }
    u32 getCode1_0x08000000() const {
        return m_code.code1 & 0x08000000;
    }

    u32 getGroundCode() const {
        return m_code.code1 >> 20 & 0x1F;
    }
    u32 getLightingCode() const {
        return m_code.code1 >> 28;
    }
    u32 getSpecialCode() const {
        return m_code.code3 & 0x1F;
    }

    u32 getAtt0Code() const {
        return m_code.code3 >> 5 & 0x3F;
    }
    u32 getAtt1Code() const {
        return m_code.code1 >> 17 & 0x7;
    }

    u8 getGrpRoomIdx() const {
        return m_code.code0 & 0x3F;
    }

    u8 getExitId() const {
        return m_code.code0 >> 6;
    }

    u32 getCode0_0x30000000() const {
        return m_code.code0 >> 0x1C & 3;
    }

    u8 getLinkNo() const {
        return m_code.code1;
    }

    u8 getCamMoveBG() const {
        return m_code.code2;
    }
    u8 getRoomCam() const {
        return m_code.code2 >> 8;
    }
    u8 getRoomPath() const {
        return m_code.code2 >> 0x10;
    }
    u8 getRoomPathPnt() const {
        return m_code.code2 >> 0x18;
    }

    u8 getRoomInf() const {
        return m_code.code4;
    }
    u8 getSnd() const {
        return m_code.code4 >> 0xB;
    }
    u32 maskNrm() {
        return m_code.code4 & 0x100;
    }

    /* 0x0 */ sBgPc m_code;
};

#endif
