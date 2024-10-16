#ifndef RVL_SDK_WPAD_MEMORY_H
#define RVL_SDK_WPAD_MEMORY_H

#include "rvl/OS/OSTime.h"
#include "rvl/WPAD/WPAD.h"

#include <common.h>

/*******************************************************************************
 * macros
 */

#define WM_MEM_ADDR(addr_) ((addr_) & 0xffff)
#define WM_EXT_REG_ADDR(type_, addr_) (((addr_) & 0xffff) | ((WPAD_EXT_REG_##type_) << 16) | (1 << 26))

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 WPADExtRegType;
enum WPADExtRegType_et {
    WPAD_EXT_REG_SPEAKER = 0xa2,
    WPAD_EXT_REG_EXTENSION = 0xa4,
    WPAD_EXT_REG_MOTION_PLUS = 0xa6,
    WPAD_EXT_REG_DPD = 0xb0,
};

// https://wiibrew.org/wiki/Wiimote#EEPROM_Memory
typedef struct WPADGameInfo {
    OSTime timestamp; // size 0x08, offset 0x00
    u16 gameName[17]; // size 0x22, offset 0x08
    char gameID[4];   // size 0x04, offset 0x2a
    u8 gameType;      // size 0x01, offset 0x2e
    u8 checksum;      // size 0x01, offset 0x2f

    /* wiibrew says this exists in the header on the Wiimote but goes unused,
     * which matches up with the code I see here
     */
    byte_t unknown[8];
} WPADGameInfo; // size 0x38

/*******************************************************************************
 * functions
 */

WPADResult
WPADWriteExtReg(WPADChannel chan, const void *data, u16 length, WPADExtRegType extReg, u16 address, WPADCallback *cb);

#ifdef __cplusplus
}
#endif

#endif // RVL_SDK_WPAD_MEMORY_H
