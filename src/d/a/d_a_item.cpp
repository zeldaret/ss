#include "d/a/d_a_item.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_talk_kensei.h"
#include "d/a/obj/d_a_obj_boomerang.h"
#include "d/a/obj/d_a_obj_warp.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_camera.h"
#include "d/d_linkage.h"
#include "d/d_player.h"
#include "d/d_pouch.h"
#include "d/d_sc_game.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_force_get_flag.h"
#include "d/t/d_t_minigame_insect_capture.h"
#include "d/t/d_t_siren.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/item_mdl.h"
#include "toBeSorted/item_mdl_item.h"
#include "toBeSorted/item_mdl_light_fruit.h"
#include "toBeSorted/item_mdl_misc.h"
#include "toBeSorted/item_mdl_rupee.h"
#include "toBeSorted/item_mdl_stamina_fruit.h"
#include "toBeSorted/minigame_mgr.h"

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

struct GlitteringSporeRupeeChance {
    s32 itemId;
    s32 weight;
};

static const GlitteringSporeRupeeChance sSporeRupeeChances[] = {
    {ITEM_GREEN_RUPEE, 4},
    { ITEM_BLUE_RUPEE, 5},
    {  ITEM_RED_RUPEE, 1},
    {     ITEM_RUPOOR, 1},
};

