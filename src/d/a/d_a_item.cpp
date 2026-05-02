#include "d/a/d_a_item.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_pouch.h"
#include "d/d_sc_game.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_force_get_flag.h"
#include "f/f_list_mg.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/item_mdl_item.h"
#include "toBeSorted/item_mdl_light_fruit.h"
#include "toBeSorted/item_mdl_misc.h"
#include "toBeSorted/item_mdl_rupee.h"
#include "toBeSorted/item_mdl_stamina_fruit.h"

const ItemFlagStruct dAcItemBase_c::sDefaultFlagStruct = {0x28, 0x3C, 0x1E, 0xFF, 0x0000, 0x02, 0x01};
const ItemFlagStruct dAcItemBase_c::sItemFlagStructs[167] = {
    {0x28, 0x3c, 0x1e, 0xff, 0x0001, 0x00, 0x01},
    {0x32, 0x46, 0x1e, 0xff, 0x0002, 0x02, 0x01},
    {0x32, 0x46, 0x1e, 0xff, 0x0003, 0x02, 0x01},
    {0x32, 0x46, 0x1e, 0xff, 0x0004, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0005, 0x02, 0x00},
    {0x32, 0x46, 0x1e, 0xff, 0x0006, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0007, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0008, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0009, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000a, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000b, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000d, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000e, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000f, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0010, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0011, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0013, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0014, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0015, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x001c, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x000c, 0x00, 0x01},
    {0x32, 0x46, 0x1e, 0xff, 0x0020, 0x00, 0x01},
    {0x32, 0x46, 0x1e, 0xff, 0x0021, 0x00, 0x01},
    {0x32, 0x46, 0x1e, 0xff, 0x0022, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0023, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0024, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0028, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0029, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x002a, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x002b, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x002c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x002d, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x002e, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0046, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0030, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0031, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0002, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0033, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0034, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0035, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0036, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0037, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0038, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0039, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x003c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0041, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0042, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0044, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0046, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0047, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0048, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x004a, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x004b, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x004c, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x004d, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x004e, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x004f, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0051, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0054, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0055, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0056, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0057, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0058, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x005a, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x005b, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x005c, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x005d, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x005e, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x005f, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0060, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0061, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0062, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0063, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0064, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0065, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0066, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0067, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0068, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0069, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x006b, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x006c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x006d, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x006e, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x006f, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0070, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0071, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0072, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0074, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0075, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0076, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0077, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0078, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0079, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x007a, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x007b, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x007c, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x007d, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x007e, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x007f, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0080, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0081, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0082, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0083, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0084, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0085, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0086, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0087, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0088, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x0089, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x008a, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x008c, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0x00, 0x008d, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x01, 0x008e, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x02, 0x007a, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x03, 0x0090, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x04, 0x0091, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x05, 0x0092, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x06, 0x0093, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x07, 0x0094, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x08, 0x0095, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x09, 0x0096, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x0a, 0x0097, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x0b, 0x0098, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x0099, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x009e, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x009f, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00a0, 0x00, 0x00},
    {0x28, 0x3c, 0x1e, 0x0c, 0x02f1, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x0d, 0x02f2, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x0e, 0x02f3, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x0f, 0x02f4, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x10, 0x02f5, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x11, 0x02f6, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x12, 0x02f7, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x13, 0x02f8, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x14, 0x02f9, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x15, 0x02fa, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x16, 0x02fb, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x17, 0x02fc, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x18, 0x02fd, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x19, 0x02fe, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x1a, 0x02ff, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0x1b, 0x0300, 0x00, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x00b1, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00b2, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00b3, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00b4, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00ba, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00bb, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00bc, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00bd, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00be, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00bf, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c0, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c1, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c2, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c3, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c4, 0x02, 0x00},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c5, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c6, 0x02, 0x01},
    {0x28, 0x3c, 0x1e, 0xff, 0x00c7, 0x00, 0x00},
};

const u32 dAcItemBase_c::sItemTypeFlags[MAX_ITEM_ID - 1] = {
    0x2000, 0x0,    0x3000, 0x3000, 0x0,    0x1000, 0x3000, 0x3000, 0x0,    0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0,    0x0,    0x1000, 0x1000, 0x1000, 0x3000, 0x3000, 0x3000, 0x1000, 0x1000, 0x1000,
    0x0,    0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x2000, 0x0,    0x2,    0x1000, 0x1000, 0x1000, 0x3000, 0x0,
    0x3000, 0x0,    0x0,    0x0,    0x0,    0x3000, 0x0,    0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x3002, 0x3002,
    0x1000, 0x3000, 0x0,    0x0,    0x0,    0x3000, 0x3000, 0x3000, 0x0,    0x220,  0x220,  0x1000, 0x1000, 0x0,
    0x10,   0x1000, 0x0,    0x0,    0x2,    0x1000, 0x1000, 0x1000, 0x10,   0x20,   0x0,    0x20,   0x0,    0x0,
    0x111,  0x111,  0x11,   0x11,   0x120,  0x0,    0x1000, 0x1000, 0x1000, 0x1000, 0x0,    0x1000, 0x1000, 0x1000,
    0x0,    0x1000, 0x121,  0x2,    0x2,    0x222,  0x4,    0x1000, 0x1000, 0x0,    0x0,    0x0,    0x0,    0x1000,
    0x1000, 0x0,    0x0,    0x3,    0x120,  0x120,  0x120,  0x120,  0x120,  0x120,  0x120,  0x120,  0x120,  0x1120,
    0x110,  0x110,  0x101,  0x202,  0x203,  0x300,  0x400,  0x500,  0x201,  0x302,  0x303,  0x1000, 0x1000, 0x1000,
    0x1000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x21,
    0x3,    0x3,    0x3,    0x3,    0x0,    0x0,    0x0,    0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000,
    0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x3000, 0x0,    0x0,    0x0,    0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x110,  0x110,
    0x110,  0x0,    0x1000, 0x4,    0x0,    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
    0x0,    0x0,    0x0,    0x0,    0x0,    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0,    0x0,    0x0,    0x0,    0x0,
    0x0,    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0,
    0xFFFF, 0xFFFF, 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
    0x0,    0x0,    0x0,    0x0,    0xFFFF, 0xFFFF, 0xFFFF, 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0xFFFF,
    0xFFFF, 0x0,    0x0,    0x0,    0x0,    0x0,
};

