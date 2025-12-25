#include "d/lyt/d_lyt_pause_disp_00.h"

#include "c/c_math.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_cs_base.h"
#include "d/d_pad_nav.h"
#include "d/d_rumble.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_arrow.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_pause.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/counters/counter.h"

#include <limits.h>

STATE_DEFINE(dLytPauseDisp00_c, None);
STATE_DEFINE(dLytPauseDisp00_c, In);
STATE_DEFINE(dLytPauseDisp00_c, Wait);
STATE_DEFINE(dLytPauseDisp00_c, Select);
STATE_DEFINE(dLytPauseDisp00_c, Ring);
STATE_DEFINE(dLytPauseDisp00_c, GetDemo);
STATE_DEFINE(dLytPauseDisp00_c, Out);

// A matrix controlling navigation with the Nunchuk stick.
// The row index is the currently selected target, and the row contains
// the list of targets, one entry per stick direction  (starting up and going clockwise)
//
// If someone was really bored they could probably replace all of these
// with constants...
static const u8 sNavTable1[][8] = {
    {0x00, 0x15, 0x15, 0x0F, 0x08, 0x18, 0x24, 0x24},
    {0x12, 0x11, 0x03, 0x1E, 0x1E, 0x1E, 0x20, 0x20},
    {0x13, 0x1C, 0x1B, 0x1F, 0x1F, 0x1E, 0x02, 0x11},
    {0x08, 0x0A, 0x0A, 0x06, 0x06, 0x00, 0x19, 0x18},
    {0x09, 0x21, 0x21, 0x00, 0x07, 0x07, 0x0B, 0x0B},
    {0x0A, 0x0B, 0x07, 0x07, 0x00, 0x00, 0x1F, 0x04},
    {0x0B, 0x05, 0x05, 0x00, 0x00, 0x06, 0x06, 0x0A},
    {0x01, 0x0F, 0x10, 0x0A, 0x0A, 0x04, 0x18, 0x24},
    {0x15, 0x21, 0x21, 0x05, 0x0B, 0x0B, 0x10, 0x0F},
    {0x08, 0x10, 0x0B, 0x07, 0x06, 0x04, 0x04, 0x08},
    {0x09, 0x09, 0x05, 0x05, 0x07, 0x06, 0x0A, 0x10},
    {0x01, 0x0E, 0x0D, 0x10, 0x08, 0x08, 0x18, 0x01},
    {0x15, 0x15, 0x00, 0x09, 0x09, 0x10, 0x0C, 0x0E},
    {0x00, 0x15, 0x0D, 0x0D, 0x10, 0x0C, 0x0C, 0x01},
    {0x01, 0x15, 0x09, 0x09, 0x10, 0x08, 0x08, 0x01},
    {0x0F, 0x09, 0x09, 0x0B, 0x0A, 0x0A, 0x08, 0x08},
    {0x14, 0x14, 0x13, 0x03, 0x02, 0x02, 0x12, 0x14},
    {0x14, 0x14, 0x11, 0x11, 0x02, 0x20, 0x20, 0x20},
    {0x14, 0x22, 0x1D, 0x1C, 0x03, 0x11, 0x11, 0x14},
    {0x00, 0x22, 0x22, 0x13, 0x11, 0x12, 0x12, 0x00},
    {0x00, 0x00, 0x21, 0x21, 0x09, 0x0F, 0x01, 0x01},
    {0x23, 0x24, 0x17, 0x17, 0x1A, 0x1D, 0x1D, 0x22},
    {0x24, 0x01, 0x01, 0x18, 0x18, 0x1B, 0x16, 0x16},
    {0x17, 0x01, 0x08, 0x04, 0x19, 0x19, 0x1C, 0x17},
    {0x18, 0x18, 0x04, 0x06, 0x1A, 0x1A, 0x1A, 0x1D},
    {0x16, 0x19, 0x19, 0x06, 0x00, 0x1F, 0x1B, 0x00},
    {0x1C, 0x17, 0x1A, 0x1A, 0x1A, 0x1F, 0x03, 0x1C},
    {0x1D, 0x1D, 0x18, 0x1B, 0x1B, 0x03, 0x13, 0x13},
    {0x22, 0x16, 0x16, 0x19, 0x1C, 0x1C, 0x13, 0x14},
    {0x02, 0x03, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x02},
    {0x03, 0x1B, 0x06, 0x00, 0x00, 0x1E, 0x1E, 0x03},
    {0x14, 0x12, 0x12, 0x02, 0x02, 0x00, 0x00, 0x00},
    {0x15, 0x00, 0x00, 0x00, 0x05, 0x05, 0x09, 0x15},
    {0x00, 0x23, 0x23, 0x16, 0x1D, 0x13, 0x14, 0x14},
    {0x00, 0x24, 0x24, 0x17, 0x16, 0x1D, 0x22, 0x22},
    {0x00, 0x01, 0x01, 0x08, 0x17, 0x16, 0x23, 0x23},
    {0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D},
};

static const u8 sNavTable2[][8] = {
    {0x00, 0x15, 0x15, 0x0E, 0x0C, 0x18, 0x24, 0x24},
    {0x12, 0x11, 0x03, 0x1E, 0x1E, 0x1E, 0x20, 0x20},
    {0x13, 0x1C, 0x1B, 0x1F, 0x1F, 0x1E, 0x02, 0x11},
    {0x08, 0x0A, 0x0A, 0x06, 0x06, 0x00, 0x19, 0x18},
    {0x09, 0x21, 0x21, 0x00, 0x07, 0x07, 0x0B, 0x0B},
    {0x0A, 0x0B, 0x07, 0x07, 0x00, 0x00, 0x1F, 0x04},
    {0x0B, 0x05, 0x05, 0x00, 0x00, 0x06, 0x06, 0x0A},
    {0x0C, 0x0C, 0x10, 0x0A, 0x0A, 0x04, 0x18, 0x24},
    {0x0D, 0x21, 0x21, 0x05, 0x0B, 0x0B, 0x10, 0x0D},
    {0x08, 0x10, 0x0B, 0x07, 0x06, 0x04, 0x04, 0x08},
    {0x09, 0x09, 0x05, 0x05, 0x07, 0x06, 0x0A, 0x10},
    {0x01, 0x0E, 0x0D, 0x10, 0x08, 0x08, 0x18, 0x01},
    {0x15, 0x15, 0x21, 0x09, 0x09, 0x10, 0x0C, 0x0E},
    {0x01, 0x15, 0x0D, 0x0D, 0x10, 0x0C, 0x0C, 0x01},
    {0x00, 0x15, 0x00, 0x09, 0x10, 0x08, 0x08, 0x01},
    {0x0E, 0x0D, 0x09, 0x0B, 0x0A, 0x0A, 0x08, 0x0C},
    {0x14, 0x14, 0x13, 0x03, 0x02, 0x02, 0x12, 0x14},
    {0x14, 0x14, 0x11, 0x11, 0x02, 0x20, 0x20, 0x20},
    {0x14, 0x22, 0x1D, 0x1C, 0x03, 0x11, 0x11, 0x14},
    {0x00, 0x22, 0x1D, 0x13, 0x11, 0x12, 0x12, 0x00},
    {0x00, 0x00, 0x21, 0x21, 0x0D, 0x0E, 0x01, 0x01},
    {0x23, 0x24, 0x17, 0x17, 0x1A, 0x1D, 0x1D, 0x22},
    {0x24, 0x01, 0x01, 0x18, 0x18, 0x1B, 0x16, 0x16},
    {0x17, 0x01, 0x08, 0x04, 0x19, 0x19, 0x1C, 0x17},
    {0x18, 0x18, 0x04, 0x06, 0x1A, 0x1A, 0x1A, 0x1D},
    {0x16, 0x19, 0x19, 0x06, 0x00, 0x1F, 0x1B, 0x00},
    {0x1C, 0x17, 0x1A, 0x1A, 0x1A, 0x1F, 0x03, 0x1C},
    {0x1D, 0x1D, 0x18, 0x1B, 0x1B, 0x03, 0x13, 0x13},
    {0x22, 0x16, 0x16, 0x19, 0x1C, 0x1C, 0x13, 0x14},
    {0x02, 0x03, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x02},
    {0x03, 0x1B, 0x06, 0x00, 0x00, 0x1E, 0x1E, 0x03},
    {0x14, 0x12, 0x12, 0x02, 0x02, 0x00, 0x00, 0x00},
    {0x15, 0x00, 0x00, 0x00, 0x05, 0x05, 0x09, 0x15},
    {0x00, 0x23, 0x23, 0x16, 0x1D, 0x13, 0x14, 0x14},
    {0x00, 0x24, 0x24, 0x17, 0x16, 0x1D, 0x22, 0x22},
    {0x00, 0x01, 0x01, 0x08, 0x17, 0x16, 0x23, 0x23},
    {0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D},
};

