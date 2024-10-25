#Take symbols from the decomp's map files, treat them as authoritative, and import them in Ghidra.
#Must run `python .\configure.py --map && ninja` beforehand!
#@author robojumper
#@category GameCube/Wii
#@keybinding 
#@menupath 
#@toolbar 

import os
import re

import demangle
demangle.mode = 'demangle'

from ghidra.app.util import NamespaceUtils
from ghidra.program.model.symbol import SymbolUtilities
from ghidra.program.model.symbol.SourceType import *
from ghidra.program.model.symbol.Namespace import *
from ghidra.program.model.listing.CodeUnit import *

AF = currentProgram.getAddressFactory()
mem = currentProgram.getMemory()
listing = currentProgram.getListing()

allowed_sections = [
    '.text',
    '.data',
    '.sdata',
    '.sdata2',
    '.bss',
    '.sbss',
    '.sbss2',
    '.rodata',
]

commit = None


def postprocess_demangled_name(demangled):
    if demangled.startswith("vtable for "):
        demangled = demangled[len("vtable for "):] + "::__vtable"

    if demangled.endswith(" const"):
        demangled = demangled[:-len("const ")]

    thunk = False
    guard = False
    if demangled.startswith("non-virtual thunk to "):
        thunk = True
        demangled = demangled[len("non-virtual thunk to "):]

    if demangled.startswith("virtual thunk to "):
        thunk = True
        demangled = demangled[len("virtual thunk to "):]

    if demangled.startswith("guard variable for "):
        guard = True
        demangled = demangled[len("guard variable for "):]

    template_open = demangled.index("<") if "<" in demangled else None
    first_space = demangled.index(" ") if " " in demangled else None
    open_paren = demangled.index("(") if "(" in demangled else None
    if template_open and first_space and open_paren and first_space < template_open and template_open < open_paren:
        # this looks like a templated return type, so drop the return type
        demangled = demangled[(first_space+1):]

    demangled = demangled.replace("(anonymous namespace)", "anonymous")
    demangled = demangled.replace("operator ", "operator_")
    demangled = demangled.replace(" ", "")
    if ")::" in demangled:
        # dFontMng_c::getFontPath(unsigned char)::TEMP_FONT_NAME
        left = demangled.split("(")[0]
        right = demangled.rsplit(")")[-1]
        # dFontMng_c::getFontPath::TEMP_FONT_NAME
        demangled = left + right
    else:
        demangled = demangled.split("(")[0]

    if thunk:
        demangled += "_thunk"
    if guard:
        demangled += "_guard"

    return demangled


def do_demangle(name):
    # try demangling
    if "__" in name:
        try:
            output = demangle.demangle_try(name)
            return output.strip()
        except Exception:
            pass
    # otherwise we try to undo the effects of the original
    # ghidra -> symbols.txt export here
    if "$" not in name and "arraydtor" not in name and not name.startswith("__"):
        name = name.replace("__", "::")
        name = name.replace("::::", "::__")
    return name


default_sym_re = re.compile(".*_[0-9A-Fa-f]{8}$")

def parse_symbol(line):
    if "entry of" in line:
        return None
    objs = line.strip().split()
    vAddr = objs[2]
    name = objs[5]

    if name.startswith("pad_") or name.startswith("gap_") or name == "*fill*" or name.startswith(".") or "........" in vAddr:
        return None
    if default_sym_re.match(name):
        return None

    return {
        'name': name,
        'vAddr': int(vAddr, 16),
    }


def parse_map_file(file):
    lines = [line for line in file]
    i = 0
    sections = {}
    while i < len(lines):
        line = lines[i]
        if "section layout" in line:
            section_name = line.split(' ')[0]
            if section_name in allowed_sections:
                sections[section_name] = []
                i += 4  # go to symbols
                while lines[i].strip() != "":
                    sym = parse_symbol(lines[i])
                    if sym is not None:
                        sections[section_name].append(sym)
                    i += 1
        i += 1

    return sections


anon_static_re = re.compile("^@[0-9]+$")


def symbol_needs_history(name):
    if anon_static_re.match(name) or "arraydtor" in name:
        return False

    return True


# This script works incrementally by recording
# the mangled name in a special plate comment.
# If the mangled name is the same, we don't even bother
# shelling out to cwdemangle, speeding up the whole process
# quite a bit.
mangled_prefix = "mangled-name"

# We also keep a history of the original Ghidra name and previous
# decomp names in this plate comment, for future reference
original_prefix = "original-name"
previous_prefix = "full-name"


