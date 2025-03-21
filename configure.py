#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProjectConfig,
    ProgressCategory,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "SOUE01",  # 0
    "SOUP01",  # 1
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v1.3.0"
config.objdiff_tag = "v2.4.0"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION_{config.version}",
]
config.linker_version = "Wii/1.5"
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    # config.ldflags.append("-g")
    config.ldflags.append("-gdwarf-2")  # -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    config.ldflags.append("-listclosure")  # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-enc SJIS",
    "-i include",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
    "-i src",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C++/MSL_Common/Include",
    "-i src/PowerPC_EABI_Support/Runtime/Inc",
    "-i src/PowerPC_EABI_Support/MetroTRK",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
    "-func_align 4",
]

# Framework flags
cflags_framework = [
    *cflags_base,
    "-inline noauto",
    "-str reuse",
    "-fp_contract off",
    "-ipa file",
]

# libms flags
cflags_libms = [
    *cflags_base,
    "-inline noauto",
    "-fp_contract off",
    "-lang=C++",
    "-Cpp_exceptions on",
]

# EGG flags
cflags_egg = [
    *cflags_base,
    "-inline noauto",
    "-fp_contract off",
    "-ipa file",
]

# nw4r flags
cflags_nw4r = [*cflags_base, "-ipa file", "-fp_contract off", ""]

# REL flags
cflags_rel = [
    *cflags_framework,
    "-sdata 0",
    "-sdata2 0",
]


# Helper function for single-object RELs
def Rel(status, rel_name, cpp_name, extra_cflags=[]):
    return {
        "lib": rel_name,
        "mw_version": "Wii/1.5",
        "cflags": cflags_rel + extra_cflags,
        "progress_category": "game",
        "host": False,
        "objects": [
            Object(status, cpp_name),
        ],
    }


def EGGLib(lib_name, objects):
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.5",
        "cflags": cflags_egg,
        "progress_category": "egg",
        "host": False,
        "objects": objects,
    }


def nw4rLib(lib_name, objects, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.3",  # most seem to be around 1.2, snd is 1.6
        "cflags": cflags_nw4r + extra_cflags,
        "progress_category": "nw4r",
        "host": False,
        "objects": objects,
    }