static const d2d::LytBrlanMapping brlanMap[] = {
    {        "pause_00_in.brlan",      "G_inOut_00"},
    {      "pause_00_have.brlan",    "G_sekiban_00"},
    {      "pause_00_have.brlan",   "G_dauzingA_00"},
    {      "pause_00_have.brlan",   "G_dauzingB_00"},
    {      "pause_00_have.brlan",   "G_dauzingC_00"},
    {      "pause_00_have.brlan",   "G_dauzingD_00"},
    {      "pause_00_have.brlan",   "G_dauzingE_00"},
    {      "pause_00_have.brlan",   "G_dauzingF_00"},
    {      "pause_00_have.brlan",   "G_dauzingG_00"},
    {      "pause_00_have.brlan",   "G_dauzingH_00"},
    {      "pause_00_have.brlan",      "G_itemA_00"},
    {      "pause_00_have.brlan",      "G_itemB_00"},
    {      "pause_00_have.brlan",      "G_itemC_00"},
    {      "pause_00_have.brlan",      "G_itemD_00"},
    {      "pause_00_have.brlan",      "G_itemE_00"},
    {      "pause_00_have.brlan",      "G_itemF_00"},
    {      "pause_00_have.brlan",      "G_itemG_00"},
    {      "pause_00_have.brlan",      "G_itemH_00"},
    {      "pause_00_have.brlan",     "G_poachA_00"},
    {      "pause_00_have.brlan",     "G_poachB_00"},
    {      "pause_00_have.brlan",     "G_poachC_00"},
    {      "pause_00_have.brlan",     "G_poachD_00"},
    {      "pause_00_have.brlan",     "G_poachE_00"},
    {      "pause_00_have.brlan",     "G_poachF_00"},
    {      "pause_00_have.brlan",     "G_poachG_00"},
    {      "pause_00_have.brlan",     "G_poachH_00"},
    {      "pause_00_have.brlan",      "G_heart_00"},
    {      "pause_00_have.brlan",      "G_saifu_00"},
    {      "pause_00_have.brlan",  "G_parashoru_00"},
    {      "pause_00_have.brlan",     "G_mogura_00"},
    {      "pause_00_have.brlan",      "G_uroko_00"},
    {      "pause_00_have.brlan",       "G_ring_00"},
    {      "pause_00_have.brlan",       "G_muse_00"},
    {      "pause_00_have.brlan",       "G_muse_01"},
    {      "pause_00_have.brlan",       "G_muse_02"},
    {      "pause_00_have.brlan",       "G_muse_03"},
    {      "pause_00_have.brlan",       "G_muse_04"},
    {      "pause_00_have.brlan",       "G_muse_05"},
    {      "pause_00_have.brlan",       "G_muse_06"},
    {      "pause_00_have.brlan",       "G_muse_07"},
    {      "pause_00_have.brlan",   "G_musicSet_00"},
    {      "pause_00_have.brlan",       "G_harp_00"},
    {      "pause_00_have.brlan",  "G_UITypeBtn_00"},
    {      "pause_00_have.brlan",   "G_miToTane_00"},
    {      "pause_00_have.brlan",     "G_shiren_00"},
    {      "pause_00_have.brlan",      "G_force_00"},
    {      "pause_00_have.brlan",      "G_force_01"},
    {      "pause_00_have.brlan",      "G_force_02"},
    {      "pause_00_have.brlan",    "G_forceBg_00"},
    {     "pause_00_onOff.brlan",    "G_sekiban_00"},
    {     "pause_00_onOff.brlan",      "G_heart_00"},
    {     "pause_00_onOff.brlan",      "G_saifu_00"},
    {     "pause_00_onOff.brlan",  "G_parashoru_00"},
    {     "pause_00_onOff.brlan",     "G_mogura_00"},
    {     "pause_00_onOff.brlan",      "G_uroko_00"},
    {     "pause_00_onOff.brlan",       "G_ring_00"},
    {     "pause_00_onOff.brlan",       "G_muse_00"},
    {     "pause_00_onOff.brlan",       "G_muse_01"},
    {     "pause_00_onOff.brlan",       "G_muse_02"},
    {     "pause_00_onOff.brlan",       "G_muse_03"},
    {     "pause_00_onOff.brlan",       "G_muse_04"},
    {     "pause_00_onOff.brlan",       "G_muse_05"},
    {     "pause_00_onOff.brlan",       "G_muse_06"},
    {     "pause_00_onOff.brlan",       "G_muse_07"},
    {     "pause_00_onOff.brlan",       "G_harp_00"},
    {     "pause_00_onOff.brlan",      "G_sword_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingA_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingB_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingC_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingD_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingE_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingF_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingG_00"},
    {     "pause_00_onOff.brlan",   "G_dauzingH_00"},
    {     "pause_00_onOff.brlan",      "G_itemA_00"},
    {     "pause_00_onOff.brlan",      "G_itemB_00"},
    {     "pause_00_onOff.brlan",      "G_itemC_00"},
    {     "pause_00_onOff.brlan",      "G_itemD_00"},
    {     "pause_00_onOff.brlan",      "G_itemE_00"},
    {     "pause_00_onOff.brlan",      "G_itemF_00"},
    {     "pause_00_onOff.brlan",      "G_itemG_00"},
    {     "pause_00_onOff.brlan",      "G_itemH_00"},
    {     "pause_00_onOff.brlan",     "G_poachA_00"},
    {     "pause_00_onOff.brlan",     "G_poachB_00"},
    {     "pause_00_onOff.brlan",     "G_poachC_00"},
    {     "pause_00_onOff.brlan",     "G_poachD_00"},
    {     "pause_00_onOff.brlan",     "G_poachE_00"},
    {     "pause_00_onOff.brlan",     "G_poachF_00"},
    {     "pause_00_onOff.brlan",     "G_poachG_00"},
    {     "pause_00_onOff.brlan",     "G_poachH_00"},
    {     "pause_00_onOff.brlan",   "G_tryForce_00"},
    {     "pause_00_onOff.brlan",     "G_shiren_00"},
    {     "pause_00_onOff.brlan",   "G_miToTane_00"},
    {     "pause_00_onOff.brlan",       "G_text_00"},
    {     "pause_00_onOff.brlan",   "G_calibBtn_00"},
    { "pause_00_bocoburin.brlan",   "G_paraBoco_00"},
    { "pause_00_bocoburin.brlan", "G_moguraBoco_00"},
    { "pause_00_bocoburin.brlan",  "G_urokoBoco_00"},
    { "pause_00_bocoburin.brlan",   "G_harpBoco_00"},
    { "pause_00_bocoburin.brlan",  "G_swordBoco_00"},
    { "pause_00_itemArrow.brlan",  "G_itemArrow_00"},
    {   "pause_00_toPoach.brlan", "G_ringChange_00"},
    {   "pause_00_toSword.brlan", "G_ringChange_00"},
    {    "pause_00_toItem.brlan", "G_ringChange_00"},
    {     "pause_00_sword.brlan",      "G_sword_00"},
    {   "pause_00_sekiban.brlan",    "G_sekiban_00"},
    {     "pause_00_heart.brlan",      "G_heart_00"},
    {     "pause_00_saifu.brlan",      "G_saifu_00"},
    {    "pause_00_mogura.brlan",     "G_mogura_00"},
    {       "pause_00_out.brlan",      "G_inOut_00"},
    {     "pause_00_onOff.brlan",  "G_UITypeBtn_00"},
    { "pause_00_btnDecide.brlan",  "G_UITypeBtn_00"},
    { "pause_00_btnDecide.brlan",   "G_calibBtn_00"},
    {    "pause_00_UIType.brlan",     "G_UIType_00"},
    {      "pause_00_call.brlan", "G_UITypeCall_00"},
    {  "pause_00_miToTane.brlan",   "G_miToTane_00"},
    {      "pause_00_loop.brlan",       "G_loop_00"},
    { "pause_00_scrollRIn.brlan",     "G_scroll_00"},
    {"pause_00_scrollROut.brlan",     "G_scroll_00"},
    { "pause_00_scrollLIn.brlan",     "G_scroll_00"},
    {"pause_00_scrollLOut.brlan",     "G_scroll_00"},
    {  "pause_00_dauzType.brlan",   "G_dauzType_00"},
    {       "pause_00_off.brlan",    "G_leftTab_00"},
    {       "pause_00_off.brlan",  "G_centerTab_00"},
    {       "pause_00_off.brlan",   "G_rightTab_00"},
    {      "pause_00_tabV.brlan",       "G_tabV_00"},
};

#define PAUSE_DISP_00_ANIM_IN 0
#define PAUSE_DISP_00_ANIM_HAVE_TABLET 1
#define PAUSE_DISP_00_ANIM_HAVE_DOWSING_OFFSET 2
#define PAUSE_DISP_00_ANIM_HAVE_ITEM_OFFSET 10
#define PAUSE_DISP_00_ANIM_HAVE_POUCH_OFFSET 18
#define PAUSE_DISP_00_ANIM_HAVE_HEART 26
#define PAUSE_DISP_00_ANIM_HAVE_POUCH 27
#define PAUSE_DISP_00_ANIM_HAVE_PARACHUTE 28
#define PAUSE_DISP_00_ANIM_HAVE_MITTS 29
#define PAUSE_DISP_00_ANIM_HAVE_SCALE 30
#define PAUSE_DISP_00_ANIM_HAVE_RING 31
#define PAUSE_DISP_00_ANIM_HAVE_MUSE_OFFSET 32
#define PAUSE_DISP_00_ANIM_HAVE_MUSE_PART_OFFSET 36
#define PAUSE_DISP_00_ANIM_HAVE_MUSE_07 39
#define PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET 40
#define PAUSE_DISP_00_ANIM_HAVE_HARP 41
#define PAUSE_DISP_00_ANIM_HAVE_UITYPE_BTN 42
#define PAUSE_DISP_00_ANIM_HAVE_MI_TO_TANE 43
#define PAUSE_DISP_00_ANIM_HAVE_SHIREN 44
#define PAUSE_DISP_00_ANIM_HAVE_FORCE_OFFSET 45
#define PAUSE_DISP_00_ANIM_HAVE_FORCE_BG 48
#define PAUSE_DISP_00_ANIM_ONOFF_TABLET 49
#define PAUSE_DISP_00_ANIM_ONOFF_HEART 50
#define PAUSE_DISP_00_ANIM_ONOFF_POUCH 51
#define PAUSE_DISP_00_ANIM_ONOFF_PARACHUTE 52
#define PAUSE_DISP_00_ANIM_ONOFF_MITTS 53
#define PAUSE_DISP_00_ANIM_ONOFF_SCALE 54
#define PAUSE_DISP_00_ANIM_ONOFF_RING 55
#define PAUSE_DISP_00_ANIM_ONOFF_MUSE_OFFSET 56
#define PAUSE_DISP_00_ANIM_ONOFF_HARP 64
#define PAUSE_DISP_00_ANIM_ONOFF_SWORD 65
#define PAUSE_DISP_00_ANIM_ONOFF_DOWSING_OFFSET 66
#define PAUSE_DISP_00_ANIM_ONOFF_ITEM_OFFSET 74
#define PAUSE_DISP_00_ANIM_ONOFF_POUCH_OFFSET 82
#define PAUSE_DISP_00_ANIM_ONOFF_TRIFORCE 90
#define PAUSE_DISP_00_ANIM_ONOFF_SHIREN 91
#define PAUSE_DISP_00_ANIM_ONOFF_MI_TO_TANE 92
#define PAUSE_DISP_00_ANIM_ONOFF_TEXT 93
#define PAUSE_DISP_00_ANIM_ONOFF_CALIB_BTN 94
#define PAUSE_DISP_00_ANIM_BOCO_PARACHUTE 95
#define PAUSE_DISP_00_ANIM_BOCO_MITTS 96
#define PAUSE_DISP_00_ANIM_BOCO_SCALE 97
#define PAUSE_DISP_00_ANIM_BOCO_HARP 98
#define PAUSE_DISP_00_ANIM_BOCO_SWORD 99
#define PAUSE_DISP_00_ANIM_ITEM_ARROW 100
#define PAUSE_DISP_00_ANIM_RING_TO_POUCH 101
#define PAUSE_DISP_00_ANIM_RING_TO_SWORD 102
#define PAUSE_DISP_00_ANIM_RING_TO_ITEM 103
#define PAUSE_DISP_00_ANIM_SWORD 104
#define PAUSE_DISP_00_ANIM_TABLETS 105
#define PAUSE_DISP_00_ANIM_HEART 106
#define PAUSE_DISP_00_ANIM_POUCH 107
#define PAUSE_DISP_00_ANIM_MITTS 108
#define PAUSE_DISP_00_ANIM_OUT 109
#define PAUSE_DISP_00_ANIM_UI_TYPE_BTN 110
#define PAUSE_DISP_00_ANIM_DECIDE_UI_TYPE_BTN 111
#define PAUSE_DISP_00_ANIM_DECIDE_CALIB_BTN 112
#define PAUSE_DISP_00_ANIM_UI_TYPE 113
#define PAUSE_DISP_00_ANIM_CALL 114
#define PAUSE_DISP_00_ANIM_MI_TO_TANE 115
#define PAUSE_DISP_00_ANIM_LOOP 116
#define PAUSE_DISP_00_ANIM_SCROLL_R_IN 117
#define PAUSE_DISP_00_ANIM_SCROLL_R_OUT 118
#define PAUSE_DISP_00_ANIM_SCROLL_L_IN 119
#define PAUSE_DISP_00_ANIM_SCROLL_L_OUT 120
#define PAUSE_DISP_00_ANIM_DOWSING_TYPE 121
#define PAUSE_DISP_00_ANIM_OFF_LEFT_TAB 122
#define PAUSE_DISP_00_ANIM_OFF_CENTER_TAB 123
#define PAUSE_DISP_00_ANIM_OFF_RIGHT_TAB 124
#define PAUSE_DISP_00_ANIM_TAB_V 125

#define PAUSE_DISP_00_NUM_ANIMS 126

#define PAUSE_DISP_00_ANIM_RING_BASE PAUSE_DISP_00_ANIM_RING_TO_POUCH
#define PAUSE_DISP_00_ANIM_ITEM_OFFSET PAUSE_DISP_00_ANIM_ONOFF_DOWSING_OFFSET

#define PAUSE_DISP_00_NUM_SUBPANES 24

#define navTargetToBounding(n) ((s32)(n - 1))
#define boundingToNavTarget(n) ((s32)(n + 1))

// clang-format off
static const u8 iconVariants[] = {
    // Wheel
    0, 0, 0, 0, 0, 0, 0, 0,
    // Dowsing
    2, 2, 2, 2, 2, 2, 2, 2,
    // Pouch
    1, 1, 1, 1, 1, 1, 1, 1, 
};
// clang-format on

static const char *sGroupName = "G_ref_00";