static const dCcD_SrcCyl sSrcCyl = {
    {{0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     {~(AT_TYPE_BEETLE | AT_TYPE_GLITTERING_SPORES | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x111, {0, 0x08, 0x407}, 0, 0},
     {0xE9}},
    {40.0f, 60.0f}
};

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

static bool isDefaultDeleteGround(s32 specialCode) {
    // Lava, Cursed Water
    return specialCode == 7 || specialCode == 17;
}

static bool isDefaultSwimGround(s32 specialCode) {
    // Deep Sand (Knee Deep), Very Deep Sand (Void, Instant), Very Deep Sand (Void, Slow)
    return specialCode == 10 || specialCode == 11 || specialCode == 16;
}

static bool isHeartDeleteGround(s32 specialCode) {
    // Lava
    return specialCode == 7;
}

static bool isHeartSwimGround(s32 specialCode) {
    return false;
}

// This might be a cM::calcTimer thing...
bool increment(u32 *value) {
    if (*value < UINT32_MAX) {
        (*value)++;
        return true;
    }
    return false;
}

u16 dAcItemBase_c::getItemId() const {
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

static const char *const sResNodeName = "Set";
static const char *const sDefaultGetItem = "DefaultGetItem";
static const char *const sItemGetGorgeous = "ItemGetGorgeous";
static const char *const sItemGetDefaultTbox = "ItemGetDefaultTBox";
static const char *const sItemGetGorgeousTbox = "ItemGetGorgeousTBox";
/* static */ extern const char *const sItemGetBird = "ItemGetBird";

const mVec3_c dAcItem_c::sFreestandingDowsingOffset(0.0f, 25.0f, 0.0f);
const mVec3_c dAcItem_c::sScale1Maybe(1.0f, 1.0f, 1.0f);
const mVec3_c dAcItem_c::sScale2Maybe(1.75f, 1.75f, 1.75f);

struct TearIdIdx {
    dAcItem_c::Tear_e idx;
    u16 itemId;
    mColor color1;
    mColor color2;
};

static TearIdIdx sTearIdxes[] = {
    {  dAcItem_c::TEAR_FARON, ITEM_FARORE_TEAR, mColor(0x00, 0x80, 0x30, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF)},
    {  dAcItem_c::TEAR_ELDIN,    ITEM_DIN_TEAR, mColor(0xFF, 0x64, 0x80, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF)},
    {dAcItem_c::TEAR_LANAYRU,  ITEM_NAYRU_TEAR, mColor(0x80, 0x80, 0x00, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF)},
    {dAcItem_c::TEAR_GODDESS, ITEM_SACRED_TEAR, mColor(0x00, 0x64, 0xC8, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF)},
};

struct TearEffectColorThing {
    mColor color1;
    mColor color2;
    mColor color3;
    mColor color4;
    mColor color5;
    mColor color6;
};

extern "C" TearEffectColorThing sTearEffectColors[] = {
    {mColor(0x80, 0xFF, 0x80, 0xFF), mColor(0x00, 0xFF, 0x40, 0xFF), mColor(0xC8, 0xFF, 0xC8, 0xFF),
     mColor(0x40, 0xFF, 0x40, 0xFF), mColor(0x40, 0xFF, 0x60, 0xFF), mColor(0x00, 0xFF, 0x40, 0xFF)},

    {mColor(0xFF, 0x80, 0xA0, 0xFF), mColor(0xFF, 0x40, 0x80, 0xFF), mColor(0xFF, 0xC8, 0xC8, 0xFF),
     mColor(0xFF, 0x40, 0x80, 0xFF), mColor(0xFF, 0x60, 0x80, 0xFF), mColor(0xFF, 0x00, 0x40, 0xFF)},

    {mColor(0xFF, 0xFF, 0x80, 0xFF), mColor(0xFF, 0xFF, 0x00, 0xFF), mColor(0xFF, 0xFF, 0xC8, 0xFF),
     mColor(0xFF, 0xFF, 0x40, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF)},

    {mColor(0x80, 0xFF, 0xFF, 0xFF), mColor(0x00, 0x80, 0xFF, 0xFF), mColor(0xC8, 0xC8, 0xFF, 0xFF),
     mColor(0x40, 0xA0, 0xFF, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF), mColor(0xFF, 0xFF, 0xFF, 0xFF)},
};

struct UnkFloat {
    f32 f;
    UnkFloat(f32 f_) : f(f_) {}
};

const mVec3_c dAcItem_c::sUnkOffset1(0.0f, 28.0f, 0.0f);
const mVec3_c dAcItem_c::sUnkOffset2(0.0f, 0.0f, 0.0f);
const mVec3_c dAcItem_c::sUnkOffset3(0.0f, 28.0f, 0.0f);
const mVec3_c dAcItem_c::sUnkOffset4(0.0f, 0.0f, 0.0f);
const mVec3_c dAcItem_c::sUnkOffset5(0.0f, 25.0f, 0.0f);
const mVec3_c dAcItem_c::sUnkOffset6(0.0f, 25.0f, 0.0f);
extern const UnkFloat f1(0.25f);
extern const UnkFloat f2(1.0f);

/* static */ extern const char *const sGetFairyBody = "GetFairy_body";
/* static */ extern const char *const sBottleFairy_body = "BottleFairy_body";

SPECIAL_ACTOR_PROFILE(ITEM, dAcItem_c, fProfile::ITEM, 0x2B, 0, 2);

const dAcItem_c::sStaticPtmf dAcItem_c::sStaticPtmfs1[] = {
    &dAcItem_c::fn_80248020, nullptr, &dAcItem_c::fn_80248010, nullptr, &dAcItem_c::fn_80255B30,
    &dAcItem_c::fn_80255BA0,
};

const dAcItem_c::sStaticPtmf dAcItem_c::sStaticPtmfs2[] = {
    &dAcItem_c::fn_80248040, nullptr, &dAcItem_c::fn_80248030, nullptr, &dAcItem_c::fn_80255BD0,
    &dAcItem_c::fn_80255C40,
};

fLiMgBa_c dAcItem_c::sItemList;
dAcRef_c<dAcObjBase_c> dAcItem_c::sItemListHead;
dAcRef_c<dAcObjBase_c> dAcItem_c::sItemListTail;
fLiMgBa_c dAcItem_c::sUnusedList;

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
    delete mpResurgeCtl;
    for (int i = 0; i < 2; i++) {
        delete mpPickupPositionCtl[i];
    }
    delete mpMdl;
    mDowsingTarget.doUnregister();
}

dAcItem_c::Tear_e dAcItem_c::getTearSubtype(u32 id) {
    // If only this could be written in a simpler way...
    bool found = false;
    int i = 0;
    while (!found && i <= (int)ARRAY_LENGTH(sTearIdxes) - 1) {
        if (id == sTearIdxes[i].itemId) {
            found = true;
        } else {
            i++;
        }
    }
    if (found) {
        return sTearIdxes[i].idx;
    }
    return TEAR_MAX;
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

    mpPickupPositionCtl[0] = new dAcItemPickupPositionOn_c();
    if (mpPickupPositionCtl[0] == nullptr) {
        return false;
    }
    mpPickupPositionCtl[1] = new dAcItemPickupPositionOff_c();
    if (mpPickupPositionCtl[1] == nullptr) {
        return false;
    }

    if (getItemInitStruct()->_0x00[6] != 0) {
        mpResurgeCtl = new dAcItemResurgeOn_c(getItemInitStruct()->_0x00[4], getItemInitStruct()->_0x00[5]);
        if (mId == ITEM_STAMINA_FRUIT) {
            mpResurgeCtl->setTimerInitVal(150);
        } else if (isLightFruit()) {
            mpResurgeCtl->setTimerInitVal(0);
        }
    } else {
        mpResurgeCtl = new dAcItemResurgeOff_c(getItemInitStruct()->_0x00[4], getItemInitStruct()->_0x00[5]);
    }

    if (mpResurgeCtl == nullptr) {
        return false;
    }

    return true;
}

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
        if (!StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_GRATITUDE_QUEST_STARTED) ||
            !dScGame_c::currentSpawnInfo.isNight()) {
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
        case 0:  mFn_0xC90 = &dAcItem_c::fn_80254DE0; break;
        case 1:  mFn_0xC90 = &dAcItem_c::fn_80254DF0; break;
        default: return FAILED;
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
        case 0:  mFnAction = &dAcItem_c::moveNormal0; break;
        case 1:  mFnAction = &dAcItem_c::moveNormal1; break;
        case 2:  mFnAction = &dAcItem_c::moveSpecial; break;
        default: return FAILED;
    }

    if (isHeartV()) {
        mFunc_0xB58 = &dAcItem_c::fn_802555F0;
    } else {
        mFunc_0xB58 = &dAcItem_c::fn_802555D0;
    }

    if (isAnyRupee()) {
        mFnBounce = &dAcItem_c::bounceRupee;
    } else if (isHeartV()) {
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
    } else if (!isAnyRupee() && !isHeartV() && mId != ITEM_STAMINA_FRUIT) {
        mCyl.SetTg_0x4C(0x6480);
    }

    if (isBabyRattle()) {
        mCyl.SetTgInfo_0x1(0x13);
    } else if (isHeartPiece()) {
        mCyl.SetTgInfo_0x1(0x15);
    } else if (!isAnyRupee() && !isHeartV() && mId != ITEM_STAMINA_FRUIT) {
        mCyl.SetTgInfo_0x1(0x13);
    }

    if (isHeartPiece()) {
        mCyl.ClrTg_0x200000();
    } else {
        mCyl.OnTg_0x200000();
    }

    mCyl.SetCoCallback(hitCallback);
    mAcchCir.SetWall(
        getItemFlagStruct(getItemId())->field_0x01 * 0.5f, getItemFlagStruct(getItemId())->field_0x00 * 0.6f
    );
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

    switch (getSubtypeFromParam(mParams)) {
        default: mStateMgr.changeState(StateID_Wait); break;
        case 1:  mStateMgr.changeState(StateID_Wait); break;
        case 2:  mStateMgr.changeState(StateID_Wait); break;
        case 3:  onTouchLink(); break;
        case 4:  mStateMgr.changeState(StateID_Get); break;
        case 5:  mStateMgr.changeState(StateID_WaitForcedGetDemo); break;
        case 6:  mStateMgr.changeState(StateID_GetBeetle); break;
        case 7:  mStateMgr.changeState(StateID_WaitTBoxGetDemo); break;
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
        case ITEM_GREEN_RUPEE:  mFnGetRupeeGravity = &dAcItem_c::getGreenRupeeGravity3; break;
        case ITEM_BLUE_RUPEE:   mFnGetRupeeGravity = &dAcItem_c::getBlueRupeeGravity3; break;
        case ITEM_RED_RUPEE:    mFnGetRupeeGravity = &dAcItem_c::getRedRupeeGravity3; break;
        case ITEM_SILVER_RUPEE: mFnGetRupeeGravity = &dAcItem_c::getSilverRupeeGravity3; break;
        case ITEM_GOLD_RUPEE:   mFnGetRupeeGravity = &dAcItem_c::getGoldRupeeGravity3; break;
        case ITEM_RUPOOR:       mFnGetRupeeGravity = &dAcItem_c::getRupoorGravity3; break;
        default:                mFnGetRupeeGravity = nullptr; break;
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

    if (isHeartV()) {
        mFnGetSinkSpeed = &dAcItem_c::getSinkSpeedHeart;
    } else {
        mFnGetSinkSpeed = &dAcItem_c::getSinkSpeedDefault;
    }

    if (mbNoDespawn) {
        dBgS::GetInstance()->SetLightingCode(this, 10.0f);
    } else if (isHeartV()) {
        mLinkage.bushTpFunc(mObjAcch);
    }

    if (isHeartV()) {
        field_0xD20 = 6;
    }

    if (isAnyTear()) {
        mSomeStr = "force";
    }

    if (isAnyTear()) {
        s32 type = getTearSubtype(mId);
        if (type == TEAR_MAX) {
            type = TEAR_FARON;
        }
        dAcObjBase_c *forceSign =
            dAcObjBase_c::create(this, fProfile::OBJ_FORCE_SIGN, type & 0xFF, nullptr, nullptr, nullptr, nullptr, 0x3F);
        if (forceSign != nullptr) {
            mForceSignRef.link(forceSign);
        }
    }

    if (isAnyTear()) {
        dAcBase_c *forceGetFlag = dAcBase_c::createActorStage(
            fProfile::TAG_FORCE_GET_FLAG, dTgForceGetFlag_c::tearIdxToParam(getTearIdx()), &mPosition, &mRotation,
            &mScale, 0, mRoomID, this
        );
        if (forceGetFlag == nullptr) {
            return FAILED;
        }
    }

    if (mId == ITEM_STAMINA_FRUIT && unk) {
        u32 p1 = (getFirstBitParams2() & 0x1) << 8;
        u32 parms = ((mParams >> 0x13) & 0x1) | (p1 & ~0x1);
        dAcObjBase_c *leaves =
            dAcObjBase_c::create(fProfile::OBJ_FRUIT_GUTS_LEAF, mRoomID, parms, &mPosition, &mRotation, &mScale, -1);
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
        setItemFlags(mItemFlags | 0x20);
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
        setItemFlags(mItemFlags | 0x80);
    }

    field_0xD48 = cM::rndInt(0x10000);
    mColorFilter.r = 0;
    mColorFilter.g = 0;
    mColorFilter.b = 0;
    mLightLevelMaybe = 0.0f;

    return SUCCEEDED;
}

int dAcItem_c::doDelete() {
    sItemList.remove(&this->mNode);
    dAcItem_c *prev = mItemQueuePrev.get();
    dAcItem_c *next = mItemQueueNext.get();

    if (prev == nullptr) {
        if (next == nullptr) {
            if (this == sItemListHead.get()) {
                sItemListHead.unlink();
            }
            if (this == sItemListTail.get()) {
                sItemListTail.unlink();
            }
        } else {
            sItemListHead.link(next);
            next->mItemQueuePrev.unlink();
        }
    } else if (next == nullptr) {
        sItemListTail.link(prev);
        prev->mItemQueueNext.unlink();
    } else {
        prev->mItemQueueNext.link(next);
        next->mItemQueuePrev.link(prev);
    }

    return SUCCEEDED;
}

int dAcItem_c::actorExecute() {
    if (!checkActorProperty(AC_PROP_0x10000000)) {
        if (mId == ITEM_STAMINA_FRUIT) {
            if (isFirstBitParams2NotSet() && !fn_802577A0()) {
                field_0xCD8 = 0.0f;
            } else {
                field_0xCD8 = 1.0f;
            }
        } else if (isLightFruit()) {
            if ((this->*mFnAction4)()) {
                field_0xCD8 = 1.0f;
            } else {
                field_0xCD8 = 0.0f;
            }
        }

        dAcOScatterSand *ac =
            static_cast<dAcOScatterSand *>(fManager_c::searchBaseByProfName(fProfile::OBJ_VSD, nullptr));
        bool keepGoing = true;
        while (keepGoing && ac != nullptr) {
            if (mPosition.squareDistance(ac->mPosition) < 90000.0f) {
                mCoveredSand.link(ac);
                keepGoing = false;
            } else {
                ac = static_cast<dAcOScatterSand *>(fManager_c::searchBaseByProfName(fProfile::OBJ_VSD, ac));
            }
        }
    }

    if (field_0xD4C < 0xFF) {
        field_0xD4C++;
    }

    fn_80254BC0();
    fn_80255B10();

    field_0xD5C = 0;
    field_0xD66 = 0;

    mStateMgr.executeState();

    updateLightingMaybe();
    if (mDespawnTimer == 0) {
        deleteRequest();
        return SUCCEEDED;
    }

    if ((mItemFlags & 0x8) != 0) {
        deleteRequest();
        return SUCCEEDED;
    }

    mRotation.y += field_0xD38;
    f32 scaleF = getCurrentScale();
    mScale.set(scaleF, scaleF, scaleF);
    mVec3_c tmp1;
    fn_802518C0(&tmp1);
    mPositionCopy2.set(tmp1);
    mVec3_c tmp2;
    fn_802518C0(&tmp2);
    mPositionCopy3.set(tmp2);

    mLinkage.fn_800511E0(this);

    if (!checkObjectProperty(OBJ_PROP_0x200)) {
        mMtx_c mtx = mWorldMtx;
        mtx.YrotM(mMdlRotY);
        mtx.ZrotM(field_0xD42);

        if (isGratitudeCrystal() || isUnkTreasureGroup1()) {
            if (mStateMgr.isState(StateID_Wait) || mStateMgr.isState(StateID_Carry)) {
                f32 off = mFreestandingOffsetH;
                f32 sc = getCurrentScale();
                mtx.m[1][3] += sc * off;
            }
        } else if (isInsect()) {
            if (mStateMgr.isState(StateID_Get)) {
                mtx.YrotM(-0x2000);
            }
        }

        mpMdl->setLocalMtx(mtx);
        mpMdl->setScale(mScale);

        if (isFairyV() || isBottleFairyV()) {
            m3d::smdl_c *mdl = mpMdl->getMdl();
            if (mdl != nullptr) {
                mdl->calc(false);
            }
        }
    }

    field_0xD51 = 0;
    mCyl.ClrTgHit();
    mCyl.ClrCoHit();
    field_0xD65 = 0;
    if ((isAnyRupeeV() || mId == ITEM_STAMINA_FRUIT) && field_0xD66 && mObjAcch.ChkGroundLanding()) {
        dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, 0.7f, field_0x1B0);
    }

    if (isStarryFireflyV() && !checkObjectProperty(OBJ_PROP_0x200)) {
        fn_802546A0();
    }

    if (isFairyV() && !checkObjectProperty(OBJ_PROP_0x200)) {
        fn_80254710();
    }

    if (isBottleFairyV() && !checkObjectProperty(OBJ_PROP_0x200)) {
        fn_80254790();
    }

    if (checkAbovePosition()) {
        mItemFlags |= 0x10;
        f32 f = mPosition.y - dBgS_ObjGndChk::GetGroundHeight();
        field_0xD18 = f < 0.0f ? 0.0f : f;
    } else {
        mItemFlags &= ~0x10;
    }

    applyBoundingBox();

    if (isField_0xD04GtZero() || mStateMgr.isState(StateID_WaitGetDemo) || mStateMgr.isState(StateID_Get) ||
        mStateMgr.isState(StateID_GetBeetle)) {
        setActorProperty(AC_PROP_0x4);
    } else {
        unsetActorProperty(AC_PROP_0x4);
    }

    return SUCCEEDED;
}

