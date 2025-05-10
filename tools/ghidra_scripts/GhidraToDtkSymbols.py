#Export symbols from an organically grown Ghidra repository to a decomp-toolkit symbols.txt file
#@author robojumper
#@category GameCube/Wii
#@keybinding 
#@menupath 
#@toolbar 

import os
import re

from ghidra.program.model.symbol.SourceType import *
from ghidra.program.model.symbol.Namespace import *
from ghidra.program.model.listing.CodeUnit import *

AF = currentProgram.getAddressFactory()
mem = currentProgram.getMemory()
listing = currentProgram.getListing()

sym_re = re.compile("(?:lbl|fn|FUN|DAT)(?:_[0-9]+_[a-z]+)?_[0-9A-Fa-f_]+ = \\.([a-z0-9]+):0x([0-9A-Fa-f]{8})(.*)\n")
default_sym_re = re.compile("_[0-9A-Fa-f]{8}$")
rel_default_sym_re = re.compile("lbl_[0-9]+_[a-z]+_[0-9A-Za-z]+$")

used_symbols = set()

def get_section_names(file_name, config_dir):
    splits_path = os.path.join(config_dir, "rels", file_name, "splits.txt")
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

def transformer_for_file(file_name, config_dir, fix_mistakes_mode):
    if file_name != "MAIN":
        section_names = get_section_names(file_name, config_dir)
        blocks = mem.getBlocks()
        blocks = [b for b in blocks if b.getName().startswith(file_name)]

    def transformer(section, address):
        if file_name == "MAIN":
            # in the main dol, each symbol is loaded at a fixed address
            addr_obj = AF.getAddress("0x%08X" % address)
        else:
            index = section_names.index("." + section)
            block = blocks[index]
            addr_obj = block.getStart().add(address)

        symbol = getSymbolAt(addr_obj)
        if symbol: #and symbol.getAddress().equals(addr_obj):
            name = symbol.getName(True)
            if fix_mistakes_mode and rel_default_sym_re.match(name):
                unit = listing.getCodeUnitAt(addr_obj)
                if unit:
                    comment_str = unit.getComment(PLATE_COMMENT)
                    if comment_str and comment_str.startswith("original-name"):
                        name = comment_str.split('\n')[0][len("original-name "):]

            if default_sym_re.search(name) or name.startswith("@"):
                return None

            # in our Ghidra the RVL symbols are helpfully namespaced...
            bad_prefixes = [
                "ai::",
                "arc::",
                "axfx::"
                "dvd::",
                "exi::",
                "gx::",
                "kpad::",
                "mem::",
                "mtx::",
                "nand::",
                "os::",
                "sc::",
                "si::",
                "tpl::",
                "vi::",
                "wenc::",
                "wpad::",
                "wud::",
            ]
            lowercase_name = name.lower()
            for prefix in bad_prefixes:
                if lowercase_name.startswith(prefix):
                    name = name[len(prefix):]
                    break
            name = name.replace("::", "__")
            name = name.replace(":", "__")
            name = name.replace("?", "Maybe")
            name = name.replace("[", "_")
            name = name.replace("]", "_")
            name = name.replace("~", "_")

            if name in used_symbols:
                i = 2
                while name + str(i) in used_symbols:
                    i += 1
                name = name + str(i)
            return name


    return transformer

def transform_symbols_file(file, transformer):
    new_file = ""
    for line in file:
        match = sym_re.match(line)
        if match:
            #       whole match     section         address         metadata
            # print(match.group(0), match.group(1), match.group(2), match.group(3))
            section = match.group(1)
            addr_str = match.group(2)
            metadata = match.group(3)
            new_name = transformer(section, int(addr_str, 16))
            if new_name:
                used_symbols.add(new_name)
                new_file += new_name + " = ." + section + ":0x" + match.group(2) + metadata + "\n"
                continue

            used_symbols.add(line.split("=")[0][:-1])

        new_file += line

    return new_file

path = str(askDirectory("Program config directory (e.g. config/SOUE01)", "Export"))
# we accidentally "lost" some symbols during our symbols migration
fix_mistakes_mode = askChoice(
    "Fix accidental symbol loss mistake",
    "Select no if you don't know what this is",
    ["Yes", "No"],
    "No"
) == "Yes"

new_contents = None
main_symbols = os.path.join(path, "symbols.txt")
with open(main_symbols, "rt") as file:
    new_contents = transform_symbols_file(file, transformer_for_file("MAIN", path, fix_mistakes_mode))

with open(main_symbols, 'w') as f:
    f.write(new_contents)


rels_dir = os.path.join(path, "rels")
for rel_name in os.listdir(rels_dir):
    if rel_name.endswith("NP"):
        rel_symbols = os.path.join(rels_dir, rel_name, "symbols.txt")
        with open(rel_symbols, "rt") as file:
            new_contents = transform_symbols_file(file, transformer_for_file(rel_name, path, fix_mistakes_mode))

        with open(rel_symbols, 'w') as f:
            f.write(new_contents)