static const char *sBoundingNames[] = {
    "B_sekiban_00",   "B_heart_00",     "B_saifu_00",    "B_parashoru_00", "B_mogura_00",    "B_uroko_00",
    "B_ring_00",      "B_muse_00",      "B_muse_01",     "B_muse_02",      "B_muse_03",      "B_muse_04",
    "B_muse_05",      "B_muse_06",      "B_muse_07",     "B_harp_00",      "B_sword_00",     "B_swordFire_00",
    "B_swordFire_01", "B_swordFire_02", "B_shiren_00",   "B_ringA_00",     "B_ringB_00",     "B_ringC_00",
    "B_ringD_00",     "B_ringE_00",     "B_ringF_00",    "B_ringG_00",     "B_ringH_00",     "B_UIType_00",
    "B_calibBtn_00",  "B_calibBtn_00",  "B_calibBtn_00", "B_leftTab_00",   "B_centerTab_00", "B_rightTab_00",
    "B_calibBtn_00",
};

#define PAUSE_DISP_00_BOUNDING_TABLETS 0
#define PAUSE_DISP_00_BOUNDING_HEART 1
#define PAUSE_DISP_00_BOUNDING_POUCH 2
#define PAUSE_DISP_00_BOUNDING_PARACHUTE 3
#define PAUSE_DISP_00_BOUNDING_MITTS 4
#define PAUSE_DISP_00_BOUNDING_SCALE 5
#define PAUSE_DISP_00_BOUNDING_RING 6
#define PAUSE_DISP_00_BOUNDING_MUSE_OFFSET 7
#define PAUSE_DISP_00_BOUNDING_MUSE_07 14
#define PAUSE_DISP_00_BOUNDING_HARP 15
#define PAUSE_DISP_00_BOUNDING_SWORD 16
#define PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET 17
#define PAUSE_DISP_00_BOUNDING_SHIREN 20
#define PAUSE_DISP_00_BOUNDING_RING_OFFSET 21
#define PAUSE_DISP_00_BOUNDING_UI_TYPE 29
#define PAUSE_DISP_00_BOUNDING_CALIB_BTN_0 30
#define PAUSE_DISP_00_BOUNDING_ARROW_LEFT 31
#define PAUSE_DISP_00_BOUNDING_ARROW_RIGHT 32
#define PAUSE_DISP_00_BOUNDING_LEFT_TAB 33
#define PAUSE_DISP_00_BOUNDING_CENTER_TAB 34
#define PAUSE_DISP_00_BOUNDING_RIGHT_TAB 35
#define PAUSE_DISP_00_BOUNDING_CALIB_BTN_3 36

#define PAUSE_DISP_00_BOUNDING_ARROW_OFFSET PAUSE_DISP_00_BOUNDING_ARROW_LEFT

#define PAUSE_DISP_00_NUM_BOUNDINGS 37

// TODO - constants to maybe move elsewhere
#define PAUSE_DISP_00_NUM_SONG_PARTS 3

#define PAUSE_DISP_00_ICONS_B_WHEEL_OFFSET 0
#define PAUSE_DISP_00_ICONS_DOWSING_OFFSET 8
#define PAUSE_DISP_00_ICONS_POUCH_OFFSET 16
#define PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL 8

dLytPauseDisp00_c::dLytPauseDisp00_c() : mStateMgr(*this) {}

bool dLytPauseDisp00_c::build() {
    dLytPauseMgr_c *pauseMgr = dLytPauseMgr_c::GetInstance();
    d2d::ResAccIf_c *resAcc = pauseMgr->getResAcc1();
    mLyt.setResAcc(resAcc);
    mLyt.build("pause_00.brlyt", nullptr);
    mLyt.setPriority(0x86);

    for (int i = 0; i < PAUSE_DISP_00_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    resAcc = pauseMgr->getResAcc2();

    for (int i = 0; i < PAUSE_DISP_00_NUM_SUBPANES; i++) {
        mIcons[i].build(resAcc, iconVariants[i]);
        mSubpanes[i].mpLytPane = &mIcons[i];
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *group = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (group != nullptr) {
            d2d::dSubPane::linkMeters(group, &mSubpaneList);
        }
    }

    mCsHitCheck.init(mLyt.getLayout()->GetRootPane(), 1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    for (int i = 0; i < PAUSE_DISP_00_NUM_BOUNDINGS; i++) {
        if (i != PAUSE_DISP_00_BOUNDING_ARROW_LEFT && i != PAUSE_DISP_00_BOUNDING_ARROW_RIGHT) {
            mpBoundings[i] = mLyt.findBounding(sBoundingNames[i]);
            mpBoundings[i]->SetVisible(true);
        }
    }

    mpBoundings[PAUSE_DISP_00_BOUNDING_ARROW_LEFT] = pauseMgr->getArrowBounding(dLytCommonArrow_c::ARROW_LEFT);
    mpBoundings[PAUSE_DISP_00_BOUNDING_ARROW_RIGHT] = pauseMgr->getArrowBounding(dLytCommonArrow_c::ARROW_RIGHT);

    u8 songLifetreeStatus = getSongLifeTreeStatus();
    if (songLifetreeStatus == SONG_LIFETREE_HAS_SOTH) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_07, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET, 0.0f);
    } else if (songLifetreeStatus != SONG_LIFETREE_NONE) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET, 1.0f);
        for (int i = 0; i < PAUSE_DISP_00_NUM_SONG_PARTS; i++) {
            setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_PART_OFFSET + i, 1.0f);
        }
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_07, 0.0f);
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET, 0.0f);
    }
    setAnm(PAUSE_DISP_00_ANIM_IN, 0.0f);
    mAnm[PAUSE_DISP_00_ANIM_IN].setFrame(mAnm[PAUSE_DISP_00_ANIM_IN].getAnimDuration());

    mLyt.calc();

    stopAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_07);
    stopAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET);
    stopAnm(PAUSE_DISP_00_ANIM_IN);
    for (int i = 0; i < PAUSE_DISP_00_NUM_SONG_PARTS; i++) {
        stopAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_PART_OFFSET + i);
    }

    mStateMgr.changeState(StateID_None);

    return true;
}

bool dLytPauseDisp00_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        if (!it->mpLytPane->LytMeter0x24()) {
            continue;
        }
        // @bug checking nullptr after invoking virtual function on it
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }
    for (int i = 0; i < PAUSE_DISP_00_NUM_SUBPANES; i++) {
        mSubpanes[i].mpLytPane->remove();
    }

    mLyt.unbindAnims();

    for (int i = 0; i < PAUSE_DISP_00_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }

    return true;
}

bool dLytPauseDisp00_c::execute() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    mHasSelection = false;
    pause->setField_0x083C(false);
    pause->setField_0x083D(false);
    pause->setField_0x0837(false);
    mStateMgr.executeState();
    executeCall();
    for (int i = 0; i < PAUSE_DISP_00_NUM_SUBPANES; i++) {
        mSubpanes[i].mpLytPane->execute();
    }
    mLyt.calc();
    mCsHitCheck.resetCachedHitboxes();
    mCsHitCheck.execute();
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        mAnm[PAUSE_DISP_00_ANIM_LOOP].play();
    }

    return true;
}

bool dLytPauseDisp00_c::draw() {
    if (mIsVisible == true) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytPauseDisp00_c::drawDirectly() {
    mLyt.draw();
}

void dLytPauseDisp00_c::requestIn(bool scroll) {
    mInRequest = true;
    mDoScrollAnim = scroll;
}

void dLytPauseDisp00_c::requestOut(bool scroll) {
    mOutRequest = true;
    mDoScrollAnim = scroll;
}

void dLytPauseDisp00_c::requestSelect() {
    mSelectToggleRequest = true;
}

void dLytPauseDisp00_c::requestRingToggle() {
    mRingToggleRequest = true;
}

void dLytPauseDisp00_c::requestUnselect() {
    mSelectToggleRequest = true;
}

bool dLytPauseDisp00_c::requestSelectGuide() {
    if (mSelectGuideRequest == true) {
        return 0;
    }

    mSelectGuideRequest = true;
    if (mCallTimerMaybe != 0) {
        mStopCallRequest = true;
        StoryflagManager::sInstance->unsetFlag(STORYFLAG_UI_MODE_NOTICE);
    }
    return true;
}

bool dLytPauseDisp00_c::requestSelectMpls() {
    if (mSelectMplsRequest == true) {
        return 0;
    }

    mSelectMplsRequest = true;
    return true;
}

void dLytPauseDisp00_c::initializeState_None() {
    for (int i = 0; i < PAUSE_DISP_00_NUM_ANIMS; i++) {
        mAnm[i].unbind();
    }

    mIsChangingState = false;
    mInRequest = false;
    mOutRequest = false;
    mIsVisible = false;
    mHasSelection = false;
    mStopCallRequest = false;

    mCallTimerMaybe = 0;
    mStep = 0;
    mPrevNavTarget = 0;
    mCurrentNavTarget = 0;
    mGetDemoTimer = 0;

    for (int i = 0; i < PAUSE_DISP_00_NUM_SUBPANES; i++) {
        mIcons[i].reset();
        mIcons[i].setVisible(false);
    }

    for (int i = 0; i < PAUSE_DISP_00_NUM_BOUNDINGS; i++) {
        mpBoundings[i]->SetVisible(false);
    }

    mpBoundings[PAUSE_DISP_00_BOUNDING_ARROW_LEFT]->SetVisible(true);
    mpBoundings[PAUSE_DISP_00_BOUNDING_ARROW_RIGHT]->SetVisible(true);

    mLyt.findPane("N_itemArrow_00")->SetVisible(false);
    setAnm(PAUSE_DISP_00_ANIM_ITEM_ARROW, 0.0f);
    setAnm(PAUSE_DISP_00_ANIM_RING_BASE + dLytControlGame_c::getInstance()->getPauseDisp00Tab(), 0.0f);

    StoryflagManager *storyFlagmanager = StoryflagManager::sInstance;

    // Check which tabs are unlocked
    if (storyFlagmanager->getFlag(STORYFLAG_DOWSING_UNLOCKED)) {
        mpBoundings[PAUSE_DISP_00_BOUNDING_LEFT_TAB]->SetVisible(true);
    }

    if (storyFlagmanager->getFlag(STORYFLAG_POUCH_UNLOCKED)) {
        mpBoundings[PAUSE_DISP_00_BOUNDING_CENTER_TAB]->SetVisible(true);
    }

    if (storyFlagmanager->getFlag(STORYFLAG_B_WHEEL_UNLOCKED)) {
        mpBoundings[PAUSE_DISP_00_BOUNDING_RIGHT_TAB]->SetVisible(true);
    }

    setAnm(PAUSE_DISP_00_ANIM_LOOP, 0.0f);
    mSelectGuideRequest = false;
    mSelectToggleRequest = false;
    mReverseTabChangeAnim = false;
    mRingToggleRequest = false;
}
void dLytPauseDisp00_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytPauseDisp00_c::finalizeState_None() {
    loadRingText(RING_TEXT_INITIAL_TAB);
}

void dLytPauseDisp00_c::initializeState_In() {
    if (mDoScrollAnim == true) {
        dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
        setAnm(PAUSE_DISP_00_ANIM_IN, 0.0f);
        mAnm[PAUSE_DISP_00_ANIM_IN].setFrame(mAnm[PAUSE_DISP_00_ANIM_IN].getAnimDuration());
        if (pause->isNavLeft()) {
            setAnm(PAUSE_DISP_00_ANIM_SCROLL_L_IN, 0.0f);
        } else {
            setAnm(PAUSE_DISP_00_ANIM_SCROLL_R_IN, 0.0f);
        }

        if (pause->getField_0x0840()) {
            if (pause->isNavLeft()) {
                mPrevNavTarget = PAUSE_DISP_00_BOUNDING_ARROW_LEFT + 1;
            } else {
                mPrevNavTarget = PAUSE_DISP_00_BOUNDING_ARROW_RIGHT + 1;
            }
        }
    } else {
        setAnm(PAUSE_DISP_00_ANIM_IN, 0.0f);
    }

    for (int i = PAUSE_DISP_00_ANIM_ONOFF_TABLET; i < PAUSE_DISP_00_ANIM_ONOFF_CALIB_BTN + 1; i++) {
        setAnm(i, 0.0f);
    }
    mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].setToEnd();

    setupDisp();
    setupRingIcons(dLytControlGame_c::getInstance()->getPauseDisp00Tab());
    setAnm(PAUSE_DISP_00_ANIM_UI_TYPE_BTN, 0.0f);

    u8 mode = dLytMeter_c::GetMain()->getUiMode();
    setAnm(PAUSE_DISP_00_ANIM_UI_TYPE, mode);
    StoryflagManager *storyflagManager = StoryflagManager::sInstance;
    if (storyflagManager->getFlag(STORYFLAG_UPGRADED_DOWSING)) {
        setAnm(PAUSE_DISP_00_ANIM_DOWSING_TYPE, 1.0f);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_DOWSING_TYPE, 0.0f);
    }

    // Control tab visibility
    f32 tabVFrame = 0.0f;
    if (storyflagManager->getFlag(STORYFLAG_POUCH_UNLOCKED)) {
        tabVFrame = 1.0f;
    }
    if (storyflagManager->getFlag(STORYFLAG_DOWSING_UNLOCKED)) {
        tabVFrame = 2.0f;
    }
    if (storyflagManager->getFlag(STORYFLAG_B_WHEEL_UNLOCKED)) {
        tabVFrame = 3.0f;
    }

    setAnm(PAUSE_DISP_00_ANIM_TAB_V, tabVFrame);
    if (tabVFrame) {
        mLyt.findPane("N_text_00")->SetVisible(true);
    } else {
        mLyt.findPane("N_text_00")->SetVisible(false);
    }

    mIsVisible = true;
}
void dLytPauseDisp00_c::executeState_In() {
    s32 anim = PAUSE_DISP_00_ANIM_IN;
    if (mDoScrollAnim == true) {
        dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
        anim = pause->isNavLeft() ? PAUSE_DISP_00_ANIM_SCROLL_L_IN : PAUSE_DISP_00_ANIM_SCROLL_R_IN;
    }

    d2d::AnmGroup_c &anm = mAnm[anim];

    if (anm.isEndReached() == true) {
        if (dLytControlGame_c::getInstance()->isPauseDemo()) {
            mStateMgr.changeState(StateID_GetDemo);
        } else {
            mStateMgr.changeState(StateID_Wait);
        }
    } else {
        anm.play();
    }
}
void dLytPauseDisp00_c::finalizeState_In() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    if (pause->isNavLeft()) {
        stopAnm(PAUSE_DISP_00_ANIM_SCROLL_L_IN);
    } else {
        stopAnm(PAUSE_DISP_00_ANIM_SCROLL_R_IN);
    }

    mDoScrollAnim = false;
    if (pause->getField_0x083E()) {
        if (!pause->getField_0x0832()) {
            mCurrentNavTarget = PAUSE_DISP_00_BOUNDING_ARROW_RIGHT + 1;
        } else {
            mCurrentNavTarget = PAUSE_DISP_00_BOUNDING_ARROW_LEFT + 1;
        }
        mPrevNavTarget = mCurrentNavTarget;
    }
}

