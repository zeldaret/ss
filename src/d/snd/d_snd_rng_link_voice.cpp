#include "d/snd/d_snd_rng_link_voice.h"

#include "common.h"
#include "d/snd/d_snd_rng_id.h"
#include "d/snd/d_snd_source_player_head.h"
#include "d/snd/d_snd_wzsound.h"

struct dSndLinkVoiceConfig {
    /* 0x00 */ u16 begin;
    /* 0x02 */ u16 end;
    /* 0x04 */ s16 mGenChance;
    /* 0x06 */ s8 priority;
    /* 0x07 */ bool priorityTiebreaker;
};

static const dSndLinkVoiceConfig sLinkVoiceConfig[] = {
    /* SND_LV_PRACTICE_S*/
    {       SE_LV_PRACTICE_S_01,        SE_LV_PRACTICE_S_ff,  70,   9, false},
    /* SND_LV_PRACTICE_M*/
    {       SE_LV_PRACTICE_M_01,        SE_LV_PRACTICE_M_ff, 100,  10, false},
    /* SND_LV_PRACTICE_L*/
    {       SE_LV_PRACTICE_L_01,        SE_LV_PRACTICE_L_ff, 100,  10, false},
    /* SND_LV_PRACTICE_KAITEN_M*/
    {SE_LV_PRACTICE_KAITEN_M_01, SE_LV_PRACTICE_KAITEN_M_ff, 100,  11, false},
    /* SND_LV_ATTACK_S*/
    {         SE_LV_ATTACK_S_01,          SE_LV_ATTACK_S_ff, 100,  10, false},
    /* SND_LV_ATTACK_M*/
    {         SE_LV_ATTACK_M_01,          SE_LV_ATTACK_M_ff, 100,  10, false},
    /* SND_LV_ATTACK_L*/
    {         SE_LV_ATTACK_L_01,          SE_LV_ATTACK_L_ff, 100,  10, false},
    /* SND_LV_SAVE_POWER*/
    {       SE_LV_SAVE_POWER_01,        SE_LV_SAVE_POWER_ff, 100,   0, false},
    /* SND_LV_ATTACK_KAITEN_M*/
    {  SE_LV_ATTACK_KAITEN_M_01,   SE_LV_ATTACK_KAITEN_M_ff, 100,  11, false},
    /* SND_LV_NOTICE*/
    {           SE_LV_NOTICE_01,            SE_LV_NOTICE_ff, 100,   0, false},
    /* SND_LV_PRACTICE_SHIELD*/
    {  SE_LV_PRACTICE_SHIELD_01,   SE_LV_PRACTICE_SHIELD_ff, 100,   8, false},
    /* SND_LV_ATTACK_SHIELD*/
    {    SE_LV_ATTACK_SHIELD_01,     SE_LV_ATTACK_SHIELD_ff, 100,   8, false},
    /* SND_LV_ATTACK_SHIELD_GJ*/
    { SE_LV_ATTACK_SHIELD_GJ_01,  SE_LV_ATTACK_SHIELD_GJ_ff, 100,   9, false},
    /* SND_LV_SHELD_GUARD*/
    {      SE_LV_SHELD_GUARD_01,       SE_LV_SHELD_GUARD_ff, 100,   8, false},
    /* SND_LV_SHELD_DOWN*/
    {       SE_LV_SHELD_DOWN_01,        SE_LV_SHELD_DOWN_ff, 100,  15, false},
    /* SND_LV_SHELD_BREAK*/
    {      SE_LV_SHELD_BREAK_01,       SE_LV_SHELD_BREAK_ff, 100,  16, false},
    /* SND_LV_ATTACK_DOWN*/
    {      SE_LV_ATTACK_DOWN_01,       SE_LV_ATTACK_DOWN_ff, 100,  16, false},
    /* SND_LV_DAMAGE_S*/
    {         SE_LV_DAMAGE_S_01,          SE_LV_DAMAGE_S_ff, 100,  15, false},
    /* SND_LV_DAMAGE_L*/
    {         SE_LV_DAMAGE_L_01,          SE_LV_DAMAGE_L_ff, 100,  16, false},
    /* SND_LV_DAMAGE_FIRE_S*/
    {    SE_LV_DAMAGE_FIRE_S_01,     SE_LV_DAMAGE_FIRE_S_ff, 100,  15, false},
    /* SND_LV_DAMAGE_FIRE_L*/
    {    SE_LV_DAMAGE_FIRE_L_01,     SE_LV_DAMAGE_FIRE_L_ff, 100,  16, false},
    /* SND_LV_DAMAGE_ELEC_S*/
    {    SE_LV_DAMAGE_ELEC_S_01,     SE_LV_DAMAGE_ELEC_S_ff, 100,  15, false},
    /* SND_LV_DAMAGE_ELEC_L*/
    {    SE_LV_DAMAGE_ELEC_L_01,     SE_LV_DAMAGE_ELEC_L_ff, 100,  15, false},
    /* SND_LV_STRUGGLE_S*/
    {       SE_LV_STRUGGLE_S_01,        SE_LV_STRUGGLE_S_ff, 100,  14, false},
    /* SND_LV_STRUGGLE_L*/
    {       SE_LV_STRUGGLE_L_01,        SE_LV_STRUGGLE_L_ff, 100,  14, false},
    /* SND_LV_GROAN*/
    {            SE_LV_GROAN_01,             SE_LV_GROAN_ff, 100, 246, false},
    /* SND_LV_ESCAPE*/
    {           SE_LV_ESCAPE_01,            SE_LV_ESCAPE_ff, 100,  15,  true},
    /* SND_LV_DAMAGE_MAGMA*/
    {     SE_LV_DAMAGE_MAGMA_01,      SE_LV_DAMAGE_MAGMA_ff, 100,  17, false},
    /* SND_LV_DAMAGE_QUICKSAND*/
    { SE_LV_DAMAGE_QUICKSAND_01,  SE_LV_DAMAGE_QUICKSAND_ff, 100,  17, false},
    /* SND_LV_DIE_L*/
    {            SE_LV_DIE_L_01,             SE_LV_DIE_L_ff, 100, 100,  true},
    /* SND_LV_DIE_S*/
    {            SE_LV_DIE_S_01,             SE_LV_DIE_S_ff, 100, 100,  true},
    /* SND_LV_DIE_WATER*/
    {        SE_LV_DIE_WATER_01,         SE_LV_DIE_WATER_ff, 100, 100,  true},
    /* SND_LV_RESTART*/
    {          SE_LV_RESTART_01,           SE_LV_RESTART_ff, 100, 100,  true},
    /* SND_LV_STAND_UP*/
    {         SE_LV_STAND_UP_01,          SE_LV_STAND_UP_ff, 100,   0, false},
    /* SND_LV_PHEW*/
    {             SE_LV_PHEW_01,              SE_LV_PHEW_ff, 100,   0, false},
    /* SND_LV_TIRED_S*/
    {          SE_LV_TIRED_S_01,           SE_LV_TIRED_S_ff, 100, 254,  true},
    /* SND_LV_TIRED_S*/
    {          SE_LV_TIRED_S_01,           SE_LV_TIRED_S_ff, 100, 254,  true},
    /* SND_LV_TIRED_M*/
    {          SE_LV_TIRED_M_01,           SE_LV_TIRED_M_ff, 100, 254,  true},
    /* SND_LV_TIRED_L*/
    {          SE_LV_TIRED_L_01,           SE_LV_TIRED_L_ff, 100, 254,  true},
    /* SND_LV_CLIMB_TIRED_S*/
    {    SE_LV_CLIMB_TIRED_S_01,     SE_LV_CLIMB_TIRED_S_ff,  60, 254, false},
    /* SND_LV_CLIMB_TIRED_S*/
    {    SE_LV_CLIMB_TIRED_S_01,     SE_LV_CLIMB_TIRED_S_ff,  60, 254, false},
    /* SND_LV_CLIMB_TIRED_M*/
    {    SE_LV_CLIMB_TIRED_M_01,     SE_LV_CLIMB_TIRED_M_ff,  60, 254, false},
    /* SND_LV_CLIMB_TIRED_L*/
    {    SE_LV_CLIMB_TIRED_L_01,     SE_LV_CLIMB_TIRED_L_ff, 100, 254, false},
    /* SND_LV_LIFT_TIRED_S*/
    {     SE_LV_LIFT_TIRED_S_01,      SE_LV_LIFT_TIRED_S_ff,  60, 254, false},
    /* SND_LV_LIFT_TIRED_S*/
    {     SE_LV_LIFT_TIRED_S_01,      SE_LV_LIFT_TIRED_S_ff,  60, 254, false},
    /* SND_LV_LIFT_TIRED_M*/
    {     SE_LV_LIFT_TIRED_M_01,      SE_LV_LIFT_TIRED_M_ff,  60, 254, false},
    /* SND_LV_LIFT_TIRED_L*/
    {     SE_LV_LIFT_TIRED_L_01,      SE_LV_LIFT_TIRED_L_ff, 100, 254, false},
    /* SND_LV_TIRED_RECOVER*/
    {    SE_LV_TIRED_RECOVER_01,     SE_LV_TIRED_RECOVER_ff, 100, 254,  true},
    /* SND_LV_TIRED_RECOVER_S*/
    {  SE_LV_TIRED_RECOVER_S_01,   SE_LV_TIRED_RECOVER_S_ff, 100, 254,  true},
    /* SND_LV_TIRED_RECOVER_M*/
    {  SE_LV_TIRED_RECOVER_M_01,   SE_LV_TIRED_RECOVER_M_ff, 100, 254,  true},
    /* SND_LV_TIRED_RECOVER_L*/
    {  SE_LV_TIRED_RECOVER_L_01,   SE_LV_TIRED_RECOVER_L_ff, 100, 254,  true},
    /* SND_LV_TIRED_EMPTY*/
    {      SE_LV_TIRED_EMPTY_01,       SE_LV_TIRED_EMPTY_ff, 100,   1,  true},
    /* SND_LV_PHEW*/
    {             SE_LV_PHEW_01,              SE_LV_PHEW_ff, 100,   1,  true},
    /* SND_LV_JUMP_S*/
    {           SE_LV_JUMP_S_01,            SE_LV_JUMP_S_ff, 100,   8, false},
    /* SND_LV_JUMP_M*/
    {           SE_LV_JUMP_M_01,            SE_LV_JUMP_M_ff, 100,   8, false},
    /* SND_LV_JUMP_L*/
    {           SE_LV_JUMP_L_01,            SE_LV_JUMP_L_ff, 100,   8, false},
    /* SND_LV_SHOOT_START*/
    {      SE_LV_SHOOT_START_01,       SE_LV_SHOOT_START_ff, 100, 255, false},
    /* SND_LV_SHOOT_SLOPE*/
    {      SE_LV_SHOOT_SLOPE_01,       SE_LV_SHOOT_SLOPE_ff, 100, 255, false},
    /* SND_LV_RUN_UP*/
    {           SE_LV_RUN_UP_01,            SE_LV_RUN_UP_ff, 100, 255, false},
    /* SND_LV_GRAB*/
    {             SE_LV_GRAB_01,              SE_LV_GRAB_ff, 100,   0, false},
    /* SND_LV_CLIMB_STEP*/
    {       SE_LV_CLIMB_STEP_01,        SE_LV_CLIMB_STEP_ff, 100,   1, false},
    /* SND_LV_HUNG*/
    {             SE_LV_HUNG_01,              SE_LV_HUNG_ff, 100,   0, false},
    /* SND_LV_JUMP_IVY*/
    {         SE_LV_JUMP_IVY_01,          SE_LV_JUMP_IVY_ff, 100,   0,  true},
    /* SND_LV_BACKFLIP*/
    {         SE_LV_BACKFLIP_01,          SE_LV_BACKFLIP_ff,  70,   0, false},
    /* SND_LV_ZENTEN*/
    {           SE_LV_ZENTEN_01,            SE_LV_ZENTEN_ff, 100,   0, false},
    /* SND_LV_ZENTEN_CLASH*/
    {     SE_LV_ZENTEN_CLASH_01,      SE_LV_ZENTEN_CLASH_ff, 100,  14, false},
    /* SND_LV_ZENTEN_UP*/
    {        SE_LV_ZENTEN_UP_01,         SE_LV_ZENTEN_UP_ff, 100, 247,  true},
    /* SND_LV_FALLING_HIGH*/
    {     SE_LV_FALLING_HIGH_01,      SE_LV_FALLING_HIGH_ff, 100,  15,  true},
    /* SND_LV_DAMAGE_FALL*/
    {      SE_LV_DAMAGE_FALL_01,       SE_LV_DAMAGE_FALL_ff, 100,  15,  true},
    /* SND_LV_SKYDIVE*/
    {          SE_LV_SKYDIVE_01,           SE_LV_SKYDIVE_ff, 100,   0, false},
    /* SND_LV_RIDE_BIRD*/
    {        SE_LV_RIDE_BIRD_01,         SE_LV_RIDE_BIRD_ff, 100,   0,  true},
    /* SND_LV_BLAST*/
    {            SE_LV_BLAST_01,             SE_LV_BLAST_ff, 100,   0, false},
    /* SND_LV_WIND_FLY*/
    {         SE_LV_WIND_FLY_01,          SE_LV_WIND_FLY_ff, 100,   0, false},
    /* SND_LV_BALANCE_BALL*/
    {     SE_LV_BALANCE_BALL_01,      SE_LV_BALANCE_BALL_ff, 100,   0, false},
    /* SND_LV_SLIDE_START*/
    {      SE_LV_SLIDE_START_01,       SE_LV_SLIDE_START_ff, 100, 255, false},
    /* SND_LV_CLIMB_ROPE*/
    {       SE_LV_CLIMB_ROPE_01,        SE_LV_CLIMB_ROPE_ff,  60,   0, false},
    /* SND_LV_SWING_ROPE*/
    {       SE_LV_SWING_ROPE_01,        SE_LV_SWING_ROPE_ff, 100,   0, false},
    /* SND_LV_JUMP_ROPE*/
    {        SE_LV_JUMP_ROPE_01,         SE_LV_JUMP_ROPE_ff, 100,   0, false},
    /* SND_LV_STOP_ROPE*/
    {        SE_LV_STOP_ROPE_01,         SE_LV_STOP_ROPE_ff, 100,   0, false},
    /* SND_LV_SHAKE_ROPE*/
    {       SE_LV_SHAKE_ROPE_01,        SE_LV_SHAKE_ROPE_ff, 100,   0, false},
    /* SND_LV_FOOT_MISS*/
    {        SE_LV_FOOT_MISS_01,         SE_LV_FOOT_MISS_ff, 100,   0, false},
    /* SND_LV_BREATH*/
    {           SE_LV_BREATH_01,            SE_LV_BREATH_ff, 100,   0, false},
    /* SND_LV_TAKE_BREATH*/
    {      SE_LV_TAKE_BREATH_01,       SE_LV_TAKE_BREATH_ff, 100,   0, false},
    /* SND_LV_GET_AIR*/
    {          SE_LV_GET_AIR_01,           SE_LV_GET_AIR_ff, 100,   0, false},
    /* SND_LV_DOLPHIN_JUMP*/
    {     SE_LV_DOLPHIN_JUMP_01,      SE_LV_DOLPHIN_JUMP_ff, 100,   0, false},
    /* SND_LV_LIFT_S*/
    {           SE_LV_LIFT_S_01,            SE_LV_LIFT_S_ff, 100,   0, false},
    /* SND_LV_THROW_S*/
    {          SE_LV_THROW_S_01,           SE_LV_THROW_S_ff, 100,   0, false},
    /* SND_LV_LIFT_L*/
    {           SE_LV_LIFT_L_01,            SE_LV_LIFT_L_ff, 100,   0, false},
    /* SND_LV_THROW_L*/
    {          SE_LV_THROW_L_01,           SE_LV_THROW_L_ff, 100,   0, false},
    /* SND_LV_PUSH_M*/
    {           SE_LV_PUSH_M_01,            SE_LV_PUSH_M_ff, 100,   0, false},
    /* SND_LV_DIG_GLOVE*/
    {        SE_LV_DIG_GLOVE_01,         SE_LV_DIG_GLOVE_ff, 100,   0, false},
    /* SND_LV_SWING_S*/
    {          SE_LV_SWING_S_01,           SE_LV_SWING_S_ff, 100,   0, false},
    /* SND_LV_DRINK*/
    {            SE_LV_DRINK_01,             SE_LV_DRINK_ff, 100,   0, false},
    /* SND_LV_DRINK_FIN*/
    {        SE_LV_DRINK_FIN_01,         SE_LV_DRINK_FIN_ff, 100, 255, false},
    /* SND_LV_CLIMB_WALL*/
    {       SE_LV_CLIMB_WALL_01,        SE_LV_CLIMB_WALL_ff,  60, 254, false},
    /* SND_LV_JUMPON_LIFT*/
    {      SE_LV_JUMPON_LIFT_01,       SE_LV_JUMPON_LIFT_ff, 100,   0, false},
    /* SND_LV_GET_SURPRISND*/
    {     SE_LV_GET_SURPRISE_01,      SE_LV_GET_SURPRISE_ff, 100,   0, false},
    /* SND_LV_FLOATAWAY*/
    {        SE_LV_FLOATAWAY_01,         SE_LV_FLOATAWAY_ff, 100,   0, false},
    /* SND_LV_CANNON_JUMP*/
    {      SE_LV_CANNON_JUMP_01,       SE_LV_CANNON_JUMP_ff, 100,   0, false},
    /* SND_LV_FIRE*/
    {             SE_LV_FIRE_01,              SE_LV_FIRE_ff, 100,   0, false},
    /* SND_LV_LOOK_BODY*/
    {        SE_LV_LOOK_BODY_01,         SE_LV_LOOK_BODY_ff, 100,   0, false},
    /* SND_LV_QUICKWATER*/
    {       SE_LV_QUICKWATER_01,        SE_LV_QUICKWATER_ff,  60,   0, false},
    /* SND_LV_DANGER_ROCK*/
    {      SE_LV_DANGER_ROCK_01,       SE_LV_DANGER_ROCK_ff, 100,   0, false},
    /* SND_LV_PULL_SWORD*/
    {       SE_LV_PULL_SWORD_01,        SE_LV_PULL_SWORD_ff, 100,   0, false},
    /* SND_LV_WHIP_BACK*/
    {        SE_LV_WHIP_BACK_01,         SE_LV_WHIP_BACK_ff, 100,   0, false},
    /* SND_LV_SW_PUSH*/
    {          SE_LV_SW_PUSH_01,           SE_LV_SW_PUSH_ff, 100,   0, false},
    /* SND_LV_SW_REJECT*/
    {        SE_LV_SW_REJECT_01,         SE_LV_SW_REJECT_ff, 100,   0, false},
    /* SND_LV_JUMP_DIVEWATER*/
    {   SE_LV_JUMP_DIVEWATER_01,    SE_LV_JUMP_DIVEWATER_ff, 100,   0, false},
    /* SND_LV_DAMAGE_WATER_M*/
    {   SE_LV_DAMAGE_WATER_M_01,    SE_LV_DAMAGE_WATER_M_ff, 100,  16, false},
    /* SND_LV_DAMAGE_WATER_S*/
    {   SE_LV_DAMAGE_WATER_S_01,    SE_LV_DAMAGE_WATER_S_ff, 100,  15, false},
    /* SND_LV_DAMAGE_ELEC_W*/
    {    SE_LV_DAMAGE_ELEC_W_01,     SE_LV_DAMAGE_ELEC_W_ff, 100,  15, false},
    /* SND_LV_STRUGGLE_W*/
    {       SE_LV_STRUGGLE_W_01,        SE_LV_STRUGGLE_W_ff, 100,   0, false},
    /* SND_LV_GROAN_W*/
    {          SE_LV_GROAN_W_01,           SE_LV_GROAN_W_ff, 100,   0, false},
    /* SND_LV_ESCAPE_W*/
    {         SE_LV_ESCAPE_W_01,          SE_LV_ESCAPE_W_ff, 100,   0, false},
    /* SND_LV_BREATH_TIRED_S*/
    {   SE_LV_BREATH_TIRED_S_01,    SE_LV_BREATH_TIRED_S_ff, 100, 254, false},
    /* SND_LV_BREATH_TIRED_S*/
    {   SE_LV_BREATH_TIRED_S_01,    SE_LV_BREATH_TIRED_S_ff,  30, 254, false},
    /* SND_LV_BREATH_TIRED_M*/
    {   SE_LV_BREATH_TIRED_M_01,    SE_LV_BREATH_TIRED_M_ff,  60, 254, false},
    /* SND_LV_BREATH_TIRED_L*/
    {   SE_LV_BREATH_TIRED_L_01,    SE_LV_BREATH_TIRED_L_ff, 100, 254, false},
    /* SND_LV_LIE_UP*/
    {           SE_LV_LIE_UP_01,            SE_LV_LIE_UP_ff, 100,   0, false},
    /* SND_LV_PRACTICE_PUNCH*/
    {   SE_LV_PRACTICE_PUNCH_01,    SE_LV_PRACTICE_PUNCH_ff, 100,   0, false},
    /* SND_LV_ATTACK_PUNCH*/
    {     SE_LV_ATTACK_PUNCH_01,      SE_LV_ATTACK_PUNCH_ff, 100,   0, false},
    /* SND_LV_SURPRISND_S*/
    {       SE_LV_SURPRISE_S_01,        SE_LV_SURPRISE_S_ff, 100,   0, false},
    /* SND_LV_SHUTTER_OPEN*/
    {     SE_LV_SHUTTER_OPEN_01,      SE_LV_SHUTTER_OPEN_ff, 100,   0, false},
    /* SND_LV_BIRDRACE_REACH*/
    {   SE_LV_BIRDRACE_REACH_01,    SE_LV_BIRDRACE_REACH_ff, 100,  10, false},
    /* SND_LV_ATTACK_A*/
    {         SE_LV_ATTACK_A_01,          SE_LV_ATTACK_A_ff, 100,   0, false},
    /* SND_LV_BADSMELL_TIRED_S*/
    { SE_LV_BADSMELL_TIRED_S_01,  SE_LV_BADSMELL_TIRED_S_ff, 100, 254, false},
    /* SND_LV_BADSMELL_TIRED_S*/
    { SE_LV_BADSMELL_TIRED_S_01,  SE_LV_BADSMELL_TIRED_S_ff,  50, 254, false},
    /* SND_LV_BADSMELL_TIRED_M*/
    { SE_LV_BADSMELL_TIRED_M_01,  SE_LV_BADSMELL_TIRED_M_ff,  60, 254, false},
    /* SND_LV_BADSMELL_TIRED_L*/
    { SE_LV_BADSMELL_TIRED_L_01,  SE_LV_BADSMELL_TIRED_L_ff, 100, 254, false},
    /* SND_LV_GIRA_SWORD_REJECT*/
    {SE_LV_GIRA_SWORD_REJECT_01, SE_LV_GIRA_SWORD_REJECT_ff, 100,   0, false},
    /* SND_LV_GIRA_SWORD_PULL*/
    {  SE_LV_GIRA_SWORD_PULL_01,   SE_LV_GIRA_SWORD_PULL_ff, 100,   0, false},
    /* SND_LV_GIRA_GRAPPLE*/
    {     SE_LV_GIRA_GRAPPLE_01,      SE_LV_GIRA_GRAPPLE_ff, 100,   0, false},
    /* SND_LV_SWORD_SIREN_IN*/
    {   SE_LV_SWORD_SIREN_IN_01,    SE_LV_SWORD_SIREN_IN_ff, 100,   0, false},
    /* SND_LV_APPEAR_SURPRISND*/
    {  SE_LV_APPEAR_SURPRISE_01,   SE_LV_APPEAR_SURPRISE_ff, 100,   0, false},
    /* SND_LV_EXIT_SURPRISND*/
    {    SE_LV_EXIT_SURPRISE_01,     SE_LV_EXIT_SURPRISE_ff, 100,   0, false},
    /* SND_LV_BIRD_DAMAGE*/
    {      SE_LV_BIRD_DAMAGE_01,       SE_LV_BIRD_DAMAGE_ff, 100,  15, false},
    /* SND_LV_BIRD_SPIN*/
    {        SE_LV_BIRD_SPIN_01,         SE_LV_BIRD_SPIN_ff, 100,  10, false},
    /* SND_LV_BIRD_ATTACK*/
    {      SE_LV_BIRD_ATTACK_01,       SE_LV_BIRD_ATTACK_ff, 100,   9, false},
    /* SND_LV_TRUCK_JUMP*/
    {       SE_LV_TRUCK_JUMP_01,        SE_LV_TRUCK_JUMP_ff, 100,   0, false},
    /* SND_LV_TRUCK_IN*/
    {         SE_LV_TRUCK_IN_01,          SE_LV_TRUCK_IN_ff, 100,   0, false},
    /* SND_LV_TRUCK_BRAKE*/
    {      SE_LV_TRUCK_BRAKE_01,       SE_LV_TRUCK_BRAKE_ff, 100,   0, false},
    /* SND_LV_DAMAGE_MUTEKI_S*/
    {  SE_LV_DAMAGE_MUTEKI_S_01,   SE_LV_DAMAGE_MUTEKI_S_ff, 100,  15, false},
    /* SND_LV_DAMAGE_MUTEKI_L*/
    {  SE_LV_DAMAGE_MUTEKI_L_01,   SE_LV_DAMAGE_MUTEKI_L_ff, 100,  15, false},
    /* SND_LV_DAMAGE_MUTEKI_W_S*/
    {SE_LV_DAMAGE_MUTEKI_W_S_01, SE_LV_DAMAGE_MUTEKI_W_S_ff, 100,  15, false},
    /* SND_LV_DAMAGE_MUTEKI_W_L*/
    {SE_LV_DAMAGE_MUTEKI_W_L_01, SE_LV_DAMAGE_MUTEKI_W_L_ff, 100,  15, false},
    /* SND_LV_RECOVER_WATER*/
    {    SE_LV_RECOVER_WATER_01,     SE_LV_RECOVER_WATER_ff, 100, 100,  true},
    /* SND_LV_SPIN_WATER*/
    {       SE_LV_SPIN_WATER_01,        SE_LV_SPIN_WATER_ff, 100,   8, false},
    /* SND_LV_SHOCKED*/
    {          SE_LV_SHOCKED_01,           SE_LV_SHOCKED_ff, 100,  14, false},
    /* SND_LV_LIGHT_SURPRISND*/
    {   SE_LV_LIGHT_SURPRISE_01,    SE_LV_LIGHT_SURPRISE_ff, 100,   0, false},
    /* SND_LV_QUIETLY_S*/
    {        SE_LV_QUIETLY_S_01,         SE_LV_QUIETLY_S_ff,  60,   0, false},
    /* SND_LV_QUIETLY_M*/
    {        SE_LV_QUIETLY_M_01,         SE_LV_QUIETLY_M_ff,  60,   0, false},
    /* SND_LV_DAMAGE_L*/
    {         SE_LV_DAMAGE_L_01,          SE_LV_DAMAGE_L_ff, 100,  17, false},
};

dSndRngLinkVoice_c::dSndRngLinkVoice_c() {}

void dSndRngLinkVoice_c::configure(const dSndLinkVoiceConfig *config, dSndSourcePlayerHead_c *source) {
    dSndRngId_c::configure(config->begin, config->end, config->mGenChance);
    mPriority = config->priority;
    mPriorityTiebreaker = config->priorityTiebreaker;
    mpSource = source;
}

bool dSndRngLinkVoice_c::handleId(u32 id) {
    if (mpSource == nullptr) {
        return false;
    }

    bool ok = mpSource->startVoiceSoundInternal(id);
    if (ok) {
        markPicked(id, true);
    }
    return ok;
}

const dSndLinkVoiceConfig *dSndRngLinkVoice_c::getConfig(s32 idx) {
    return &sLinkVoiceConfig[idx];
}