int dAcItem_c::draw() {
    if (mMdlScaleType == 2 || 0.0f < field_0xCD8) {
        bool shouldDraw = true;
        if (isMdlHidden()) {
            shouldDraw = false;
        } else if ((mStateMgr.isState(StateID_Wait) || mStateMgr.isState(StateID_Carry)) && mbIsWaiting) {
            shouldDraw = isBlinkBeforeDespawnShown(mDespawnTimer);
        }
        if (shouldDraw) {
            mpMdl->draw();
        }

        if (mLinkage.checkConnection(dLinkage_c::CONNECTION_9) ||
            (mId == ITEM_STAMINA_FRUIT && getParams2Lower_shift1_0x7()) || isLightFruit() || (mItemFlags & 0x10) == 0) {
            /* nothing */
        } else if (mMdlScaleType == 2) {
            mQuat_c v(mVec3_c(0.0f, 0.0f, 0.0f), getItemFlagStruct(getItemId())->field_0x02);
            drawShadow(mShdw, nullptr, mWorldMtx, &v, -1, -1, -1, -1, -1, field_0xD18);
        } else {
            mQuat_c v(
                mVec3_c(0.0f, field_0xCD8 * 0.5f * getItemFlagStruct(getItemId())->field_0x01, 0.0f),
                field_0xCD8 * getItemFlagStruct(getItemId())->field_0x02
            );
            drawShadow(mShdw, nullptr, mWorldMtx, &v, -1, -1, -1, -1, -1, field_0xD18);
        }
    }
    return SUCCEEDED;
}

void dAcItem_c::setSpawnQuantity(s32 quantity) {
    sGetItemQuantity = quantity;
}

void dAcItem_c::setFreestandingYOffset(f32 off) {
    mFreestandingOffsetH = off;
}

void dAcItem_c::setFramesInAir(u32 numFrames) {
    mFramesInAir = numFrames;
}

void dAcItem_c::setItemFlags(u32 flags) {
    mItemFlags = flags;
}

bool dAcItem_c::isItemDropFromEnemy() const {
    return mObjID != 0xFFFF;
}

void dAcItem_c::unsetSpawnQuantity() {
    sGetItemQuantity = 0;
}

void dAcItem_c::unsetFramesInAir() {
    setFramesInAir(0);
}

void dAcItem_c::incrementFramesInAir() {
    u32 v = mFramesInAir;
    increment(&v);
    setFramesInAir(v);
}

void dAcItem_c::initializeState_Wait() {
    if (mbNoGravity) {
        setActorProperty(AC_PROP_0x1);
        mSpeed = 0.0f;
        mVelocity.y = 0.0f;
    }
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Put);
    field_0xD5B = 0;
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    fn_80253E20();
    if (mbNoGravity) {
        mObjAcch.SetMoveBGOnly();
    }
    mMdlScaleType = 0;
    mSinkOffset = 0.0f;
    mYOffset = 0.0f;
}