static const dCcD_SrcCyl sSrcCyl = {
    {{0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     {~(AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND),
      0x111,
      {0, 0x08, 0x407},
      0,
      0},
     {0xE9}},
    {40.0f, 60.0f}
};

static const char *const sResNodeName = "Set";
/* static */ extern const char *const sDefaultGetItem = "DefaultGetItem";
/* static */ extern const char *const sItemGetGorgeous = "ItemGetGorgeous";
/* static */ extern const char *const sItemGetDefaultTbox = "ItemGetDefaultTBox";
/* static */ extern const char *const sItemGetGorgeousTbox = "ItemGetGorgeousTBox";
/* static */ extern const char *const sItemGetBird = "ItemGetBird";

struct TearIdIdx {
    s32 idx;
    u16 itemId;
    u32 _0x04;
    u32 _0x08;
};

/* static */ extern "C" TearIdIdx sTearIdxes[] = {
    {0, ITEM_FARORE_TEAR},
    {1, ITEM_DIN_TEAR},
    {2, ITEM_NAYRU_TEAR},
    {3, ITEM_SACRED_TEAR},
};

/* static */ extern const char *const sGetFairyBody = "GetFairy_body";
/* static */ extern const char *const sBottleFairy_body = "BottleFairy_body";


bool dAcItemBase_c::isValidItemTypeIdx(int itemId) {
    return itemId < ARRAY_LENGTH(sItemTypeFlags);
}

bool dAcItemBase_c::getItemTypeFromId(u16 itemID, u32 *pOutType) {
    bool valid = isValidItemTypeIdx(itemID);
    if (valid) {
        *pOutType = sItemTypeFlags[itemID];
    }
    return valid;
}

bool dAcItemBase_c::getItemAnimType(u16 itemId, u32 *pOutType) {
    u32 type;
    bool valid = getItemTypeFromId(itemId, &type);
    if (valid) {
        *pOutType = type >> 12 & 0xF;
    }
    return valid;
}

int dAcItemBase_c::getHeartContainerHeartIncrease() {
    return 1;
}

int dAcItemBase_c::getCompletedHeartPieceHeartIncrease() {
    return 1;
}

u8 dAcItemBase_c::getTearIdx(int tearNum) {
    return tearNum / 15;
}
bool dAcItemBase_c::getFlagIdxForTear(int tearNum, u32 *pOutFlag) {
    u8 idx = getTearIdx(tearNum);
    if (idx == 0) {
        *pOutFlag = 0xd;
        return true;
    } else if (idx == 1) {
        *pOutFlag = 0xe;
        return true;
    } else {
        return false;
    }
}
u16 dAcItemBase_c::getTearBit(int tearNum) {
    return 1 << tearNum % 15;
}

bool dAcItemBase_c::setDungeonFlagForTear(int tearNum) {
    u32 flag_idx;
    if (!getFlagIdxForTear(tearNum, &flag_idx)) {
        return false;
    }
    u16 oldVal = DungeonflagManager::sInstance->getCounterOrFlag(flag_idx, 8);
    u16 tearBit = getTearBit(tearNum);
    u16 newVal = tearBit | oldVal;
    DungeonflagManager::sInstance->setToValue(flag_idx, newVal);
    return true;
}

u16 dAcItemBase_c::getItemId() {
    return mId;
}

// TODO: Incorrect location
bool isItemThatHasStruct(u16 itemId) {
    return itemId;
}

// TODO: Incorrect location
u16 getIndex(u16 itemId) {
    return itemId;
}

// TODO: Incorrect location
// const ItemFlagStruct *getItemFlagStruct(u16 itemId) {
//     if (isItemThatHasStruct(itemId)) {
//         return &sItemFlagStructs[getIndex(itemId)];
//     } else {
//         return &sDefaultFlagStruct;
//     }
// }

fLiMgBa_c dAcItem_c::sItemList;

SPECIAL_ACTOR_PROFILE(ITEM, dAcItem_c, fProfile::ITEM, 0x2B, 0, 2);

const mVec3_c dAcItem_c::sFreestandingDowsingOffset(0.0f, 25.0f, 0.0f);

const dAcItem_c::sStaticPtmf dAcItem_c::sStaticPtmfs[] = {
    &dAcItem_c::fn_80248020,
    nullptr,
    &dAcItem_c::fn_80248010,
    nullptr,
    &dAcItem_c::fn_80255B30,
    &dAcItem_c::fn_80255BA0,
    &dAcItem_c::fn_80248040,
    nullptr,
    &dAcItem_c::fn_80248030,
    nullptr,
    &dAcItem_c::fn_80255BD0,
    &dAcItem_c::fn_80255C40,
};

dAcItem_c::dAcItem_c()
    : mStateMgr(*this),
      mEff_0x8F4(this),
      mEff_0x928(this),
      mEff_0x95C(this),
      mEff_0x990(this),
      mEff_0x9C4(this),
      mEff_0x9F8(this),
      mEff_0xA2C(this),
      mEff_0xA60(this),
      mNode(this),
      mEventRelated(*this, nullptr),
      mDowsingTarget(this, DowsingTarget::SLOT_NONE),
      mItemQueuePrev(nullptr),
      mItemQueueNext(nullptr) {
    field_0xCD8 = 1.0f;
    mBaseScale = 1.0f;
    mCurrentScale = 1.0f;
    field_0xCE4 = 1.0f;
    field_0xCE8 = 1.0f;
    field_0xCEC = 1.0f;

    mItemFlags = 0;
    field_0xD4C = 0;

    mbNoDespawn = false;
    mbNoGravity = false;

    field_0xD51 = 0;
    field_0xD56 = 0;

    mbShowItemLighting = false;

    field_0xD62 = 0;
    field_0xD63 = 0;
    field_0xD65 = 0;
    field_0xD66 = 0;

    sItemList.append(&mNode);
    mTargetFiRelated = 3;
}

dAcItem_c::~dAcItem_c() {
    delete field_0xB3C;
    for (int i = 0; i < 2; i++) {
        delete field_0xB34[i];
    }
    delete mpMdl;
    mDowsingTarget.doUnregister();
}

bool dAcItem_c::createHeap() {
    if (dItemMdlPut_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlPut_c();
    } else if (dItemMdlShield_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlShield_c();
    } else if (dItemMdlBottle_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlBottle_c();
    } else if (dItemMdlTear_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlTear_c();
    } else if (dItemMdlFirefly_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlFirefly_c();
    } else if (dItemMdlPotion_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlPotion_c();
    } else if (dItemMdlItem_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlItem_c();
    } else if (dItemMdlRupee_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlRupee_c();
    } else if (dItemMdlLightFruit_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlLightFruit_c();
    } else if (dItemMdlStaminaFruit_c::isMdlForItemId(mRealItemId)) {
        mpMdl = new dItemMdlStaminaFruit_c();
    } else {
        return false;
    }

    if (mpMdl == nullptr) {
        return false;
    }

    if (!mpMdl->init(mRealItemId, this, &mAllocator)) {
        return false;
    }

    field_0xB34[0] = new dAcItem_0xB34_1();
    if (field_0xB34[0] == nullptr) {
        return false;
    }
    field_0xB34[1] = new dAcItem_0xB34_2();
    if (field_0xB34[1] == nullptr) {
        return false;
    }

    if (getItemInitStruct()->_0x00[6] != 0) {
        field_0xB3C = new dAcItem_0xB3C_2(getItemInitStruct()->_0x00[4], getItemInitStruct()->_0x00[5]);
        if (mId == ITEM_STAMINA_FRUIT) {
            // TODO ...
            field_0xB3C->vt_0x0C(150 /* 0x96 */);
        } else if (isLightFruit()) {
            field_0xB3C->vt_0x0C(0 /* 0x0 */);
        }
    } else {
        field_0xB3C = new dAcItem_0xB3C_1(getItemInitStruct()->_0x00[4], getItemInitStruct()->_0x00[5]);
    }

    if (field_0xB3C == nullptr) {
        return false;
    }

    return true;
}

// TODO this function pools .data but doesn't pool .bss for some reason
int dAcItem_c::create() {
    u32 flag = getFromParams(0xA, 0xFF);
    if (flag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag)) {
        return FAILED;
    }

    u32 paramId = getItemIdFromParams();
    if (paramId == ITEM_NONE) {
        return FAILED;
    }

    if (isItemDropFromEnemy()) {
        mId = determineActualItemIdFromItem(paramId);
    } else {
        mId = paramId;
    }
    
    if (isGratitudeCrystal()) {
        if (!StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_GRATITUDE_QUEST_STARTED) || !dScGame_c::currentSpawnInfo.isNight()) {
            return FAILED;
        }
    } else if (isHeart()) {
        if (isItemDropFromEnemy() && dScGame_c::isHeroMode()) {
            switch (adventurePouchCountItem(ITEM_HEART_MEDAL)) {
                case 0: return FAILED;
                case 1:
                    if (cM::rndInt(2) < 1) {
                        return FAILED;
                    }
            }
        }
    }

    setItemId(getItemId2());

    switch (getItemInitStruct()->_0x00[9]) {
        case 0:
            mFn_0xC90 = &dAcItem_c::fn_80254DE0;
            break;
        case 1:
            mFn_0xC90 = &dAcItem_c::fn_80254DF0;
            break;
        default:
            return FAILED;
    }

    if (isSmallKey()) {
        mFnGetFreestandingScale = &dAcItem_c::getSmallKeyFreestandingScale;
    } else if (isGreenRupee() || isBlueRupee() || isRedRupee() || isRupoor()) {
        mFnGetFreestandingScale = &dAcItem_c::getLowValueRupeeFreestandingScale;
    } else if (isSilverRupee() || isGoldRupee()) {
        mFnGetFreestandingScale = &dAcItem_c::getHighValueRupeeFreestandingScale;
    } else if (isSlingshot()) {
        mFnGetFreestandingScale = &dAcItem_c::getSlingshotFreestandingScale;
    } else if (isEldinOre()) {
        mFnGetFreestandingScale = &dAcItem_c::getEldinOreFreestandingScale;
    } else if (isJellyBlob()) {
        mFnGetFreestandingScale = &dAcItem_c::getJellyBlobFreestandingScale;
    } else if (isUnkTreasureGroup1()) {
        mFnGetFreestandingScale = &dAcItem_c::getUnkTreasureGroup1FreestandingScale;
    } else if (isHeartPiece()) {
        mFnGetFreestandingScale = &dAcItem_c::getHeartPieceFreestandingScale;
    } else if (isGratitudeCrystal()) {
        mFnGetFreestandingScale = &dAcItem_c::getGratitudeCrystalFreestandingScale;
    } else {
        mFnGetFreestandingScale = &dAcItem_c::getDefaultFreestandingScale;
    }

    if (isTreasure3()) {
        mFnGetScale = &dAcItem_c::getTreasure3Scale;
    } else if (isInsect()) {
        mFnGetScale = &dAcItem_c::getInsectScale;
    } else {
        mFnGetScale = &dAcItem_c::getDefaultScale;
    }

    if (isSilverRupee() || isGoldRupee()) {
        mFnGetPickupDemoScale = &dAcItem_c::getHighValueRupeePickupScale;
    } else {
        mFnGetPickupDemoScale = &dAcItem_c::getDefaultPickupScale;
    }

    mFnCallGetFreestandingScale = &dAcItem_c::callGetFreestandingModelScale;

    switch (getItemInitStruct()->_0x00[10]) {
        case 0:
            mFnAction = &dAcItem_c::moveNormal0;
            break;
        case 1:
            mFnAction = &dAcItem_c::moveNormal1;
            break;
        case 2:
            mFnAction = &dAcItem_c::moveSpecial;
            break;
        default:
            return FAILED;
    }

    if (isHeart2()) {
        mFunc_0xB58 = &dAcItem_c::fn_802555F0;
    } else {
        mFunc_0xB58 = &dAcItem_c::fn_802555D0;
    }

    if (isAnyRupee()) {
        mFnBounce = &dAcItem_c::bounceRupee;
    } else if (isHeart2()) {
        mFnBounce = &dAcItem_c::bounceNone;
    } else {
        mFnBounce = &dAcItem_c::bounceNormal;
    }

    if (isAnyRupee()) {
        mFnAction3 = &dAcItem_c::fn_802558F0;
    } else if (isAnyTear() || isTriforce() || isHeartPiece()) {
        mFnAction3 = &dAcItem_c::fn_80255AE0;
    } else {
        mFnAction3 = &dAcItem_c::fn_802558E0;
    }

    if (getItemInitStruct()->_0x00[8] == 0) {
        mFnAction4 = &dAcItem_c::fn_80248060;
    } else if (getItemInitStruct()->_0x00[8] == 1) {
        mFnAction4 = &dAcItem_c::fn_80248050;
    } else if (getItemInitStruct()->_0x00[8] == 2) {
        mFnAction4 = &dAcItem_c::fn_80255D50;
    } else if (getItemInitStruct()->_0x00[8] == 3) {
        mFnAction4 = &dAcItem_c::fn_80255DB0;
    } else if (getItemInitStruct()->_0x00[8] == 4) {
        mFnAction4 = &dAcItem_c::fn_80248060;
    } else {
        return FAILED;
    }

    mSubtype = mId;
    
    CREATE_ALLOCATOR(dAcItem_c);

    if (!getFromParams(0x13, 0x1)) {
        mpMdl->setPriorityDraw();
    }
    mStts.SetRank(1);
    mCyl.Set(sSrcCyl);
    mCyl.SetStts(mStts);
    mCyl.SetR(getItemFlagStruct(mId)->field_0x00);
    mCyl.SetH(getItemFlagStruct(mId)->field_0x01);

    setupUnkColliderFlags2();

    if (isAnyRupee()) {
        mCyl.OnTgInfo_0x2(0x8);
        mCyl.SetTgSrcField_0x0C(0x8);
    }

    u32 unk = getParams2Lower_shift1_0x7();
    if ((mId == ITEM_STAMINA_FRUIT && unk) || isLightFruit() || isAnyTear() || isTriforce() || isGratitudeCrystal()) {
        mCyl.SetCoGrp(0x2);
    } else {
        mCyl.SetCoGrp(0xE);
    }

    if (mId == ITEM_STAMINA_FRUIT || isHeartPiece() || isTriforce()) {
        mCyl.ClrTg_0x800000();
    } else {
        mCyl.OnTg_0x800000();
    }

    if (isHeartPiece()) {
        mCyl.OnTg_0x1000000();
    }

    if (isHeartPiece()) {
        mCyl.SetTg_0x4C(0x6CC0);
    } else if (isBabyRattle()) {
        mCyl.SetTg_0x4C(0x6480);
    } else if (!isAnyRupee() && !isHeart2() && mId != ITEM_STAMINA_FRUIT) {
        mCyl.SetTg_0x4C(0x6480);
    }

    if (isBabyRattle()) {
        mCyl.SetTgInfo_0x1(0x13);
    } else if (isHeartPiece()) {
        mCyl.SetTgInfo_0x1(0x15);
    } else if (!isAnyRupee() && !isHeart2() && mId != ITEM_STAMINA_FRUIT) {
        mCyl.SetTgInfo_0x1(0x13);
    }

    if (isHeartPiece()) {
        mCyl.ClrTg_0x200000();
    } else {
        mCyl.OnTg_0x200000();
    }

    mCyl.SetCoCallback(hitCallback);
    mAcchCir.SetWall(getItemFlagStruct(getItemId())->field_0x01 * 0.5f, getItemFlagStruct(getItemId())->field_0x00 * 0.6f);
    mObjAcch.Set(this, 1, &mAcchCir);
    mObjAcch.field_0x0D4 = 50.0f;
    mObjAcch.SetClrSpeedY();
    mObjAcch.SetField_0xC(4);
    mAcceleration = -2.0f;
    mMaxSpeed = -80.0f;

    mGetItemPouchSlot = sGetItemPouchSlot;
    mItemQuantity = sGetItemQuantity;
    s32 subType = getSubtypeFromParam(mParams);
    if (subType != 1 && subType != 2 && subType != 3 && subType != 4 && subType != 5 && subType != 6) {
        fn_80253F90(true);
    } else {
        fn_80253F90(false);
    }

    if (getSubtypeFromParam(mParams) == 2) {
        fn_80256790();
    } else {
        fn_802567D0();
    }

    // These need to not pool...
    switch (getSubtypeFromParam(mParams)) {
        default:
            mStateMgr.changeState(StateID_Wait);
            break;
        case 1:
            mStateMgr.changeState(StateID_Wait);
            break;
        case 2:
            mStateMgr.changeState(StateID_Wait);
            break;
        case 3:
            onTouchLink();
            break;
        case 4:
            mStateMgr.changeState(StateID_Get);
            break;
        case 5:
            mStateMgr.changeState(StateID_WaitForcedGetDemo);
            break;
        case 6:
            mStateMgr.changeState(StateID_GetBeetle);
            break;
        case 7:
            mStateMgr.changeState(StateID_WaitTBoxGetDemo);
            break;
    }

    fn_80255DF0();
    fn_80254BB0();

    if (isHeart()) {
        setDespawnTimer(600);
    } else {
        setDespawnTimer(300);
    }

    if (isGreenRupee() || isBlueRupee() || isRedRupee() || isRupoor()) {
        mFnSetBoundingBox = &dAcItem_c::setLowValueRupeeBoundingBox;
    } else if (isSilverRupee() || isGoldRupee()) {
        mFnSetBoundingBox = &dAcItem_c::setHighValueRupeeBoundingBox;
    } else if (isArrowBundle()) {
        mFnSetBoundingBox = &dAcItem_c::setArrowBundleBoundingBox;
    } else {
        mFnSetBoundingBox = &dAcItem_c::setDefaultBoundingBox;
    }

    applyBoundingBox();

    switch (mId) {
        case ITEM_SMALL_KEY:     mFnGetGravity1 = &dAcItem_c::getSmallKeyGravity1; break;
        case ITEM_GREEN_RUPEE:   mFnGetGravity1 = &dAcItem_c::getGreenRupeeGravity1; break;
        case ITEM_BLUE_RUPEE:    mFnGetGravity1 = &dAcItem_c::getBlueRupeeGravity1; break;
        case ITEM_RED_RUPEE:     mFnGetGravity1 = &dAcItem_c::getRedRupeeGravity1; break;
        case ITEM_HEART:         mFnGetGravity1 = &dAcItem_c::getHeartGravity1; break;
        case ITEM_SINGLE_ARROW:  mFnGetGravity1 = &dAcItem_c::getSingleArrowGravity1; break;
        case ITEM_BUNDLE_ARROWS: mFnGetGravity1 = &dAcItem_c::getArrowBundleGravity1; break;
        case ITEM_SILVER_RUPEE:  mFnGetGravity1 = &dAcItem_c::getSilverRupeeGravity1; break;
        case ITEM_GOLD_RUPEE:    mFnGetGravity1 = &dAcItem_c::getGoldRupeeGravity1; break;
        case ITEM_RUPOOR:        mFnGetGravity1 = &dAcItem_c::getRupoorGravity1; break;
        case ITEM_5_BOMBS:       mFnGetGravity1 = &dAcItem_c::get5BombsGravity1; break;
        case ITEM_10_BOMBS:      mFnGetGravity1 = &dAcItem_c::get10BombsGravity1; break;
        case ITEM_FARORE_TEAR:   mFnGetGravity1 = &dAcItem_c::getFaroreTearGravity1; break;
        case ITEM_5_DEKU_SEEDS:  mFnGetGravity1 = &dAcItem_c::get5SeedsGravity1; break;
        default:                 mFnGetGravity1 = &dAcItem_c::getDefaultGravity1; break;
    }

    switch (mId) {
        case ITEM_HEART: mFnGetHeartGravity = &dAcItem_c::getHeartGravity2; break;
        default:         mFnGetHeartGravity = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:   mFnGetRupeeGravity = &dAcItem_c::getGreenRupeeGravity3; break;
        case ITEM_BLUE_RUPEE:    mFnGetRupeeGravity = &dAcItem_c::getBlueRupeeGravity3; break;
        case ITEM_RED_RUPEE:     mFnGetRupeeGravity = &dAcItem_c::getRedRupeeGravity3; break;
        case ITEM_SILVER_RUPEE:  mFnGetRupeeGravity = &dAcItem_c::getSilverRupeeGravity3; break;
        case ITEM_GOLD_RUPEE:    mFnGetRupeeGravity = &dAcItem_c::getGoldRupeeGravity3; break;
        case ITEM_RUPOOR:        mFnGetRupeeGravity = &dAcItem_c::getRupoorGravity3; break;
        default:                 mFnGetRupeeGravity = nullptr; break;
    }

    switch (mId) {
        case ITEM_SMALL_KEY:     mFnGetGravity4 = &dAcItem_c::getSmallKeyGravity4; break;
        case ITEM_GREEN_RUPEE:   mFnGetGravity4 = &dAcItem_c::getGreenRupeeGravity4; break;
        case ITEM_BLUE_RUPEE:    mFnGetGravity4 = &dAcItem_c::getBlueRupeeGravity4; break;
        case ITEM_RED_RUPEE:     mFnGetGravity4 = &dAcItem_c::getRedRupeeGravity4; break;
        case ITEM_HEART:         mFnGetGravity4 = &dAcItem_c::getHeartGravity4; break;
        case ITEM_SINGLE_ARROW:  mFnGetGravity4 = &dAcItem_c::getSingleArrowGravity4; break;
        case ITEM_BUNDLE_ARROWS: mFnGetGravity4 = &dAcItem_c::getArrowBundleGravity4; break;
        case ITEM_SILVER_RUPEE:  mFnGetGravity4 = &dAcItem_c::getSilverRupeeGravity4; break;
        case ITEM_GOLD_RUPEE:    mFnGetGravity4 = &dAcItem_c::getGoldRupeeGravity4; break;
        case ITEM_RUPOOR:        mFnGetGravity4 = &dAcItem_c::getRupoorGravity4; break;
        case ITEM_5_BOMBS:       mFnGetGravity4 = &dAcItem_c::get5BombsGravity4; break;
        case ITEM_10_BOMBS:      mFnGetGravity4 = &dAcItem_c::get10BombsGravity4; break;
        case ITEM_FARORE_TEAR:   mFnGetGravity4 = &dAcItem_c::getFaroreTearGravity4; break;
        case ITEM_5_DEKU_SEEDS:  mFnGetGravity4 = &dAcItem_c::get5SeedsGravity4; break;
        default:                 mFnGetGravity4 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetDirHitKnockback = &dAcItem_c::getGreenRupeeDirHitKnockback; break;
        case ITEM_BLUE_RUPEE:   mFnGetDirHitKnockback = &dAcItem_c::getBlueRupeeDirHitKnockback; break;
        case ITEM_RED_RUPEE:    mFnGetDirHitKnockback = &dAcItem_c::getRedRupeeDirHitKnockback; break;
        case ITEM_SILVER_RUPEE: mFnGetDirHitKnockback = &dAcItem_c::getSilverRupeeDirHitKnockback; break;
        case ITEM_GOLD_RUPEE:   mFnGetDirHitKnockback = &dAcItem_c::getGoldRupeeDirHitKnockback; break;
        case ITEM_RUPOOR:       mFnGetDirHitKnockback = &dAcItem_c::getRupoorDirHitKnockback; break;
        default:                mFnGetDirHitKnockback = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetDirHitKnockbackRand = &dAcItem_c::getGreenRupeeDirHitKnockbackRand; break;
        case ITEM_BLUE_RUPEE:   mFnGetDirHitKnockbackRand = &dAcItem_c::getBlueRupeeDirHitKnockbackRand; break;
        case ITEM_RED_RUPEE:    mFnGetDirHitKnockbackRand = &dAcItem_c::getRedRupeeDirHitKnockbackRand; break;
        case ITEM_SILVER_RUPEE: mFnGetDirHitKnockbackRand = &dAcItem_c::getSilverRupeeDirHitKnockbackRand; break;
        case ITEM_GOLD_RUPEE:   mFnGetDirHitKnockbackRand = &dAcItem_c::getGoldRupeeDirHitKnockbackRand; break;
        case ITEM_RUPOOR:       mFnGetDirHitKnockbackRand = &dAcItem_c::getRupoorDirHitKnockbackRand; break;
        default:                mFnGetDirHitKnockbackRand = nullptr; break;
    }


    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitKnockback = &dAcItem_c::getGreenRupeeHitKnockback; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitKnockback = &dAcItem_c::getBlueRupeeHitKnockback; break;
        case ITEM_RED_RUPEE:    mFnGetHitKnockback = &dAcItem_c::getRedRupeeHitKnockback; break;
        case ITEM_SILVER_RUPEE: mFnGetHitKnockback = &dAcItem_c::getSilverRupeeHitKnockback; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitKnockback = &dAcItem_c::getGoldRupeeHitKnockback; break;
        case ITEM_RUPOOR:       mFnGetHitKnockback = &dAcItem_c::getRupoorHitKnockback; break;
        default:                mFnGetHitKnockback = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitKnockbackRand = &dAcItem_c::getGreenRupeeHitKnockbackRand; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitKnockbackRand = &dAcItem_c::getBlueRupeeHitKnockbackRand; break;
        case ITEM_RED_RUPEE:    mFnGetHitKnockbackRand = &dAcItem_c::getRedRupeeHitKnockbackRand; break;
        case ITEM_SILVER_RUPEE: mFnGetHitKnockbackRand = &dAcItem_c::getSilverRupeeHitKnockbackRand; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitKnockbackRand = &dAcItem_c::getGoldRupeeHitKnockbackRand; break;
        case ITEM_RUPOOR:       mFnGetHitKnockbackRand = &dAcItem_c::getRupoorHitKnockbackRand; break;
        default:                mFnGetHitKnockbackRand = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitSquareDistance2 = &dAcItem_c::getGreenRupeeHitSquareDistance2; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitSquareDistance2 = &dAcItem_c::getBlueRupeeHitSquareDistance2; break;
        case ITEM_RED_RUPEE:    mFnGetHitSquareDistance2 = &dAcItem_c::getRedRupeeHitSquareDistance2; break;
        case ITEM_SILVER_RUPEE: mFnGetHitSquareDistance2 = &dAcItem_c::getSilverRupeeHitSquareDistance2; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitSquareDistance2 = &dAcItem_c::getGoldRupeeHitSquareDistance2; break;
        case ITEM_RUPOOR:       mFnGetHitSquareDistance2 = &dAcItem_c::getRupoorHitSquareDistance2; break;
        case ITEM_HEART:        mFnGetHitSquareDistance2 = &dAcItem_c::getHeartHitSquareDistance2; break;
        default:                mFnGetHitSquareDistance2 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetDirHitKnockback2 = &dAcItem_c::getGreenRupeeDirHitKnockback2; break;
        case ITEM_BLUE_RUPEE:   mFnGetDirHitKnockback2 = &dAcItem_c::getBlueRupeeDirHitKnockback2; break;
        case ITEM_RED_RUPEE:    mFnGetDirHitKnockback2 = &dAcItem_c::getRedRupeeDirHitKnockback2; break;
        case ITEM_SILVER_RUPEE: mFnGetDirHitKnockback2 = &dAcItem_c::getSilverRupeeDirHitKnockback2; break;
        case ITEM_GOLD_RUPEE:   mFnGetDirHitKnockback2 = &dAcItem_c::getGoldRupeeDirHitKnockback2; break;
        case ITEM_RUPOOR:       mFnGetDirHitKnockback2 = &dAcItem_c::getRupoorDirHitKnockback2; break;
        case ITEM_HEART:        mFnGetDirHitKnockback2 = &dAcItem_c::getHeartDirHitKnockback2; break;
        default:                mFnGetDirHitKnockback2 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetDirHitKnockbackRand2 = &dAcItem_c::getGreenRupeeDirHitKnockbackRand2; break;
        case ITEM_BLUE_RUPEE:   mFnGetDirHitKnockbackRand2 = &dAcItem_c::getBlueRupeeDirHitKnockbackRand2; break;
        case ITEM_RED_RUPEE:    mFnGetDirHitKnockbackRand2 = &dAcItem_c::getRedRupeeDirHitKnockbackRand2; break;
        case ITEM_SILVER_RUPEE: mFnGetDirHitKnockbackRand2 = &dAcItem_c::getSilverRupeeDirHitKnockbackRand2; break;
        case ITEM_GOLD_RUPEE:   mFnGetDirHitKnockbackRand2 = &dAcItem_c::getGoldRupeeDirHitKnockbackRand2; break;
        case ITEM_RUPOOR:       mFnGetDirHitKnockbackRand2 = &dAcItem_c::getRupoorDirHitKnockbackRand2; break;
        case ITEM_HEART:        mFnGetDirHitKnockbackRand2 = &dAcItem_c::getHeartDirHitKnockbackRand2; break;
        default:                mFnGetDirHitKnockbackRand2 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitKnockback2 = &dAcItem_c::getGreenRupeeHitKnockback2; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitKnockback2 = &dAcItem_c::getBlueRupeeHitKnockback2; break;
        case ITEM_RED_RUPEE:    mFnGetHitKnockback2 = &dAcItem_c::getRedRupeeHitKnockback2; break;
        case ITEM_SILVER_RUPEE: mFnGetHitKnockback2 = &dAcItem_c::getSilverRupeeHitKnockback2; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitKnockback2 = &dAcItem_c::getGoldRupeeHitKnockback2; break;
        case ITEM_RUPOOR:       mFnGetHitKnockback2 = &dAcItem_c::getRupoorHitKnockback2; break;
        case ITEM_HEART:        mFnGetHitKnockback2 = &dAcItem_c::getHeartHitKnockback2; break;
        default:                mFnGetHitKnockback2 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitKnockbackRand2 = &dAcItem_c::getGreenRupeeHitKnockbackRand2; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitKnockbackRand2 = &dAcItem_c::getBlueRupeeHitKnockbackRand2; break;
        case ITEM_RED_RUPEE:    mFnGetHitKnockbackRand2 = &dAcItem_c::getRedRupeeHitKnockbackRand2; break;
        case ITEM_SILVER_RUPEE: mFnGetHitKnockbackRand2 = &dAcItem_c::getSilverRupeeHitKnockbackRand2; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitKnockbackRand2 = &dAcItem_c::getGoldRupeeHitKnockbackRand2; break;
        case ITEM_RUPOOR:       mFnGetHitKnockbackRand2 = &dAcItem_c::getRupoorHitKnockbackRand2; break;
        case ITEM_HEART:        mFnGetHitKnockbackRand2 = &dAcItem_c::getHeartHitKnockbackRand2; break;
        default:                mFnGetHitKnockbackRand2 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetDirHitKnockback3 = &dAcItem_c::getGreenRupeeDirHitKnockback3; break;
        case ITEM_BLUE_RUPEE:   mFnGetDirHitKnockback3 = &dAcItem_c::getBlueRupeeDirHitKnockback3; break;
        case ITEM_RED_RUPEE:    mFnGetDirHitKnockback3 = &dAcItem_c::getRedRupeeDirHitKnockback3; break;
        case ITEM_SILVER_RUPEE: mFnGetDirHitKnockback3 = &dAcItem_c::getSilverRupeeDirHitKnockback3; break;
        case ITEM_GOLD_RUPEE:   mFnGetDirHitKnockback3 = &dAcItem_c::getGoldRupeeDirHitKnockback3; break;
        case ITEM_RUPOOR:       mFnGetDirHitKnockback3 = &dAcItem_c::getRupoorDirHitKnockback3; break;
        case ITEM_HEART:        mFnGetDirHitKnockback3 = &dAcItem_c::getHeartDirHitKnockback3; break;
        default:                mFnGetDirHitKnockbackRand2 = nullptr; break; // @bug copypasta mistake
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetDirHitKnockbackRand3 = &dAcItem_c::getGreenRupeeDirHitKnockbackRand3; break;
        case ITEM_BLUE_RUPEE:   mFnGetDirHitKnockbackRand3 = &dAcItem_c::getBlueRupeeDirHitKnockbackRand3; break;
        case ITEM_RED_RUPEE:    mFnGetDirHitKnockbackRand3 = &dAcItem_c::getRedRupeeDirHitKnockbackRand3; break;
        case ITEM_SILVER_RUPEE: mFnGetDirHitKnockbackRand3 = &dAcItem_c::getSilverRupeeDirHitKnockbackRand3; break;
        case ITEM_GOLD_RUPEE:   mFnGetDirHitKnockbackRand3 = &dAcItem_c::getGoldRupeeDirHitKnockbackRand3; break;
        case ITEM_RUPOOR:       mFnGetDirHitKnockbackRand3 = &dAcItem_c::getRupoorDirHitKnockbackRand3; break;
        case ITEM_HEART:        mFnGetDirHitKnockbackRand3 = &dAcItem_c::getHeartDirHitKnockbackRand3; break;
        default:                mFnGetDirHitKnockbackRand2 = nullptr; break; // @bug copypasta mistake
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitKnockback3 = &dAcItem_c::getGreenRupeeHitKnockback3; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitKnockback3 = &dAcItem_c::getBlueRupeeHitKnockback3; break;
        case ITEM_RED_RUPEE:    mFnGetHitKnockback3 = &dAcItem_c::getRedRupeeHitKnockback3; break;
        case ITEM_SILVER_RUPEE: mFnGetHitKnockback3 = &dAcItem_c::getSilverRupeeHitKnockback3; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitKnockback3 = &dAcItem_c::getGoldRupeeHitKnockback3; break;
        case ITEM_RUPOOR:       mFnGetHitKnockback3 = &dAcItem_c::getRupoorHitKnockback3; break;
        case ITEM_HEART:        mFnGetHitKnockback3 = &dAcItem_c::getHeartHitKnockback3; break;
        default:                mFnGetHitKnockback3 = nullptr; break;
    }

    switch (mId) {
        case ITEM_GREEN_RUPEE:  mFnGetHitKnockbackRand3 = &dAcItem_c::getGreenRupeeHitKnockbackRand3; break;
        case ITEM_BLUE_RUPEE:   mFnGetHitKnockbackRand3 = &dAcItem_c::getBlueRupeeHitKnockbackRand3; break;
        case ITEM_RED_RUPEE:    mFnGetHitKnockbackRand3 = &dAcItem_c::getRedRupeeHitKnockbackRand3; break;
        case ITEM_SILVER_RUPEE: mFnGetHitKnockbackRand3 = &dAcItem_c::getSilverRupeeHitKnockbackRand3; break;
        case ITEM_GOLD_RUPEE:   mFnGetHitKnockbackRand3 = &dAcItem_c::getGoldRupeeHitKnockbackRand3; break;
        case ITEM_RUPOOR:       mFnGetHitKnockbackRand3 = &dAcItem_c::getRupoorHitKnockbackRand3; break;
        case ITEM_HEART:        mFnGetHitKnockbackRand3 = &dAcItem_c::getHeartHitKnockbackRand3; break;
        default:                mFnGetHitKnockbackRand3 = nullptr; break;
    }

    if (isHeart2()) {
        mFn_0xCB4 = &dAcItem_c::fn_802577C0;
    } else {
        mFn_0xCB4 = &dAcItem_c::fn_802577D0;
    }

    if (mbNoDespawn) {
        dBgS::GetInstance()->SetLightingCode(this, 10.0f);
    } else if (isHeart2()) {
        mLinkage.bushTpFunc(mObjAcch);
    }

    if (isHeart2()) {
        field_0xD20 = 6;
    }

    if (isAnyTear()) {
        mSomeStr = "force";
    }

    if (isAnyTear()) {
        s32 type = getTearSubtype((ITEM_ID)mId);
        if (type == TEAR_MAX) {
            type = TEAR_0;
        }
        dAcObjBase_c *forceSign = dAcObjBase_c::create(this, fProfile::OBJ_FORCE_SIGN, type & 0xFF, nullptr, nullptr, nullptr, nullptr, 0x3F);
        if (forceSign != nullptr) {
            mForceSignRef.link(forceSign);
        }
    }

    if (isAnyTear()) {
        dAcBase_c *forceGetFlag = dAcBase_c::createActorStage(fProfile::TAG_FORCE_GET_FLAG, dTgForceGetFlag_c::tearIdxToParam(getTearIdx()), &mPosition, &mRotation, &mScale, 0, mRoomID, this);
        if (forceGetFlag == nullptr) {
            return FAILED;
        }
    }

    if (mId == ITEM_STAMINA_FRUIT && unk) {
        // ((getFirstBitParams2__9dAcBase_cCFv((dAcBase_c *) this) << 8) & 0x100 & ~1) | (((u32) this->unk4 >> 0x13U) & 1)
        
        u32 p1 = (getFirstBitParams2() & 0x1) << 8;
        u32 parms = ((mParams >> 0x13) & 0x1) | (p1 & ~0x1);
        dAcObjBase_c *leaves = dAcObjBase_c::create(fProfile::OBJ_FRUIT_GUTS_LEAF, mRoomID, parms, &mPosition, &mRotation, &mScale, -1);
        if (leaves == nullptr) {
            return FAILED;
        }
    }

    if (isGreenRupee() || isBlueRupee() || isRedRupee() || isSilverRupee() || isGoldRupee()) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_RUPEE, 0, &sFreestandingDowsingOffset, 0.0f);
        mDowsingTarget.doRegister();
    } else if (isHeart()) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_HEART, 0, &sFreestandingDowsingOffset, 0.0f);
        mDowsingTarget.doRegister();
    } else if (is5GratitudeCrystals() || isGratitudeCrystal()) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_CRYSTAL, 0, &sFreestandingDowsingOffset, 0.0f);
        mDowsingTarget.doRegister();
    } else if (isTreasure2((ITEM_ID)mId)) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_TREASURE, 0, &sFreestandingDowsingOffset, 0.0f);
        mDowsingTarget.doRegister();
    }

    if (isGratitudeCrystal() || isUnkTreasureGroup1()) {
        setFreestandingYOffset(-mpMdl->getMdl()->getResMdl().GetResNode(sResNodeName).GetTranslate().y);
    }

    if (mId == ITEM_STAMINA_FRUIT && unk == 0) {
        mItemFlags |= 4;
    }

    if ((isGratitudeCrystal() || is5GratitudeCrystals()) && !isItemDropFromEnemy() && mStateMgr.isState(StateID_Wait)) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_ENERGY_SOURCE_OUTWELL);
    }

    if (EventManager::isInEvent() && EventManager::isCurrentEvent("GetGenkiItem")) {
        fn_8024DF30(mItemFlags | 0x20);
    }

    // okay
    bool is = isHeartPiece();
    if (is) {
        is = dScGame_c::isCurrentStage("D100");
    }
    if (is) {
        is = mPosition.squareDistance(mVec3_c(0.0f, -190.0f, -9590.0f)) < 10000.0f;
    }
    if (is) {
        // Skyview Temple - Heart Piece behind bars in main room
        fn_8024DF30(mItemFlags | 0x80);
    }

    field_0xD48 = cM::rndInt(0x10000);
    mColorFilter.r = 0;
    mColorFilter.g = 0;
    mColorFilter.b = 0;
    mLightLevelMaybe = 0.0f;

    return SUCCEEDED;
}

