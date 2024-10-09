"""
REL Setup sets up headers and a cpp file template for RELs.

We have about 600 actor RELs, and quite a lot of them use a heavily
templated state system with 5 vtables, 24 function instances, and another
3 actual actor functions per state. This is not difficult to set up,
but it *is* quite tedious. But even if setting up a REL takes 5 minutes,
we're looking at 50+ hours, so if I can write this script in less than 50
hours, it was worth it.
"""

from dataclasses import dataclass
import glob
import os
import pathlib
import re
import string
from typing import List

CLASS_DIR_TREE = {
    'd': {
        'a': {
            'b': {},
            'e': {},
            'npc': {},
            'obj': {},
        },
        't': {}
    }
}

def get_file_path(file_name: str, tree = CLASS_DIR_TREE):
    name_components = file_name.split('_', 1)
    if len(name_components) == 1:
        return name_components[0]
    elif (subtree := tree.get(name_components[0], None)):
        return name_components[0] + "/" + get_file_path(name_components[1], subtree)
    else:
        return ""
    


def L(items):
    """Join items and add a length prefix"""
    s = None
    if isinstance(items, list):
        s = "".join(items)
    else:
        s = items
    
    return str(len(s)) + s

def m_size(size: int):
    return lambda f: f.size == size

def m_instructions(instrs: list):
    return lambda f: all(instr in f.instructions for instr in instrs)