void dAcItem_c::executeState_Wait() {
    bool isFixedPosition;

    bool tmp = false;
    mVec3_c topPos = getPosition() + mVec3_c::Ey * mCyl.GetH();
    bool chk = dBgS_ObjRoofChk::CheckPos(&getPosition());
    if (chk) {
        tmp = dBgS_ObjRoofChk::GetRoofHeight() < topPos.y;
    }
    if (tmp) {
        isFixedPosition = false;
    } else {
        f32 f = chk ? dBgS_ObjRoofChk::GetRoofHeight() : 500.0f;
        mVec3_c chkPos = getPosition() + mVec3_c::Ey * f;
        if (dBgS_ObjGndChk::CheckPos(chkPos)) {
            if (topPos.y < dBgS_ObjGndChk::GetGroundHeight()) {
                const dAcObjBase_c *obj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjGndChk::GetInstance());
                if (obj == nullptr) {
                    isFixedPosition = false;
                } else {
                    isFixedPosition = obj->mProfileName == fProfile::OBJ_DESERT;
                }
            } else {
                isFixedPosition = false;
            }
        } else {
            isFixedPosition = false;
        }
    }

    if (isBabyRattle() && !isFixedPosition) {
        isFixedPosition = mCoveredSand.isLinked();
    }

    if (fn_80255CF0() && field_0xD4C > 16 && (!mbNoGravity || !isFixedPosition)) {
        mCyl.OnTgCoFlag(0x1);
    } else {
        mCyl.OffTgCoFlag(0x1);
    }

    if ((mItemFlags & 0x80) != 0) {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, 0x1D)) {
            mCyl.OnTgType(0x2);
        } else {
            mCyl.OffTgType(0x2);
        }
    }

    dAcPy_c *link = dAcPy_c::GetLinkM();
    bool tgHit = mCyl.ChkTgHit();
    bool tgBit1;
    if (tgHit) {
        tgBit1 = mCyl.ChkTgBit1();
    } else {
        tgBit1 = false;
    }
    if (!link->checkFlags0x340(0x10)) {
        if (field_0xD65 || (tgHit && (mCyl.ChkTgAtHitType(AT_TYPE_SWORD) || mCyl.ChkTgAtHitType(AT_TYPE_BUGNET)))) {
            onTouchLink();
            return;
        }

        if (fn_802574A0() && !tgBit1) {
            // TODO weird double load of link
            link = dAcPy_c::GetLinkM();
            if (!link->getBeetleInFlight()) {
                addToGetQueue();

                if ((link->ifHasHealthAndSomethingElse() && this == sItemListHead.get() &&
                     FileManager::GetInstance()->getCurrentHealth() != 0)) {
                    if (isSmallKey() || isBabyRattle()) {
                        mStateMgr.changeState(StateID_WaitForcedGetDemo);
                    } else {
                        mStateMgr.changeState(StateID_Get);
                    }
                } else {
                    mStateMgr.changeState(StateID_Get);
                }
            } else {
                mStateMgr.changeState(StateID_GetBeetle);
            }

            return;
        }
    }

    u32 fruitParam = getParams2Lower_shift1_0x7();
    if (tgHit) {
        if (mId == ITEM_STAMINA_FRUIT) {
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_373_, mPosition, &mRotation, &mScale, nullptr, nullptr, 0, 0
            );
            startSound(SE_O_REFRESH_FRUIT_BREAK);
            if (!fruitParam) {
                deleteRequest();
            } else {
                mStateMgr.changeState(StateID_ResurgeWait);
            }
            return;
        } else if (isLightFruit()) {
            mStateMgr.changeState(StateID_ResurgeWait);
            return;
        }

        if (mCyl.ChkTgAtHitType(AT_TYPE_GLITTERING_SPORES)) {
            if (isHeart()) {
                if (fn_8024A230()) {
                    mVec3_c spawnPos(mPosition.x, mPosition.y + 30.0f, mPosition.z);
                    dAcObjBase_c::create(fProfile::OBJ_FAIRY, mRoomID, 0x3F, &spawnPos, nullptr, nullptr, -1);
                    dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_375_, spawnPos, nullptr, nullptr, nullptr, nullptr, 0, 0
                    );
                    deleteRequest();
                    return;
                }
            } else {
                s32 accWeight = 0;
                for (int i = 0; i <= (int)ARRAY_LENGTH(sSporeRupeeChances) - 1; i++) {
                    if (getItemId() != sSporeRupeeChances[i].itemId) {
                        accWeight += sSporeRupeeChances[i].weight;
                    }
                }
                s32 roll = cM::rndInt(accWeight);
                s32 acc = 0;
                bool found = false;
                int i = 0;
                while (!found && i <= (int)ARRAY_LENGTH(sSporeRupeeChances) - 1) {
                    if (getItemId() == sSporeRupeeChances[i].itemId) {
                        i++;
                    } else {
                        acc += sSporeRupeeChances[i].weight;
                        if (roll < acc) {
                            found = true;
                        } else {
                            i++;
                        }
                    }
                }
                setItemId(sSporeRupeeChances[i].itemId);
                mpMdl->changeItemId(sSporeRupeeChances[i].itemId);
                unsetHaveNoGravity();
                mVelocity.y = 10.0f;
                fn_80252A80();
            }
        }
    }

    tickDespawnTimer();
    if (mDespawnTimer == 0) {
        return;
    }

    if (mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        mStateMgr.changeState(StateID_Carry);
        return;
    }

    fn_80253D50();

    if (mId == ITEM_STAMINA_FRUIT) {
        if (isFirstBitParams2NotSet() && !fn_802577A0()) {
            sLib::chase(&field_0xCD8, 0.0f, 0.05f);
        } else {
            sLib::chase(&field_0xCD8, 1.0f, 0.05f);
        }
    } else if (isLightFruit()) {
        if ((this->*mFnAction4)()) {
            sLib::chase(&field_0xCD8, 1.0f, 0.05f);
        } else {
            field_0xCD8 = 0.0f;
        }
    }

    bool tgHitArrow = tgHit && mCyl.ChkTgAtHitType(AT_TYPE_ARROW);
    bool tgHitClawshot = tgHit && mCyl.ChkTgAtHitType(AT_TYPE_CLAWSHOT);
    if (mbNoGravity && tgHit &&
        ((isAnyRupee() && tgHitArrow) || (isHeartV() && mCyl.ChkTgAtHitType(AT_TYPE_BELLOWS)) ||
         ((isAnyRupee() || isHeartV()) &&
          (mCyl.ChkTgAtHitType(AT_TYPE_SLINGSHOT) || mCyl.ChkTgAtHitType(AT_TYPE_CLAWSHOT))) ||
         mCyl.ChkTgAtHitType(AT_TYPE_BOMB) || mCyl.ChkTgAtHitType(AT_TYPE_0x800000))) {
        unsetHaveNoGravity();
    }

    if (mbNoGravity && mCyl.ChkCoHit()) {
        unsetHaveNoGravity();
    }

    if (!mbNoGravity) {
        unsetActorProperty(AC_PROP_0x1);
    }

    if (mbNoGravity) {
        mObjAcch.SetMoveBGOnly();
    } else {
        mObjAcch.ClrMoveBGOnly();
    }

    if (tgHitArrow) {
        field_0xD62 = true;
    } else if (tgHit || (!mObjAcch.ChkGroundLanding() && mObjAcch.ChkGndHit())) {
        field_0xD62 = false;
    }
    if (tgHitClawshot) {
        field_0xD63 = true;
    } else if (tgHit || (!mObjAcch.ChkGroundLanding() && mObjAcch.ChkGndHit())) {
        field_0xD63 = false;
    }

    if (!mbNoGravity) {
        if (!field_0xD51) {
            if (isAnyRupee()) {
                s32 which;
                if (field_0xD62 || field_0xD63) {
                    which = 1;
                } else {
                    which = 2;
                }
                if (which == 1 && mFnGetRupeeGravity == nullptr) {
                    which = 2;
                }
                if (which == 2 && mFnGetGravity1 == nullptr) {
                    which = 0;
                }

                switch (which) {
                    case 1: mAcceleration = (this->*mFnGetRupeeGravity)(); break;
                    case 2: mAcceleration = (this->*mFnGetGravity1)(); break;
                }
            } else if (isHeartV()) {
                if (field_0xD52) {
                    if (mFnGetHeartGravity != nullptr) {
                        mAcceleration = (this->*mFnGetHeartGravity)();
                    }
                } else {
                    if (mFnGetGravity1 != nullptr) {
                        mAcceleration = (this->*mFnGetGravity1)();
                    }
                }
            } else {
                if (mFnGetRupeeGravity == nullptr || !field_0xD62) {
                    if (mFnGetGravity1 != nullptr) {
                        mAcceleration = (this->*mFnGetGravity1)();
                    }
                } else {
                    if (mFnGetRupeeGravity != nullptr) {
                        mAcceleration = (this->*mFnGetRupeeGravity)();
                    }
                }
            }

            if (mFnGetGravity4 != nullptr) {
                mMaxSpeed = (this->*mFnGetGravity4)();
            }

            fn_80256F20();
            (this->*mFnAction)();
            calcVelocity();
            mPosition += mVelocity;
            mPosition += mStts.GetCcMove();
        }

        if ((mId == ITEM_STAMINA_FRUIT && fruitParam) || isLightFruit()) {
            /* nothing */
        } else {
            if (!mbNoGravity && !field_0xD51 && 0.0f < mVelocity.y) {
                mObjAcch.ClrRoofNone();
                mObjAcch.Clr_0x2000000();
            } else {
                mObjAcch.SetRoofNone();
                mObjAcch.Set_0x2000000();
            }

            fn_80256E80();

            if (mObjAcch.ChkGndHit() && mVelocity.y <= 0.0f) {
                u32 specialCode = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());
                if (isItemDeleteGround(specialCode)) {
                    if (sLib::chase(&mSinkOffset, -100.0f, (this->*mFnGetSinkSpeed)())) {
                        deleteRequest();
                    }
                } else if (isItemSwimGround(specialCode)) {
                    sLib::chase(&mSinkOffset, -30.0f, (this->*mFnGetSinkSpeed)());
                } else {
                    sLib::chase(&mSinkOffset, 0.0f, 6.0f);
                }
            } else {
                mSinkOffset = 0.0f;
            }

            mYOffset = mSinkOffset;
            if (isAnyTear() && mObjAcch.ChkGroundLanding()) {
                startSoundWithFloatParam(SE_Item_A43_FALL, mVelocity.y);
            }
        }
    }

    /* This Function keeps Triforce postion locked 8024edbc */
    if (isTriforce()) {
        f32 factor = field_0xD48.sin();
        mPosition = mPositionCopy + mVec3_c::Ey * (factor * 16.0f);
        field_0xD48.mVal += 182;
    }

    if ((mItemFlags & 0x4) != 0 && 3000.0f < mPositionCopy.y - mPosition.y) {
        mItemFlags |= 0x8;
    }

    if ((mItemFlags & 0x8) == 0) {
        (this->*mFnAction3)();
        if (isAnyRupee() && field_0xD3A > 0) {
            holdSoundWithIntParam(SE_O_RUPEE_ROULETTE, field_0xD3A);
        }

        if (!field_0xD51) {
            mCyl.SetC(mPosition);
            dCcS::GetInstance()->Set(&mCyl);
        }

        if (isSmallKey()) {
            if (dScGame_c::isCurrentStage("F302")) {
                f32 fScale;
                getCurrentModelScale(&fScale);
                mVec3_c scale(fScale, fScale, fScale);
                mEff_0x8F4.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_820_, mPosition, nullptr, &scale, nullptr, nullptr);
            }
        } else if (isTriforce()) {
            f32 fScale;
            getCurrentModelScale(&fScale);
            mVec3_c scale(fScale, fScale, fScale);
            mVec3_c offsetPos(mPosition.x, fScale * 20.0f + mPosition.y, mPosition.z);
            dCamera_c *cam = dScGame_c::getCamera();
            mVec3_c dir = offsetPos - cam->getPosition();
            if (!dir.normalizeRS()) {
                dir = cam->getTarget() - cam->getPosition();
                if (!dir.normalizeRS()) {
                    dir.set(mVec3_c::Ez);
                    dir.rotX(cam->getYAngle());
                    dir.rotY(cam->getXZAngle());
                }
            }
            dir *= -40.0f;
            mVec3_c efPos = offsetPos + dir;
            mEff_0x928.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_821_, efPos, nullptr, &scale, nullptr, nullptr);
        }
        makeLinkLookTowardItem();
        if (!mbNoDespawn && !isItemSmallKeyOrHeartPieceOrStaminaFruit()) {
            fn_80254CA0();
        }
    }
}

