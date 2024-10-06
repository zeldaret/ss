#Take symbols from the decomp's map files, treat them as authoritative, and import them in Ghidra.
#Requires cwdemangle in the PATH
#Must run `python .\configure.py --map && ninja` beforehand!
#@author robojumper
#@category GameCube/Wii
#@keybinding 
#@menupath 
#@toolbar 

import os
import re
import subprocess

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

def demangle(name):
    # try demangling
    if "__" in name:
        try:
            output = subprocess.check_output(["cwdemangle", name], stderr=subprocess.STDOUT)
            return output.strip().split("(")[0]
        except subprocess.CalledProcessError as e:
            if "Failed to demangle symbol" not in e.output:
                raise
    # otherwise we try to undo the effects of the original
    # ghidra -> symbols.txt export here
    if not "$" in name and not "arraydtor" in name and not name.startswith("__"):
        name = name.replace("__", "::")
        name = name.replace("::::", "::__")
    return name

def parse_symbol(line):
    if "entry of" in line:
        return None
    objs = line.strip().split()
    vAddr = objs[2]
    name = objs[5]

    if name.startswith("gap_") or name == "*fill*" or name.startswith(".") or "........" in vAddr:
        return None
    if default_sym_re.match(name):
        return None

    return {
        'name': name,
        'vAddr': int(vAddr, 16),
    }

default_sym_re = re.compile(".*_[0-9A-Za-z]{8}$")

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
                i += 4 # go to symbols
                while lines[i].strip() != "":
                    sym = parse_symbol(lines[i])
                    if sym is not None:
                        sections[section_name].append(sym)
                    i += 1
        i += 1

    return sections

# This script works incrementally by recording
# the mangled name in a special plate comment.
# If the mangled name is the same, we don't even bother
# shelling out to cwdemangle, speeding up the whole process
# quite a bit.
mangled_prefix = "mangled-decomp-name-v1: "
def update_addr(addr, mangled_name):
    unit = listing.getCodeUnitAt(addr)
    if not unit:
        return
    
    new_comment_line = mangled_prefix + mangled_name

    comment = unit.getComment(PLATE_COMMENT)
    update_symbol = False
    new_comment = None
    if not comment:
        # no plate comment here, add one and set symbol
        update_symbol = True
        new_comment = [new_comment_line]
    else:
        comment_lines = comment.splitlines()
        if any (line.startswith(mangled_prefix) for line in comment_lines):
            # replace with new mangled name
            new_comment = []
            for line in comment_lines:
                if line.startswith(mangled_prefix) and new_comment_line not in line:
                    update_symbol = True
                    new_comment.append(new_comment_line)
                else:
                    new_comment.append(line)

        else:
            # existing plate comment without symbol, append
            update_symbol = True
            new_comment = [comment, new_comment_line]

    if update_symbol:
        new_comment = '\n'.join(new_comment)
        demangled_name = demangle(mangled_name)

        # print(new_comment, demangled_name)

        name_list = [SymbolUtilities.replaceInvalidChars(part, True) for part in demangled_name.split("::")]
        symbol_str = name_list[-1]
        namespace = None
        if len(name_list) > 1:
            namespace_str = "::".join(name_list[:-1])
            print(mangled_name)
            namespace = NamespaceUtils.createNamespaceHierarchy(namespace_str, None, currentProgram, IMPORTED)

        sym = getSymbolAt(addr)
        if namespace is None:
            namespace = currentProgram.getGlobalNamespace()
        if sym:
            sym.setNameAndNamespace(symbol_str, namespace, IMPORTED)
        else:
            createLabel(addr, symbol_str, namespace, True, IMPORTED)
        unit.setComment(PLATE_COMMENT, new_comment)

def apply_symbols_map(symbols_map, file_name):
    for section, syms in symbols_map.items():
        for sym in syms:
            if file_name == "MAIN":
                # in the main dol, each symbol is loaded at a fixed address
                addr_obj = AF.getAddress("0x%08X" % sym["vAddr"])
            else:
                # REL sections can't be reliably identified
                if section != ".text":
                    continue
                # in rels, every section is relocated indivdually, so treat
                # this as an offset
                block_name = file_name + "_" + section + "0"
                block = mem.getBlock(block_name)
                addr_obj = block.getStart().add(sym["vAddr"])
            update_addr(addr_obj, sym["name"])


path = str(askDirectory("Program build directory (e.g. build/SOUE01)", "Import"))

new_contents = None
main_symbols = os.path.join(path, "main.elf.MAP")
symbols_map = None
with open(main_symbols, "rt") as file:
    symbols_map = parse_map_file(file)

apply_symbols_map(symbols_map, "MAIN")

for rel_name in os.listdir(path):
    if rel_name.endswith("NP"):
        rel_symbols = os.path.join(path, rel_name, rel_name + ".plf.MAP")
        symbols_map = None
        with open(rel_symbols, "rt") as file:
            symbols_map = parse_map_file(file)

        apply_symbols_map(symbols_map, rel_name)