STATE_DEFINE(dAcItem_c, Wait);
STATE_DEFINE(dAcItem_c, Carry);
STATE_DEFINE(dAcItem_c, GetBeetle);
STATE_DEFINE(dAcItem_c, WaitGet);
STATE_DEFINE(dAcItem_c, Get);
STATE_DEFINE(dAcItem_c, WaitGetDemo);
STATE_DEFINE(dAcItem_c, WaitForcedGetDemo);
STATE_DEFINE(dAcItem_c, GetDemo);
STATE_DEFINE(dAcItem_c, WaitTBoxGetDemo);
STATE_DEFINE(dAcItem_c, ResurgeWait);
STATE_DEFINE(dAcItem_c, WaitTurnOff);
STATE_DEFINE(dAcItem_c, WaitSacredDewGetEffect);

void dAcItem_c::initializeState_Wait() {}
void dAcItem_c::executeState_Wait() {}
void dAcItem_c::finalizeState_Wait() {}

void dAcItem_c::initializeState_Carry() {}
void dAcItem_c::executeState_Carry() {}
void dAcItem_c::finalizeState_Carry() {}

void dAcItem_c::initializeState_GetBeetle() {}
void dAcItem_c::executeState_GetBeetle() {}
void dAcItem_c::finalizeState_GetBeetle() {}