void dLytPauseDisp00_c::initializeState_Wait() {
    mStep = 0;
    mIsChangingState = true;
}
void dLytPauseDisp00_c::executeState_Wait() {
    d2d::AnmGroup_c *anm;
    if (mIsChangingState == true) {
        mIsChangingState = false;
    }
    switch (mStep) {
        case 0: {
            if (mOutRequest == true) {
                mOutRequest = false;
                hideItemIcons();
                mStateMgr.changeState(StateID_Out);
                return;
            }
            if (mSelectToggleRequest == true) {
                mStateMgr.changeState(StateID_Select);
                return;
            }
            if (mRingToggleRequest == true) {
                mRingToggleRequest = false;
                mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].setFrame(0.0f);
                mStateMgr.changeState(StateID_Ring);
                return;
            }

            if (mSelectGuideRequest == true) {
                mStep = 1;
                setAnm(PAUSE_DISP_00_ANIM_DECIDE_UI_TYPE_BTN, 0.0f);
                u8 uiMode = dLytMeter_c::GetMain()->getUiMode();
                f32 frame = uiMode + 1;
                if (frame >= 3.0f) {
                    frame = 0.0f;
                }
                mAnm[PAUSE_DISP_00_ANIM_UI_TYPE].setFrame(frame);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_P1_SELECT_GUIDE);
            } else if (mSelectMplsRequest == true) {
                mStep = 2;
                setAnm(PAUSE_DISP_00_ANIM_DECIDE_CALIB_BTN, 0.0f);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_P1_SELECT_MPLUS);
            } else if (updateSelection() != 0) {
                mHasSelection = true;
            }
            break;
        }
        case 1: {
            anm = &mAnm[PAUSE_DISP_00_ANIM_DECIDE_UI_TYPE_BTN];
            if (anm->isEndReached() == true) {
                mStep = 0;
                mSelectGuideRequest = false;
                stopAnm(PAUSE_DISP_00_ANIM_DECIDE_UI_TYPE_BTN);
                mIsChangingState = true;
            } else {
                anm->play();
            }
            break;
        }
        case 2: {
            anm = &mAnm[PAUSE_DISP_00_ANIM_DECIDE_CALIB_BTN];
            if (anm->isEndReached() == true) {
                mStep = 0;
                mSelectMplsRequest = false;
                stopAnm(PAUSE_DISP_00_ANIM_DECIDE_CALIB_BTN);
                mIsChangingState = true;
            } else {
                anm->play();
            }
            break;
        }
    }
    mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].play();
}
void dLytPauseDisp00_c::finalizeState_Wait() {}

void dLytPauseDisp00_c::initializeState_Select() {
    // Reminder: item=0, pouch=1, dowsing=2

    // Subtle detail - the layout has three animations for changing tabs:
    // 1. B-Wheel -> Pouch
    // 2. Pouch -> Dowsing
    // 3. Dowsing -> B-Wheel
    // If we're changing tabs in this order, we can simply play the
    // animation forward. If we're changing tabs in a different order,
    // we need to play the reverse change animation, backwards.

    // For a given tab index, what's the natural next tab index?
    static const s32 sNaturalTabOrder[] = {
        /* TAB_ITEM    -> */ dLytPauseMgr_c::TAB_POUCH,
        /* TAB_POUCH   -> */ dLytPauseMgr_c::TAB_DOWSING,
        /* TAB_DOWSING -> */ dLytPauseMgr_c::TAB_ITEM,
    };

    // For a given tab index, what's the change anim when changing to the natural next tab?
    static const s32 sNaturalTabChangeAnim[] = {
        PAUSE_DISP_00_ANIM_RING_TO_POUCH,
        PAUSE_DISP_00_ANIM_RING_TO_SWORD,
        PAUSE_DISP_00_ANIM_RING_TO_ITEM,
    };

    // TODO - possibly fakematch, but we need to force a reload.
    // Note that this is part of a .rodata pool, so it
    // must be const and in the same file...

    // For a given tab index, what's the change anim when changing to the *other* tab,
    // in not-natural order?
    static const volatile s32 sReverseTabChangeAnim[] = {
        // E.g. we're on the item tab, and we're changing to dowsing,
        // so we need to play the dowsing->item animation in reverse.
        PAUSE_DISP_00_ANIM_RING_TO_ITEM,
        PAUSE_DISP_00_ANIM_RING_TO_POUCH,
        PAUSE_DISP_00_ANIM_RING_TO_SWORD,
    };

    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    mSelectToggleRequest = false;
    mStep = 0;
    if (pause->getField_0x0838() == true) {
        mStep = 1;
        stopAnm(PAUSE_DISP_00_ANIM_RING_TO_POUCH);
        stopAnm(PAUSE_DISP_00_ANIM_RING_TO_SWORD);
        stopAnm(PAUSE_DISP_00_ANIM_RING_TO_ITEM);

        s32 oldTab = dLytControlGame_c::getInstance()->getPauseDisp00Tab();
        if (sNaturalTabOrder[oldTab] == pause->getCurrentDisp00Tab()) {
            // Natural
            setAnm(sNaturalTabChangeAnim[oldTab], 0.0f);
            mReverseTabChangeAnim = false;
        } else {
            // Opposite order
            stopAnm(oldTab + PAUSE_DISP_00_ANIM_RING_BASE);
            setAnm(sReverseTabChangeAnim[oldTab], 0.0f);
            mAnm[sReverseTabChangeAnim[oldTab]].setToEnd();
            mReverseTabChangeAnim = true;
        }

        for (int i = PAUSE_DISP_00_ANIM_ITEM_OFFSET;
             i < PAUSE_DISP_00_ANIM_ITEM_OFFSET + 3 * PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
            mAnm[i].setFrame(0.0f);
        }
    }
    mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].setFrame(0.0f);
}
void dLytPauseDisp00_c::executeState_Select() {
    static const s32 sReverseTabChangeAnim[] = {
        PAUSE_DISP_00_ANIM_RING_TO_ITEM,
        PAUSE_DISP_00_ANIM_RING_TO_POUCH,
        PAUSE_DISP_00_ANIM_RING_TO_SWORD,
    };

    // When our normal tab change anim is done (frame == 1.0f),
    // we change to the next anim with frame == 0.0f.
    // Not entirely sure why.
    static const s32 sNextTabChangeAnim[] = {
        // E.g. we're changing from B-Wheel to Pouch. When
        // we're done with that transition
        PAUSE_DISP_00_ANIM_RING_TO_SWORD,
        PAUSE_DISP_00_ANIM_RING_TO_ITEM,
        PAUSE_DISP_00_ANIM_RING_TO_POUCH,
    };

    switch (mStep) {
        case 0: {
            if (mSelectToggleRequest == true) {
                mStateMgr.changeState(StateID_Wait);
            }
            break;
        }
        case 1: {
            // TODO Regswaps - maybe enum types, maybe int, maybe the order of these lines...
            s32 controlGameTab = dLytControlGame_c::getInstance()->getPauseDisp00Tab();
            s32 lytPauseTab = dLytPauseMgr_c::GetInstance()->getCurrentDisp00Tab();
            d2d::AnmGroup_c *anm;
            if (mReverseTabChangeAnim) {
                anm = &mAnm[sReverseTabChangeAnim[controlGameTab]];
                if (cM::isZero(anm->getFrame()) == true) {
                    setupRingIcons(lytPauseTab);
                    mStateMgr.changeState(StateID_Wait);
                }
                playBackwards(*anm);
            } else {
                anm = &mAnm[controlGameTab + PAUSE_DISP_00_ANIM_RING_BASE];
                if (anm->isEndReached() == true) {
                    stopAnm(controlGameTab + PAUSE_DISP_00_ANIM_RING_BASE);
                    setAnm(sNextTabChangeAnim[controlGameTab], 0.0f);
                    setupRingIcons(lytPauseTab);
                    mStateMgr.changeState(StateID_Wait);
                } else {
                    anm->play();
                }
            }
            break;
        }
    }
    playOnOffTabAnim();
}
void dLytPauseDisp00_c::finalizeState_Select() {
    mSelectToggleRequest = false;
    if (mStep == 0) {
        return;
    }
    loadRingText(RING_TEXT_CURRENT_TAB);
}