Matching = True
NonMatching = False
Equivalent = (
    config.non_matching
)  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = False
config.warn_missing_source = False
config.libs = [
    {
        "lib": "game_code",
        "mw_version": "Wii/1.5",
        "cflags": cflags_framework,
        "progress_category": "game",
        "host": False,
        "objects": [
            Object(Matching, "d/flag/flag_managers.cpp"),
            Object(NonMatching, "toBeSorted/special_item_drop_mgr.cpp"),
            Object(Matching, "d/d_base.cpp"),
            Object(NonMatching, "d/d_carry.cpp"),
            Object(Matching, "d/d_cc.cpp"),
            Object(Matching, "d/d_dvd.cpp"),
            Object(NonMatching, "d/d_dvd_unk.cpp"),
            Object(NonMatching, "d/d_dylink.cpp"),
            Object(Matching, "d/d_fader.cpp"),
            Object(Matching, "d/d_font_manager.cpp"),
            Object(Matching, "d/d_heap.cpp"),
            Object(Matching, "d/d_rawarchive.cpp"),
            Object(Matching, "d/d_scene.cpp"),
            Object(Matching, "d/d_stage_parse.cpp"),
            Object(NonMatching, "d/d_sys.cpp"),
            Object(NonMatching, "toBeSorted/sound_info.cpp"),
            Object(Matching, "toBeSorted/arc_callback_handler.cpp"),
            Object(NonMatching, "d/a/d_a_base.cpp"),
            Object(NonMatching, "d/a/d_a_item.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_base.cpp"),
            Object(NonMatching, "d/a/npc/d_a_npc.cpp"),
            Object(NonMatching, "d/a/npc/d_a_ordinary_npc.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_bomb.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_arrow.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_boomerang.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_fairy.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_tbox.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_time_area.cpp"),
            Object(Matching, "d/a/obj/d_a_obj_switch.cpp"),
            Object(Matching, "d/a/obj/d_a_obj_water_spout.cpp"),
            Object(Matching, "d/tg/d_t_switch.cpp"),
            Object(Matching, "toBeSorted/arc_managers/current_stage_arc_manager.cpp"),
            Object(Matching, "toBeSorted/arc_managers/oarc_manager.cpp"),
            Object(Matching, "toBeSorted/arc_managers/layout_arc_manager.cpp"),
            Object(NonMatching, "toBeSorted/attention.cpp"),
            Object(Matching, "toBeSorted/dowsing_target.cpp"),
            Object(NonMatching, "toBeSorted/time_area_mgr.cpp"),
            Object(NonMatching, "toBeSorted/nand_request_thread.cpp"),
            Object(Matching, "toBeSorted/save_file.cpp"),
            Object(Matching, "toBeSorted/counters/counter.cpp"),
            Object(Matching, "toBeSorted/counters/rupee_counter.cpp"),
            Object(Matching, "toBeSorted/counters/arrow_counter.cpp"),
            Object(Matching, "toBeSorted/counters/bomb_counter.cpp"),
            Object(Matching, "toBeSorted/counters/tear_counter.cpp"),
            Object(Matching, "toBeSorted/counters/slingshot_seed_counter.cpp"),
            Object(Matching, "toBeSorted/counters/key_piece_counter.cpp"),
            Object(Matching, "toBeSorted/counters/extra_wallet_counter.cpp"),
            Object(NonMatching, "d/lyt/d_structd.cpp"),
            Object(NonMatching, "d/lyt/d2d.cpp"),
            Object(NonMatching, "d/lyt/d_textbox.cpp"),
            Object(Matching, "d/lyt/d_window.cpp"),
            Object(Matching, "d/d_textunk.cpp"),
            Object(NonMatching, "d/d_tag_processor.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_unk.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_a_btn.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_z_btn.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_key.cpp"),
            Object(Matching, "d/lyt/meter/d_lyt_meter_drink.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_remocon_bg.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_nunchaku_bg.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_nun_stk.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_item_select.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_plus_btn.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_minus_btn.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_cross_btn.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_dowsing.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_rupy.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_ganbari_gauge.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_event_skip.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_timer.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_heart.cpp"),
            Object(NonMatching, "d/lyt/meter/d_lyt_meter_shield_gauge.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_do_button.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_common_icon_material.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_common_icon_item.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_select_btn.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_talk.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_link.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_get.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_sword.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_wood.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_stone.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_msg_window_demo.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_simple_window.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_auto_caption.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_auto_explain.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_map_capture.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_map.cpp"),
            Object(Matching, "d/lyt/d_lyt_note.cpp"),
            Object(Matching, "d/lyt/d_lyt_battery.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_area_caption.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_wipe.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_help.cpp"),
            Object(Matching, "d/lyt/d_lyt_bird_gauge.cpp"),
            Object(Matching, "d/lyt/d_lyt_sky_gauge.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_boss_gauge.cpp"),
            Object(NonMatching, "d/lyt/d_lyt_pause_disp_00.cpp"),
            Object(Matching, "d/lyt/d_lyt_fader.cpp"),
            Object(Matching, "d/lyt/d_screen_fader.cpp"),
            Object(NonMatching, "d/a/d_a_insect.cpp"),
            Object(NonMatching, "d/d_stage_mgr.cpp"),
            Object(NonMatching, "d/d_last.cpp"),
            Object(NonMatching, "d/d_stage.cpp"),
            Object(Matching, "d/d_room.cpp"),
            Object(NonMatching, "d/d_cs_base.cpp"),
            Object(NonMatching, "d/d_cs_game.cpp"),
            Object(Matching, "d/d_sc_title.cpp"),
            Object(NonMatching, "d/d_sc_game.cpp"),
            Object(Matching, "d/lyt/d_lyt_common_arrow.cpp"),
            Object(NonMatching, "toBeSorted/file_manager.cpp"),
            Object(NonMatching, "toBeSorted/save_manager.cpp"),
            Object(NonMatching, "toBeSorted/d_d3d.cpp"),
            Object(
                Matching,
                "toBeSorted/d_scn_callback.cpp",
                extra_cflags=["-opt noloop"],
            ),
            Object(NonMatching, "toBeSorted/d_unk_proc.cpp"),
            Object(NonMatching, "toBeSorted/blur_and_palette_manager.cpp"),
            Object(Matching, "DynamicLink.cpp"),
        ],
    },
    {
        "lib": "collision",
        "mw_version": "Wii/1.5",
        "cflags": cflags_framework + ["-str reuse,readonly"],
        "progress_category": "game",
        "host": False,
        "objects": [
            Object(Matching, "d/col/c/c_bg_s_chk.cpp"),
            Object(Matching, "d/col/c/c_bg_s_gnd_chk.cpp"),
            Object(Matching, "d/col/c/c_bg_s_lin_chk.cpp"),
            Object(Matching, "d/col/c/c_bg_s_poly_info.cpp"),
            Object(NonMatching, "d/col/c/c_bg_s_shdw_draw.cpp"),
            Object(Matching, "d/col/c/c_bg_w.cpp"),
            Object(NonMatching, "d/col/c/c_cc_d.cpp"),
            Object(NonMatching, "d/col/c/c_m2d.cpp"),
            Object(NonMatching, "d/col/c/c_m3d.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_aab.cpp"),
            # Object(NonMatching, "d/col/c/c_m3d_g_cir.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_cps.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_cyl.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_lin.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_pla.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_sph.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_tri.cpp"),
            Object(Matching, "d/col/c/c_m3d_g_unk.cpp"),
            Object(NonMatching, "d/col/c/c_partition.cpp"),
            Object(Matching, "d/col/bg/d_bg_pc.cpp"),
            Object(Matching, "d/col/bg/d_bg_plc.cpp"),
            Object(NonMatching, "d/col/bg/d_bg_s.cpp"),
            Object(NonMatching, "d/col/bg/d_bg_s_acch.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_gnd_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_grp_pass_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_lin_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_poly_pass_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_roof_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_sph_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_spl_grp_chk.cpp"),
            Object(Matching, "d/col/bg/d_bg_s_wtr_chk.cpp"),
            Object(NonMatching, "d/col/bg/d_bg_w.cpp"),
            Object(Matching, "d/col/bg/d_bg_w_base.cpp"),
            Object(NonMatching, "d/col/bg/d_bg_w_kcol.cpp"),
            Object(Matching, "d/col/bg/d_bg_w_sv.cpp"),
            Object(NonMatching, "d/col/bg/d_bg_w_time.cpp"),
            Object(Matching, "d/col/cc/d_cc_d.cpp"),
            Object(Matching, "d/col/cc/d_cc_mass_s.cpp"),
            Object(NonMatching, "d/col/cc/d_cc_s.cpp"),
        ],
    },
    {
        "lib": "clib",
        "mw_version": "Wii/1.6",
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(Matching, "c/c_list.cpp"),
            Object(NonMatching, "c/c_math.cpp"),
            Object(NonMatching, "c/c_rand.cpp"),
            Object(Matching, "c/c_tree.cpp"),
        ],
    },
    {
        "lib": "mlib",
        "mw_version": "Wii/1.5",
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(Matching, "m/m3d/m3d.cpp"),
            Object(Matching, "m/m3d/m_proc.cpp"),
            Object(Matching, "m/m3d/m_anmchr.cpp"),
            Object(Matching, "m/m3d/m_anmchrblend.cpp"),
            Object(Matching, "m/m3d/m_anmmdl.cpp"),
            Object(Matching, "m/m3d/m_anmshp.cpp"),
            Object(Matching, "m/m3d/m_anmtexpat.cpp"),
            Object(Matching, "m/m3d/m_anmtexsrt.cpp"),
            Object(Matching, "m/m3d/m_anmmatclr.cpp"),
            Object(Matching, "m/m3d/m_anmvis.cpp"),
            Object(Matching, "m/m3d/m_banm.cpp"),
            Object(NonMatching, "m/m3d/m_bline.cpp"),
            Object(NonMatching, "m/m3d/m_bmdl.cpp"),
            Object(Matching, "m/m3d/m_calc_ratio.cpp"),
            Object(Matching, "m/m3d/m_fanm.cpp"),
            Object(Matching, "m/m3d/m_mdl.cpp"),
            Object(NonMatching, "m/m3d/m_shadow.cpp"),
            Object(Matching, "m/m3d/m_scnleaf.cpp"),
            Object(Matching, "m/m3d/m_smdl.cpp"),
            Object(Matching, "m/m2d.cpp"),
            Object(Matching, "m/m_allocator.cpp"),
            Object(Matching, "m/m_angle.cpp"),
            Object(Matching, "m/m_color_fader.cpp"),
            Object(Matching, "m/m_dvd.cpp"),
            Object(Matching, "m/m_fader.cpp"),
            Object(Matching, "m/m_fader_base.cpp"),
            Object(Matching, "m/m_heap.cpp"),
            Object(NonMatching, "m/m_mtx.cpp"),
            Object(Matching, "m/m_pad.cpp"),
        ],
    },
    {
        "lib": "libms",
        "mw_version": "Wii/1.5",
        "cflags": cflags_libms,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(NonMatching, "libms/commonlib.c"),
            Object(NonMatching, "libms/flowfile.c"),
            Object(NonMatching, "libms/libms.c"),
            Object(NonMatching, "libms/msgfile.c"),
        ],
    },
    {
        "lib": "slib",
        "mw_version": "Wii/1.5",
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(Matching, "s/s_Crc.cpp"),
            Object(NonMatching, "s/s_Math.cpp"),
            Object(Matching, "s/s_FPhase.cpp"),
            Object(Matching, "s/s_StateID.cpp"),
            Object(Matching, "s/s_StateMethod.cpp"),
            Object(Matching, "s/s_StateMethodUsr_FI.cpp"),
            Object(Matching, "s/s_Phase.cpp"),
        ],
    },
    {
        "lib": "flib",
        "mw_version": "Wii/1.6",
        "cflags": cflags_framework,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(NonMatching, "f/f_base.cpp"),
            Object(Matching, "f/f_list.cpp"),
            Object(Matching, "f/f_manager.cpp"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": "Wii/1.6",
        "cflags": cflags_runtime,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(
                Matching, "PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"
            ),
            Object(
                Matching, "PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp"
            ),
        ],
    },
    # NW4R
    nw4rLib(
        "db",
        [
            Object(Matching, "nw4r/db/db_assert.cpp"),
            Object(Matching, "nw4r/db/db_directPrint.cpp"),
            Object(Matching, "nw4r/db/db_console.cpp"),
            Object(Matching, "nw4r/db/db_exception.cpp"),
            Object(Matching, "nw4r/db/db_mapFile.cpp"),
        ],
    ),
    nw4rLib(
        "ut",
        [
            Object(Matching, "nw4r/ut/ut_list.cpp"),
            Object(Matching, "nw4r/ut/ut_LinkList.cpp"),
            Object(Matching, "nw4r/ut/ut_binaryFileFormat.cpp"),
            Object(Matching, "nw4r/ut/ut_CharStrmReader.cpp"),
            Object(Matching, "nw4r/ut/ut_IOStream.cpp"),
            Object(Matching, "nw4r/ut/ut_TagProcessorBase.cpp"),
            Object(Matching, "nw4r/ut/ut_FileStream.cpp"),
            Object(Matching, "nw4r/ut/ut_DvdFileStream.cpp"),
            Object(Matching, "nw4r/ut/ut_DvdLockedFileStream.cpp"),
            Object(Matching, "nw4r/ut/ut_NandFileStream.cpp"),
            Object(Matching, "nw4r/ut/ut_LockedCache.cpp"),
            Object(Matching, "nw4r/ut/ut_Font.cpp"),
            Object(Matching, "nw4r/ut/ut_RomFont.cpp"),
            Object(Matching, "nw4r/ut/ut_ResFontBase.cpp"),
            Object(Matching, "nw4r/ut/ut_ResFont.cpp"),
            Object(Matching, "nw4r/ut/ut_CharWriter.cpp"),
            Object(Matching, "nw4r/ut/ut_TextWriterBase.cpp"),
        ],
    ),
    nw4rLib(
        "math",
        [
            Object(NonMatching, "nw4r/math/math_arithmetic.cpp"),
            Object(NonMatching, "nw4r/math/math_triangular.cpp"),
            Object(NonMatching, "nw4r/math/math_types.cpp"),
            Object(NonMatching, "nw4r/math/math_geometry.cpp"),
        ],
    ),
    nw4rLib(
        "g3d",
        [
            Object(Matching, "nw4r/g3d/res/g3d_rescommon.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resdict.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resfile.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resmdl.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resshp.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_restev.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resmat.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resvtx.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_restex.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resnode.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanm.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmvis.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmclr.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmtexpat.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmtexsrt.cpp"),
            Object(
                Matching,
                "nw4r/g3d/res/g3d_resanmchr.cpp",
                extra_cflags=["-opt level=3"],
            ),
            Object(Matching, "nw4r/g3d/res/g3d_reslightset.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmamblight.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmlight.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmfog.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmcamera.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmscn.cpp"),
            Object(Matching, "nw4r/g3d/res/g3d_resanmshp.cpp"),
            Object(Matching, "nw4r/g3d/g3d_transform.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmvis.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmclr.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmtexpat.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmtexsrt.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmchr.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmshp.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmscn.cpp"),
            Object(Matching, "nw4r/g3d/g3d_obj.cpp"),
            Object(Matching, "nw4r/g3d/g3d_anmobj.cpp"),
            Object(Matching, "nw4r/g3d/platform/g3d_gpu.cpp"),
            Object(Matching, "nw4r/g3d/platform/g3d_tmem.cpp"),
            Object(Matching, "nw4r/g3d/platform/g3d_cpu.cpp"),
            Object(Matching, "nw4r/g3d/g3d_state.cpp"),
            Object(Matching, "nw4r/g3d/g3d_draw1mat1shp.cpp"),
            Object(Matching, "nw4r/g3d/g3d_calcview.cpp"),
            Object(Matching, "nw4r/g3d/g3d_dcc.cpp"),
            Object(Matching, "nw4r/g3d/g3d_workmem.cpp"),
            Object(Matching, "nw4r/g3d/g3d_calcworld.cpp"),
            Object(Matching, "nw4r/g3d/g3d_draw.cpp"),
            Object(Matching, "nw4r/g3d/g3d_camera.cpp"),
            Object(Matching, "nw4r/g3d/dcc/g3d_basic.cpp"),
            Object(Matching, "nw4r/g3d/dcc/g3d_maya.cpp"),
            Object(Matching, "nw4r/g3d/dcc/g3d_xsi.cpp"),
            Object(Matching, "nw4r/g3d/dcc/g3d_3dsmax.cpp"),
            Object(Matching, "nw4r/g3d/g3d_scnobj.cpp"),
            Object(Matching, "nw4r/g3d/g3d_scnroot.cpp"),
            Object(Matching, "nw4r/g3d/g3d_scnmdlsmpl.cpp"),
            Object(Matching, "nw4r/g3d/g3d_scnmdl.cpp"),
            # Object(NonMatching, "nw4r/g3d/g3d_scnchoice.cpp"),
            # Object(NonMatching, "nw4r/g3d/g3d_scngroupex.cpp"),
            # Object(NonMatching, "nw4r/g3d/g3d_scnmdl1mat1shp.cpp"),
            # Object(NonMatching, "nw4r/g3d/g3d_scnmdlexpand.cpp"),
            Object(Matching, "nw4r/g3d/g3d_calcmaterial.cpp"),
            Object(Matching, "nw4r/g3d/g3d_init.cpp"),
            Object(Matching, "nw4r/g3d/g3d_scnproc.cpp"),
            Object(Matching, "nw4r/g3d/g3d_fog.cpp"),
            Object(Matching, "nw4r/g3d/g3d_light.cpp"),
            Object(Matching, "nw4r/g3d/g3d_calcvtx.cpp"),
        ],
    ),
    nw4rLib(
        "snd",
        [
            Object(NonMatching, "nw4r/snd/snd_AnimSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxVoice.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxVoiceManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxfxImpl.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Bank.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BankFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BasicPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BasicSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BiquadFilterPreset.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Channel.cpp"),
            Object(NonMatching, "nw4r/snd/snd_DisposeCallbackManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_debug.cpp"),
            Object(NonMatching, "nw4r/snd/snd_DvdSoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_EnvGenerator.cpp"),
            Object(NonMatching, "nw4r/snd/snd_ExternalSoundPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FrameHeap.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxBase.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxChorus.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxDelay.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxReverbHi.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxReverbHiDpl2.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxReverbStd.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxReverbStdDpl2.cpp"),
            Object(NonMatching, "nw4r/snd/snd_InstancePool.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Lfo.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MemorySoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MidiSeqPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MmlParser.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MmlSeqTrack.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MmlSeqTrackAllocator.cpp"),
            Object(NonMatching, "nw4r/snd/snd_NandSoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_PlayerHeap.cpp"),
            Object(NonMatching, "nw4r/snd/snd_RemoteSpeaker.cpp"),
            Object(NonMatching, "nw4r/snd/snd_RemoteSpeakerManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqSoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqTrack.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Sound3DActor.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Sound3DCalculator.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Sound3DEngine.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Sound3DListener.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Sound3DManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundActor.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchiveFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchiveLoader.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchivePlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundHeap.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundStartable.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundSystem.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundThread.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmChannel.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmSoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Task.cpp"),
            Object(NonMatching, "nw4r/snd/snd_TaskManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_TaskThread.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Voice.cpp"),
            Object(NonMatching, "nw4r/snd/snd_VoiceManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Util.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveSoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WsdFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WsdPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_adpcm.cpp"),
        ],
    ),
    nw4rLib(
        "lyt",
        [
            Object(Matching, "nw4r/lyt/lyt_init.cpp"),
            Object(Matching, "nw4r/lyt/lyt_pane.cpp"),
            Object(Matching, "nw4r/lyt/lyt_group.cpp"),
            Object(Matching, "nw4r/lyt/lyt_layout.cpp"),
            Object(Matching, "nw4r/lyt/lyt_picture.cpp"),
            Object(Matching, "nw4r/lyt/lyt_textBox.cpp"),
            Object(Matching, "nw4r/lyt/lyt_window.cpp"),
            Object(Matching, "nw4r/lyt/lyt_bounding.cpp"),
            Object(Matching, "nw4r/lyt/lyt_material.cpp"),
            Object(Matching, "nw4r/lyt/lyt_texMap.cpp"),
            Object(Matching, "nw4r/lyt/lyt_drawInfo.cpp"),
            Object(Matching, "nw4r/lyt/lyt_animation.cpp"),
            Object(Matching, "nw4r/lyt/lyt_resourceAccessor.cpp"),
            Object(Matching, "nw4r/lyt/lyt_arcResourceAccessor.cpp"),
            Object(Matching, "nw4r/lyt/lyt_common.cpp"),
            Object(Matching, "nw4r/lyt/lyt_util.cpp"),
        ],
    ),
    # EGG
    EGGLib(
        "core",
        [
            Object(Matching, "egg/core/eggArchive.cpp"),
            Object(Matching, "egg/core/eggDvdFile.cpp"),
            Object(Matching, "egg/core/eggDvdRipper.cpp"),
            Object(Matching, "egg/core/eggStreamDecomp.cpp"),
            Object(Matching, "egg/core/eggAllocator.cpp"),
            Object(Matching, "egg/core/eggHeap.cpp"),
            Object(Matching, "egg/core/eggExpHeap.cpp"),
            Object(Matching, "egg/core/eggFrmHeap.cpp"),
            Object(Matching, "egg/core/eggAssertHeap.cpp"),
            Object(Matching, "egg/core/eggDisposer.cpp"),
            Object(Matching, "egg/core/eggThread.cpp"),
            Object(NonMatching, "egg/core/eggUnk.cpp"),
            Object(NonMatching, "egg/core/eggSystem.cpp"),
            Object(Matching, "egg/core/eggDisplay.cpp"),
            Object(Matching, "egg/core/eggColorFader.cpp"),
            Object(Matching, "egg/core/eggAsyncDisplay.cpp"),
            Object(Matching, "egg/core/eggVideo.cpp"),
            Object(Matching, "egg/core/eggXfb.cpp"),
            Object(Matching, "egg/core/eggXfbManager.cpp"),
            Object(Matching, "egg/core/eggGraphicsFifo.cpp"),
            Object(Matching, "egg/core/eggController.cpp"),
        ],
    ),
    EGGLib(
        "math",
        [
            Object(NonMatching, "egg/math/eggMath.cpp"),
            Object(NonMatching, "egg/math/eggMatrix.cpp"),
            Object(NonMatching, "egg/math/eggQuat.cpp"),
            Object(Matching, "egg/math/eggVector.cpp"),
        ],
    ),
    EGGLib(
        "prim",
        [
            Object(Matching, "egg/prim/eggAssert.cpp"),
        ],
    ),
    EGGLib(
        "gfx",
        [
            Object(NonMatching, "egg/gfx/eggCamera.cpp"),
            Object(NonMatching, "egg/gfx/eggPalette.cpp"),
            Object(NonMatching, "egg/gfx/eggTexture.cpp"),
            Object(NonMatching, "egg/gfx/eggUnk1.cpp"),  # Unknown
            Object(Matching, "egg/gfx/eggCapTexture.cpp"),
            Object(NonMatching, "egg/gfx/eggCpuTexture.cpp"),
            Object(NonMatching, "egg/gfx/eggDrawGX.cpp"),
            Object(Matching, "egg/gfx/eggDrawPathBase.cpp"),
            Object(NonMatching, "egg/gfx/eggDrawPathUnk1.cpp"),
            Object(NonMatching, "egg/gfx/eggDrawPathDOF.cpp"),
            Object(NonMatching, "egg/gfx/eggDrawPathUnk2.cpp"),
            Object(NonMatching, "egg/gfx/eggFog.cpp"),
            Object(NonMatching, "egg/gfx/eggFogManager.cpp"),
            Object(NonMatching, "egg/gfx/eggFrustum.cpp"),
            Object(NonMatching, "egg/gfx/eggG3DUtility.cpp"),  # Unknown Guess
            Object(NonMatching, "egg/gfx/eggGfxEngine.cpp"),
            Object(Matching, "egg/gfx/eggGlobalDrawState.cpp"),
            Object(NonMatching, "egg/gfx/eggGXUtility.cpp"),
            Object(Matching, "egg/gfx/eggIScnProc.cpp"),
            Object(NonMatching, "egg/gfx/eggLightManager.cpp"),
            Object(NonMatching, "egg/gfx/eggLightObject.cpp"),
            Object(NonMatching, "egg/gfx/eggLightTexture.cpp"),
            Object(NonMatching, "egg/gfx/eggLightTextureMgr.cpp"),
            Object(NonMatching, "egg/gfx/eggModelEx.cpp"),
            Object(NonMatching, "egg/gfx/eggPostEffectBase.cpp"),
            Object(NonMatching, "egg/gfx/eggPostEffectBlur.cpp"),
            Object(NonMatching, "egg/gfx/eggPostEffectUnk1.cpp"),  # Unknown
            Object(NonMatching, "egg/gfx/eggPostEffectUnk2.cpp"),  # Unknown
            Object(Matching, "egg/gfx/eggPostEffectMask.cpp"),
            Object(NonMatching, "egg/gfx/eggPostEffectMaskDOF.cpp"),
            Object(Matching, "egg/gfx/eggPostEffectSimple.cpp"),
            Object(Matching, "egg/gfx/eggScreen.cpp"),
            Object(Matching, "egg/gfx/eggScreenEffectBase.cpp"),
            Object(NonMatching, "egg/gfx/eggScreenEffectBlur.cpp"),  # Unknown Guess
            Object(NonMatching, "egg/gfx/eggStateEfb.cpp"),
            Object(NonMatching, "egg/gfx/eggStateGX.cpp"),
            Object(Matching, "egg/gfx/eggTextureBuffer.cpp"),
        ],
    ),
    EGGLib(
        "audio",
        [
            Object(Matching, "egg/audio/eggAudioArcPlayerMgr.cpp"),
            Object(Matching, "egg/audio/eggAudioExpMgr.cpp"),
            Object(Matching, "egg/audio/eggAudioHeapMgr.cpp"),
            Object(Matching, "egg/audio/eggAudioMgr.cpp"),
            Object(Matching, "egg/audio/eggAudioRmtSpeakerMgr.cpp"),
            Object(NonMatching, "egg/audio/eggAudioUtility.cpp"),
            Object(Matching, "egg/audio/eggAudioSystem.cpp"),
        ],
    ),
    EGGLib(
        "util",
        [
            Object(Matching, "egg/util/eggException.cpp"),
        ],
    ),
    # {
    #     "lib": "MSL_C",
    #     "mw_version": "GC/1.3.2",
    #     "cflags": cflags_runtime,
    #     "host": False,
    #     "objects": [
    #     ],
    # },
    # {
    #     "lib": "TRK_MINNOW_DOLPHIN",
    #     "mw_version": "GC/1.3.2",
    #     "cflags": cflags_runtime,
    #     "host": False,
    #     "objects": [
    #     ],
    # },
    # Begin RELs
    {
        "lib": "REL",
        "mw_version": "Wii/1.6",
        "cflags": cflags_rel,
        "progress_category": "core",
        "host": False,
        "objects": [
            Object(Matching, "REL/executor.c"),
            Object(
                Matching,
                "REL/__init_cpp_exceptions.cpp",
                source="PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp",
            ),
            Object(
                Matching,
                "REL/global_destructor_chain.c",
                source="PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c",
            ),
        ],
    },
    Rel(NonMatching, "d_a_asura_bullet", "REL/d/a/d_a_asura_bullet.cpp"),
    Rel(NonMatching, "d_a_avater_bullet", "REL/d/a/d_a_avater_bullet.cpp"),
    Rel(NonMatching, "d_a_avater_race_mng", "REL/d/a/d_a_avater_race_mng.cpp"),
    Rel(NonMatching, "d_a_bird", "REL/d/a/d_a_bird.cpp"),
    Rel(NonMatching, "d_a_bird_kobunA", "REL/d/a/d_a_bird_kobunA.cpp"),
    Rel(NonMatching, "d_a_bird_kobunB", "REL/d/a/d_a_bird_kobunB.cpp"),
    Rel(NonMatching, "d_a_bird_mob", "REL/d/a/d_a_bird_mob.cpp"),
    Rel(NonMatching, "d_a_bird_npc", "REL/d/a/d_a_bird_npc.cpp"),
    Rel(NonMatching, "d_a_bird_rival", "REL/d/a/d_a_bird_rival.cpp"),
    Rel(NonMatching, "d_a_bird_target", "REL/d/a/d_a_bird_target.cpp"),
    Rel(NonMatching, "d_a_bird_zelda_training", "REL/d/a/d_a_bird_zelda_training.cpp"),
    Rel(NonMatching, "d_a_bombf", "REL/d/a/d_a_bombf.cpp"),
    Rel(NonMatching, "d_a_bullet_gue", "REL/d/a/d_a_bullet_gue.cpp"),
    Rel(NonMatching, "d_a_bullet_maguppo", "REL/d/a/d_a_bullet_maguppo.cpp"),
    Rel(NonMatching, "d_a_b_asura", "REL/d/a/b/d_a_b_asura.cpp"),
    Rel(NonMatching, "d_a_b_asura_arm", "REL/d/a/b/d_a_b_asura_arm.cpp"),
    Rel(NonMatching, "d_a_b_asura_foot", "REL/d/a/b/d_a_b_asura_foot.cpp"),
    Rel(NonMatching, "d_a_b_bbshockwave", "REL/d/a/b/d_a_b_bbshockwave.cpp"),
    Rel(NonMatching, "d_a_b_bigboss2", "REL/d/a/b/d_a_b_bigboss2.cpp"),
    Rel(NonMatching, "d_a_b_bigboss3", "REL/d/a/b/d_a_b_bigboss3.cpp"),
    Rel(NonMatching, "d_a_b_bigbossbase", "REL/d/a/b/d_a_b_bigbossbase.cpp"),
    Rel(NonMatching, "d_a_b_bigboss", "REL/d/a/b/d_a_b_bigboss.cpp"),
    Rel(NonMatching, "d_a_b_girahimu2", "REL/d/a/b/d_a_b_girahimu2.cpp"),
    Rel(NonMatching, "d_a_b_girahimu3_base", "REL/d/a/b/d_a_b_girahimu3_base.cpp"),
    Rel(NonMatching, "d_a_b_girahimu3_first", "REL/d/a/b/d_a_b_girahimu3_first.cpp"),
    Rel(NonMatching, "d_a_b_girahimu3_second", "REL/d/a/b/d_a_b_girahimu3_second.cpp"),
    Rel(NonMatching, "d_a_b_girahimu3_third", "REL/d/a/b/d_a_b_girahimu3_third.cpp"),
    Rel(NonMatching, "d_a_b_girahimu", "REL/d/a/b/d_a_b_girahimu.cpp"),
    Rel(NonMatching, "d_a_b_girahimu_base", "REL/d/a/b/d_a_b_girahimu_base.cpp"),
    Rel(NonMatching, "d_a_b_kr", "REL/d/a/b/d_a_b_kr.cpp"),
    Rel(NonMatching, "d_a_b_kr_arm", "REL/d/a/b/d_a_b_kr_arm.cpp"),
    Rel(NonMatching, "d_a_b_kr_hair", "REL/d/a/b/d_a_b_kr_hair.cpp"),
    Rel(NonMatching, "d_a_b_lastboss", "REL/d/a/b/d_a_b_lastboss.cpp"),
    Rel(NonMatching, "d_a_b_mg", "REL/d/a/b/d_a_b_mg.cpp"),
    Rel(NonMatching, "d_a_b_nusi", "REL/d/a/b/d_a_b_nusi.cpp"),
    Rel(NonMatching, "d_a_b_nusi_bullet", "REL/d/a/b/d_a_b_nusi_bullet.cpp"),
    Rel(NonMatching, "d_a_b_nusi_tentakle", "REL/d/a/b/d_a_b_nusi_tentakle.cpp"),
    Rel(NonMatching, "d_a_b_vd", "REL/d/a/b/d_a_b_vd.cpp"),
    Rel(NonMatching, "d_a_col_stp", "REL/d/a/d_a_col_stp.cpp"),
    Rel(NonMatching, "d_a_control", "REL/d/a/d_a_control.cpp"),
    Rel(NonMatching, "d_a_co_test", "REL/d/a/d_a_co_test.cpp"),
    Rel(NonMatching, "d_a_demo_npc_bird", "REL/d/a/d_a_demo_npc_bird.cpp"),
    Rel(NonMatching, "d_a_eel", "REL/d/a/d_a_eel.cpp"),
    Rel(NonMatching, "d_a_e_am", "REL/d/a/e/d_a_e_am.cpp"),
    Rel(NonMatching, "d_a_e_battleshipfish", "REL/d/a/e/d_a_e_battleshipfish.cpp"),
    Rel(NonMatching, "d_a_e_bcal", "REL/d/a/e/d_a_e_bcal.cpp"),
    Rel(NonMatching, "d_a_e_bcal_arrow", "REL/d/a/e/d_a_e_bcal_arrow.cpp"),
    Rel(NonMatching, "d_a_e_bce", "REL/d/a/e/d_a_e_bce.cpp"),
    Rel(NonMatching, "d_a_e_bc", "REL/d/a/e/d_a_e_bc.cpp"),
    Rel(NonMatching, "d_a_e_bcz", "REL/d/a/e/d_a_e_bcz.cpp"),
    Rel(NonMatching, "d_a_e_bc_arrow", "REL/d/a/e/d_a_e_bc_arrow.cpp"),
    Rel(NonMatching, "d_a_e_beamos", "REL/d/a/e/d_a_e_beamos.cpp"),
    Rel(NonMatching, "d_a_e_bs", "REL/d/a/e/d_a_e_bs.cpp"),
    Rel(NonMatching, "d_a_e_cactus", "REL/d/a/e/d_a_e_cactus.cpp"),
    Rel(NonMatching, "d_a_e_captain", "REL/d/a/e/d_a_e_captain.cpp"),
    Rel(NonMatching, "d_a_e_chaser_b", "REL/d/a/e/d_a_e_chaser_b.cpp"),
    Rel(NonMatching, "d_a_e_eye", "REL/d/a/e/d_a_e_eye.cpp"),
    Rel(NonMatching, "d_a_e_geko", "REL/d/a/e/d_a_e_geko.cpp"),
    Rel(NonMatching, "d_a_e_ge", "REL/d/a/e/d_a_e_ge.cpp"),
    Rel(NonMatching, "d_a_e_gerock", "REL/d/a/e/d_a_e_gerock.cpp"),
    Rel(NonMatching, "d_a_e_gue", "REL/d/a/e/d_a_e_gue.cpp"),
    Rel(NonMatching, "d_a_e_gumarm", "REL/d/a/e/d_a_e_gumarm.cpp"),
    Rel(NonMatching, "d_a_e_gunhob", "REL/d/a/e/d_a_e_gunhob.cpp"),
    Rel(NonMatching, "d_a_e_gunho", "REL/d/a/e/d_a_e_gunho.cpp"),
    Rel(NonMatching, "d_a_e_hb", "REL/d/a/e/d_a_e_hb.cpp"),
    Rel(Matching, "d_a_e_hb_leaf", "REL/d/a/e/d_a_e_hb_leaf.cpp"),
    Rel(NonMatching, "d_a_e_hidokari", "REL/d/a/e/d_a_e_hidokari.cpp"),
    Rel(NonMatching, "d_a_e_hidokaris", "REL/d/a/e/d_a_e_hidokaris.cpp"),
    Rel(NonMatching, "d_a_e_hidory", "REL/d/a/e/d_a_e_hidory.cpp"),
    Rel(NonMatching, "d_a_e_hoc", "REL/d/a/e/d_a_e_hoc.cpp"),
    Rel(NonMatching, "d_a_e_hp", "REL/d/a/e/d_a_e_hp.cpp"),
    Rel(NonMatching, "d_a_e_hydra", "REL/d/a/e/d_a_e_hydra.cpp"),
    Rel(NonMatching, "d_a_e_kg", "REL/d/a/e/d_a_e_kg.cpp"),
    Rel(NonMatching, "d_a_e_ks", "REL/d/a/e/d_a_e_ks.cpp"),
    Rel(NonMatching, "d_a_e_kumite_gira", "REL/d/a/e/d_a_e_kumite_gira.cpp"),
    Rel(NonMatching, "d_a_e_lizarufos", "REL/d/a/e/d_a_e_lizarufos.cpp"),
    Rel(NonMatching, "d_a_e_liza_tail", "REL/d/a/e/d_a_e_liza_tail.cpp"),
    Rel(NonMatching, "d_a_e_magma", "REL/d/a/e/d_a_e_magma.cpp"),
    Rel(NonMatching, "d_a_e_maguppo", "REL/d/a/e/d_a_e_maguppo.cpp"),
    Rel(NonMatching, "d_a_e_mr", "REL/d/a/e/d_a_e_mr.cpp"),
    Rel(NonMatching, "d_a_e_mr_shield", "REL/d/a/e/d_a_e_mr_shield.cpp"),
    Rel(NonMatching, "d_a_e_oc", "REL/d/a/e/d_a_e_oc.cpp"),
    Rel(NonMatching, "d_a_e_or", "REL/d/a/e/d_a_e_or.cpp"),
    Rel(NonMatching, "d_a_e_or_cannon", "REL/d/a/e/d_a_e_or_cannon.cpp"),
    Rel(NonMatching, "d_a_e_ph", "REL/d/a/e/d_a_e_ph.cpp"),
    Rel(NonMatching, "d_a_e_po", "REL/d/a/e/d_a_e_po.cpp"),
    Rel(NonMatching, "d_a_e_remly", "REL/d/a/e/d_a_e_remly.cpp"),
    Rel(NonMatching, "d_a_e_rupee_gue", "REL/d/a/e/d_a_e_rupee_gue.cpp"),
    Rel(NonMatching, "d_a_e_sf4", "REL/d/a/e/d_a_e_sf4.cpp"),
    Rel(NonMatching, "d_a_e_sf", "REL/d/a/e/d_a_e_sf.cpp"),
    Rel(NonMatching, "d_a_e_siren", "REL/d/a/e/d_a_e_siren.cpp"),
    Rel(NonMatching, "d_a_e_skytail", "REL/d/a/e/d_a_e_skytail.cpp"),
    Rel(NonMatching, "d_a_e_sm", "REL/d/a/e/d_a_e_sm.cpp"),
    Rel(NonMatching, "d_a_e_spark", "REL/d/a/e/d_a_e_spark.cpp"),
    Rel(NonMatching, "d_a_e_st", "REL/d/a/e/d_a_e_st.cpp"),
    Rel(NonMatching, "d_a_e_st_wire", "REL/d/a/e/d_a_e_st_wire.cpp"),
    Rel(NonMatching, "d_a_e_syakomaito", "REL/d/a/e/d_a_e_syakomaito.cpp"),
    Rel(NonMatching, "d_a_e_tn2", "REL/d/a/e/d_a_e_tn2.cpp"),
    Rel(NonMatching, "d_a_e_ws", "REL/d/a/e/d_a_e_ws.cpp"),
    Rel(NonMatching, "d_a_fish", "REL/d/a/d_a_fish.cpp"),
    Rel(NonMatching, "d_a_fish_mgr", "REL/d/a/d_a_fish_mgr.cpp"),
    Rel(NonMatching, "d_a_fly_slb", "REL/d/a/d_a_fly_slb.cpp"),
    Rel(NonMatching, "d_a_girahimu_sword_beam", "REL/d/a/d_a_girahimu_sword_beam.cpp"),
    Rel(NonMatching, "d_a_heartf", "REL/d/a/d_a_heartf.cpp"),
    Rel(NonMatching, "d_a_hidory_fire", "REL/d/a/d_a_hidory_fire.cpp"),
    Rel(NonMatching, "d_a_insect_ant", "REL/d/a/d_a_insect_ant.cpp"),
    Rel(NonMatching, "d_a_insect_beetle", "REL/d/a/d_a_insect_beetle.cpp"),
    Rel(NonMatching, "d_a_insect_butterfly", "REL/d/a/d_a_insect_butterfly.cpp"),
    Rel(NonMatching, "d_a_insect_cicada", "REL/d/a/d_a_insect_cicada.cpp"),
    Rel(NonMatching, "d_a_insect_dragonfly", "REL/d/a/d_a_insect_dragonfly.cpp"),
    Rel(NonMatching, "d_a_insect_firefly", "REL/d/a/d_a_insect_firefly.cpp"),
    Rel(NonMatching, "d_a_insect_grasshopper", "REL/d/a/d_a_insect_grasshopper.cpp"),
    Rel(NonMatching, "d_a_insect_ladybug", "REL/d/a/d_a_insect_ladybug.cpp"),
    Rel(NonMatching, "d_a_insect_scarab", "REL/d/a/d_a_insect_scarab.cpp"),
    Rel(NonMatching, "d_a_invisible", "REL/d/a/d_a_invisible.cpp"),
    Rel(NonMatching, "d_a_kanban", "REL/d/a/d_a_kanban.cpp"),
    Rel(NonMatching, "d_a_knight_leader_bird", "REL/d/a/d_a_knight_leader_bird.cpp"),
    Rel(NonMatching, "d_a_little_bird", "REL/d/a/d_a_little_bird.cpp"),
    Rel(NonMatching, "d_a_little_bird_mgr", "REL/d/a/d_a_little_bird_mgr.cpp"),
    Rel(NonMatching, "d_a_message", "REL/d/a/d_a_message.cpp"),
    Rel(NonMatching, "d_a_morita_test", "REL/d/a/d_a_morita_test.cpp"),
    Rel(NonMatching, "d_a_npc_akumakun", "REL/d/a/npc/d_a_npc_akumakun.cpp"),
    Rel(NonMatching, "d_a_npc_aku_human", "REL/d/a/npc/d_a_npc_aku_human.cpp"),
    Rel(NonMatching, "d_a_npc_aqua_dragon", "REL/d/a/npc/d_a_npc_aqua_dragon.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_azukariya_father",
        "REL/d/a/npc/d_a_npc_azukariya_father.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_azukariya_night",
        "REL/d/a/npc/d_a_npc_azukariya_night.cpp",
    ),
    Rel(NonMatching, "d_a_npc_bamboo_goron", "REL/d/a/npc/d_a_npc_bamboo_goron.cpp"),
    Rel(NonMatching, "d_a_npc_bbrvl", "REL/d/a/npc/d_a_npc_bbrvl.cpp"),
    Rel(NonMatching, "d_a_npc_bee", "REL/d/a/npc/d_a_npc_bee.cpp"),
    Rel(NonMatching, "d_a_npc_bird", "REL/d/a/npc/d_a_npc_bird.cpp"),
    Rel(NonMatching, "d_a_npc_bird_sowoman", "REL/d/a/npc/d_a_npc_bird_sowoman.cpp"),
    Rel(NonMatching, "d_a_npc_bird_zelda", "REL/d/a/npc/d_a_npc_bird_zelda.cpp"),
    Rel(NonMatching, "d_a_npc_ce_friend", "REL/d/a/npc/d_a_npc_ce_friend.cpp"),
    Rel(NonMatching, "d_a_npc_ce_lady", "REL/d/a/npc/d_a_npc_ce_lady.cpp"),
    Rel(NonMatching, "d_a_npc_chef", "REL/d/a/npc/d_a_npc_chef.cpp"),
    Rel(NonMatching, "d_a_npc_daishinkan", "REL/d/a/npc/d_a_npc_daishinkan.cpp"),
    Rel(NonMatching, "d_a_npc_daishinkan_n", "REL/d/a/npc/d_a_npc_daishinkan_n.cpp"),
    Rel(NonMatching, "d_a_npc_desertrobot", "REL/d/a/npc/d_a_npc_desertrobot.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_desertrobot_captain",
        "REL/d/a/npc/d_a_npc_desertrobot_captain.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_dive_game_judge",
        "REL/d/a/npc/d_a_npc_dive_game_judge.cpp",
    ),
    Rel(NonMatching, "d_a_npc_douguyanight", "REL/d/a/npc/d_a_npc_douguyanight.cpp"),
    Rel(
        NonMatching, "d_a_npc_douguya_mother", "REL/d/a/npc/d_a_npc_douguya_mother.cpp"
    ),
    Rel(
        NonMatching,
        "d_a_npc_douguya_mother_lod",
        "REL/d/a/npc/d_a_npc_douguya_mother_lod.cpp",
    ),
    Rel(NonMatching, "d_a_npc_fire_dragon", "REL/d/a/npc/d_a_npc_fire_dragon.cpp"),
    Rel(NonMatching, "d_a_npc_girahim", "REL/d/a/npc/d_a_npc_girahim.cpp"),
    Rel(NonMatching, "d_a_npc_goddess_zelda", "REL/d/a/npc/d_a_npc_goddess_zelda.cpp"),
    Rel(NonMatching, "d_a_npc_gra", "REL/d/a/npc/d_a_npc_gra.cpp"),
    Rel(NonMatching, "d_a_npc_grc", "REL/d/a/npc/d_a_npc_grc.cpp"),
    Rel(NonMatching, "d_a_npc_grd", "REL/d/a/npc/d_a_npc_grd.cpp"),
    Rel(NonMatching, "d_a_npc_honeycomb", "REL/d/a/npc/d_a_npc_honeycomb.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_insect_capture_game_kobun",
        "REL/d/a/npc/d_a_npc_insect_capture_game_kobun.cpp",
    ),
    Rel(NonMatching, "d_a_npc_junk_mother", "REL/d/a/npc/d_a_npc_junk_mother.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_junk_mother_lod",
        "REL/d/a/npc/d_a_npc_junk_mother_lod.cpp",
    ),
    Rel(NonMatching, "d_a_npc_junk_night", "REL/d/a/npc/d_a_npc_junk_night.cpp"),
    Rel(NonMatching, "d_a_npc_kensei", "REL/d/a/npc/d_a_npc_kensei.cpp"),
    Rel(NonMatching, "d_a_npc_knight_leader", "REL/d/a/npc/d_a_npc_knight_leader.cpp"),
    Rel(NonMatching, "d_a_npc_kobun2", "REL/d/a/npc/d_a_npc_kobun2.cpp"),
    Rel(NonMatching, "d_a_npc_kobun", "REL/d/a/npc/d_a_npc_kobun.cpp"),
    Rel(NonMatching, "d_a_npc_kobun_b_night", "REL/d/a/npc/d_a_npc_kobun_b_night.cpp"),
    Rel(NonMatching, "d_a_npc_kyui4", "REL/d/a/npc/d_a_npc_kyui4.cpp"),
    Rel(NonMatching, "d_a_npc_kyui_elder", "REL/d/a/npc/d_a_npc_kyui_elder.cpp"),
    Rel(NonMatching, "d_a_npc_kyui_first", "REL/d/a/npc/d_a_npc_kyui_first.cpp"),
    Rel(NonMatching, "d_a_npc_kyui_third", "REL/d/a/npc/d_a_npc_kyui_third.cpp"),
    Rel(NonMatching, "d_a_npc_kyui_wizard", "REL/d/a/npc/d_a_npc_kyui_wizard.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_medicinewifenight",
        "REL/d/a/npc/d_a_npc_medicinewifenight.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_medicine_husband_night",
        "REL/d/a/npc/d_a_npc_medicine_husband_night.cpp",
    ),
    Rel(NonMatching, "d_a_npc_mole", "REL/d/a/npc/d_a_npc_mole.cpp"),
    Rel(NonMatching, "d_a_npc_mole_elder", "REL/d/a/npc/d_a_npc_mole_elder.cpp"),
    Rel(NonMatching, "d_a_npc_mole_es_nml", "REL/d/a/npc/d_a_npc_mole_es_nml.cpp"),
    Rel(NonMatching, "d_a_npc_mole_mg", "REL/d/a/npc/d_a_npc_mole_mg.cpp"),
    Rel(NonMatching, "d_a_npc_mole_normal2", "REL/d/a/npc/d_a_npc_mole_normal2.cpp"),
    Rel(NonMatching, "d_a_npc_mole_normal", "REL/d/a/npc/d_a_npc_mole_normal.cpp"),
    Rel(NonMatching, "d_a_npc_mole_sal", "REL/d/a/npc/d_a_npc_mole_sal.cpp"),
    Rel(NonMatching, "d_a_npc_mole_tackle2", "REL/d/a/npc/d_a_npc_mole_tackle2.cpp"),
    Rel(NonMatching, "d_a_npc_mole_tackle", "REL/d/a/npc/d_a_npc_mole_tackle.cpp"),
    Rel(NonMatching, "d_a_npc_old_impa", "REL/d/a/npc/d_a_npc_old_impa.cpp"),
    Rel(NonMatching, "d_a_npc_ord_kyui", "REL/d/a/npc/d_a_npc_ord_kyui.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_pumpkin_clay_shooting",
        "REL/d/a/npc/d_a_npc_pumpkin_clay_shooting.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_pumpkin_daughter",
        "REL/d/a/npc/d_a_npc_pumpkin_daughter.cpp",
    ),
    Rel(
        NonMatching, "d_a_npc_pumpkin_master", "REL/d/a/npc/d_a_npc_pumpkin_master.cpp"
    ),
    Rel(NonMatching, "d_a_npc_regret_rival", "REL/d/a/npc/d_a_npc_regret_rival.cpp"),
    Rel(NonMatching, "d_a_npc_rescue", "REL/d/a/npc/d_a_npc_rescue.cpp"),
    Rel(NonMatching, "d_a_npc_rival", "REL/d/a/npc/d_a_npc_rival.cpp"),
    Rel(Matching, "d_a_npc_rival_lod", "REL/d/a/npc/d_a_npc_rival_lod.cpp"),
    Rel(NonMatching, "d_a_npc_salbage_morry", "REL/d/a/npc/d_a_npc_salbage_morry.cpp"),
    Rel(NonMatching, "d_a_npc_salbage_robot", "REL/d/a/npc/d_a_npc_salbage_robot.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_salbage_robot_fly",
        "REL/d/a/npc/d_a_npc_salbage_robot_fly.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_salbage_robot_fly_base",
        "REL/d/a/npc/d_a_npc_salbage_robot_fly_base.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_salbage_robot_repair",
        "REL/d/a/npc/d_a_npc_salbage_robot_repair.cpp",
    ),
    Rel(NonMatching, "d_a_npc_salesman_s", "REL/d/a/npc/d_a_npc_salesman_s.cpp"),
    Rel(Matching, "d_a_npc_saltalk", "REL/d/a/npc/d_a_npc_saltalk.cpp"),
    Rel(
        NonMatching, "d_a_npc_senpaia_mother", "REL/d/a/npc/d_a_npc_senpaia_mother.cpp"
    ),
    Rel(
        NonMatching,
        "d_a_npc_senpaia_mother_lod",
        "REL/d/a/npc/d_a_npc_senpaia_mother_lod.cpp",
    ),
    Rel(NonMatching, "d_a_npc_senpai", "REL/d/a/npc/d_a_npc_senpai.cpp"),
    Rel(NonMatching, "d_a_npc_senpai_b", "REL/d/a/npc/d_a_npc_senpai_b.cpp"),
    Rel(NonMatching, "d_a_npc_shinkan2", "REL/d/a/npc/d_a_npc_shinkan2.cpp"),
    Rel(NonMatching, "d_a_npc_shinkan", "REL/d/a/npc/d_a_npc_shinkan.cpp"),
    Rel(NonMatching, "d_a_npc_sorajima_boy", "REL/d/a/npc/d_a_npc_sorajima_boy.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_sorajima_father",
        "REL/d/a/npc/d_a_npc_sorajima_father.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_sorajima_female",
        "REL/d/a/npc/d_a_npc_sorajima_female.cpp",
    ),
    Rel(NonMatching, "d_a_npc_sorajima_girl", "REL/d/a/npc/d_a_npc_sorajima_girl.cpp"),
    Rel(
        NonMatching, "d_a_npc_sorajima_male2", "REL/d/a/npc/d_a_npc_sorajima_male2.cpp"
    ),
    Rel(
        NonMatching, "d_a_npc_sorajima_male3", "REL/d/a/npc/d_a_npc_sorajima_male3.cpp"
    ),
    Rel(NonMatching, "d_a_npc_sorajima_male", "REL/d/a/npc/d_a_npc_sorajima_male.cpp"),
    Rel(
        NonMatching, "d_a_npc_sorajima_man_d", "REL/d/a/npc/d_a_npc_sorajima_man_d.cpp"
    ),
    Rel(
        NonMatching, "d_a_npc_sorajima_man_e", "REL/d/a/npc/d_a_npc_sorajima_man_e.cpp"
    ),
    Rel(
        NonMatching,
        "d_a_npc_sorajima_mother",
        "REL/d/a/npc/d_a_npc_sorajima_mother.cpp",
    ),
    Rel(NonMatching, "d_a_npc_suisei", "REL/d/a/npc/d_a_npc_suisei.cpp"),
    Rel(NonMatching, "d_a_npc_suisei_normal", "REL/d/a/npc/d_a_npc_suisei_normal.cpp"),
    Rel(NonMatching, "d_a_npc_suisei_sub", "REL/d/a/npc/d_a_npc_suisei_sub.cpp"),
    Rel(NonMatching, "d_a_npc_terry", "REL/d/a/npc/d_a_npc_terry.cpp"),
    Rel(NonMatching, "d_a_npc_testman", "REL/d/a/npc/d_a_npc_testman.cpp"),
    Rel(
        NonMatching, "d_a_npc_thunder_dragon", "REL/d/a/npc/d_a_npc_thunder_dragon.cpp"
    ),
    Rel(
        NonMatching,
        "d_a_npc_thunder_dragon_bone",
        "REL/d/a/npc/d_a_npc_thunder_dragon_bone.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_npc_thunder_dragon_skull",
        "REL/d/a/npc/d_a_npc_thunder_dragon_skull.cpp",
    ),
    Rel(NonMatching, "d_a_npc_toilet_ghost", "REL/d/a/npc/d_a_npc_toilet_ghost.cpp"),
    Rel(NonMatching, "d_a_npc_uranaiya", "REL/d/a/npc/d_a_npc_uranaiya.cpp"),
    Rel(
        NonMatching,
        "d_a_npc_volcano_f2_salbo",
        "REL/d/a/npc/d_a_npc_volcano_f2_salbo.cpp",
    ),
    Rel(NonMatching, "d_a_npc_young_impa", "REL/d/a/npc/d_a_npc_young_impa.cpp"),
    Rel(NonMatching, "d_a_npc_zelda", "REL/d/a/npc/d_a_npc_zelda.cpp"),
    Rel(NonMatching, "d_a_nusi_base", "REL/d/a/d_a_nusi_base.cpp"),
    Rel(NonMatching, "d_a_nusi_npc", "REL/d/a/d_a_nusi_npc.cpp"),
    Rel(NonMatching, "d_a_obj_amber", "REL/d/a/obj/d_a_obj_amber.cpp"),
    Rel(
        NonMatching, "d_a_obj_ancient_jewels", "REL/d/a/obj/d_a_obj_ancient_jewels.cpp"
    ),
    # matching except for IScnObjCallback weak function order issue
    Rel(NonMatching, "d_a_obj_appear_bridge", "REL/d/a/obj/d_a_obj_appear_bridge.cpp"),
    Rel(NonMatching, "d_a_obj_arrow_switch", "REL/d/a/obj/d_a_obj_arrow_switch.cpp"),
    Rel(NonMatching, "d_a_obj_asura_pillar", "REL/d/a/obj/d_a_obj_asura_pillar.cpp"),
    Rel(NonMatching, "d_a_obj_asura_sword", "REL/d/a/obj/d_a_obj_asura_sword.cpp"),
    Rel(NonMatching, "d_a_obj_B300_sand", "REL/d/a/obj/d_a_obj_B300_sand.cpp"),
    Rel(NonMatching, "d_a_obj_ballista", "REL/d/a/obj/d_a_obj_ballista.cpp"),
    Rel(NonMatching, "d_a_obj_ballista_f3", "REL/d/a/obj/d_a_obj_ballista_f3.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_ballista_handle",
        "REL/d/a/obj/d_a_obj_ballista_handle.cpp",
    ),
    Rel(NonMatching, "d_a_obj_bamboo", "REL/d/a/obj/d_a_obj_bamboo.cpp"),
    Rel(Matching, "d_a_obj_bamboo_island", "REL/d/a/obj/d_a_obj_bamboo_island.cpp"),
    Rel(NonMatching, "d_a_obj_barrel", "REL/d/a/obj/d_a_obj_barrel.cpp"),
    Rel(NonMatching, "d_a_obj_bblargebomb", "REL/d/a/obj/d_a_obj_bblargebomb.cpp"),
    Rel(NonMatching, "d_a_obj_bbstone", "REL/d/a/obj/d_a_obj_bbstone.cpp"),
    Rel(NonMatching, "d_a_obj_bb_bridge", "REL/d/a/obj/d_a_obj_bb_bridge.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_bb_broken_parts",
        "REL/d/a/obj/d_a_obj_bb_broken_parts.cpp",
    ),
    Rel(NonMatching, "d_a_obj_bb_objects", "REL/d/a/obj/d_a_obj_bb_objects.cpp"),
    Rel(NonMatching, "d_a_obj_bell", "REL/d/a/obj/d_a_obj_bell.cpp"),
    Rel(NonMatching, "d_a_obj_belt_cvr", "REL/d/a/obj/d_a_obj_belt_cvr.cpp"),
    Rel(NonMatching, "d_a_obj_belt_obstacle", "REL/d/a/obj/d_a_obj_belt_obstacle.cpp"),
    Rel(NonMatching, "d_a_obj_bg", "REL/d/a/obj/d_a_obj_bg.cpp"),
    Rel(
        NonMatching, "d_a_obj_bigbomb_flower", "REL/d/a/obj/d_a_obj_bigbomb_flower.cpp"
    ),
    Rel(NonMatching, "d_a_obj_bird_sp", "REL/d/a/obj/d_a_obj_bird_sp.cpp"),
    Rel(NonMatching, "d_a_obj_bird_statue", "REL/d/a/obj/d_a_obj_bird_statue.cpp"),
    Rel(NonMatching, "d_a_obj_blade", "REL/d/a/obj/d_a_obj_blade.cpp"),
    Rel(NonMatching, "d_a_obj_blast_rock", "REL/d/a/obj/d_a_obj_blast_rock.cpp"),
    Rel(NonMatching, "d_a_obj_block_rope", "REL/d/a/obj/d_a_obj_block_rope.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_block_underground",
        "REL/d/a/obj/d_a_obj_block_underground.cpp",
    ),
    Rel(NonMatching, "d_a_obj_blow_coal", "REL/d/a/obj/d_a_obj_blow_coal.cpp"),
    Rel(NonMatching, "d_a_obj_boat", "REL/d/a/obj/d_a_obj_boat.cpp"),
    Rel(NonMatching, "d_a_obj_boxcage_F300", "REL/d/a/obj/d_a_obj_boxcage_F300.cpp"),
    Rel(NonMatching, "d_a_obj_bridge_bone", "REL/d/a/obj/d_a_obj_bridge_bone.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_bridge_building",
        "REL/d/a/obj/d_a_obj_bridge_building.cpp",
    ),
    Rel(NonMatching, "d_a_obj_bridge_fall", "REL/d/a/obj/d_a_obj_bridge_fall.cpp"),
    Rel(NonMatching, "d_a_obj_bridge_step", "REL/d/a/obj/d_a_obj_bridge_step.cpp"),
    Rel(
        NonMatching, "d_a_obj_bridge_stretch", "REL/d/a/obj/d_a_obj_bridge_stretch.cpp"
    ),
    Rel(NonMatching, "d_a_obj_bridge_time", "REL/d/a/obj/d_a_obj_bridge_time.cpp"),
    Rel(Matching, "d_a_obj_bstone", "REL/d/a/obj/d_a_obj_bstone.cpp"),
    Rel(NonMatching, "d_a_obj_bubble", "REL/d/a/obj/d_a_obj_bubble.cpp"),
    Rel(NonMatching, "d_a_obj_building", "REL/d/a/obj/d_a_obj_building.cpp"),
    Rel(NonMatching, "d_a_obj_bulb_switch", "REL/d/a/obj/d_a_obj_bulb_switch.cpp"),
    Rel(NonMatching, "d_a_obj_cannon", "REL/d/a/obj/d_a_obj_cannon.cpp"),
    Rel(NonMatching, "d_a_obj_cannon_cover", "REL/d/a/obj/d_a_obj_cannon_cover.cpp"),
    Rel(NonMatching, "d_a_obj_carry_stone", "REL/d/a/obj/d_a_obj_carry_stone.cpp"),
    Rel(NonMatching, "d_a_obj_chair", "REL/d/a/obj/d_a_obj_chair.cpp"),
    Rel(NonMatching, "d_a_obj_chandelier", "REL/d/a/obj/d_a_obj_chandelier.cpp"),
    Rel(NonMatching, "d_a_obj_chest", "REL/d/a/obj/d_a_obj_chest.cpp"),
    Rel(NonMatching, "d_a_obj_city", "REL/d/a/obj/d_a_obj_city.cpp"),
    Rel(NonMatching, "d_a_obj_city_water", "REL/d/a/obj/d_a_obj_city_water.cpp"),
    Rel(NonMatching, "d_a_obj_claw_shot_tg", "REL/d/a/obj/d_a_obj_claw_shot_tg.cpp"),
    Rel(
        NonMatching, "d_a_obj_clearness_wall", "REL/d/a/obj/d_a_obj_clearness_wall.cpp"
    ),
    Rel(NonMatching, "d_a_obj_clef", "REL/d/a/obj/d_a_obj_clef.cpp"),
    Rel(NonMatching, "d_a_obj_cloud_dive", "REL/d/a/obj/d_a_obj_cloud_dive.cpp"),
    Rel(NonMatching, "d_a_obj_col", "REL/d/a/obj/d_a_obj_col.cpp"),
    Rel(NonMatching, "d_a_obj_control_panel", "REL/d/a/obj/d_a_obj_control_panel.cpp"),
    Rel(NonMatching, "d_a_obj_cumul_cloud", "REL/d/a/obj/d_a_obj_cumul_cloud.cpp"),
    Rel(NonMatching, "d_a_obj_D300", "REL/d/a/obj/d_a_obj_D300.cpp"),
    Rel(NonMatching, "d_a_obj_D3_dummy", "REL/d/a/obj/d_a_obj_D3_dummy.cpp"),
    Rel(NonMatching, "d_a_obj_daynight", "REL/d/a/obj/d_a_obj_daynight.cpp"),
    Rel(NonMatching, "d_a_obj_decoA", "REL/d/a/obj/d_a_obj_decoA.cpp"),
    Rel(NonMatching, "d_a_obj_decoB", "REL/d/a/obj/d_a_obj_decoB.cpp"),
    Rel(NonMatching, "d_a_obj_desert", "REL/d/a/obj/d_a_obj_desert.cpp"),
    Rel(NonMatching, "d_a_obj_desert_ago", "REL/d/a/obj/d_a_obj_desert_ago.cpp"),
    Rel(NonMatching, "d_a_obj_desert_debris", "REL/d/a/obj/d_a_obj_desert_debris.cpp"),
    Rel(NonMatching, "d_a_obj_desert_meter", "REL/d/a/obj/d_a_obj_desert_meter.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_display_only_nbs",
        "REL/d/a/obj/d_a_obj_display_only_nbs.cpp",
    ),
    Rel(NonMatching, "d_a_obj_distant_ship", "REL/d/a/obj/d_a_obj_distant_ship.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_diviner_crystal",
        "REL/d/a/obj/d_a_obj_diviner_crystal.cpp",
    ),
    Rel(NonMatching, "d_a_obj_door", "REL/d/a/obj/d_a_obj_door.cpp"),
    Rel(NonMatching, "d_a_obj_door_bossD101", "REL/d/a/obj/d_a_obj_door_bossD101.cpp"),
    Rel(NonMatching, "d_a_obj_door_boss", "REL/d/a/obj/d_a_obj_door_boss.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_door_dungeonD200",
        "REL/d/a/obj/d_a_obj_door_dungeonD200.cpp",
    ),
    Rel(NonMatching, "d_a_obj_door_dungeon", "REL/d/a/obj/d_a_obj_door_dungeon.cpp"),
    Rel(
        NonMatching, "d_a_obj_dormitory_gate", "REL/d/a/obj/d_a_obj_dormitory_gate.cpp"
    ),
    Rel(NonMatching, "d_a_obj_down_lava", "REL/d/a/obj/d_a_obj_down_lava.cpp"),
    Rel(NonMatching, "d_a_obj_drum", "REL/d/a/obj/d_a_obj_drum.cpp"),
    Rel(NonMatching, "d_a_obj_dungeon_ship", "REL/d/a/obj/d_a_obj_dungeon_ship.cpp"),
    Rel(
        NonMatching, "d_a_obj_electric_light", "REL/d/a/obj/d_a_obj_electric_light.cpp"
    ),
    Rel(NonMatching, "d_a_obj_evil_field", "REL/d/a/obj/d_a_obj_evil_field.cpp"),
    Rel(NonMatching, "d_a_obj_F302_light", "REL/d/a/obj/d_a_obj_F302_light.cpp"),
    Rel(
        NonMatching, "d_a_obj_F400_gate_leaf", "REL/d/a/obj/d_a_obj_F400_gate_leaf.cpp"
    ),
    Rel(
        NonMatching, "d_a_obj_F400_gate_seal", "REL/d/a/obj/d_a_obj_F400_gate_seal.cpp"
    ),
    Rel(NonMatching, "d_a_obj_farmland", "REL/d/a/obj/d_a_obj_farmland.cpp"),
    Rel(NonMatching, "d_a_obj_fence", "REL/d/a/obj/d_a_obj_fence.cpp"),
    Rel(Matching, "d_a_obj_fence_boko2", "REL/d/a/obj/d_a_obj_fence_boko2.cpp"),
    Rel(Matching, "d_a_obj_fence_boko", "REL/d/a/obj/d_a_obj_fence_boko.cpp"),
    Rel(NonMatching, "d_a_obj_fence_iron", "REL/d/a/obj/d_a_obj_fence_iron.cpp"),
    Rel(Matching, "d_a_obj_fence_konsai", "REL/d/a/obj/d_a_obj_fence_konsai.cpp"),
    Rel(NonMatching, "d_a_obj_fire", "REL/d/a/obj/d_a_obj_fire.cpp"),
    Rel(NonMatching, "d_a_obj_firewall", "REL/d/a/obj/d_a_obj_firewall.cpp"),
    Rel(NonMatching, "d_a_obj_fire_obstacle", "REL/d/a/obj/d_a_obj_fire_obstacle.cpp"),
    Rel(NonMatching, "d_a_obj_fire_pillar", "REL/d/a/obj/d_a_obj_fire_pillar.cpp"),
    Rel(NonMatching, "d_a_obj_flag", "REL/d/a/obj/d_a_obj_flag.cpp"),
    Rel(
        NonMatching, "d_a_obj_flower_ancient", "REL/d/a/obj/d_a_obj_flower_ancient.cpp"
    ),
    Rel(
        Matching,
        "d_a_obj_flying_clawshot_target",
        "REL/d/a/obj/d_a_obj_flying_clawshot_target.cpp",
    ),
    Rel(NonMatching, "d_a_obj_fortune_ring", "REL/d/a/obj/d_a_obj_fortune_ring.cpp"),
    Rel(NonMatching, "d_a_obj_fruitB", "REL/d/a/obj/d_a_obj_fruitB.cpp"),
    Rel(NonMatching, "d_a_obj_fruit", "REL/d/a/obj/d_a_obj_fruit.cpp"),
    Rel(
        Matching,
        "d_a_obj_fruit_guts_leaf",
        "REL/d/a/obj/d_a_obj_fruit_guts_leaf.cpp",
    ),
    Rel(NonMatching, "d_a_obj_fruit_tree", "REL/d/a/obj/d_a_obj_fruit_tree.cpp"),
    Rel(NonMatching, "d_a_obj_gear", "REL/d/a/obj/d_a_obj_gear.cpp"),
    Rel(
        NonMatching, "d_a_obj_girahimu_floor", "REL/d/a/obj/d_a_obj_girahimu_floor.cpp"
    ),
    Rel(
        NonMatching, "d_a_obj_girahimu_knife", "REL/d/a/obj/d_a_obj_girahimu_knife.cpp"
    ),
    Rel(
        NonMatching,
        "d_a_obj_girahimu_sword_link",
        "REL/d/a/obj/d_a_obj_girahimu_sword_link.cpp",
    ),
    Rel(NonMatching, "d_a_obj_girahim_foot", "REL/d/a/obj/d_a_obj_girahim_foot.cpp"),
    Rel(NonMatching, "d_a_obj_goddess_cube", "REL/d/a/obj/d_a_obj_goddess_cube.cpp"),
    Rel(
        NonMatching, "d_a_obj_goddess_statue", "REL/d/a/obj/d_a_obj_goddess_statue.cpp"
    ),
    Rel(NonMatching, "d_a_obj_god_mark", "REL/d/a/obj/d_a_obj_god_mark.cpp"),
    Rel(NonMatching, "d_a_obj_grass_coil", "REL/d/a/obj/d_a_obj_grass_coil.cpp"),
    Rel(NonMatching, "d_a_obj_grave", "REL/d/a/obj/d_a_obj_grave.cpp"),
    Rel(NonMatching, "d_a_obj_ground_cover", "REL/d/a/obj/d_a_obj_ground_cover.cpp"),
    Rel(NonMatching, "d_a_obj_guard_log", "REL/d/a/obj/d_a_obj_guard_log.cpp"),
    Rel(NonMatching, "d_a_obj_harp_hint", "REL/d/a/obj/d_a_obj_harp_hint.cpp"),
    Rel(NonMatching, "d_a_obj_himo", "REL/d/a/obj/d_a_obj_himo.cpp"),
    Rel(NonMatching, "d_a_obj_hit_lever_sw", "REL/d/a/obj/d_a_obj_hit_lever_sw.cpp"),
    Rel(NonMatching, "d_a_obj_hole", "REL/d/a/obj/d_a_obj_hole.cpp"),
    Rel(NonMatching, "d_a_obj_hole_minigame", "REL/d/a/obj/d_a_obj_hole_minigame.cpp"),
    Rel(NonMatching, "d_a_obj_impa_door", "REL/d/a/obj/d_a_obj_impa_door.cpp"),
    Rel(NonMatching, "d_a_obj_insect_island", "REL/d/a/obj/d_a_obj_insect_island.cpp"),
    Rel(NonMatching, "d_a_obj_into_hole", "REL/d/a/obj/d_a_obj_into_hole.cpp"),
    Rel(Matching, "d_a_obj_iron_stage", "REL/d/a/obj/d_a_obj_iron_stage.cpp"),
    Rel(NonMatching, "d_a_obj_island_LOD", "REL/d/a/obj/d_a_obj_island_LOD.cpp"),
    Rel(Matching, "d_a_obj_island_nusi", "REL/d/a/obj/d_a_obj_island_nusi.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_item_heart_container",
        "REL/d/a/obj/d_a_obj_item_heart_container.cpp",
    ),
    Rel(NonMatching, "d_a_obj_ivy_rope", "REL/d/a/obj/d_a_obj_ivy_rope.cpp"),
    Rel(Matching, "d_a_obj_junk_repairing", "REL/d/a/obj/d_a_obj_junk_repairing.cpp"),
    Rel(NonMatching, "d_a_obj_kanban_stone", "REL/d/a/obj/d_a_obj_kanban_stone.cpp"),
    Rel(NonMatching, "d_a_obj_kibako", "REL/d/a/obj/d_a_obj_kibako.cpp"),
    Rel(NonMatching, "d_a_obj_kumite_wall", "REL/d/a/obj/d_a_obj_kumite_wall.cpp"),
    Rel(NonMatching, "d_a_obj_lamp", "REL/d/a/obj/d_a_obj_lamp.cpp"),
    Rel(NonMatching, "d_a_obj_lava_d201", "REL/d/a/obj/d_a_obj_lava_d201.cpp"),
    Rel(Matching, "d_a_obj_lava_F200", "REL/d/a/obj/d_a_obj_lava_F200.cpp"),
    Rel(NonMatching, "d_a_obj_lava_plate", "REL/d/a/obj/d_a_obj_lava_plate.cpp"),
    Rel(NonMatching, "d_a_obj_leaf_swing", "REL/d/a/obj/d_a_obj_leaf_swing.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_lighthouse_harp",
        "REL/d/a/obj/d_a_obj_lighthouse_harp.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_obj_lighthouse_light",
        "REL/d/a/obj/d_a_obj_lighthouse_light.cpp",
    ),
    Rel(NonMatching, "d_a_obj_light_line", "REL/d/a/obj/d_a_obj_light_line.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_light_shaft_small",
        "REL/d/a/obj/d_a_obj_light_shaft_small.cpp",
    ),
    Rel(NonMatching, "d_a_obj_lock", "REL/d/a/obj/d_a_obj_lock.cpp"),
    Rel(NonMatching, "d_a_obj_log", "REL/d/a/obj/d_a_obj_log.cpp"),
    Rel(NonMatching, "d_a_obj_log_water", "REL/d/a/obj/d_a_obj_log_water.cpp"),
    Rel(NonMatching, "d_a_obj_lotus", "REL/d/a/obj/d_a_obj_lotus.cpp"),
    Rel(NonMatching, "d_a_obj_lotus_flower", "REL/d/a/obj/d_a_obj_lotus_flower.cpp"),
    Rel(NonMatching, "d_a_obj_lotus_seed", "REL/d/a/obj/d_a_obj_lotus_seed.cpp"),
    Rel(NonMatching, "d_a_obj_mapparts", "REL/d/a/obj/d_a_obj_mapparts.cpp"),
    Rel(Matching, "d_a_obj_megami_island", "REL/d/a/obj/d_a_obj_megami_island.cpp"),
    Rel(NonMatching, "d_a_obj_mg_pumpkin", "REL/d/a/obj/d_a_obj_mg_pumpkin.cpp"),
    Rel(NonMatching, "d_a_obj_mole_cover", "REL/d/a/obj/d_a_obj_mole_cover.cpp"),
    Rel(Matching, "d_a_obj_mole_soil", "REL/d/a/obj/d_a_obj_mole_soil.cpp"),
    Rel(NonMatching, "d_a_obj_move_elec", "REL/d/a/obj/d_a_obj_move_elec.cpp"),
    Rel(NonMatching, "d_a_obj_move_lift_vol", "REL/d/a/obj/d_a_obj_move_lift_vol.cpp"),
    Rel(NonMatching, "d_a_obj_musasabi", "REL/d/a/obj/d_a_obj_musasabi.cpp"),
    Rel(NonMatching, "d_a_obj_mushroom", "REL/d/a/obj/d_a_obj_mushroom.cpp"),
    Rel(NonMatching, "d_a_obj_needle_desert", "REL/d/a/obj/d_a_obj_needle_desert.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_needle_underground",
        "REL/d/a/obj/d_a_obj_needle_underground.cpp",
    ),
    Rel(NonMatching, "d_a_obj_oct_grass", "REL/d/a/obj/d_a_obj_oct_grass.cpp"),
    Rel(
        NonMatching, "d_a_obj_oct_grass_leaf", "REL/d/a/obj/d_a_obj_oct_grass_leaf.cpp"
    ),
    Rel(NonMatching, "d_a_obj_paint", "REL/d/a/obj/d_a_obj_paint.cpp"),
    Rel(NonMatching, "d_a_obj_pinwheel", "REL/d/a/obj/d_a_obj_pinwheel.cpp"),
    Rel(NonMatching, "d_a_obj_pipe", "REL/d/a/obj/d_a_obj_pipe.cpp"),
    Rel(NonMatching, "d_a_obj_piston", "REL/d/a/obj/d_a_obj_piston.cpp"),
    Rel(NonMatching, "d_a_obj_pole_stony", "REL/d/a/obj/d_a_obj_pole_stony.cpp"),
    Rel(Matching, "d_a_obj_pool_cock", "REL/d/a/obj/d_a_obj_pool_cock.cpp"),
    Rel(NonMatching, "d_a_obj_pot_sal", "REL/d/a/obj/d_a_obj_pot_sal.cpp"),
    Rel(
        NonMatching, "d_a_obj_propeller_lift", "REL/d/a/obj/d_a_obj_propeller_lift.cpp"
    ),
    Rel(NonMatching, "d_a_obj_propera", "REL/d/a/obj/d_a_obj_propera.cpp"),
    Rel(Matching, "d_a_obj_pumpkin_bar", "REL/d/a/obj/d_a_obj_pumpkin_bar.cpp"),
    Rel(Matching, "d_a_obj_pumpkin_leaf", "REL/d/a/obj/d_a_obj_pumpkin_leaf.cpp"),
    Rel(NonMatching, "d_a_obj_push_block", "REL/d/a/obj/d_a_obj_push_block.cpp"),
    Rel(NonMatching, "d_a_obj_puzzle_island", "REL/d/a/obj/d_a_obj_puzzle_island.cpp"),
    Rel(NonMatching, "d_a_obj_rail_end", "REL/d/a/obj/d_a_obj_rail_end.cpp"),
    Rel(NonMatching, "d_a_obj_rail_post", "REL/d/a/obj/d_a_obj_rail_post.cpp"),
    Rel(NonMatching, "d_a_obj_ride_rock", "REL/d/a/obj/d_a_obj_ride_rock.cpp"),
    Rel(Matching, "d_a_obj_ring", "REL/d/a/obj/d_a_obj_ring.cpp"),
    Rel(NonMatching, "d_a_obj_rock_boat", "REL/d/a/obj/d_a_obj_rock_boat.cpp"),
    Rel(NonMatching, "d_a_obj_rock_dragon", "REL/d/a/obj/d_a_obj_rock_dragon.cpp"),
    Rel(NonMatching, "d_a_obj_rock_sky", "REL/d/a/obj/d_a_obj_rock_sky.cpp"),
    Rel(NonMatching, "d_a_obj_roll_pillar", "REL/d/a/obj/d_a_obj_roll_pillar.cpp"),
    Rel(NonMatching, "d_a_obj_roll_rock", "REL/d/a/obj/d_a_obj_roll_rock.cpp"),
    Rel(NonMatching, "d_a_obj_rope_base", "REL/d/a/obj/d_a_obj_rope_base.cpp"),
    Rel(NonMatching, "d_a_obj_rope_igaiga", "REL/d/a/obj/d_a_obj_rope_igaiga.cpp"),
    Rel(
        NonMatching, "d_a_obj_rotation_light", "REL/d/a/obj/d_a_obj_rotation_light.cpp"
    ),
    Rel(NonMatching, "d_a_obj_roulette", "REL/d/a/obj/d_a_obj_roulette.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_roulette_island_c",
        "REL/d/a/obj/d_a_obj_roulette_island_c.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_obj_roulette_island_r",
        "REL/d/a/obj/d_a_obj_roulette_island_r.cpp",
    ),
    Rel(NonMatching, "d_a_obj_ro_at_target", "REL/d/a/obj/d_a_obj_ro_at_target.cpp"),
    Rel(NonMatching, "d_a_obj_ruined_save", "REL/d/a/obj/d_a_obj_ruined_save.cpp"),
    Rel(NonMatching, "d_a_obj_sail", "REL/d/a/obj/d_a_obj_sail.cpp"),
    Rel(NonMatching, "d_a_obj_sandbag", "REL/d/a/obj/d_a_obj_sandbag.cpp"),
    Rel(NonMatching, "d_a_obj_sand_D301", "REL/d/a/obj/d_a_obj_sand_D301.cpp"),
    Rel(NonMatching, "d_a_obj_sand_floor", "REL/d/a/obj/d_a_obj_sand_floor.cpp"),
    Rel(NonMatching, "d_a_obj_save", "REL/d/a/obj/d_a_obj_save.cpp"),
    Rel(NonMatching, "d_a_obj_scattersand", "REL/d/a/obj/d_a_obj_scattersand.cpp"),
    Rel(NonMatching, "d_a_obj_sealed_door", "REL/d/a/obj/d_a_obj_sealed_door.cpp"),
    Rel(Matching, "d_a_obj_seat_sword", "REL/d/a/obj/d_a_obj_seat_sword.cpp"),
    Rel(NonMatching, "d_a_obj_sea_F301", "REL/d/a/obj/d_a_obj_sea_F301.cpp"),
    Rel(NonMatching, "d_a_obj_shed", "REL/d/a/obj/d_a_obj_shed.cpp"),
    Rel(NonMatching, "d_a_obj_ship_window", "REL/d/a/obj/d_a_obj_ship_window.cpp"),
    Rel(Matching, "d_a_obj_shrine_after", "REL/d/a/obj/d_a_obj_shrine_after.cpp"),
    Rel(Matching, "d_a_obj_shrine_before", "REL/d/a/obj/d_a_obj_shrine_before.cpp"),
    Rel(
        Matching,
        "d_a_obj_shrine_bef_inside",
        "REL/d/a/obj/d_a_obj_shrine_bef_inside.cpp",
    ),
    Rel(NonMatching, "d_a_obj_shutter", "REL/d/a/obj/d_a_obj_shutter.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_shutter_change_scene",
        "REL/d/a/obj/d_a_obj_shutter_change_scene.cpp",
    ),
    Rel(NonMatching, "d_a_obj_shutter_fence", "REL/d/a/obj/d_a_obj_shutter_fence.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_shutter_waterD101",
        "REL/d/a/obj/d_a_obj_shutter_waterD101.cpp",
    ),
    Rel(NonMatching, "d_a_obj_side_shutter", "REL/d/a/obj/d_a_obj_side_shutter.cpp"),
    Rel(NonMatching, "d_a_obj_sink_floor_f", "REL/d/a/obj/d_a_obj_sink_floor_f.cpp"),
    Rel(NonMatching, "d_a_obj_siren_2dmap", "REL/d/a/obj/d_a_obj_siren_2dmap.cpp"),
    Rel(NonMatching, "d_a_obj_siren_barrier", "REL/d/a/obj/d_a_obj_siren_barrier.cpp"),
    Rel(NonMatching, "d_a_obj_skull", "REL/d/a/obj/d_a_obj_skull.cpp"),
    Rel(NonMatching, "d_a_obj_slice_log", "REL/d/a/obj/d_a_obj_slice_log.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_slice_log_parts",
        "REL/d/a/obj/d_a_obj_slice_log_parts.cpp",
    ),
    Rel(Matching, "d_a_obj_smoke", "REL/d/a/obj/d_a_obj_smoke.cpp"),
    Rel(NonMatching, "d_a_obj_soil", "REL/d/a/obj/d_a_obj_soil.cpp"),
    Rel(NonMatching, "d_a_obj_spider_line", "REL/d/a/obj/d_a_obj_spider_line.cpp"),
    Rel(Matching, "d_a_obj_spike", "REL/d/a/obj/d_a_obj_spike.cpp"),
    Rel(Matching, "d_a_obj_stage_cover", "REL/d/a/obj/d_a_obj_stage_cover.cpp"),
    Rel(NonMatching, "d_a_obj_stage_crack", "REL/d/a/obj/d_a_obj_stage_crack.cpp"),
    Rel(NonMatching, "d_a_obj_stage_debris", "REL/d/a/obj/d_a_obj_stage_debris.cpp"),
    Rel(NonMatching, "d_a_obj_stage_kraken", "REL/d/a/obj/d_a_obj_stage_kraken.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_stage_kraken_parts",
        "REL/d/a/obj/d_a_obj_stage_kraken_parts.cpp",
    ),
    Rel(NonMatching, "d_a_obj_stage_sink", "REL/d/a/obj/d_a_obj_stage_sink.cpp"),
    Rel(NonMatching, "d_a_obj_stage_water", "REL/d/a/obj/d_a_obj_stage_water.cpp"),
    Rel(NonMatching, "d_a_obj_step_gumarm", "REL/d/a/obj/d_a_obj_step_gumarm.cpp"),
    Rel(NonMatching, "d_a_obj_step_statue", "REL/d/a/obj/d_a_obj_step_statue.cpp"),
    Rel(
        NonMatching, "d_a_obj_step_time_slip", "REL/d/a/obj/d_a_obj_step_time_slip.cpp"
    ),
    Rel(NonMatching, "d_a_obj_stone_stand", "REL/d/a/obj/d_a_obj_stone_stand.cpp"),
    Rel(NonMatching, "d_a_obj_stopper_rock", "REL/d/a/obj/d_a_obj_stopper_rock.cpp"),
    Rel(NonMatching, "d_a_obj_stopping_rope", "REL/d/a/obj/d_a_obj_stopping_rope.cpp"),
    Rel(NonMatching, "d_a_obj_stream_lava", "REL/d/a/obj/d_a_obj_stream_lava.cpp"),
    Rel(Matching, "d_a_obj_sun_light", "REL/d/a/obj/d_a_obj_sun_light.cpp"),
    Rel(NonMatching, "d_a_obj_swhit", "REL/d/a/obj/d_a_obj_swhit.cpp"),
    Rel(
        NonMatching, "d_a_obj_switch_shutter", "REL/d/a/obj/d_a_obj_switch_shutter.cpp"
    ),
    Rel(NonMatching, "d_a_obj_switch_wall", "REL/d/a/obj/d_a_obj_switch_wall.cpp"),
    Rel(NonMatching, "d_a_obj_sword_candle", "REL/d/a/obj/d_a_obj_sword_candle.cpp"),
    Rel(NonMatching, "d_a_obj_sword_stab", "REL/d/a/obj/d_a_obj_sword_stab.cpp"),
    Rel(NonMatching, "d_a_obj_swrd_prj", "REL/d/a/obj/d_a_obj_swrd_prj.cpp"),
    Rel(NonMatching, "d_a_obj_sw_bank", "REL/d/a/obj/d_a_obj_sw_bank.cpp"),
    Rel(NonMatching, "d_a_obj_sw_bank_small", "REL/d/a/obj/d_a_obj_sw_bank_small.cpp"),
    Rel(NonMatching, "d_a_obj_sw_dir", "REL/d/a/obj/d_a_obj_sw_dir.cpp"),
    Rel(NonMatching, "d_a_obj_sw_dir_door", "REL/d/a/obj/d_a_obj_sw_dir_door.cpp"),
    Rel(NonMatching, "d_a_obj_sw_harp", "REL/d/a/obj/d_a_obj_sw_harp.cpp"),
    Rel(NonMatching, "d_a_obj_sw_sword_beam", "REL/d/a/obj/d_a_obj_sw_sword_beam.cpp"),
    Rel(NonMatching, "d_a_obj_sw_syako", "REL/d/a/obj/d_a_obj_sw_syako.cpp"),
    Rel(NonMatching, "d_a_obj_sw_whiplever", "REL/d/a/obj/d_a_obj_sw_whiplever.cpp"),
    Rel(NonMatching, "d_a_obj_syako_shutter", "REL/d/a/obj/d_a_obj_syako_shutter.cpp"),
    Rel(NonMatching, "d_a_obj_tableware", "REL/d/a/obj/d_a_obj_tableware.cpp"),
    Rel(NonMatching, "d_a_obj_tackle", "REL/d/a/obj/d_a_obj_tackle.cpp"),
    Rel(NonMatching, "d_a_obj_tarzan_pole", "REL/d/a/obj/d_a_obj_tarzan_pole.cpp"),
    Rel(NonMatching, "d_a_obj_tenijima", "REL/d/a/obj/d_a_obj_tenijima.cpp"),
    Rel(NonMatching, "d_a_obj_teni_rail", "REL/d/a/obj/d_a_obj_teni_rail.cpp"),
    Rel(
        NonMatching, "d_a_obj_teni_rail_post", "REL/d/a/obj/d_a_obj_teni_rail_post.cpp"
    ),
    Rel(NonMatching, "d_a_obj_terry_bicycle", "REL/d/a/obj/d_a_obj_terry_bicycle.cpp"),
    Rel(NonMatching, "d_a_obj_terry_gimmick", "REL/d/a/obj/d_a_obj_terry_gimmick.cpp"),
    Rel(NonMatching, "d_a_obj_terry_hole", "REL/d/a/obj/d_a_obj_terry_hole.cpp"),
    Rel(NonMatching, "d_a_obj_terry_island", "REL/d/a/obj/d_a_obj_terry_island.cpp"),
    Rel(NonMatching, "d_a_obj_terry_shop", "REL/d/a/obj/d_a_obj_terry_shop.cpp"),
    Rel(NonMatching, "d_a_obj_terry_switch", "REL/d/a/obj/d_a_obj_terry_switch.cpp"),
    Rel(NonMatching, "d_a_obj_time_base", "REL/d/a/obj/d_a_obj_time_base.cpp"),
    Rel(NonMatching, "d_a_obj_time_block", "REL/d/a/obj/d_a_obj_time_block.cpp"),
    Rel(NonMatching, "d_a_obj_time_boat", "REL/d/a/obj/d_a_obj_time_boat.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_time_boat_bullet",
        "REL/d/a/obj/d_a_obj_time_boat_bullet.cpp",
    ),
    Rel(NonMatching, "d_a_obj_time_door", "REL/d/a/obj/d_a_obj_time_door.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_time_door_before",
        "REL/d/a/obj/d_a_obj_time_door_before.cpp",
    ),
    Rel(NonMatching, "d_a_obj_time_stage_bg", "REL/d/a/obj/d_a_obj_time_stage_bg.cpp"),
    Rel(NonMatching, "d_a_obj_time_stone", "REL/d/a/obj/d_a_obj_time_stone.cpp"),
    Rel(
        Matching,
        "d_a_obj_toD3_stone_figure",
        "REL/d/a/obj/d_a_obj_toD3_stone_figure.cpp",
    ),
    Rel(NonMatching, "d_a_obj_toge_trap", "REL/d/a/obj/d_a_obj_toge_trap.cpp"),
    Rel(NonMatching, "d_a_obj_tornado", "REL/d/a/obj/d_a_obj_tornado.cpp"),
    Rel(NonMatching, "d_a_obj_tower_bomb", "REL/d/a/obj/d_a_obj_tower_bomb.cpp"),
    Rel(NonMatching, "d_a_obj_tower_D101", "REL/d/a/obj/d_a_obj_tower_D101.cpp"),
    Rel(Matching, "d_a_obj_tower_gearD101", "REL/d/a/obj/d_a_obj_tower_gearD101.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_tower_hand_D101",
        "REL/d/a/obj/d_a_obj_tower_hand_D101.cpp",
    ),
    Rel(
        NonMatching, "d_a_obj_trap_bird_wood", "REL/d/a/obj/d_a_obj_trap_bird_wood.cpp"
    ),
    Rel(Matching, "d_a_obj_trap_rock_1", "REL/d/a/obj/d_a_obj_trap_rock_1.cpp"),
    Rel(
        Matching,
        "d_a_obj_treasure_island",
        "REL/d/a/obj/d_a_obj_treasure_island.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_obj_treasure_island_b",
        "REL/d/a/obj/d_a_obj_treasure_island_b.cpp",
    ),
    Rel(Matching, "d_a_obj_triforce", "REL/d/a/obj/d_a_obj_triforce.cpp"),
    Rel(NonMatching, "d_a_obj_trolley", "REL/d/a/obj/d_a_obj_trolley.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_trolley_shutter",
        "REL/d/a/obj/d_a_obj_trolley_shutter.cpp",
    ),
    Rel(NonMatching, "d_a_obj_truck", "REL/d/a/obj/d_a_obj_truck.cpp"),
    Rel(
        NonMatching, "d_a_obj_truck_rail_col", "REL/d/a/obj/d_a_obj_truck_rail_col.cpp"
    ),
    Rel(NonMatching, "d_a_obj_truck_stopper", "REL/d/a/obj/d_a_obj_truck_stopper.cpp"),
    Rel(NonMatching, "d_a_obj_tr_shutter_cs", "REL/d/a/obj/d_a_obj_tr_shutter_cs.cpp"),
    Rel(NonMatching, "d_a_obj_tubo", "REL/d/a/obj/d_a_obj_tubo.cpp"),
    Rel(NonMatching, "d_a_obj_tubo_big", "REL/d/a/obj/d_a_obj_tubo_big.cpp"),
    Rel(NonMatching, "d_a_obj_tumble_weed", "REL/d/a/obj/d_a_obj_tumble_weed.cpp"),
    Rel(NonMatching, "d_a_obj_underground", "REL/d/a/obj/d_a_obj_underground.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_underground_switch",
        "REL/d/a/obj/d_a_obj_underground_switch.cpp",
    ),
    Rel(NonMatching, "d_a_obj_under_cloud", "REL/d/a/obj/d_a_obj_under_cloud.cpp"),
    Rel(NonMatching, "d_a_obj_updown_lava", "REL/d/a/obj/d_a_obj_updown_lava.cpp"),
    Rel(Matching, "d_a_obj_utajima", "REL/d/a/obj/d_a_obj_utajima.cpp"),
    Rel(Matching, "d_a_obj_utajima_island", "REL/d/a/obj/d_a_obj_utajima_island.cpp"),
    Rel(Matching, "d_a_obj_utajima_lv2", "REL/d/a/obj/d_a_obj_utajima_lv2.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_utajima_main_mecha",
        "REL/d/a/obj/d_a_obj_utajima_main_mecha.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_obj_utajima_pedestal",
        "REL/d/a/obj/d_a_obj_utajima_pedestal.cpp",
    ),
    Rel(
        NonMatching,
        "d_a_obj_utajima_stopper",
        "REL/d/a/obj/d_a_obj_utajima_stopper.cpp",
    ),
    Rel(
        Matching,
        "d_a_obj_uta_demo_pedest",
        "REL/d/a/obj/d_a_obj_uta_demo_pedest.cpp",
    ),
    Rel(NonMatching, "d_a_obj_vacu_dust", "REL/d/a/obj/d_a_obj_vacu_dust.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_vacu_dust_parts",
        "REL/d/a/obj/d_a_obj_vacu_dust_parts.cpp",
    ),
    Rel(NonMatching, "d_a_obj_vd_bullet", "REL/d/a/obj/d_a_obj_vd_bullet.cpp"),
    Rel(NonMatching, "d_a_obj_vent_fan", "REL/d/a/obj/d_a_obj_vent_fan.cpp"),
    Rel(NonMatching, "d_a_obj_vortex", "REL/d/a/obj/d_a_obj_vortex.cpp"),
    Rel(NonMatching, "d_a_obj_warp", "REL/d/a/obj/d_a_obj_warp.cpp"),
    Rel(NonMatching, "d_a_obj_warp_hole", "REL/d/a/obj/d_a_obj_warp_hole.cpp"),
    Rel(
        NonMatching, "d_a_obj_waterfall_D100", "REL/d/a/obj/d_a_obj_waterfall_D100.cpp"
    ),
    Rel(
        NonMatching, "d_a_obj_waterfall_D101", "REL/d/a/obj/d_a_obj_waterfall_D101.cpp"
    ),
    Rel(
        NonMatching, "d_a_obj_waterfall_F102", "REL/d/a/obj/d_a_obj_waterfall_F102.cpp"
    ),
    Rel(NonMatching, "d_a_obj_water_F100", "REL/d/a/obj/d_a_obj_water_F100.cpp"),
    Rel(NonMatching, "d_a_obj_water_igaiga", "REL/d/a/obj/d_a_obj_water_igaiga.cpp"),
    Rel(NonMatching, "d_a_obj_water_jar", "REL/d/a/obj/d_a_obj_water_jar.cpp"),
    Rel(NonMatching, "d_a_obj_water_nut", "REL/d/a/obj/d_a_obj_water_nut.cpp"),
    Rel(
        NonMatching, "d_a_obj_water_nut_leaf", "REL/d/a/obj/d_a_obj_water_nut_leaf.cpp"
    ),
    Rel(NonMatching, "d_a_obj_water_shield", "REL/d/a/obj/d_a_obj_water_shield.cpp"),
    Rel(NonMatching, "d_a_obj_water_surface", "REL/d/a/obj/d_a_obj_water_surface.cpp"),
    Rel(NonMatching, "d_a_obj_water_switch", "REL/d/a/obj/d_a_obj_water_switch.cpp"),
    Rel(NonMatching, "d_a_obj_wind02", "REL/d/a/obj/d_a_obj_wind02.cpp"),
    Rel(NonMatching, "d_a_obj_wind03", "REL/d/a/obj/d_a_obj_wind03.cpp"),
    Rel(NonMatching, "d_a_obj_wind04", "REL/d/a/obj/d_a_obj_wind04.cpp"),
    Rel(NonMatching, "d_a_obj_windmill", "REL/d/a/obj/d_a_obj_windmill.cpp"),
    Rel(
        NonMatching,
        "d_a_obj_windmill_desert",
        "REL/d/a/obj/d_a_obj_windmill_desert.cpp",
    ),
    Rel(NonMatching, "d_a_obj_wind", "REL/d/a/obj/d_a_obj_wind.cpp"),
    Rel(NonMatching, "d_a_obj_wood_board", "REL/d/a/obj/d_a_obj_wood_board.cpp"),
    Rel(NonMatching, "d_a_or_cannon_bullet", "REL/d/a/d_a_or_cannon_bullet.cpp"),
    Rel(NonMatching, "d_a_pumpkin", "REL/d/a/d_a_pumpkin.cpp"),
    Rel(NonMatching, "d_a_shop_sample", "REL/d/a/d_a_shop_sample.cpp"),
    Rel(NonMatching, "d_a_t_wood_area", "REL/d/a/d_a_t_wood_area.cpp"),
    Rel(NonMatching, "d_lyt_file_select", "REL/d/d_lyt_file_select.cpp"),
    Rel(NonMatching, "d_lyt_seeker_stone", "REL/d/d_lyt_seeker_stone.cpp"),
    Rel(NonMatching, "d_lyt_title_BG", "REL/d/d_lyt_title_BG.cpp"),
    Rel(Matching, "d_profile", "REL/d/d_profile.cpp"),
    Rel(NonMatching, "d_SoftwareKeyboard", "REL/d/d_SoftwareKeyboard.cpp"),
    Rel(NonMatching, "d_s_boot", "REL/d/d_s_boot.cpp"),
    Rel(NonMatching, "d_title_manager", "REL/d/d_title_manager.cpp"),
    Rel(NonMatching, "d_t_action", "REL/d/t/d_t_action.cpp"),
    Rel(NonMatching, "d_t_at0", "REL/d/t/d_t_at0.cpp"),
    Rel(NonMatching, "d_t_auto_message", "REL/d/t/d_t_auto_message.cpp"),
    Rel(NonMatching, "d_t_barrel", "REL/d/t/d_t_barrel.cpp"),
    Rel(NonMatching, "d_t_barrel_pos", "REL/d/t/d_t_barrel_pos.cpp"),
    Rel(NonMatching, "d_t_bb_quake", "REL/d/t/d_t_bb_quake.cpp"),
    Rel(NonMatching, "d_t_bcal_tag", "REL/d/t/d_t_bcal_tag.cpp"),
    Rel(NonMatching, "d_t_bcz", "REL/d/t/d_t_bcz.cpp"),
    Rel(NonMatching, "d_t_beetle", "REL/d/t/d_t_beetle.cpp"),
    Rel(NonMatching, "d_t_belt_obstacle", "REL/d/t/d_t_belt_obstacle.cpp"),
    Rel(NonMatching, "d_t_clef_game", "REL/d/t/d_t_clef_game.cpp"),
    Rel(NonMatching, "d_t_clef_manager", "REL/d/t/d_t_clef_manager.cpp"),
    Rel(Matching, "d_t_col_bomb_shield", "REL/d/t/d_t_col_bomb_shield.cpp"),
    Rel(NonMatching, "d_t_D3_scene_change", "REL/d/t/d_t_D3_scene_change.cpp"),
    Rel(NonMatching, "d_t_defeat_boss", "REL/d/t/d_t_defeat_boss.cpp"),
    Rel(Matching, "d_t_dowsing", "REL/d/t/d_t_dowsing.cpp"),
    Rel(NonMatching, "d_t_drum", "REL/d/t/d_t_drum.cpp"),
    Rel(NonMatching, "d_t_dungeon_start", "REL/d/t/d_t_dungeon_start.cpp"),
    Rel(NonMatching, "d_t_effect_gen", "REL/d/t/d_t_effect_gen.cpp"),
    Rel(Matching, "d_t_fairytag", "REL/d/t/d_t_fairytag.cpp"),
    Rel(NonMatching, "d_t_fence_synchronizer", "REL/d/t/d_t_fence_synchronizer.cpp"),
    Rel(NonMatching, "d_t_gate_to_ground", "REL/d/t/d_t_gate_to_ground.cpp"),
    Rel(Matching, "d_t_gekotag", "REL/d/t/d_t_gekotag.cpp"),
    Rel(Matching, "d_t_genki_dws_tgt", "REL/d/t/d_t_genki_dws_tgt.cpp"),
    Rel(NonMatching, "d_t_group_summon", "REL/d/t/d_t_group_summon.cpp"),
    Rel(NonMatching, "d_t_group_test", "REL/d/t/d_t_group_test.cpp"),
    Rel(NonMatching, "d_t_harp", "REL/d/t/d_t_harp.cpp"),
    Rel(NonMatching, "d_t_heat_resist", "REL/d/t/d_t_heat_resist.cpp"),
    Rel(Matching, "d_t_holy_water", "REL/d/t/d_t_holy_water.cpp"),
    Rel(NonMatching, "d_t_insect", "REL/d/t/d_t_insect.cpp"),
    Rel(NonMatching, "d_t_ks", "REL/d/t/d_t_ks.cpp"),
    Rel(NonMatching, "d_t_kytag", "REL/d/t/d_t_kytag.cpp"),
    Rel(NonMatching, "d_t_lb_thunder", "REL/d/t/d_t_lb_thunder.cpp"),
    Rel(Matching, "d_t_light_shaft_eff", "REL/d/t/d_t_light_shaft_eff.cpp"),
    Rel(Matching, "d_t_map_inst", "REL/d/t/d_t_map_inst.cpp"),
    Rel(NonMatching, "d_t_map_mark", "REL/d/t/d_t_map_mark.cpp"),
    Rel(NonMatching, "d_t_megami_diving", "REL/d/t/d_t_megami_diving.cpp"),
    Rel(NonMatching, "d_t_miechan", "REL/d/t/d_t_miechan.cpp"),
    Rel(
        NonMatching,
        "d_t_minigame_insect_capture",
        "REL/d/t/d_t_minigame_insect_capture.cpp",
    ),
    Rel(NonMatching, "d_t_mist", "REL/d/t/d_t_mist.cpp"),
    Rel(NonMatching, "d_t_mole_mgr", "REL/d/t/d_t_mole_mgr.cpp"),
    Rel(Matching, "d_t_mole_prohibit", "REL/d/t/d_t_mole_prohibit.cpp"),
    Rel(NonMatching, "d_t_musasabi", "REL/d/t/d_t_musasabi.cpp"),
    Rel(NonMatching, "d_t_musou", "REL/d/t/d_t_musou.cpp"),
    Rel(Matching, "d_t_noeffect_area", "REL/d/t/d_t_noeffect_area.cpp"),
    Rel(NonMatching, "d_t_player_restart", "REL/d/t/d_t_player_restart.cpp"),
    Rel(NonMatching, "d_t_plight", "REL/d/t/d_t_plight.cpp"),
    Rel(NonMatching, "d_t_pltchg", "REL/d/t/d_t_pltchg.cpp"),
    Rel(NonMatching, "d_t_pumpkin_clay", "REL/d/t/d_t_pumpkin_clay.cpp"),
    Rel(NonMatching, "d_t_reaction", "REL/d/t/d_t_reaction.cpp"),
    Rel(NonMatching, "d_t_restart_time_stone", "REL/d/t/d_t_restart_time_stone.cpp"),
    Rel(NonMatching, "d_t_ride_rock_set", "REL/d/t/d_t_ride_rock_set.cpp"),
    Rel(Matching, "d_t_rock_boat", "REL/d/t/d_t_rock_boat.cpp"),
    Rel(NonMatching, "d_t_roll_attack_log", "REL/d/t/d_t_roll_attack_log.cpp"),
    Rel(
        NonMatching, "d_t_ro_at_target_manager", "REL/d/t/d_t_ro_at_target_manager.cpp"
    ),
    Rel(NonMatching, "d_t_scene_change", "REL/d/t/d_t_scene_change.cpp"),
    Rel(NonMatching, "d_t_ship_flood", "REL/d/t/d_t_ship_flood.cpp"),
    Rel(NonMatching, "d_t_ship_slope", "REL/d/t/d_t_ship_slope.cpp"),
    Rel(Matching, "d_t_ship_window", "REL/d/t/d_t_ship_window.cpp"),
    Rel(
        NonMatching,
        "d_t_shutter_fence_forbiddance",
        "REL/d/t/d_t_shutter_fence_forbiddance.cpp",
    ),
    Rel(
        NonMatching,
        "d_t_shutter_fence_permission",
        "REL/d/t/d_t_shutter_fence_permission.cpp",
    ),
    Rel(NonMatching, "d_t_siren_time_attack", "REL/d/t/d_t_siren_time_attack.cpp"),
    Rel(NonMatching, "d_t_skyEnemy", "REL/d/t/d_t_skyEnemy.cpp"),
    Rel(NonMatching, "d_t_sound", "REL/d/t/d_t_sound.cpp"),
    Rel(Matching, "d_t_sound_area", "REL/d/t/d_t_sound_area.cpp"),
    Rel(Matching, "d_t_sound_area_mgr", "REL/d/t/d_t_sound_area_mgr.cpp"),
    Rel(NonMatching, "d_t_sparks2", "REL/d/t/d_t_sparks2.cpp"),
    Rel(NonMatching, "d_t_sparks", "REL/d/t/d_t_sparks.cpp"),
    Rel(NonMatching, "d_t_spore", "REL/d/t/d_t_spore.cpp"),
    Rel(NonMatching, "d_t_stream", "REL/d/t/d_t_stream.cpp"),
    Rel(NonMatching, "d_t_sword_battle_game", "REL/d/t/d_t_sword_battle_game.cpp"),
    Rel(Matching, "d_t_sw_area", "REL/d/t/d_t_sw_area.cpp"),
    Rel(Matching, "d_t_tackle", "REL/d/t/d_t_tackle.cpp"),
    Rel(Matching, "d_t_telop", "REL/d/t/d_t_telop.cpp"),
    Rel(Matching, "d_t_timer", "REL/d/t/d_t_timer.cpp"),
    Rel(Matching, "d_t_time_area_check", "REL/d/t/d_t_time_area_check.cpp"),
    Rel(Matching, "d_t_time_door_beam", "REL/d/t/d_t_time_door_beam.cpp"),
    Rel(Matching, "d_t_touch", "REL/d/t/d_t_touch.cpp"),
    Rel(NonMatching, "d_t_tumble_weed", "REL/d/t/d_t_tumble_weed.cpp"),
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects


# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback

# Optional extra categories for progress tracking
config.progress_categories = [
    ProgressCategory("core", "Core Game Engine"),
    ProgressCategory("game", "SS Game Code"),
    ProgressCategory("egg", "EGG Code"),
    ProgressCategory("nw4r", "NW4R Code"),
    ProgressCategory("rvl", "Wii Specific Code"),
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