void dAcItem_c::finalizeState_Wait() {
    unsetActorProperty(AC_PROP_0x1);
    mObjAcch.SetRoofNone();
    mObjAcch.Clr_0x2000000();
    mSinkOffset = 0.0f;
    mYOffset = 0.0f;
    if (isTriforce() && mEff_0x928.hasEmitters()) {
        mEff_0x928.remove(true);
    }
}

void dAcItem_c::initializeState_Carry() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Put);
    mSpeed = 0.0f;
    mVelocity.y = 0.0f;
    unsetHaveNoGravity();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mCyl.OnTgSet();
    mCyl.ClrCoSet();
    fn_80254BA0();
    field_0xD4A = 0;
    mMdlScaleType = 0;
}

void dAcItem_c::executeState_Carry() {
    if (!mbNoDespawn && !isItemSmallKeyOrHeartPieceOrStaminaFruit() && !mbIsWaiting &&
        mLinkage.checkConnection(dLinkage_c::CONNECTION_8)) {
        fn_80254CA0();
    }
    tickDespawnTimer();
    if (mDespawnTimer == 0) {
        return;
    }

    if (fn_802574A0()) {
        // TODO weird double load
        dAcPy_c *link = dAcPy_c::GetLinkM();
        if (!link->getBeetleInFlight()) {
            addToGetQueue();
            if (link->ifHasHealthAndSomethingElse() && this == sItemListHead.get() &&
                FileManager::sInstance->getCurrentHealth() != 0) {
                mStateMgr.changeState(StateID_Get);
            } else {
                mStateMgr.changeState(StateID_WaitGet);
            }
        } else {
            mStateMgr.changeState(StateID_GetBeetle);
        }
    } else if (!mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        mStateMgr.changeState(StateID_Wait);
    } else if (mLinkage.checkConnection(dLinkage_c::CONNECTION_9) && mCyl.ChkTgHit() &&
               !mCyl.ChkTgAtHitType(AT_TYPE_WHIP)) {
        mLinkage.forceRemove(this);
        mStateMgr.changeState(StateID_Wait);
    } else {
        fn_80256E80();
        if (mLinkage.checkConnection(dLinkage_c::CONNECTION_9) || mLinkage.checkConnection(dLinkage_c::CONNECTION_8)) {
            mCyl.SetC(mPosition);
            dCcS::GetInstance()->Set(&mCyl);
        }

        if (mLinkage.checkConnection(dLinkage_c::CONNECTION_7)) {
            makeLinkLookTowardItem();
        }
    }
}

void dAcItem_c::finalizeState_Carry() {
    setupUnkColliderFlags2();
}

void dAcItem_c::initializeState_GetBeetle() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Put);
    setNotWaiting();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    field_0xD0C = 0.0f;
    if (isAnyRupee()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BE_GET_RUPEE);
    } else if (isHeartV()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BE_GET_HART);
    } else if (isSmallKey()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BE_GET_KEY);
    } else if (mId == ITEM_STAMINA_FRUIT) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BE_GET_REFRESH_FRUIT);
    } else if (isUnkTreasureGroup1()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BE_GET_MATERIAL);
    } else {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BE_GET_CONSUMP_ITEM);
    }

    addToGetQueue();
    mMdlScaleType = 3;
    fn_802548A0();
}

void dAcItem_c::executeState_GetBeetle() {
    // TODO weird double load
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link == nullptr) {
        return;
    }
    dAcBoomerang_c *beetlePtr = (dAcBoomerang_c *)link->getBeetleInFlight();
    if (beetlePtr == nullptr) {
        if (link->ifHasHealthAndSomethingElse() && this == sItemListHead.get() &&
            FileManager::sInstance->getCurrentHealth() != 0) {
            decideOnGetOrForcedGet();
        } else {
            mStateMgr.changeState(StateID_WaitGet);
        }
    } else if (37.6f - field_0xD0C < 1.6f) {
        // TODO what are these numbers
        mStateMgr.changeState(StateID_WaitGet);
    } else {
        mVec3_c v(0.0f, 5.0f, 0.0f);
        // This + is weird... inline?
        v.rotY(+beetlePtr->mRotation.y);
        mPosition = beetlePtr->mPosition + v;
        mPosition.y += field_0xD0C;
        rotateTowardsCamera();
        sLib::addCalcScaledDiff(&field_0xD0C, 37.6f, 0.1f, 10000.0f);
    }
}

void dAcItem_c::finalizeState_GetBeetle() {}

void dAcItem_c::initializeState_WaitGet() {
    field_0xD64 = 0;
    setObjectProperty(OBJ_PROP_0x200);
    // TODO double load
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link != nullptr) {
        mPosition.set(link->getPosition());
    }
    mMdlScaleType = 1;
    fn_802548A0();
}

void dAcItem_c::executeState_WaitGet() {
    // TODO double load
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link != nullptr) {
        if (link->ifHasHealthAndSomethingElse()) {
            field_0xD64 = true;
        }
        if (field_0xD64 && this == sItemListHead.get() && FileManager::GetInstance()->getCurrentHealth() != 0) {
            decideOnGetOrForcedGet();
        }
    }
}

void dAcItem_c::finalizeState_WaitGet() {
    unsetObjectProperty(OBJ_PROP_0x200);
}

void dAcItem_c::initializeState_Get() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Put);
    performCollection();
    if (isAnyTear()) {
        mVec3_c tmp;
        fn_80256710(tmp);
        fn_80257560(tmp);
    }
    fn_80254D10();
    fn_80254D70();
    setNotWaiting();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    field_0xD4D = false;
    mMdlScaleType = 1;
    fn_802548A0();
}

void dAcItem_c::executeState_Get() {
    if (!fn_80254D70()) {
        fn_802542E0();
    } else {
        // @bug always true
        if (field_0xD4D < 0xFFFF) {
            field_0xD4D++;
        }
        if (field_0xD4D > 12) {
            removeFromGetQueue();
        }
    }
}

void dAcItem_c::finalizeState_Get() {
    removeFromGetQueue();
}

void dAcItem_c::initializeState_WaitGetDemo() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Get);
    setObjectProperty(OBJ_PROP_0x200);
    setNotWaiting();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mMdlScaleType = 2;
    fn_802548A0();
    unsetFramesInAir();
}

void dAcItem_c::executeState_WaitGetDemo() {
    s32 which = 0;
    if (EventManager::isInEvent()) {
        which = 2;
    } else if (fn_80247BB0()) {
        which = 2;
    } else if ((isInsect() || isTreasure3()) && checkTreasureTempCollect(getItemId())) {
        which = 2;
    } else if (checkShouldDemo() && fn_80254EC0()) {
        which = 1;
    } else if (mFramesInAir > 10) {
        which = 2;
    }

    switch (which) {
        case 1: addGetEvent(); break;
        case 2:
            unsetObjectProperty(OBJ_PROP_0x200);
            mStateMgr.changeState(StateID_Get);
            break;
    }
    unsetFramesInAir();
}

void dAcItem_c::finalizeState_WaitGetDemo() {}

void dAcItem_c::initializeState_WaitForcedGetDemo() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Get);
    setObjectProperty(OBJ_PROP_0x200);
    setNotWaiting();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mMdlScaleType = 2;
    fn_802548A0();
}

void dAcItem_c::executeState_WaitForcedGetDemo() {
    if (isAnyTear() && mFramesInAir < 10) {
        incrementFramesInAir();
        if (mFramesInAir == 10) {
            fn_80257560(mPosition);
        }
    }
    addGetEvent();
}

void dAcItem_c::finalizeState_WaitForcedGetDemo() {}