void dLytPauseDisp00_c::initializeState_Ring() {
    mStep = 0;
    mLyt.findPane("N_itemArrow_00")->SetVisible(true);
    dAcPy_c::calcItemWheelSelection(true, PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL);
    mPrevNavTarget = 0;
    loadRingText(RING_TEXT_RELEASE_TO_CONFIRM);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_P1_HOLD_POINTER);
}
void dLytPauseDisp00_c::executeState_Ring() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    f32 rot = dLytPauseMgr_c::sDisp00ArrowRotation;
    f32 len = dLytPauseMgr_c::sDisp00ArrowLength;
    if (len < 0.0f) {
        len = 0.0f;
    }
    if (len > 1.0f) {
        len = 1.0f;
    }
    // Length
    len *= 99.0f;
    mAnm[PAUSE_DISP_00_ANIM_ITEM_ARROW].setFrame(len);
    // Rotate the arrow
    mVec3_c t1(0.0f, 0.0f, 0.0f);
    t1.z = rot;
    mLyt.findPane("N_itemArrow_00")->SetRotate(t1);
    // But rotate the button and the pointer back so that
    // they point up
    t1.z = -t1.z;
    mLyt.findPane("N_arrowHand_00")->SetRotate(t1);

    if (mSelectToggleRequest == true) {
        mStateMgr.changeState(StateID_Select);
    } else if (mRingToggleRequest == true) {
        mRingToggleRequest = false;
        mStateMgr.changeState(StateID_Wait);
    } else {
        s32 idx = dAcPy_c::calcItemWheelSelection(false, PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL);
        s32 navTarget = 0;
        if (idx == -1) {
            mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].setFrame(0.0f);
            pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
        } else {
            mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].play();
            static const u8 sButtonOrder[] = {
                0, 7, 6, 5, 4, 3, 2, 1,
            };
            idx = sButtonOrder[idx];

            u16 item;
            dLytPauseMgr_c::SelectionType_e selectionType;
            s32 b = PAUSE_DISP_00_BOUNDING_RING_OFFSET + idx;
            if (mpBoundings[b]->IsVisible()) {
                s32 tab = dLytControlGame_c::getInstance()->getPauseDisp00Tab();
                if (tab == dLytPauseMgr_c::TAB_POUCH) {
                    item = getPouchItemIdForIndex(idx, true);
                    if (item == 0) {
                        item = ITEM_POUCH_EXPANSION;
                    }
                    selectionType = dLytPauseMgr_c::SELECT_POUCH;
                } else if (tab == dLytPauseMgr_c::TAB_DOWSING) {
                    item = getDowsingItemIdForIndex(idx);
                    selectionType = dLytPauseMgr_c::SELECT_DOWSING;
                } else {
                    item = getBWheelItemIdForIndex(idx);
                    selectionType = dLytPauseMgr_c::SELECT_BWHEEL;
                }
                pause->setSelection(selectionType, item, false);
                navTarget = idx + 1;
                mCurrentNavTarget = navTarget;
                mHasSelection = true;
            } else {
                idx = -1;
                pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
                mAnm[PAUSE_DISP_00_ANIM_ONOFF_TEXT].setFrame(0.0f);
            }
        }

        if (mPrevNavTarget != navTarget && idx != -1) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_P1_POINT_ITEM);
            dRumble_c::start(dRumble_c::sRumblePreset1, 1);
        }
        mPrevNavTarget = navTarget;

        s32 tab = dLytControlGame_c::getInstance()->getPauseDisp00Tab();
        s32 anmIdx = tab == dLytPauseMgr_c::TAB_POUCH   ? PAUSE_DISP_00_ANIM_ONOFF_POUCH_OFFSET :
                     tab == dLytPauseMgr_c::TAB_DOWSING ? PAUSE_DISP_00_ANIM_ONOFF_DOWSING_OFFSET :
                                                          PAUSE_DISP_00_ANIM_ONOFF_ITEM_OFFSET;

        int i;
        d2d::AnmGroup_c *anm = &mAnm[anmIdx];
        for (i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
            if (idx == i) {
                anm->play();
            } else {
                playBackwards(*anm);
            }
            anm++;
        }
        playOnOffTabAnim();
    }
}
void dLytPauseDisp00_c::finalizeState_Ring() {
    mLyt.findPane("N_itemArrow_00")->SetVisible(false);
    loadRingText(RING_TEXT_INITIAL_TAB);
}

void dLytPauseDisp00_c::initializeState_GetDemo() {
    mIsChangingState = true;
    mStep = 0;
    mGetDemoTimer = 0;
}
void dLytPauseDisp00_c::executeState_GetDemo() {
    switch (mStep) {
        case 0: {
            mIsChangingState = false;
            if (mGetDemoTimer < 2) {
                mGetDemoTimer++;
            } else {
                mGetDemoTimer = 0;
                mStep = 1;
            }
            break;
        }
        case 1: {
            mStep = 2;
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_ITEM_SET_FX);
            nw4r::math::MTX34 mtx = mLyt.findPane("N_saifuAll_00")->GetGlobalMtx();
            nw4r::math::VEC3 v(0.0f, 0.0f, 0.0f);
            MTXMultVec(mtx, v, v);
            mVec3_c v1;
            v1.set(v.x, v.y, v.z);
            dJEffManager_c::spawnUIEffect(PARTICLE_RESOURCE_ID_MAPPING_990_, v1, nullptr, nullptr, nullptr, nullptr);
            break;
        }
        case 2: {
            if (mGetDemoTimer < 35) {
                mGetDemoTimer++;
            } else {
                mGetDemoTimer = 0;
                mStep = 3;
                dTextBox_c *box;
                s32 count = dAcItem_c::getExtraWalletCount();
                box = mLyt.getTextBox("T_rupeeNum_00");
                box->SetVisible(true);
                SizedWString<32> buf;
                buf.sprintf(L"+%d", count * 300);
                box->setTextWithGlobalTextProcessor(buf);
                // Yes, the r is actually there in the Lyt files
                box = mLyt.getTextBox("T_rupeeNumrS_00");
                box->SetVisible(true);
                box->setTextWithGlobalTextProcessor(buf);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_ITEM_SET_COUNT_UP);
            }
            break;
        }
        case 3: {
            if (mGetDemoTimer < 37) {
                mGetDemoTimer++;
            } else {
                mGetDemoTimer = 0;
                mStateMgr.changeState(StateID_Wait);
            }
            break;
        }
    }
}
void dLytPauseDisp00_c::finalizeState_GetDemo() {}

void dLytPauseDisp00_c::initializeState_Out() {
    stopAnm(PAUSE_DISP_00_ANIM_IN);
    if (mDoScrollAnim == true) {
        if (dLytPauseMgr_c::GetInstance()->isNavLeft()) {
            setAnm(PAUSE_DISP_00_ANIM_SCROLL_R_OUT, 0.0f);
        } else {
            setAnm(PAUSE_DISP_00_ANIM_SCROLL_L_OUT, 0.0f);
        }
    } else {
        setAnm(PAUSE_DISP_00_ANIM_OUT, 0.0f);
    }
    mStep = 0;
}
void dLytPauseDisp00_c::executeState_Out() {
    s32 anim = PAUSE_DISP_00_ANIM_OUT;
    if (mDoScrollAnim == true) {
        anim = dLytPauseMgr_c::GetInstance()->isNavLeft() ? PAUSE_DISP_00_ANIM_SCROLL_R_OUT :
                                                            PAUSE_DISP_00_ANIM_SCROLL_L_OUT;
    }

    d2d::AnmGroup_c &anm = mAnm[anim];

    switch (mStep) {
        case 0: {
            if (anm.isEndReached() == true) {
                mStep = 1;
                mIsChangingState = true;
            }
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_None);
            return;
        }
    }

    anm.play();
}
void dLytPauseDisp00_c::finalizeState_Out() {}

void dLytPauseDisp00_c::setAnm(int idx, f32 value) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.bind(false);
    anm.setAnimEnable(true);
    anm.setFrame(value);
}

void dLytPauseDisp00_c::stopAnm(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytPauseDisp00_c::playBackwards(d2d::AnmGroup_c &anm) {
    f32 frame = anm.getFrame();
    if (frame) {
        frame -= 1.0f;
        if (frame <= 0.0f) {
            frame = 0.0f;
        }
        anm.setFrame(frame);
    }
}

void dLytPauseDisp00_c::setupDisp() {
    setupInventoryWheel();
    setupHeartPieces();
    setupWallets();
    setupMitts();
    setupSailcloth();
    setupWaterDragonScale();
    setupFireshieldEarrings();
    setupSongsAndLifeTree();
    setupTabletTriforce();
    setupSword();
    setupStoneOfTrials();

    mpBoundings[PAUSE_DISP_00_BOUNDING_CALIB_BTN_0]->SetVisible(true);

    dLytCommonIconItem_c *icon;
    icon = &mIcons[PAUSE_DISP_00_ICONS_B_WHEEL_OFFSET];
    for (s32 i = 0; i < PAUSE_DISP_00_NUM_SUBPANES; i++) {
        icon->setVisible(false);
        icon++;
    }

    if (StoryflagManager::sInstance->getFlag(STORYFLAG_UI_MODE_UNLOCKED)) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_UITYPE_BTN, 1.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_UI_TYPE]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_UITYPE_BTN, 0.0f);
    }

    if (StoryflagManager::sInstance->getFlag(STORYFLAG_UI_MODE_NOTICE)) {
        setAnm(PAUSE_DISP_00_ANIM_CALL, 0.0f);
        mCallTimerMaybe = 1;
    }
}

void dLytPauseDisp00_c::setupInventoryWheel() {
    dLytCommonIconItem_c *icon;
    s32 animIndex;
    f32 haveFrame;

    // B-wheel
    icon = &mIcons[PAUSE_DISP_00_ICONS_B_WHEEL_OFFSET];
    for (s32 i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
        animIndex = i + PAUSE_DISP_00_ANIM_HAVE_ITEM_OFFSET;
        haveFrame = 0.0f;
        icon->setUnk(false);
        if (isBWheelIndexWithNumber(i)) {
            icon->setHasNumber(true);
            icon->setNumber(getNumberForBWheelIndex(i));
        } else {
            icon->setHasNumber(false);
        }

        if (getItemLevelForBWheelIndex(i) != 0) {
            u8 id = getLytItemIdForBWheelIndex(i);
            if (id != LYT_CMN_ItemInvalid) {
                if (isBWheelIndexBocoburinLocked(i, true)) {
                    icon->setBocoburinLocked(true);
                    icon->setHasNumber(false);
                }
                haveFrame = 1.0f;
                icon->setItem(id);
            }
        }
        if (isBWheelIndexWithNumber(i)) {
            u8 color = getNumberColorForBWheelIndex(i);
            if (color != LYT_ITEM_COLOR_NONE) {
                icon->setNumberColor(color);
            }
        }

        setAnm(animIndex, haveFrame);
        icon++;
    }

    // Pouch
    icon = &mIcons[PAUSE_DISP_00_ICONS_POUCH_OFFSET];
    for (s32 i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
        animIndex = i + PAUSE_DISP_00_ANIM_HAVE_POUCH_OFFSET;
        haveFrame = 0.0f;
        icon->setUnk(false);
        s32 number = getPouchItemAmount(i, true);
        if (number == -1) {
            icon->setHasNumber(false);
        } else {
            icon->setHasNumber(true);
            icon->setNumber(number);
        }

        u32 item = getPouchItemForSlot(i, true);
        // TODO why LYT_CMN_PouchPotionHealthPlusPlusHalf
        if (item != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
            if (isPouchBocoburinLocked()) {
                icon->setBocoburinLocked(true);
                icon->setHasNumber(false);
            }
            haveFrame = 1.0f;
            icon->setItem(item);
            f32 durability = getPouchShieldDurability(i, true);
            if (durability >= 0.0f) {
                icon->setShieldDurability(durability);
            }

            u8 color = getPouchItemNumberColor(i, false);
            if (color != LYT_ITEM_COLOR_NONE) {
                icon->setNumberColor(color);
            }
        }

        setAnm(animIndex, haveFrame);
        icon++;
    }

    // Dowsing
    icon = &mIcons[PAUSE_DISP_00_ICONS_DOWSING_OFFSET];
    for (s32 i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
        animIndex = i + PAUSE_DISP_00_ANIM_HAVE_DOWSING_OFFSET;
        haveFrame = 0.0f;
        icon->setUnk(false);
        icon->setHasNumber(false);

        if (hasDowsingInIndex(i)) {
            u32 item = getLytIndexForDowsingIndex(i);
            if (item != LYT_CMN_DowsingInvalid) {
                haveFrame = 1.0f;
                icon->setItem(item);
            }
        }

        setAnm(animIndex, haveFrame);
        icon++;
    }

    setAnm(PAUSE_DISP_00_ANIM_OFF_LEFT_TAB, 0.0f);
    mAnm[PAUSE_DISP_00_ANIM_OFF_LEFT_TAB].setToEnd();
    setAnm(PAUSE_DISP_00_ANIM_OFF_CENTER_TAB, 0.0f);
    mAnm[PAUSE_DISP_00_ANIM_OFF_CENTER_TAB].setToEnd();
    setAnm(PAUSE_DISP_00_ANIM_OFF_RIGHT_TAB, 0.0f);
    mAnm[PAUSE_DISP_00_ANIM_OFF_RIGHT_TAB].setToEnd();
}

