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

import sys
import argparse

from pathlib import Path
from tools.project import (
    Object,
    ProjectConfig,
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

if len(VERSIONS) > 1:
    versions_str = ", ".join(VERSIONS[:-1]) + f" or {VERSIONS[-1]}"
else:
    versions_str = VERSIONS[0]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    default="configure",
    help="configure or progress (default: configure)",
    nargs="?",
)
parser.add_argument(
    "--version",
    dest="version",
    default=VERSIONS[DEFAULT_VERSION],
    help=f"version to build ({versions_str})",
)
parser.add_argument(
    "--build-dir",
    dest="build_dir",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--compilers",
    dest="compilers",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    dest="map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    dest="debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        dest="wrapper",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--build-dtk",
    dest="build_dtk",
    type=Path,
    help="path to decomp-toolkit source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    dest="sjiswrap",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    dest="verbose",
    action="store_true",
    help="print verbose output",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = args.version.upper()
if config.version not in VERSIONS:
    sys.exit(f"Invalid version '{config.version}', expected {versions_str}")
version_num = VERSIONS.index(config.version)
# config.build_rels = False

# Apply arguments
config.build_dir = args.build_dir
config.build_dtk_path = args.build_dtk
config.compilers_path = args.compilers
config.debug = args.debug
config.generate_map = args.map
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper

# Tool versions
config.compilers_tag = "20231018"
config.dtk_tag = "v0.7.5"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("orig") / f"{config.version}.sha1"
config.linker_version = "Wii/1.6"
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
    "-listclosure",  # Uncomment for Wii linkers
]

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
    "-i include/MSL_C",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]
if config.debug:
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
]

# Dolphin library flags
cflags_dolphin = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-inline auto",
]

# Framework flags
cflags_framework = [
    *cflags_base,
    "-inline noauto",
    "-str reuse",
    "-ipa file",
]

# EGG flags
cflags_egg = [
    *cflags_base,
    "-inline noauto",
    "-fp_contract off",
    "-ipa file",
]

# nw4r flags
cflags_nw4r = [
    *cflags_base,
    "-ipa file",
    "-fp_contract off",
    ""
]

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
        "mw_version": "Wii/1.6",
        "cflags": cflags_rel + extra_cflags,
        "host": False,
        "objects": [
            Object(status, cpp_name),
        ],
    }


# From tww. IDK if it needs changing for Wii (probably)
# Helper function for Dolphin libraries
def DolphinLib(lib_name, objects):
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.0", # from version strings
        "cflags": cflags_dolphin,
        "host": False,
        "objects": objects,
    }


def EGGLib(lib_name, objects):
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.6",
        "cflags": cflags_egg,
        "host": False,
        "objects": objects,
    }


def nw4rLib(lib_name, objects, extra_cflags=[]):
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.1", # most seem to be around 1.2, snd is 1.6
        "cflags": cflags_nw4r + extra_cflags,
        "host": False,
        "objects": objects,
    }


Matching = True
NonMatching = False