void dAcItem_c::initializeState_GetDemo() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Get);
    switch (getItemId()) {
        case ITEM_FARON_GRASSHOPPER:     sCollectionCurrentCount = getFaronGrasshooperCounter(); break;
        case ITEM_WOODLAND_RHINO_BEETLE: sCollectionCurrentCount = getWoodlandRhinoBeetleCounter(); break;
        case ITEM_DEKU_HORNET:           sCollectionCurrentCount = getDekuHornetCounter(); break;
        case ITEM_SKYLOFT_MANTIS:        sCollectionCurrentCount = getSkyloftMantisCounter(); break;
        case ITEM_VOLCANIC_LADYBUG:      sCollectionCurrentCount = getVolcanicLadybugCounter(); break;
        case ITEM_BLESSED_BUTTERFLY:     sCollectionCurrentCount = getBlessedButterflyCounter(); break;
        case ITEM_LANAYRU_ANT:           sCollectionCurrentCount = getLanayruAntCounter(); break;
        case ITEM_SAND_CICADA:           sCollectionCurrentCount = getSandCicadaCounter(); break;
        case ITEM_GERUDO_DRAGONFLY:      sCollectionCurrentCount = getGerudoDragonflyCounter(); break;
        case ITEM_ELDIN_ROLLER:          sCollectionCurrentCount = getEldinRollerCounter(); break;
        case ITEM_SKY_STAG_BEETLE:       sCollectionCurrentCount = getSkyStagBeetleCounter(); break;
        case ITEM_STARRY_FIREFLY:        sCollectionCurrentCount = getStarryFireflyCounter(); break;
        case ITEM_HORNET_LARVAE:         sCollectionCurrentCount = getHornetLarvaeCounter(); break;
        case ITEM_BIRD_FEATHER:          sCollectionCurrentCount = getBirdFeatherCounter(); break;
        case ITEM_TUMBLE_WEED:           sCollectionCurrentCount = getTumbleWeedCounter(); break;
        case ITEM_LIZARD_TAIL:           sCollectionCurrentCount = getLizardTailCounter(); break;
        case ITEM_ELDIN_ORE:             sCollectionCurrentCount = getEldinOreCounter(); break;
        case ITEM_ANCIENT_FLOWER:        sCollectionCurrentCount = getAncientFlowerCounter(); break;
        case ITEM_AMBER_RELIC:           sCollectionCurrentCount = getAmberRelicCounter(); break;
        case ITEM_DUSK_RELIC:            sCollectionCurrentCount = getDuskRelicCounter(); break;
        case ITEM_JELLY_BLOB:            sCollectionCurrentCount = getJellyBlobCounter(); break;
        case ITEM_MONSTER_CLAW:          sCollectionCurrentCount = getMonsterClawCounter(); break;
        case ITEM_MONSTER_HORN:          sCollectionCurrentCount = getMonsterHornCounter(); break;
        case ITEM_ORNAMENTAL_SKULL:      sCollectionCurrentCount = getSkullCounter(); break;
        case ITEM_EVIL_CRYSTAL:          sCollectionCurrentCount = getEvilCrystalCounter(); break;
        case ITEM_BLUE_BIRD_FEATHER:     sCollectionCurrentCount = getBlueBirdFeatherCounter(); break;
        case ITEM_GOLDEN_SKULL:          sCollectionCurrentCount = getGoldenSkullCounter(); break;
        case ITEM_GODDESS_PLUME:         sCollectionCurrentCount = getGoddessPlumeCounter(); break;
    }

    performCollectionPart1();
    if (checkItemFlagV()) {
        if (isAnyTear()) {
            dAcOWarp_c *warp = static_cast<dAcOWarp_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_WARP));
            if (warp != nullptr) {
                warp->onCollectFirstTear();
            }
        }
    } else {
        bool doCollect = true;
        if (ITEM_FARON_GRASSHOPPER <= mId && mId <= ITEM_STARRY_FIREFLY &&
            MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
            doCollect = false;
        }
        if (doCollect) {
            setItemFlagV();
        }
    }

    setTreasureTempCollect(getItemId());
    setNotWaiting();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mMdlRotY = 0;
    setObjectProperty(OBJ_PROP_0x200);
    mMdlScaleType = 2;
    if (isBirdStatuette()) {
        mFn_0xC9C = &dAcItem_c::rotateFixedBirdStatuette;
    } else {
        mFn_0xC9C = &dAcItem_c::rotateTowardsCamera;
    }
}

void dAcItem_c::executeState_GetDemo() {
    switch (mEventRelated.getCurrentEventCommand()) {
        case 'act0': mEventRelated.advanceNext(); break;
        case 'wait': mEventRelated.advanceNext(); break;
        case 'halo': {
            mVec3_c efPos;
            fn_80254590(efPos);
            mVec3_c diff = dScGame_c::getCamera()->getPosition() - mPosition;
            // TODO close but an extsh is missing
            // Same pattern as in rotateTowardsCamera
            mAng3_c rot;
            rot.set(-diff.atan2sY_XZ(), diff.atan2sX_Z(), mAng(0));
            u32 alpha = 0xFF;
            if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
                alpha = 0x80;
            }
            mEff_0x95C.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_82_, efPos, &rot, nullptr, nullptr, nullptr);
            mEff_0x95C.setGlobalAlpha(alpha);
            mEventRelated.advanceNext();
            break;
        }
        case '????': break;
        default:     mEventRelated.advanceNext(); break;
    }

    if (!checkObjectProperty(OBJ_PROP_0x200)) {
        mPosition = getLinkPtr()->vt_0x1A4();
        if (isUpgradedPotion((ITEM_ID)getItemId())) {
            fn_80254680();
        } else if (isAnyTear()) {
            fn_80254810();
        }
        mbShowItemLighting = true;
        field_0x340 = mPosition;
        (this->*mFn_0xC9C)();
        if (!isGratitudeCrystal() && !is5GratitudeCrystals()) {
            mMdlRotY += getItemRotateAngle();
        }
        field_0xD42 = mAng::d2s_c(0.0f);

        // TODO - ...
    }
    makeLinkLookTowardItem();
}

void dAcItem_c::finalizeState_GetDemo() {
    mMdlRotY = 0;
    field_0xD42 = 0;
    performCollectionPart2();
    mbShowItemLighting = false;
    if (mEff_0x95C.hasEmitters()) {
        mEff_0x95C.remove(true);
    }
    if (isAnyTear() && mEff_0xA60.hasEmitters()) {
        mEff_0xA60.remove(true);
    }
}

void dAcItem_c::initializeState_WaitTBoxGetDemo() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Get);
    setNotWaiting();
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    setObjectProperty(OBJ_PROP_0x200);
    mMdlScaleType = 2;
    fn_802548A0();
}

void dAcItem_c::executeState_WaitTBoxGetDemo() {
    const char *name;
    getItemGetTboxEventName(getItemId(), &name);
    Event ev(name, 1, 0x100001, (void *)itemGetEventStart, (void *)itemGetEventEnd);
    EventManager::alsoSetAsCurrentEvent(this, &ev, nullptr);
}

void dAcItem_c::finalizeState_WaitTBoxGetDemo() {}

void dAcItem_c::initializeState_ResurgeWait() {
    mpMdl->setDrawMode(dItemMdl_c::DrawMode_Put);
    // TODO FPR swap
    mPosition = mPositionCopy;
    mRotation = mRotationCopy;
    mpResurgeCtl->startResurgeTimer();
    setNotWaiting();
    // TODO ???
    mbNoGravity = mbNoDespawn;
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mMdlScaleType = 0;
    field_0xCD8 = 0.0f;
}

void dAcItem_c::executeState_ResurgeWait() {
    if (mpResurgeCtl->canResurge(this)) {
        if (!mpResurgeCtl->isTimerExpired()) {
            mpResurgeCtl->execute();
            if (mpResurgeCtl->isTimerExpired()) {
                if (mId == ITEM_STAMINA_FRUIT) {
                    startSound(SE_O_REFRESH_FRUIT_SPROUT);
                }
                mStateMgr.changeState(StateID_Wait);
            }
        }
    } else {
        mpResurgeCtl->resetTimer();
    }
}

void dAcItem_c::finalizeState_ResurgeWait() {}

void dAcItem_c::initializeState_WaitTurnOff() {
    setNotWaiting();
    setObjectProperty(OBJ_PROP_0x200);
    mSpeed = 0.0f;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mMdlScaleType = 2;
}

void dAcItem_c::executeState_WaitTurnOff() {
    if (!isField_0xD04GtZero()) {
        deleteRequest();
    }
}

void dAcItem_c::finalizeState_WaitTurnOff() {}

void dAcItem_c::initializeState_WaitSacredDewGetEffect() {
    setNotWaiting();
    setObjectProperty(OBJ_PROP_0x200);
    mSpeed = 0.0f;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    field_0xD38 = field_0xD3A = field_0xD3C = 0;
    mMdlScaleType = 2;
}