void dLytPauseDisp00_c::setupHeartPieces() {
    u32 heartPieceCount = getCounterByIndex(8);
    u32 containerCount = dAcItem_c::getHeartContainerHealthCount();
    if (containerCount == 24) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_HEART, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_HEART, 3.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_HEART]->SetVisible(true);
    } else if (heartPieceCount != 0) {
        f32 frame = heartPieceCount;
        frame -= 1.0f;
        setAnm(PAUSE_DISP_00_ANIM_HAVE_HEART, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_HEART, frame);
        mpBoundings[PAUSE_DISP_00_BOUNDING_HEART]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_HEART, 0.0f);
    }
}

void dLytPauseDisp00_c::setupWallets() {
    setAnm(PAUSE_DISP_00_ANIM_HAVE_POUCH, 1.0f);
    f32 frame = 0.0f;
    if (dAcItem_c::checkFlag(ITEM_TYCOON_WALLET)) {
        frame = 4.0f;
    } else if (dAcItem_c::checkFlag(ITEM_GIANT_WALLET)) {
        frame = 3.0f;
    } else if (dAcItem_c::checkFlag(ITEM_BIG_WALLET)) {
        frame = 2.0f;
    } else if (dAcItem_c::checkFlag(ITEM_MEDIUM_WALLET)) {
        frame = 1.0f;
    }
    setAnm(PAUSE_DISP_00_ANIM_POUCH, frame);
    mpBoundings[PAUSE_DISP_00_BOUNDING_POUCH]->SetVisible(true);
    s32 walletCount = dAcItem_c::getExtraWalletCount();
    SizedWString<32> buf;
    if (walletCount != 0) {
        bool bVisible = true;
        // Apparently extra wallets are the ONLY thing that can cause
        // the Gear screen to show the demo (dLytControlGame_c::openCollectionScreenDemo),
        // so we don't have to check that an extra wallets are actually what cause the
        // demo.
        if (dLytControlGame_c::getInstance()->isPauseDemo()) {
            walletCount--;
            if (walletCount == 0) {
                bVisible = false;
            }
        }
        dTextBox_c *box;
        box = mLyt.getTextBox("T_rupeeNum_00");
        box->SetVisible(bVisible);
        buf.empty();
        buf.sprintf(L"+%d", walletCount * 300);
        box->setTextWithGlobalTextProcessor(buf);
        box = mLyt.getTextBox("T_rupeeNumrS_00");
        box->SetVisible(bVisible);
        box->setTextWithGlobalTextProcessor(buf);
    } else {
        mLyt.getTextBox("T_rupeeNum_00")->SetVisible(false);
        mLyt.getTextBox("T_rupeeNumrS_00")->SetVisible(false);
    }
}

void dLytPauseDisp00_c::setupMitts() {
    s32 mittsLevel = getCurrentMittsLevel();
    if (mittsLevel != 0) {
        f32 mittsLevelF = 0.0f;
        if (mittsLevel > 1) {
            mittsLevelF = 1.0f;
        }
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MITTS, 1.0f);
        if (isMittsRestricted() == true) {
            setAnm(PAUSE_DISP_00_ANIM_BOCO_MITTS, 1.0f);
        } else {
            setAnm(PAUSE_DISP_00_ANIM_BOCO_MITTS, 0.0f);
        }
        mpBoundings[PAUSE_DISP_00_BOUNDING_MITTS]->SetVisible(true);
        setAnm(PAUSE_DISP_00_ANIM_MITTS, mittsLevelF);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MITTS, 0.0f);
    }
}

void dLytPauseDisp00_c::setupSailcloth() {
    if (hasSailcloth()) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_PARACHUTE, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_BOCO_PARACHUTE, 0.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_PARACHUTE]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_PARACHUTE, 0.0f);
    }
}

void dLytPauseDisp00_c::setupWaterDragonScale() {
    if (hasWaterDragonScale()) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_SCALE, 1.0f);
        if (isWaterDragonScaleRestricted()) {
            setAnm(PAUSE_DISP_00_ANIM_BOCO_SCALE, 1.0f);
        } else {
            setAnm(PAUSE_DISP_00_ANIM_BOCO_SCALE, 0.0f);
        }
        mpBoundings[PAUSE_DISP_00_BOUNDING_SCALE]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_SCALE, 0.0f);
    }
}

void dLytPauseDisp00_c::setupFireshieldEarrings() {
    if (dAcItem_c::checkFlag(ITEM_FIRESHIELD_EARRINGS)) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_RING, 1.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_RING]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_RING, 0.0f);
    }
}

void dLytPauseDisp00_c::setupSongsAndLifeTree() {
    if (hasGoddessHarp()) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_HARP, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_BOCO_HARP, 0.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_HARP]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_HARP, 0.0f);
    }

    s32 anm;
    for (int i = 0; i < 4; i++) {
        anm = PAUSE_DISP_00_ANIM_HAVE_MUSE_OFFSET + i;
        if (hasSong(i)) {
            setAnm(anm, 1.0f);
            mpBoundings[PAUSE_DISP_00_BOUNDING_MUSE_OFFSET + i]->SetVisible(true);
        } else {
            setAnm(anm, 0.0f);
        }
    }

    u8 songLifetreeStatus = getSongLifeTreeStatus();
    if (songLifetreeStatus == SONG_LIFETREE_HAS_SOTH) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_07, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET, 0.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_MUSE_07]->SetVisible(true);
    } else if (songLifetreeStatus != SONG_LIFETREE_NONE) {
        for (int i = 0; i < PAUSE_DISP_00_NUM_SONG_PARTS; i++) {
            if (hasSong(i + 4)) {
                setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_PART_OFFSET + i, 1.0f);
                mpBoundings[PAUSE_DISP_00_BOUNDING_MUSE_OFFSET + i + 4]->SetVisible(true);
                if (i == 2) {
                    setAnm(PAUSE_DISP_00_ANIM_HAVE_MI_TO_TANE, 0.0f);
                }
            } else {
                if (i == 2) {
                    if (dAcItem_c::checkFlag(ITEM_LIFE_TREE_FRUIT)) {
                        setAnm(PAUSE_DISP_00_ANIM_HAVE_MI_TO_TANE, 1.0f);
                        setAnm(PAUSE_DISP_00_ANIM_MI_TO_TANE, 1.0f);
                        mpBoundings[PAUSE_DISP_00_BOUNDING_MUSE_OFFSET + i + 4]->SetVisible(true);
                    } else if (dAcItem_c::checkFlag(ITEM_LIFE_TREE_SEED) &&
                               ItemflagManager::sInstance->getFlagDirect(497)) {
                        setAnm(PAUSE_DISP_00_ANIM_HAVE_MI_TO_TANE, 1.0f);
                        setAnm(PAUSE_DISP_00_ANIM_MI_TO_TANE, 0.0f);
                        mpBoundings[PAUSE_DISP_00_BOUNDING_MUSE_OFFSET + i + 4]->SetVisible(true);
                    } else {
                        setAnm(PAUSE_DISP_00_ANIM_HAVE_MI_TO_TANE, 0.0f);
                    }

                    setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_PART_OFFSET + i, 0.0f);
                } else {
                    setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_PART_OFFSET + i, 0.0f);
                }
            }
        }
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET, 1.0f);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSE_07, 0.0f);
        setAnm(PAUSE_DISP_00_ANIM_HAVE_MUSIC_SET, 0.0f);
    }
}

void dLytPauseDisp00_c::setupTabletTriforce() {
    static const s32 sTriforceHaveBoundings[] = {
        PAUSE_DISP_00_ANIM_HAVE_FORCE_OFFSET + 1,
        PAUSE_DISP_00_ANIM_HAVE_FORCE_OFFSET + 0,
        PAUSE_DISP_00_ANIM_HAVE_FORCE_OFFSET + 2,
    };

    s32 count = 0;
    for (int i = 0; i < 3; i++) {
        if (dAcItem_c::checkFlag(getTabletItemIdForIndex(i))) {
            count++;
        }
    }

    if (count != 0) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_TABLET, 1.0f);
        setAnm(PAUSE_DISP_00_ANIM_TABLETS, count - 1.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_TABLETS]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_TABLET, 0.0f);
    }

    count = 0;
    for (int i = 0; i < 3; i++) {
        if (dAcItem_c::checkFlag(getTriforceItemIdForIndex(i))) {
            count++;
            setAnm(sTriforceHaveBoundings[i], 1.0f);
        } else {
            setAnm(sTriforceHaveBoundings[i], 0.0f);
        }
    }

    if (count != 0) {
        mpBoundings[PAUSE_DISP_00_BOUNDING_TABLETS]->SetVisible(true);
        mAnm[PAUSE_DISP_00_ANIM_TABLETS].setFrame(3.0f);
        setAnm(PAUSE_DISP_00_ANIM_HAVE_FORCE_BG, 1.0f);
        // Hide tablets
        mAnm[PAUSE_DISP_00_ANIM_HAVE_TABLET].setFrame(0.0f);
    } else {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_FORCE_BG, 0.0f);
    }
}

void dLytPauseDisp00_c::setupSword() {
    static const u8 sSwordFireBoundings[] = {
        PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET + 1,
        PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET + 0,
        PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET + 2,
    };

    static const f32 sSwordFrames[] = {0.0f, 1.0f, 2.0f, 3.0f, 6.0f, 4.0f, 5.0f};

    s32 currentSwordLevel = getCurrentSwordLevel();
    if (currentSwordLevel != 0) {
        if (isSwordRestrictedBokoBase()) {
            setAnm(PAUSE_DISP_00_ANIM_BOCO_SWORD, 1.0f);
        } else {
            setAnm(PAUSE_DISP_00_ANIM_BOCO_SWORD, 0.0f);
        }
        mpBoundings[PAUSE_DISP_00_BOUNDING_SWORD]->SetVisible(true);
    }

    setAnm(PAUSE_DISP_00_ANIM_SWORD, sSwordFrames[currentSwordLevel]);

    s32 currentFire = currentSwordLevel - 2;
    if (currentFire < 0) {
        currentFire = 0;
    } else if (currentFire > 3) {
        currentFire = 3;
    }

    // Note: Funny unrolled loop here, for a loop that's known to never
    // have more than three iterations
    for (int i = 0; i < currentFire; i++) {
        mpBoundings[sSwordFireBoundings[i]]->SetVisible(true);
    }
}