void dAcItem_c::initializeState_WaitGet() {}
void dAcItem_c::executeState_WaitGet() {}
void dAcItem_c::finalizeState_WaitGet() {}

void dAcItem_c::initializeState_Get() {}
void dAcItem_c::executeState_Get() {}
void dAcItem_c::finalizeState_Get() {}

void dAcItem_c::initializeState_WaitGetDemo() {}
void dAcItem_c::executeState_WaitGetDemo() {}
void dAcItem_c::finalizeState_WaitGetDemo() {}

void dAcItem_c::initializeState_WaitForcedGetDemo() {}
void dAcItem_c::executeState_WaitForcedGetDemo() {}
void dAcItem_c::finalizeState_WaitForcedGetDemo() {}

void dAcItem_c::initializeState_GetDemo() {}
void dAcItem_c::executeState_GetDemo() {}
void dAcItem_c::finalizeState_GetDemo() {}

void dAcItem_c::initializeState_WaitTBoxGetDemo() {}
void dAcItem_c::executeState_WaitTBoxGetDemo() {}
void dAcItem_c::finalizeState_WaitTBoxGetDemo() {}

void dAcItem_c::initializeState_ResurgeWait() {}
void dAcItem_c::executeState_ResurgeWait() {}
void dAcItem_c::finalizeState_ResurgeWait() {}

void dAcItem_c::initializeState_WaitTurnOff() {}
void dAcItem_c::executeState_WaitTurnOff() {}
void dAcItem_c::finalizeState_WaitTurnOff() {}

void dAcItem_c::initializeState_WaitSacredDewGetEffect() {}
void dAcItem_c::executeState_WaitSacredDewGetEffect() {}
void dAcItem_c::finalizeState_WaitSacredDewGetEffect() {}