def simple_state_function(fn_name: str, load_02: str):
    instrs = m_instructions([f"lwzu r12, 0x18(r3)", f"lwz r12, {load_02}(r12)"])
    return ([m_size(0x10), instrs], lambda x: fn_name + L(["sStateMgr_c<", L(x), ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "FRC12sStateIDIf_c")

def state_proxy_function(fn_name: str, load_01: str):
    instrs = m_instructions(["mr r4, r3", "lwz r3, 0x8(r3)", "lwz r4, 0x4(r4)", "lwz r12, 0x0(r3)", f"lwz r12, {load_01}(r12)"])
    return ([m_size(0x1C), instrs], lambda x: fn_name + L(["sFState_c<", L(x), ">"]) + "Fv")

def state_proxy_function_c(fn_name: str, load_01: str):
    instrs = m_instructions(["mr r4, r3", "lwz r3, 0x8(r3)", "lwz r4, 0x4(r4)", "lwz r12, 0x0(r3)", f"lwz r12, {load_01}(r12)"])
    return ([m_size(0x1C), instrs], lambda x: fn_name + L(["sFState_c<", L(x), ">"]) + "CFv")

def state_invoker_function(fn_name: str, ptmf_offset: str):
    instrs = m_instructions([f"addi r12, r5, {ptmf_offset}", "bl __ptmf_scall"])
    return ([m_size(0x30), instrs], lambda x: fn_name + L(["sFStateID_c<", L(x), ">"]) + "CFR" + L(x))

FUNCTION_MATCHERS = [
    ([m_size(0x58), m_instructions(["bl __dt__10sStateIf_cFv"])], lambda x : "__dt__" + L(["sFState_c<", L(x), ">"]) + "Fv"),
    ([m_size(0x6C), m_instructions(["bl __dt__13sStateFctIf_cFv"])], lambda x : "__dt__" + L(["sFStateFct_c<", L(x), ">"]) + "Fv"),
    ([m_size(0xA0), m_instructions(["bl __dt__13sStateMgrIf_cFv"])], lambda x : "__dt__" + L(["sStateMgr_c<", L(x), ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "Fv"),
    ([m_size(0xA4), m_instructions(["bl __dt__13sStateMgrIf_cFv"])], lambda x : "__dt__" + L(["sFStateMgr_c<", L(x), ",20sStateMethodUsr_FI_c>"]) + "Fv"),
    simple_state_function("changeState__", "0x18"),
    simple_state_function("executeState__", "0x10"),
    state_proxy_function_c("getStateID__", "0x28"),
    ([m_size(0x60), m_instructions(["lwz r12, 0xc(r12)"])], lambda x : "build__" + L(["sFStateFct_c<", L(x), ">"]) + "FRC12sStateIDIf_c"),
    state_proxy_function("initialize__", "0x28"),
    state_proxy_function("execute__", "0x2c"),
    state_proxy_function("finalize__", "0x30"),
    simple_state_function("initializeState__", "0xc"),
    simple_state_function("finalizeState__", "0x14"),
    simple_state_function("refreshState__", "0x1c"),
    state_proxy_function_c("getState__", "0x20"),
    state_proxy_function_c("getNewStateID__", "0x24"),
    state_proxy_function_c("getOldStateID__", "0x2c"),
    state_invoker_function("finalizeState__", "0x24"),
    state_invoker_function("executeState__", "0x18"),
    state_invoker_function("initializeState", "0xc"),
    ([m_size(0x58), m_instructions(["bl __dt__10sStateID_cFv"])], lambda x : "__dt__" + L(["sFStateID_c<", L(x), ">"]) + "Fv"),
    ([m_size(0x88), m_instructions(["bl strrchr", "bl strcmp"])], lambda x : "isSameName__" + L(["sFStateID_c<", L(x), ">"]) + "CFPCc"),
]

# ctor call, base class, include file
BASE_CLASSES = [
    ("bl __ct__7dBase_cFv", "dBase_c", "d/d_base.h"),
    ("bl __ct__9dAcBase_cFv", "dAcBase_c", "d/a/d_a_base.h"),
    ("bl __ct__12dAcObjBase_cFv", "dAcObjBase_c", "d/a/obj/d_a_obj_base.h"),
    ("bl __ct__11dAcEnBase_cFv", "dAcEnBase_c", "d/a/en/d_a_en_base.h"),
    ("bl __ct__8dAcNpc_cFv", "dAcNpc_c", "d/a/npc/d_a_npc_base.h"),
    ("bl ActorNpcBase__ctor2", "dAcNpc_c", "d/a/npc/d_a_npc_base.h"),
    ("bl __ct__16dAcObjDoor_cFv", "dAcObjDoor_c", "d/a/obj/d_a_obj_door.h"),
]

@dataclass
class Function:
    name: str
    size: int
    instructions: list

@dataclass
class DataObj:
    name: str
    size: int
    values: list

def get_strings(block: DataObj):
    strings = []
    curr_str = ""
    for val in block.values:
        if not val.startswith(".4byte 0x"):
            if curr_str:
                strings.append(curr_str)
            curr_str = ""

            if val.startswith(".string"):
                strings.append(val[9:-1])
            continue
        num_val = val[9:]
        for i in range(0, 4):
            byte_val = int(num_val[(2*i):(2*(i+1))], 16)
            if byte_val >= ord('0') and byte_val <= ord('9') or byte_val >= ord('a') and byte_val <= ord('z') or byte_val >= ord('A') and byte_val <= ord('Z') or byte_val == ord(':') or byte_val == ord('_'):
                curr_str += chr(byte_val)
            else:
                if curr_str:
                    strings.append(curr_str)
                curr_str = ""
                continue
    return strings

def load_profiles_list():
    text = pathlib.Path('./include/f/f_profile_name.h').read_text()
    result = []
    for line in text.splitlines():
        spl = line.split(" */ ")
        if len(spl) > 1:
            result.append(spl[1][:-1])

    return result

PROFILES_LIST = load_profiles_list()

def get_profile_name(id: int):
    return PROFILES_LIST[id]

def match_class_profile(block: DataObj, class_name: str, static_ctor: str):
    """
    Returns new_ctor_function_name, new_data_name, decl_header
    """
    if not len(block.values) or not static_ctor in block.values[0]:
        return None

    if not all(v.startswith(".4byte") for v in block.values):
        return None
    
    if block.values[0].startswith(".4byte 0x"):
        return None
    
    if not all(v.startswith(".4byte 0x") for v in block.values[1:4]):
        return None

    
    prof_id = int(block.values[1][9:13], 16)
    draw_order = int(block.values[1][13:17], 16)
    properties = int(block.values[2][9:], 16)
    base_properties = int(block.values[3][9:], 16) if block.size >= 0x10 else None

    prof_name = get_profile_name(prof_id)

    new_fn_name = class_name + "_classInit__Fv"
    new_block_name = "g_profile_" + prof_name
    if base_properties is not None:
        decl = f"SPECIAL_ACTOR_PROFILE({prof_name}, {class_name}, fProfile::{prof_name}, {hex(draw_order).upper()}, {str(properties)}, {str(base_properties)});"
    else:
        decl = f"SPECIAL_BASE_PROFILE({prof_name}, {class_name}, fProfile::{prof_name}, {hex(draw_order).upper()}, {str(properties)});"

    return new_fn_name, new_block_name, decl


data_re = re.compile("# \\.data:0x([0-9A-F]+) \\| 0x([0-9A-F]+) \\| size: 0x([0-9A-F]+)")

def parse_data(file):
    blocks = []
    lines = file.splitlines()
    i = 0
    while i < len(lines):
        if (m := data_re.match(lines[i])):
            block_lines = []
            size = int(m.group(3), 16)
            i += 1
            obj_name = lines[i].split(",")[0][4:]
            i += 1
            while not lines[i].startswith(".endobj"):
                block_lines.append(lines[i].strip())
                i += 1
            
            blocks.append(DataObj(obj_name, size, block_lines))
        i += 1

    return blocks

text_re = re.compile("# \\.text:0x([0-9A-F]+) \\| 0x([0-9A-F]+) \\| size: 0x([0-9A-F]+)")

def parse_function(file):
    blocks = []
    lines = file.splitlines()
    i = 0
    while i < len(lines):
        if (m := text_re.match(lines[i])):
            block_lines = []
            size = int(m.group(3), 16)
            i += 1
            obj_name = lines[i].split(",")[0][3:]
            i += 1
            while not lines[i].startswith(".endfn"):
                if lines[i].startswith("/*"):
                    block_lines.append(lines[i].split("\t")[1].strip())
                i += 1

            blocks.append(Function(obj_name, size, block_lines))
        i += 1

    return blocks

DEFAULT_NAME_FALLBACKS = [
    ("d_a_b_", "dAcB"),
    ("d_a_e_", "dAcEn"),
    ("d_a_obj_", "dAcO"),
    ("d_a_npc_", "dAcNpc"),
    ("d_t_", "dTg"),
    ("d_a_", "dAc"),
    ("d_", "d"),
]

def build_class_name(strings, file_name):
    for s in strings:
        if "::StateID_" in s:
            return s.split("::")[0]
    
    for s in strings:
        if "::m_allocator" in s:
            return s.split("::")[0]
        
    for base, cl in DEFAULT_NAME_FALLBACKS:
        if file_name.startswith(base):
            l = len(base)
            return cl + string.capwords(file_name[l:], "_").replace("_", "") + "_c"

    return None

def find_static_ctor(fns: List[Function]):
    for fn in fns:
        for instr in fn.instructions:
            if instr == "bl __nw__7fBase_cFUl":
                has_inline_ctor = fn.size > 0x34
                return fn.name, has_inline_ctor

def find_dtor(fns: List[Function]):
    for fn in fns:
        for instr in fn.instructions:
            if instr == "bl __dl__7fBase_cFPv":
                return fn.name

def find_state_candidates(data_blocks: List[DataObj], num_states):
    for block in data_blocks:
        fns = []
        i = 0
        while i + 2 < len(block.values):
            if block.values[i] == ".4byte 0x00000000" and block.values[i+1] == ".4byte 0xFFFFFFFF":
                fn_candidate = block.values[i+2]
                if fn_candidate.startswith(".4byte") and not fn_candidate[7:].startswith("0x"):
                    fns.append(fn_candidate[7:])

            i += 1

        if len(fns) == num_states * 3:
            return fns

    return None

def find_state_names(strings):
    states = []
    for s in strings:
        spl = s.split("::StateID_")
        if len(spl) == 2:
            states.append(spl[1])

    return states

def process_file(file_name, data_blocks: List[DataObj], fns: List[Function]):
    strings = [s for b in data_blocks for s in get_strings(b)]
    name = build_class_name(strings, file_name)

    renames = []
    new_ctor_name = "__ct__" + L(name) + "Fv"
    new_dtor_name = "__dt__" + L(name) + "Fv"

    old_static_ctor_name, has_inline_ctor = find_static_ctor(fns)

    if not old_static_ctor_name:
        print(file_name, "no ctor!")
        return
    
    old_dtor_name = find_dtor(fns)
    if not old_dtor_name:
        print(file_name, "no dtor")
        return
    
    renames.append((old_dtor_name, new_dtor_name))

    for b in data_blocks:
        if (res := match_class_profile(b, name, old_static_ctor_name)):
            new_static_ctor_name, new_data_name, decl_header = res
            old_profile_data_name = b.name
            renames.append((old_static_ctor_name, new_static_ctor_name))
            renames.append((b.name, new_data_name))
            break
    else:
        print(file_name, "no profile")
        return



    for f in fns:
        for ctor, base_class_, include_ in BASE_CLASSES:
            if any(instr == ctor for instr in f.instructions):
                renames.append((f.name, new_ctor_name))
                base_class = base_class_
                include = include_

    uses_state_system = any("bl __ct__10sStateID_cFPCc" in fn.instructions for fn in fns)
    auto_functions = []
    state_decls = []
    state_defs = []
    includes = [include]
    if uses_state_system:
        state_names = find_state_names(strings)
        state_candidates = find_state_candidates(data_blocks, len(state_names))
        if state_candidates:
            includes.append("s/s_State.hpp")
            includes.append("s/s_StateMgr.hpp")

            for idx, orig in enumerate(state_candidates):
                if idx % 3 == 0:
                    method = "initialize"
                elif idx % 3 == 1:
                    method = "execute"
                elif idx % 3 == 2:
                    method = "finalize"
                
                state_name = state_names[idx // 3]

                readable_function = f"void {name}::{method}State_{state_name}" + " {}"
                mangled_function = f"{method}State_{state_name}__" + L(name) + "Fv"

                renames.append((orig, mangled_function))
                auto_functions.append(readable_function)

            for state_name in state_names:
                state_decls.append(f"STATE_FUNC_DECLARE({name}, {state_name});")
                state_defs.append(f"STATE_DEFINE({name}, {state_name});")
        else:
            print(file_name, "no matching state functions (maybe virtual?)")

    header_guard = file_name.upper() + "_H"
    file_path = get_file_path(file_name) + file_name
    with open(os.path.join('./include', file_path + '.h'), 'w') as f:
        f.write(f"#ifndef {header_guard}\n")
        f.write(f"#define {header_guard}\n\n")

        for i in includes:
            f.write(f"#include <{i}>\n")
        f.write("\n")

        f.write(f"class {name} : public {base_class} {{ \n")
        f.write(f"public:\n")

        if has_inline_ctor:
            f.write(f"\t{name}()")
            if uses_state_system:
                f.write(" : mStateMgr(*this, sStateID::null)")
            f.write(" {}\n")
            f.write(f"\tvirtual ~{name}() {{}}\n\n")
        else:
            f.write(f"\t{name}();\n")
            f.write(f"\tvirtual ~{name}();\n\n")

        if state_decls:
            for s in state_decls:
                f.write(f"\t{s}\n")
            f.write("\n")

        f.write("private:\n")

        if uses_state_system:
            f.write(f"\t/* 0x??? */ STATE_MGR_DECLARE({name});\n")

        f.write("};\n")
        f.write("#endif")

    with open(os.path.join('./src/REL/', file_path + '.cpp'), 'w+') as f:
        f.write(f"#include <{file_path}.h>\n\n")

        if state_defs:
            for s in state_defs:
                f.write(f"{s}\n")
            f.write("\n")

        if not has_inline_ctor:
            f.write(f"{name}::{name}()")
            if uses_state_system:
                f.write(" : mStateMgr(*this, sStateID::null)")
            f.write(" {}\n")
            f.write(f"virtual {name}::~{name}() {{}}\n\n")

    if uses_state_system:
        raise ValueError("test")

def main():
    for folder in os.listdir('./build/SOUE01'):
        if folder.startswith('d_'):
            text_s_files = glob.glob(f'./build/SOUE01/{folder}/asm/REL/**/*.s', recursive=True)
            fns = []
            for f in text_s_files:
                text = pathlib.Path(f).read_text()
                fns.extend(parse_function(text))

            data_s_files = glob.glob(f'./build/SOUE01/{folder}/asm/*_data.s')
            datas = []
            for f in data_s_files:
                text = pathlib.Path(f).read_text()
                datas.extend(parse_data(text))
        
            if len(datas) == 0:
                continue
            # find class name
            process_file(folder[:-2], datas, fns)


if __name__ == "__main__":
    main()