void dLytPauseDisp00_c::setupStoneOfTrials() {
    if (StoryflagManager::sInstance->getFlag(STORYFLAG_STONE_OF_TRIALS_PLACED)) {
        setAnm(PAUSE_DISP_00_ANIM_HAVE_SHIREN, 2.0f);
        mpBoundings[PAUSE_DISP_00_BOUNDING_SHIREN]->SetVisible(true);
    } else {
        if (dAcItem_c::checkFlag(ITEM_STONE_OF_TRIALS)) {
            setAnm(PAUSE_DISP_00_ANIM_HAVE_SHIREN, 1.0f);
            mpBoundings[PAUSE_DISP_00_BOUNDING_SHIREN]->SetVisible(true);
        } else {
            setAnm(PAUSE_DISP_00_ANIM_HAVE_SHIREN, 0.0f);
        }
    }
}

void dLytPauseDisp00_c::setupRingIcons(s32 tab) {
    switch (tab) {
        case dLytPauseMgr_c::TAB_POUCH: {
            s32 offset = PAUSE_DISP_00_BOUNDING_RING_OFFSET;
            for (int i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
                bool visible = false;
                // TODO why LYT_CMN_PouchPotionHealthPlusPlusHalf
                if (getPouchItemForSlot(i, true) != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
                    visible = true;
                }
                mpBoundings[offset]->SetVisible(visible);
                offset++;
            }
            break;
        }
        case dLytPauseMgr_c::TAB_DOWSING: {
            s32 offset = PAUSE_DISP_00_BOUNDING_RING_OFFSET;
            for (int i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
                bool visible = false;
                if (hasDowsingInIndex(i)) {
                    visible = true;
                }
                mpBoundings[offset]->SetVisible(visible);
                offset++;
            }
            break;
        }
        case dLytPauseMgr_c::TAB_ITEM:
        default:                       {
            s32 offset = PAUSE_DISP_00_BOUNDING_RING_OFFSET;
            for (int i = 0; i < PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
                bool visible = false;
                if (getItemLevelForBWheelIndex(i) != 0) {
                    visible = true;
                }
                mpBoundings[offset]->SetVisible(visible);
                offset++;
            }
            break;
        }
    }
}

