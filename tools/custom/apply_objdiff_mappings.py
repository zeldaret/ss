# Thanks KooShnoo! https://discord.com/channels/727908905392275526/1024871155804426310/1329716224971767891

"""
objdiff allows one to map symbols from the target binary to the source binary.
this script will take those symbols you have mapped, and write them to `symbols.txt`.

use case: say you have changed the signature/name of a function.
now, your source file says `foo_Ful`, but symbols.txt says `bar__Fl`.
objdiff allows you to map the `foo_Ful` in the target object to `bar__Fl` in the source object.
this script allows you to replace `foo_Ful` with `bar__Fl` in `symbols.txt`.
"""

import json
import re

game_id = "SOUE01"

with open("./objdiff.json") as f:
    objdiff_config = json.load(f)

main_obj_re = re.compile(f"^build/{game_id}/obj/")
rel_obj_re = re.compile(f"^build/{game_id}/([A-Za-z_0-9\\-]+)/obj/")


def module_for_unit(unit):
    """Finds the REL name, or MAIN for the given unit, so that the
    correct symbols.txt can be updated"""
    if main_obj_re.match(unit["target_path"]):
        return "MAIN"
    elif match := rel_obj_re.match(unit["target_path"]):
        return match.group(1)


# lol python has types. it's typython. typescript all over again
units: list = objdiff_config["units"]

symbol_mappings_per_file = {}
for unit in units:
    module = module_for_unit(unit)
    symbol_mapping = unit.get("symbol_mappings")
    if symbol_mapping is None:
        continue
    symbol_mappings_per_file.setdefault(module, {}).update(symbol_mapping)
    del unit["symbol_mappings"]

for module, symbol_mappings in symbol_mappings_per_file.items():
    symbols_file = (
        f"./config/{game_id}/symbols.txt"
        if module == "MAIN"
        else f"./config/{game_id}/rels/{module}/symbols.txt"
    )
    with open(symbols_file) as f:
        symbols = f.readlines()

    for i, line in enumerate(symbols):
        tokens = line.split()
        old_symbol = tokens[0]

        new_symbol = symbol_mappings.get(old_symbol)

        if new_symbol is None:
            continue

        tokens[0] = new_symbol
        symbols[i] = " ".join(tokens) + "\n"

    with open(symbols_file, "w") as f:
        f.writelines(symbols)

with open("./objdiff.json", "w") as f:
    json.dump(objdiff_config, f)