void dAcItem_c::executeState_WaitSacredDewGetEffect() {
    if (!EventManager::isInEvent()) {
        mVec3_c tmp;
        fn_80247540(tmp);
        fn_80247560(tmp);
        mStateMgr.changeState(StateID_WaitTurnOff);
    }
}

void dAcItem_c::finalizeState_WaitSacredDewGetEffect() {}

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

void dAcItem_c::getItemFromBWheelItem() {
    if (mStateMgr.isState(StateID_Get) || mStateMgr.isState(StateID_WaitGetDemo)) {
        return;
    }
    if (!mStateMgr.isState(StateID_ResurgeWait)) {
        onTouchLink();
    }
}

void dAcItem_c::setItemPosition(const mVec3_c &pos) {
    mPosition = pos;
    field_0xD51 = 1;
    if (!field_0xD5E) {
        return;
    }
    field_0xD5F = 1;
    mItemPosCopy = pos;
}

void dAcItem_c::setItemVelocity(f32 f) {
    mVelocity.y = f;
    field_0xD51 = 0;
    if (!field_0xD5E) {
        return;
    }
    field_0xD60 = 0;
    if (!field_0xD61) {
        return;
    }
    field_0xD5F = 0;
    field_0xD08 = 1.0f;
}

bool dAcItem_c::isStateWait() const {
    return mStateMgr.isState(StateID_Wait);
}

void dAcItem_c::stopCarryAndGet() {
    mLinkage.forceRemove(this);
    mStateMgr.changeState(StateID_Get);
}

bool dAcItem_c::getEventNameForAnim(u32 anim, const char **outName) {
    if (outName == nullptr) {
        return false;
    }
    switch (anim) {
        default:            return false;
        case ANIM_DEFAULT:  *outName = sDefaultGetItem; break;
        case ANIM_GORGEOUS: *outName = sItemGetGorgeous; break;
        case ANIM_FROWN:    *outName = sDefaultGetItem; break;
        case ANIM_SMALL:    *outName = sDefaultGetItem; break;
    }
    return true;
}

bool dAcItem_c::getTboxEventNameForAnim(u32 anim, const char **outName) {
    if (outName == nullptr) {
        return false;
    }
    switch (anim) {
        default:            return false;
        case ANIM_DEFAULT:  *outName = sItemGetDefaultTbox; break;
        case ANIM_GORGEOUS: *outName = sItemGetGorgeousTbox; break;
        case ANIM_FROWN:    *outName = sItemGetDefaultTbox; break;
        case ANIM_SMALL:    *outName = sItemGetDefaultTbox; break;
    }
    return true;
}

bool dAcItem_c::getItemGetEventName(u16 item, const char **outName) {
    u32 anim;
    if (getItemAnimType(item, &anim) == false) {
        return false;
    }
    return getEventNameForAnim(anim, outName);
}

bool dAcItem_c::getItemGetTboxEventName(u16 item, const char **outName) {
    u32 anim;
    if (getItemAnimType(item, &anim) == false) {
        return false;
    }
    return getTboxEventNameForAnim(anim, outName);
}

bool dAcItem_c::playHeartStemCutSound() {
    dSoundSourceIf_c *src = getSoundSource();
    if (src == nullptr) {
        return false;
    }
    return src->startSoundAtPosition(SE_O_HEART_STEM_CUT, mPosition);
}

void dAcItem_c::getOffsetPosition(mVec3_c &position) const {
    position.set(mPosition.x, mPosition.y + getYOffset(), mPosition.z);
}

f32 dAcItem_c::scaleBy(f32 f) const {
    return f * mBaseScale;
}

void dAcItem_c::itemGetEventStart(dAcBase_c *arg) {
    // TODO
    dAcItem_c *item = static_cast<dAcItem_c *>(arg);
    item->mStateMgr.changeState(StateID_GetDemo);
    item->setObtainedItemId(item->getItemId(), true);
    if (item->mId == ITEM_DUNGEON_MAP_FI && ItemflagManager::sInstance->getItemCounterOrFlag(0x32) == 0) {
        Event ev("fays_map_guide", 100, 0x100001, nullptr, nullptr);
        EventManager::changeOwnEvent(item, dAcNpcTalkKensei_c::GetInstance(), &ev, 0);
    }
}

void dAcItem_c::applyBoundingBox() {
    (this->*mFnSetBoundingBox)();
}

void dAcItem_c::performCollection() {
    performCollectionPart1();
    performCollectionPart2();
}

void dAcItem_c::performCollectionPart1() {
    u16 id = getItemId();
    if (isKeyPieceV()) {
        increaseKeyPieceCounter(1);
    } else if (isLightFruit()) {
        dTgSiren_c::setLightFruitTime(getLightFruitTime());
    } else if (isHeartPiece()) {
        increaseHeartPieceCounter(1);
        increaseTotalHeartPieceCounter(1);
    } else {
        f32 count;
        switch (id) {
            case ITEM_FARON_GRASSHOPPER:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseFaronGrasshopperCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_WOODLAND_RHINO_BEETLE:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseWoodlandRhinoBeetleCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_DEKU_HORNET:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseDekuHornetCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_SKYLOFT_MANTIS:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseSkyloftMantisCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_VOLCANIC_LADYBUG:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseVolcanicLadybugCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_BLESSED_BUTTERFLY:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseBlessedButterflyCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_LANAYRU_ANT:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseLanayruAntCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_SAND_CICADA:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseSandCicadaCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_GERUDO_DRAGONFLY:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseGerudoDragonflyCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_ELDIN_ROLLER:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseEldinRollerCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_SKY_STAG_BEETLE:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseSkyStagBeetleCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_STARRY_FIREFLY:
                if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                    increaseStarryFireflyCounter(mItemQuantity != 0 ? mItemQuantity : 1);
                }
                break;
            case ITEM_HORNET_LARVAE: increaseHornetLarvaeCounter(1); break;
            case ITEM_BIRD_FEATHER:
                // ???
                count = mItemQuantity != 0 ? mItemQuantity : 1;
                increaseBirdFeatherCounter(0.0f < count ? count : 1.0f);
                break;
            case ITEM_TUMBLE_WEED:      increaseTumbleWeedCounter(1); break;
            case ITEM_LIZARD_TAIL:      increaseLizardTailCounter(1); break;
            case ITEM_ELDIN_ORE:        increaseEldinOreCounter(1); break;
            case ITEM_ANCIENT_FLOWER:   increaseAncientFlowerCounter(1); break;
            case ITEM_AMBER_RELIC:      increaseAmberRelicCounter(1); break;
            case ITEM_DUSK_RELIC:       increaseDuskRelicCounter(1); break;
            case ITEM_JELLY_BLOB:       increaseJellyBlobCounter(1); break;
            case ITEM_MONSTER_CLAW:     increaseMonsterClawCounter(1); break;
            case ITEM_MONSTER_HORN:     increaseMonsterHornCounter(1); break;
            case ITEM_ORNAMENTAL_SKULL: increaseSkullCounter(1); break;
            case ITEM_EVIL_CRYSTAL:     increaseEvilCrystalCounter(1); break;
            case ITEM_BLUE_BIRD_FEATHER:
                // ???
                count = mItemQuantity != 0 ? mItemQuantity : 1;
                increaseBlueBirdFeatherCounter(0.0f < count ? count : 1.0f);
                break;
            case ITEM_GOLDEN_SKULL:  increaseGoldenSkullCounter(1); break;
            case ITEM_GODDESS_PLUME: increaseGoddessPlumeCounter(1); break;
            default:                 {
                if (isTriforce()) {
                    doFullHeal();
                } else if (isGratitudeCrystalV()) {
                    increaseGratitudeCrystalCounter(1);
                } else if (is5GratitudeCrystalsV()) {
                    increaseGratitudeCrystalCounter(5);
                } else if (id == ITEM_EXTRA_WALLET) {
                    increaseExtraWalletCounter(1);
                }
                break;
            }
        }
    }

    if (mStateMgr.isState(StateID_GetDemo)) {
        sIsPerformingInitialCollection = !checkFlagV();
    }

    bool doCollect = true;
    if (ITEM_FARON_GRASSHOPPER <= mId && mId <= ITEM_STARRY_FIREFLY &&
        MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
        doCollect = false;
    }
    if (doCollect) {
        setFlagV();
    }
    setSceneFlag();
    setDungeonFlag();

    if (getSubtypeFromParam(mParams) == 7) {
        return;
    }

    if (isGreenRupee()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_RUPEE_GREEN);
    } else if (isBlueRupee()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_RUPEE_BLUE);
    } else if (isRedRupee()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_RUPEE_RED);
    } else if (isSilverRupee()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_RUPEE_SILVER);
    } else if (isGoldRupee()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_RUPEE_BLUE);
    } else if (isRupoor()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_RUPEE_BABA);
    } else if (isHeartV()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_HEART_PIECE_GET);
    } else if (isAnyTear()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SIREN_GET_SHINE_DEW);
    } else if (mId == ITEM_STAMINA_FRUIT) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_REFRESH_FRUIT);
    } else if (isLightFruit()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SIREN_GET_HOLY_FRUIT);
    } else if (mId == ITEM_5_DEKU_SEEDS || is10DekuSeeds()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CONSUMP_ITEM_GET);
    } else if (isAnyArrow()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CONSUMP_ITEM_GET);
    } else if (isAnyBombs()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CONSUMP_ITEM_GET);
    } else if (isInsect()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_MATERIAL);
    } else if (isTreasureV()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_MATERIAL);
    } else if (is5GratitudeCrystals() || isGratitudeCrystal()) {
        if ((mItemFlags & 0x20) == 0) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_SOURCE_OUTWELL);
        }
    } else if (isBabyRattle()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_GARAGARA);
    } else if (isLifeTreeFruit()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_LIFE_FRUIT);
    } else if (mId == ITEM_HEART_CONTAINER) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_HEART_CONTAINER);
    } else if (isTriforce()) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_GET_TRIFORCE);
    }
}