s32 dLytPauseDisp00_c::updateSelection() {
    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    d2d::AnmGroup_c *pAnm;
    int i;

    if (!pause->isStateWait()) {
        pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
        return 0;
    }

    s32 target = 0;
    if (dPadNav::isPointerVisible()) {
        target = getPointerPane();
        if (target != 0) {
            mCurrentNavTarget = target;
        }
    } else {
        if (mCurrentNavTarget != 0) {
            if (!dPadNav::isPrevPointerVisible()) {
                s32 nav = dPadNav::getFSStickNavDirection();
                if (nav != dPadNav::FS_STICK_NONE) {
                    const u8 *directions = sNavTable1[navTargetToBounding(mCurrentNavTarget)];
                    s32 newTarget = directions[nav - 1];
                    if (getSongLifeTreeStatus() == SONG_LIFETREE_HAS_SOTH_PARTS_OR_SEED) {
                        const u8 *directions = sNavTable2[navTargetToBounding(mCurrentNavTarget)];
                        newTarget = directions[nav - 1];
                    }
                    if (newTarget != 0) {
                        mCurrentNavTarget = newTarget;
                    }
                }
            }
        } else {
            mCurrentNavTarget = boundingToNavTarget(PAUSE_DISP_00_BOUNDING_UI_TYPE);
        }

        if (mCurrentNavTarget != 0) {
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundings[navTargetToBounding(mCurrentNavTarget)]);
        }

        target = mCurrentNavTarget;
    }

    if (target == 0) {
        pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
    } else {
        u8 tab;
        switch (navTargetToBounding(target)) {
            case PAUSE_DISP_00_BOUNDING_CALIB_BTN_3:
                pause->setSelection(dLytPauseMgr_c::SELECT_CATEGORY, 5, false);
                break;
            case PAUSE_DISP_00_BOUNDING_CALIB_BTN_0:
                pause->setSelection(dLytPauseMgr_c::SELECT_CATEGORY, 5, false);
                break;
            case PAUSE_DISP_00_BOUNDING_UI_TYPE:
                if (mpBoundings[PAUSE_DISP_00_BOUNDING_UI_TYPE]->IsVisible()) {
                    pause->setSelection(dLytPauseMgr_c::SELECT_CATEGORY, 4, false);
                } else {
                    pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
                }
                break;
            case PAUSE_DISP_00_BOUNDING_ARROW_LEFT:
            case PAUSE_DISP_00_BOUNDING_ARROW_RIGHT:
                pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
                pause->setSelectedArrowBounding(navTargetToBounding(target - PAUSE_DISP_00_BOUNDING_ARROW_OFFSET));
                break;
            case PAUSE_DISP_00_BOUNDING_LEFT_TAB: {
                tab = dLytPauseMgr_c::TAB_DOWSING + 1;
                goto anyTab;
            }
            case PAUSE_DISP_00_BOUNDING_CENTER_TAB: {
                tab = dLytPauseMgr_c::TAB_POUCH + 1;
                goto anyTab;
            }
            case PAUSE_DISP_00_BOUNDING_RIGHT_TAB: {
                tab = dLytPauseMgr_c::TAB_ITEM + 1;
                goto anyTab;
            }
            anyTab: {
                pause->setCurrentSelectionTab(tab);
                if (mpBoundings[navTargetToBounding(target)]->IsVisible()) {
                    pause->setSelection(dLytPauseMgr_c::SELECT_CATEGORY, tab, false);
                } else {
                    pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
                }
                static const u8 sTabLookup[] = {
                    PAUSE_DISP_00_BOUNDING_RIGHT_TAB,
                    PAUSE_DISP_00_BOUNDING_CENTER_TAB,
                    PAUSE_DISP_00_BOUNDING_LEFT_TAB,
                };
                u8 pauseTab = pause->getCurrentSelectionTab() - 1;
                if (pauseTab != lytControl->getPauseDisp00Tab() && mpBoundings[sTabLookup[pauseTab]]->IsVisible()) {
                    pause->setField_0x0837(true);
                    mHasSelection = true;
                }
                break;
            }

            default: {
                u16 id;
                dLytPauseMgr_c::SelectionType_e selectionType;
                bool locked = false;
                s32 paneIdx = navTargetToBounding(target);
                switch (paneIdx) {
                    default: {
                        s32 idx = navTargetToBounding(target) - PAUSE_DISP_00_BOUNDING_RING_OFFSET;
                        if (lytControl->getPauseDisp00Tab() == dLytPauseMgr_c::TAB_POUCH) {
                            id = getPouchItemIdForIndex(idx, true);
                            if (id == ITEM_NONE) {
                                id = ITEM_POUCH_EXPANSION;
                            }
                            selectionType = dLytPauseMgr_c::SELECT_POUCH;
                            if (isPouchBocoburinLocked()) {
                                locked = true;
                            }
                        } else if (lytControl->getPauseDisp00Tab() == dLytPauseMgr_c::TAB_DOWSING) {
                            id = getDowsingItemIdForIndex(idx);
                            selectionType = dLytPauseMgr_c::SELECT_DOWSING;
                        } else {
                            id = getBWheelItemIdForIndex(idx);
                            selectionType = dLytPauseMgr_c::SELECT_BWHEEL;
                            if (isBWheelIndexBocoburinLocked(idx, true)) {
                                locked = true;
                            }
                        }
                        break;
                    }
                    case PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET + 0: {
                        id = 2;
                        goto anyFire;
                    }
                    case PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET + 1: {
                        id = 1;
                        goto anyFire;
                    }
                    case PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET + 2: {
                        id = 3;
                        goto anyFire;
                    }
                    anyFire: {
                        selectionType = dLytPauseMgr_c::SELECT_FIRE;
                        break;
                    }
                    // yes
                    case INT_MIN ... PAUSE_DISP_00_BOUNDING_SWORD_FIRE_OFFSET - 1:
                    case PAUSE_DISP_00_BOUNDING_SHIREN:                            {
                        id = getPointedItemSpecial(paneIdx, &locked);
                        selectionType = dLytPauseMgr_c::SELECT_ITEM;
                        break;
                    }
                }
                if (!dPadNav::isPrevPointerVisible()) {
                    if (!mpBoundings[navTargetToBounding(target)]->IsVisible()) {
                        selectionType = dLytPauseMgr_c::SELECT_NONE;
                        id = 0;
                    }
                }
                pause->setSelection(selectionType, id, locked);
            }
        }
    }

    // TODO: this part needs to be fixed before we can deal with the regswaps.
    // Part of the `mpBoundings[navTargetToBounding(target)]->IsVisible()` here is kept around
    // until the end of the function where the same call appears.
    pAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_TABLET];
    for (i = PAUSE_DISP_00_BOUNDING_TABLETS; i < PAUSE_DISP_00_BOUNDING_HARP; i++) {
        s32 anim = PAUSE_DISP_00_ANIM_ONOFF_TABLET + i;
        if (target != 0 && navTargetToBounding(target) == i && mpBoundings[navTargetToBounding(target)]->IsVisible()) {
            pAnm->play();
        } else {
            playBackwards(*pAnm);
        }

        if (anim == PAUSE_DISP_00_ANIM_ONOFF_MUSE_OFFSET + 6) {
            d2d::AnmGroup_c *museAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_MI_TO_TANE];
            if (target != 0 && navTargetToBounding(target) == i) {
                museAnm->play();
            } else {
                playBackwards(*museAnm);
            }
        }
        pAnm++;
    }

    pAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_HARP];
    for (i = PAUSE_DISP_00_BOUNDING_HARP; i < PAUSE_DISP_00_BOUNDING_SHIREN + 1; i++) {
        if (target != 0 && navTargetToBounding(target) == i) {
            pAnm->play();
        } else {
            playBackwards(*pAnm);
        }
        pAnm++;
    }

    pAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_SWORD];
    if (target != 0 && navTargetToBounding(target) == PAUSE_DISP_00_BOUNDING_SWORD) {
        pAnm->play();
    } else {
        playBackwards(*pAnm);
    }

    pAnm = &mAnm[PAUSE_DISP_00_ANIM_UI_TYPE_BTN];
    if (target != 0 && navTargetToBounding(target) == PAUSE_DISP_00_BOUNDING_UI_TYPE) {
        pause->setField_0x083C(true);
        pAnm->play();
    } else {
        playBackwards(*pAnm);
    }

    pAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_CALIB_BTN];
    if (target != 0 && navTargetToBounding(target) == PAUSE_DISP_00_BOUNDING_CALIB_BTN_0) {
        pause->setField_0x083D(true);
        pAnm->play();
    } else {
        playBackwards(*pAnm);
    }

    pAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_TRIFORCE];
    if (target != 0 && navTargetToBounding(target) == PAUSE_DISP_00_BOUNDING_TABLETS &&
        mpBoundings[PAUSE_DISP_00_BOUNDING_TABLETS]->IsVisible()) {
        pAnm->play();
    } else {
        playBackwards(*pAnm);
    }

    pAnm = &mAnm[PAUSE_DISP_00_ANIM_ONOFF_SHIREN];
    if (target != 0 && navTargetToBounding(target) == PAUSE_DISP_00_BOUNDING_SHIREN) {
        pAnm->play();
    } else {
        playBackwards(*pAnm);
    }

    s32 tab = lytControl->getPauseDisp00Tab();
    s32 anmIdx = tab == dLytPauseMgr_c::TAB_POUCH   ? PAUSE_DISP_00_ANIM_ONOFF_POUCH_OFFSET :
                 tab == dLytPauseMgr_c::TAB_DOWSING ? PAUSE_DISP_00_ANIM_ONOFF_DOWSING_OFFSET :
                                                      PAUSE_DISP_00_ANIM_ONOFF_ITEM_OFFSET;

    pAnm = &mAnm[anmIdx];
    for (i = PAUSE_DISP_00_BOUNDING_RING_OFFSET;
         i < PAUSE_DISP_00_BOUNDING_RING_OFFSET + PAUSE_DISP_00_ICONS_NUM_ITEMS_ON_WHEEL; i++) {
        if (target != 0 && navTargetToBounding(target) == i) {
            f32 frame = pAnm->getFrame();
            if (pAnm->getAnimDuration() - 1.0f > frame) {
                pAnm->setFrame(frame + 1.0f);
            }
        } else {
            f32 frame = pAnm->getFrame();
            if (frame) {
                frame -= 1.0f;
                if (frame <= 0.0f) {
                    frame = 0.0f;
                }
                pAnm->setFrame(frame);
            }
        }
        pAnm++;
    }

    s32 tab1 = lytControl->getPauseDisp00Tab();
    playOnOffTabAnim();
    switch (navTargetToBounding(target)) {
        case PAUSE_DISP_00_BOUNDING_LEFT_TAB: {
            if (tab1 != 2) {
                mAnm[PAUSE_DISP_00_ANIM_OFF_LEFT_TAB].setFrame(0.0f);
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_CENTER_TAB: {
            if (tab1 != 1) {
                mAnm[PAUSE_DISP_00_ANIM_OFF_CENTER_TAB].setFrame(0.0f);
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_RIGHT_TAB: {
            if (tab1 != 0) {
                mAnm[PAUSE_DISP_00_ANIM_OFF_RIGHT_TAB].setFrame(0.0f);
            }
            break;
        }
    }

    if (target == 0 && isPointingAtRingIcon() == true) {
        u16 tab = dLytPauseMgr_c::TAB_ITEM + 1;
        if (lytControl->getPauseDisp00Tab() == dLytPauseMgr_c::TAB_POUCH) {
            tab = dLytPauseMgr_c::TAB_POUCH + 1;
        } else if (lytControl->getPauseDisp00Tab() == dLytPauseMgr_c::TAB_DOWSING) {
            tab = dLytPauseMgr_c::TAB_DOWSING + 1;
        }
        pause->setSelection(dLytPauseMgr_c::SELECT_RING, tab, false);
    }

    dSndSmallEffectMgr_c *soundMgr = dSndSmallEffectMgr_c::GetInstance();
    if (target != 0 && mPrevNavTarget != target) {
        s32 tab2 = lytControl->getPauseDisp00Tab();
        bool rumble = true;
        switch (navTargetToBounding(target)) {
            case PAUSE_DISP_00_BOUNDING_UI_TYPE: {
                soundMgr->playSound(SE_S_MENU_P1_POINT_GUIDE);
                break;
            }
            case PAUSE_DISP_00_BOUNDING_CALIB_BTN_0: {
                soundMgr->playSound(SE_S_MENU_P1_POINT_MPLUS);
                break;
            }
            case PAUSE_DISP_00_BOUNDING_LEFT_TAB: {
                if (mpBoundings[PAUSE_DISP_00_BOUNDING_LEFT_TAB]->IsVisible()) {
                    if (cM::isZero(mAnm[PAUSE_DISP_00_ANIM_OFF_LEFT_TAB].getFrame())) {
                        soundMgr->playSound(SE_S_MENU_P1_POINT_C);
                    }
                    if (tab2 == dLytPauseMgr_c::TAB_DOWSING) {
                        rumble = false;
                    }
                }
                break;
            }
            case PAUSE_DISP_00_BOUNDING_CENTER_TAB: {
                if (mpBoundings[PAUSE_DISP_00_BOUNDING_CENTER_TAB]->IsVisible()) {
                    if (cM::isZero(mAnm[PAUSE_DISP_00_ANIM_OFF_CENTER_TAB].getFrame())) {
                        soundMgr->playSound(SE_S_MENU_P1_POINT_MINUS);
                    }
                    if (tab2 == dLytPauseMgr_c::TAB_POUCH) {
                        rumble = false;
                    }
                }
                break;
            }
            case PAUSE_DISP_00_BOUNDING_RIGHT_TAB: {
                if (mpBoundings[PAUSE_DISP_00_BOUNDING_RIGHT_TAB]->IsVisible()) {
                    if (cM::isZero(mAnm[PAUSE_DISP_00_ANIM_OFF_RIGHT_TAB].getFrame())) {
                        soundMgr->playSound(SE_S_MENU_P1_POINT_B);
                    }
                    if (tab2 == dLytPauseMgr_c::TAB_ITEM) {
                        rumble = false;
                    }
                }
                break;
            }
            case PAUSE_DISP_00_BOUNDING_CALIB_BTN_3: {
                soundMgr->playSound(SE_S_MENU_P1_POINT_TOGGLE);
                break;
            }
            // yes
            case INT_MIN ... PAUSE_DISP_00_BOUNDING_UI_TYPE - 1: {
                soundMgr->playSound(SE_S_MENU_P1_POINT_ITEM);
                break;
            }
        }

        if (rumble == true) {
            dRumble_c::start(dRumble_c::sRumblePreset1, 1);
        }
    }

    mPrevNavTarget = target;
    if (target != 0 && navTargetToBounding(target) != PAUSE_DISP_00_BOUNDING_CALIB_BTN_3 &&
        !dPadNav::isPointerVisible() && !mpBoundings[navTargetToBounding(target)]->IsVisible()) {
        target = 0;
    }

    if (navTargetToBounding(target) >= PAUSE_DISP_00_BOUNDING_ARROW_LEFT &&
        navTargetToBounding(target) <= PAUSE_DISP_00_BOUNDING_RIGHT_TAB) {
        target = 0;
    }

    return target;
}

s32 dLytPauseDisp00_c::getPointerPane() const {
    int idx = 0;
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (d != nullptr && d->getType() == 'lyt ') {
        for (int i = 0; i < PAUSE_DISP_00_NUM_BOUNDINGS - 1; i++) {
            if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundings[i]) {
                idx = i + 1;
                // missed optimization:
                // break;
            }
        }
    }
    return idx;
}

bool dLytPauseDisp00_c::isPointingAtRingIcon() {
    if (!mLyt.findPane("N_text_00")->IsVisible()) {
        return false;
    }

    bool ret = false;
    nw4r::lyt::Size sz = mLyt.findPane("N_ringIcon_00")->GetSize();
    f32 radiusSq = sz.width * 0.5f;
    radiusSq *= radiusSq;
    mVec2_c csPos = dCsBase_c::GetInstance()->getCursorIf()->getCursorPos();
    nw4r::math::MTX34 mtx = mLyt.findPane("N_ringIcon_00")->GetGlobalMtx();
    nw4r::math::VEC3 v(0.0f, 0.0f, 0.0f);
    MTXMultVec(mtx, v, v);
    // TODO maybe fake
    mVec2_c pos2 = mVec2_c(v.x, v.y);
    mVec2_c dist = pos2;
    dist -= csPos;
    if (dist.squaredLength() < radiusSq) {
        ret = true;
    }

    return ret;
}

void dLytPauseDisp00_c::hideItemIcons() {
    for (int i = 0; i < PAUSE_DISP_00_NUM_SUBPANES; i++) {
        mIcons[i].setOff();
        mIcons[i].setVisible(false);
    }
}

u16 dLytPauseDisp00_c::getPointedItemSpecial(s32 paneIdx, bool *pLocked) {
    u16 id = 0;
    switch (paneIdx) {
        case PAUSE_DISP_00_BOUNDING_TABLETS: {
            s32 count = 0;
            for (int i = 0; i < 3; i++) {
                if (dAcItem_c::checkFlag(getTabletItemIdForIndex(i))) {
                    count++;
                }
            }
            if (count != 0) {
                count--;
            }
            id = getTabletItemIdForIndex(count);
            count = 0;
            for (int i = 0; i < 3; i++) {
                if (dAcItem_c::checkFlag(getTriforceItemIdForIndex(i))) {
                    count++;
                }
            }
            if (count != 0) {
                id = getTriforceItemIdForIndex(count - 1);
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_HEART: {
            id = getCurrentHeartPieceItemId();
            break;
        }
        case PAUSE_DISP_00_BOUNDING_POUCH: {
            id = getCurrentWalletItemId();
            break;
        }
        case PAUSE_DISP_00_BOUNDING_PARACHUTE: {
            id = getCurrentSailclothItemId();
            break;
        }
        case PAUSE_DISP_00_BOUNDING_MITTS: {
            id = getCurrentMittsItemId();
            if (isMittsRestricted() == true) {
                *pLocked = true;
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_SCALE: {
            id = getCurrentWaterDragonScaleItemId();
            if (isWaterDragonScaleRestricted()) {
                *pLocked = true;
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_RING: {
            id = getCurrentEarringsItemId();
            break;
        }
        case PAUSE_DISP_00_BOUNDING_MUSE_OFFSET ... PAUSE_DISP_00_BOUNDING_MUSE_OFFSET + 7: {
            s32 song = paneIdx - PAUSE_DISP_00_BOUNDING_MUSE_OFFSET;
            if (paneIdx == PAUSE_DISP_00_BOUNDING_MUSE_OFFSET + 6) {
                if (StoryflagManager::sInstance->getFlag(STORYFLAG_THUNDER_DRAGON_SONG)) {
                    id = getSongItemIdForIndex(song);
                } else if (dAcItem_c::checkFlag(ITEM_LIFE_TREE_FRUIT)) {
                    id = ITEM_LIFE_TREE_FRUIT;
                } else if (dAcItem_c::checkFlag(ITEM_LIFE_TREE_SEED)) {
                    id = ITEM_LIFE_TREE_SEED;
                }
            } else {
                id = getSongItemIdForIndex(song);
            }
        } break;
        case PAUSE_DISP_00_BOUNDING_HARP: {
            id = getCurrentGoddessHarpItemId();
            if (isHarpRestrictedBokoBase()) {
                *pLocked = true;
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_SWORD: {
            id = getCurrentSwordItemId();
            if (isSwordRestrictedBokoBase()) {
                *pLocked = true;
            }
            break;
        }
        case PAUSE_DISP_00_BOUNDING_SHIREN: {
            id = ITEM_STONE_OF_TRIALS;
            break;
        }
    }

    return id;
}

void dLytPauseDisp00_c::executeCall() {
    if (mCallTimerMaybe == 1) {
        d2d::AnmGroup_c &anm = mAnm[PAUSE_DISP_00_ANIM_CALL];
        if (cM::isZero(anm.getFrame()) == true) {
            if (mStopCallRequest == true) {
                mStopCallRequest = false;
                mCallTimerMaybe = 0;
                stopAnm(PAUSE_DISP_00_ANIM_CALL);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_PLAY_GUIDE_BUTTON_BLINK);
            }
        }
        anm.play();
    }
}

void dLytPauseDisp00_c::playOnOffTabAnim() {
    d2d::AnmGroup_c *anm = &mAnm[PAUSE_DISP_00_ANIM_OFF_LEFT_TAB];
    for (int i = 0; i < 3; i++) {
        anm->play();
        anm++;
    }
}

void dLytPauseDisp00_c::loadRingText(u32 cmd) {
    if (cmd == RING_TEXT_RELEASE_TO_CONFIRM) {
        // "Release to confirm."
        mLyt.loadTextVariant("T_decide_00", 0);
        mLyt.loadTextVariant("T_decideS_00", 0);
    } else {
        s32 msgIdx = 1;
        s32 tab = dLytControlGame_c::getInstance()->getPauseDisp00Tab();
        if (cmd == RING_TEXT_CURRENT_TAB) {
            tab = dLytPauseMgr_c::GetInstance()->getCurrentDisp00Tab();
        }
        if (tab == dLytPauseMgr_c::TAB_POUCH) {
            msgIdx = 2;
        } else if (tab == dLytPauseMgr_c::TAB_DOWSING) {
            msgIdx = 3;
        }
        // "Items"/"Pouch"/"Dowsing"
        mLyt.loadTextVariant("T_decide_00", msgIdx);
        mLyt.loadTextVariant("T_decideS_00", msgIdx);
    }

    dWindow_c *window = mLyt.getWindow("W_bgP_01");
    window->UpdateSize(mLyt.getSizeBoxInWindow("W_bgP_01"), 32.0f);
}