config.warn_missing_config = False
config.warn_missing_source = False  # TODO
config.libs = [
    {
        "lib": "framework",
        "mw_version": "Wii/1.6",
        "cflags": cflags_framework,
        "host": False,
        "objects": [
            # machine (m_name
            Object(NonMatching, "Runtime/__init_cpp_exceptions.cpp"),
            Object(Matching, "toBeSorted/unk_flag_stuff.cpp"),
            Object(Matching, "toBeSorted/bitwise_flag_helper.cpp"),
            Object(Matching, "toBeSorted/sceneflag_manager.cpp"),
            Object(NonMatching, "toBeSorted/flag_space.cpp"),
            Object(NonMatching, "toBeSorted/misc_flag_managers.cpp"),
            Object(Matching, "toBeSorted/dungeonflag_manager.cpp"),
            Object(Matching, "toBeSorted/skipflag_manager.cpp"),
            Object(Matching, "c/c_list.cpp"),
            Object(Matching, "c/c_tree.cpp"),
            Object(Matching, "d/d_base.cpp"),
            Object(NonMatching, "d/d_heap.cpp"),
            Object(NonMatching, "d/d_stage.cpp"),
            Object(NonMatching, "d/d_sys.cpp"),
            Object(NonMatching, "d/a/d_a_base.cpp"),
            Object(NonMatching, "d/a/obj/d_a_obj_base.cpp"),
            Object(Matching, "toBeSorted/save_file.cpp"),
            Object(Matching, "toBeSorted/counters/counter.cpp"),
            Object(Matching, "toBeSorted/counters/rupee_counter.cpp"),
            Object(Matching, "toBeSorted/counters/arrow_counter.cpp"),
            Object(Matching, "toBeSorted/counters/bomb_counter.cpp"),
            Object(Matching, "toBeSorted/counters/tear_counter.cpp"),
            Object(Matching, "toBeSorted/counters/slingshot_seed_counter.cpp"),
            Object(Matching, "toBeSorted/counters/key_piece_counter.cpp"),
            Object(Matching, "toBeSorted/counters/extra_wallet_counter.cpp"),
            Object(NonMatching, "toBeSorted/file_manager.cpp"),
            Object(NonMatching, "toBeSorted/save_manager.cpp"),
            Object(NonMatching, "f/f_base.cpp"),
            Object(Matching, "f/f_list.cpp"),
            Object(Matching, "f/f_manager.cpp"),
            Object(Matching, "m/m_angle.cpp"),
            Object(Matching, "m/m_heap.cpp"),
            Object(NonMatching, "m/m_mtx.cpp"),
            # framework (f_name)
            # d stuff (d_name)
        ],
    },
    # DolphinLib(
    #     "name",
    #     [ ],
    # ),
    # {
    #     "lib": "Runtime.PPCEABI.H",
    #     "mw_version": "Wii/1.7", #TODO: change
    #     "cflags": cflags_runtime,
    #     "host": False,
    #     "objects": [ ],
    # },
    # NW4R
    nw4rLib(
        "db",
        [
            Object(Matching, "nw4r/db/db_directPrint.cpp"),
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
            Object(Matching, "nw4r/lyt/lyt_resourceAccessor.cpp"),
            Object(Matching, "nw4r/lyt/lyt_arcResourceAccessor.cpp"),
            Object(Matching, "nw4r/lyt/lyt_common.cpp"),
        ], [""]
    ),
    # EGG
    EGGLib(
        "core",
        [
            Object(NonMatching, "egg/core/eggArchive.cpp"),
            Object(NonMatching, "egg/core/eggDvdFile.cpp"),
            Object(NonMatching, "egg/core/eggDvdRipper.cpp"),
            Object(NonMatching, "egg/core/eggStream.cpp"),
            Object(Matching, "egg/core/eggAllocator.cpp"),
            Object(NonMatching, "egg/core/eggHeap.cpp"),
            Object(Matching, "egg/core/eggExpHeap.cpp"),
            Object(NonMatching, "egg/core/eggFrmHeap.cpp"),
            Object(NonMatching, "egg/core/eggAssertHeap.cpp"),
            Object(Matching, "egg/core/eggDisposer.cpp"),
            Object(Matching, "egg/core/eggThread.cpp"),
            Object(NonMatching, "egg/core/eggUnk.cpp"),
            Object(NonMatching, "egg/core/eggSystem.cpp"),
            Object(Matching, "egg/core/eggDisplay.cpp"),
            Object(Matching, "egg/core/eggColorFader.cpp"),
            Object(Matching, "egg/core/eggAsyncDisplay.cpp"),
            Object(Matching, "egg/core/eggXfb.cpp"),
            Object(Matching, "egg/core/eggVideo.cpp"),
            Object(Matching, "egg/core/eggXfb.cpp"),
            Object(Matching, "egg/core/eggXfbManager.cpp"),
            Object(Matching, "egg/core/eggGraphicsFifo.cpp"),
            Object(NonMatching, "egg/core/eggController.cpp"),
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
    # {
    #     "lib": "REL",
    #     "mw_version": "GC/1.3.2",
    #     "cflags": cflags_rel,
    #     "host": False,
    #     "objects": [
    #         Object(Matching, "REL/executor.c"),
    #         Object(
    #             Matching,
    #             "REL/global_destructor_chain.c",
    #             source="Runtime.PPCEABI.H/global_destructor_chain.c",
    #         ),
    #     ],
    # },
    # Rel(Matching/NonMatching, "rel_name", "rel_folder/rel_name.cpp"),
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