void dAcItem_c::performCollectionPart2() {
    u16 id = getItemId();
    if (id == ITEM_SMALL_KEY) {
        increaseSmallKeyCounter(1);
    } else if (id == ITEM_GREEN_RUPEE) {
        if (!MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER)) {
            increaseRupeeCounter(1);
        }
    } else if (id == ITEM_BLUE_RUPEE) {
        if (!MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER)) {
            increaseRupeeCounter(5);
        }
    } else if (id == ITEM_RED_RUPEE) {
        if (!MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER)) {
            increaseRupeeCounter(20);
        }
    } else if (isHeartV()) {
        healLink(4, true);
    } else if (id == ITEM_SINGLE_ARROW) {
        increaseArrowAndPouchCounter(1);
    } else if (id == ITEM_BUNDLE_ARROWS) {
        increaseArrowAndPouchCounter(10);
    } else if (id == ITEM_SILVER_RUPEE) {
        if (!MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER)) {
            increaseRupeeCounter(100);
        }
    } else if (id == ITEM_GOLD_RUPEE) {
        if (!MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER)) {
            increaseRupeeCounter(300);
        }
    } else if (id == ITEM_RUPOOR) {
        if (!MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER)) {
            increaseRupeeCounter(-10);
        }
    } else if (id == ITEM_5_BOMBS) {
        increaseBombAndPouchCounter(5);
    } else if (id == ITEM_10_BOMBS) {
        increaseBombAndPouchCounter(10);
    } else if (mId == ITEM_STAMINA_FRUIT) {
        restoreStamina();
    } else if (id == ITEM_5_DEKU_SEEDS) {
        increaseDekuSeedAndPouchCounter(5);
    } else if (is10DekuSeeds()) {
        increaseDekuSeedAndPouchCounter(10);
    } else if (id == ITEM_POUCH_EXPANSION) {
        increaseExtraPouchCounter(1);
    } else if (id == ITEM_HEART_CONTAINER) {
        increaseHealthCapacity(getHeartContainerHeartIncrease());
        doFullHeal();
    } else if (isHeartPiece()) {
        if (getNumRemainingHeartPiecesForNextHeart() == 0) {
            increaseHealthCapacity(getCompletedHeartPieceHeartIncrease());
        }
        doFullHeal();
    } else if (id == ITEM_LIFE_TREE_SEED) {
        setLifeTreeSeedlingFlag(1);
    } else if (id == ITEM_BOW || id == ITEM_IRON_BOW || id == ITEM_SACRED_BOW) {
        if (!checkFlagV()) {
            increaseArrowCounter(getBowArrowCapacity() - getBowArrowCount());
        }
    } else if (id == ITEM_SLINGSHOT || id == ITEM_MIGHTY_SCATTERSHOT) {
        if (!checkFlagV()) {
            increaseDekuSeedCounter(getSlingshotSeedCapacity() - getSlingshotSeedCount());
        }
    } else if (isAnyPouchItem((ITEM_ID)id)) {
        if (mGetItemPouchSlot == -1) {
            collectPouchItem(id);
        } else {
            collectPouchItemIntoSlot(mGetItemPouchSlot, id, -1, false);
        }
    }

    if (MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE) &&
        dTgMinigameInsectCapture_c::GetInstance() != nullptr) {
        dTgMinigameInsectCapture_c::GetInstance()->recordCollectedInsect(mId, mItemQuantity != 0 ? mItemQuantity : 1);
    }
}

bool dAcItem_c::isItemDeleteGround(s32 specialCode) const {
    if (isHeartV()) {
        return isHeartDeleteGround(specialCode);
    } else {
        return isDefaultDeleteGround(specialCode);
    }
}

bool dAcItem_c::isItemSwimGround(s32 specialCode) const {
    if (isHeartV()) {
        return isHeartSwimGround(specialCode);
    } else {
        return isDefaultSwimGround(specialCode);
    }
}

void dAcItem_c::rotateTowardsCamera() {
    dCamera_c *cam = dScGame_c::getCamera();
    if (cam == nullptr) {
        return;
    }
    mVec3_c diff = cam->getPosition() - mPosition;
    // TODO close but an extsh is missing
    // Same pattern as in executeState_GetDemo
    mRotation.set(-diff.atan2sY_XZ(), diff.atan2sX_Z(), mAng(0));
}

void dAcItem_c::rotateFixedBirdStatuette() {
    mRotation.x.mVal = 1237;
    mRotation.y.mVal = 20330;
    mRotation.z.mVal = 0;
}

void dAcItemPickupPositionOff_c::init() {
    // no-op
}

bool dAcItemPickupPositionOff_c::execute(dAcItem_c *item) {
    return false;
}

void dAcItemPickupPositionOn_c::init() {
    // TODO weird double load
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link != nullptr) {
        mHighestRelativeHeadHeight = link->getHeadTranslation().y - link->getPosition().y;
    }
    mNumFrames = 0;
}

bool dAcItemPickupPositionOn_c::execute(dAcItem_c *item) {
    if (mNumFrames >= 22) {
        return false;
    }

    // TODO weird double load
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link == nullptr) {
        return false;
    }

    mVec3_c linkPos = link->getPosition();
    mVec3_c headPos = link->getHeadTranslation();
    f32 diff = headPos.y - linkPos.y;
    if (mHighestRelativeHeadHeight < diff) {
        mHighestRelativeHeadHeight = diff;
    }

    item->mPosition.set(headPos.x, linkPos.y + mHighestRelativeHeadHeight, headPos.z);

    f32 f = 12.0f;
    if (mNumFrames != 0) {
        f = (f32)mNumFrames * 0.5f * ((mNumFrames - 1) * -0.3f + 12.0f) + 12.0f;
    }
    item->mPosition.y += f;
    item->rotateTowardsCamera();
    mNumFrames++;
    return true;
}

void dAcItemResurgeIf_c::startResurgeTimer() {
    resetTimer();
    postReset();
}

bool dAcItemResurgeIf_c::canResurge(dAcItem_c *item) {
    if (item->getSquareDistToPlayer() < 40000.0f) {
        return false;
    }
    return isResurgeAllowed(item);
}

void dAcItemResurgeIf_c::resetTimer() {
    mTimer = getTimerInitVal();
}

void dAcItemResurgeIf_c::execute() {
    if (mTimer == 0) {
        return;
    }
    if (!EventManager::isInEvent()) {
        mTimer--;
    }
}

bool dAcItemResurgeIf_c::isTimerExpired() const {
    return mTimer == 0;
}

void dAcItemResurgeOff_c::setTimerInitVal(u16 arg) {
    // no-op
}

void dAcItemResurgeOff_c::postReset() {
    // no-op
}

bool dAcItemResurgeOff_c::isResurgeAllowed(dAcItem_c *) {
    return false;
}

s32 dAcItemResurgeOff_c::getTimerInitVal() {
    return 0;
}

void dAcItemResurgeOn_c::setTimerInitVal(u16 arg) {
    mInitVal = arg;
}

void dAcItemResurgeOn_c::postReset() {
    // no-op
}

bool dAcItemResurgeOn_c::isResurgeAllowed(dAcItem_c *) {
    return (this->*mCb2)();
}

s32 dAcItemResurgeOn_c::getTimerInitVal() {
    return mInitVal;
}