def parse_comment(plate_comment):
    ret = {
        "other": [],
        "original": None,
        "history": [],
        "mangled": None,
    }

    if plate_comment:
        for line in plate_comment.splitlines():
            if line.startswith(mangled_prefix):
                ret["mangled"] = line.split(" ", 1)[1].strip()
            elif line.startswith(original_prefix):
                ret["original"] = line.split(" ", 1)[1].strip()
            elif line.startswith(previous_prefix):
                ret["history"].append(line.split(" ", 1)[1].strip())
            else:
                ret["other"].append(line.strip())
    return ret


def update_addr(addr, mangled_name, create_function=False):
    unit = listing.getCodeUnitAt(addr)
    if not unit:
        return

    comment_str = unit.getComment(PLATE_COMMENT)
    comment = parse_comment(comment_str)

    existing_symbol = getSymbolAt(addr)
    existing_name = existing_symbol.getName(True) if existing_symbol else None

    if comment["mangled"] and comment["mangled"] == mangled_name:
        # skip updating
        return

    if not comment["mangled"] and not comment["original"] and existing_name and not default_sym_re.match(existing_name):
        comment["original"] = existing_name

    demangled_name = do_demangle(mangled_name)
    postprocessed = postprocess_demangled_name(demangled_name)
    comment["mangled"] = mangled_name
    comment["history"].append(commit + " " + demangled_name)

    complete_plate_comment = comment["other"]
    if comment["original"]:
        complete_plate_comment.append(original_prefix + " " + comment["original"])
    for h in comment["history"]:
        complete_plate_comment.append(previous_prefix + " " + h)
    complete_plate_comment.append(mangled_prefix + " " + mangled_name)

    complete_plate_comment = "\n".join(complete_plate_comment)

    name_list = [SymbolUtilities.replaceInvalidChars(part, True) for part in postprocessed.split("::")]
    symbol_str = name_list[-1]
    namespace = None
    if len(name_list) > 1:
        namespace_str = "::".join(name_list[:-1])
        namespace = NamespaceUtils.createNamespaceHierarchy(namespace_str, None, currentProgram, IMPORTED)

    sym = getSymbolAt(addr)
    if namespace is None:
        namespace = currentProgram.getGlobalNamespace()
    if sym:
        sym.setNameAndNamespace(symbol_str, namespace, IMPORTED)
    else:
        createLabel(addr, symbol_str, namespace, True, IMPORTED)

    if create_function:
        createFunction(addr, name_list[-1])

    if symbol_needs_history(mangled_name):
        unit.setComment(PLATE_COMMENT, complete_plate_comment)


def get_section_names(file_name, build_dir):
    splits_path = build_dir.replace("build", "config")
    splits_path = os.path.join(splits_path, "rels", file_name, "splits.txt")
    section_names = []
    with open(splits_path, "rt") as file:
        for line in file:
            line = line.strip()
            if line == "":
                break
            elif line == "Sections:":
                continue
            else:
                section_names.append(line.split()[0])
    return section_names


def apply_symbols_map(symbols_map, file_name, build_dir):
    if file_name != "MAIN":
        section_names = get_section_names(file_name, build_dir)
        blocks = mem.getBlocks()
        blocks = [b for b in blocks if b.getName().startswith(file_name)]

    for section, syms in symbols_map.items():
        for sym in syms:
            if file_name == "MAIN":
                # in the main dol, each symbol is loaded at a fixed address
                addr_obj = AF.getAddress("0x%08X" % sym["vAddr"])
            else:
                index = section_names.index(section)
                block = blocks[index]
                # in rels, every section is relocated individually, so treat
                # this as an offset
                addr_obj = block.getStart().add(sym["vAddr"])
            is_text = section == ".text"
            update_addr(addr_obj, sym["name"], create_function=is_text)


path = str(askDirectory("Program build directory (e.g. build/SOUE01)", "Import"))
commit = askString("Commit hash for symbol history", "Confirm")
new_contents = None
main_symbols = os.path.join(path, "main.elf.MAP")
symbols_map = None
with open(main_symbols, "rt") as file:
    symbols_map = parse_map_file(file)

apply_symbols_map(symbols_map, "MAIN", path)

for rel_name in os.listdir(path):
    if rel_name.endswith("NP"):
        rel_symbols = os.path.join(path, rel_name, rel_name + ".plf.MAP")
        symbols_map = None
        with open(rel_symbols, "rt") as file:
            symbols_map = parse_map_file(file)

        apply_symbols_map(symbols_map, rel_name, path)
