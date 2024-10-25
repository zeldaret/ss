"""
Appropriately names state manager weak functions, given a .s file with
correctly named vtables.

Call with:
    python ./tools/custom/apply_state_mgr_fns.py PATH_TO_S_FILE PATH_TO_SYMBOLS_FILE
"""

from dataclasses import dataclass
import pathlib
import re
import sys
from typing import Callable, List


@dataclass
class Vtable:
    # A vtable pattern with a match named class_name
    vtable_pat: re.Pattern
    # A factory for function names
    functions: Callable[[str], List[str | None]]


def L(items):
    """Join items and add a length prefix"""
    s = None
    if isinstance(items, list):
        s = "".join(items)
    else:
        s = items

    return str(len(s)) + s


sFStateMgr_c = Vtable(
    re.compile("__vt__[0-9]+sFStateMgr_c<(?P<class_name>[A-Za-z0-9_]+),20sStateMethodUsr_FI_c>"),
    lambda name: [
        None,
        None,
        "__dt__" + L(["sFStateMgr_c<", name, ",20sStateMethodUsr_FI_c>"]) + "Fv",
        "initializeState__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "Fv",
        "executeState__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "Fv",
        "finalizeState__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "Fv",
        "changeState__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "FRC12sStateIDIf_c",
        "refreshState__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "Fv",
        "getState__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "CFv",
        "getNewStateID__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "CFv",
        "getStateID__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "CFv",
        "getOldStateID__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "CFv",
    ]
)

sStateMgr_c = Vtable(
    re.compile("__vt__[0-9]+sStateMgr_c<(?P<class_name>[A-Za-z0-9_]+),20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"),
    lambda name: [
        None,
        None,
        "__dt__" + L(["sStateMgr_c<", name, ",20sStateMethodUsr_FI_c,12sFStateFct_c,13sStateIDChk_c>"]) + "Fv",
        # the rest are dupes
    ]
)


sFStateFct_c = Vtable(
    re.compile("__vt__[0-9]+sFStateFct_c<(?P<class_name>[A-Za-z0-9_]+)>"),
    lambda name: [
        None,
        None,
        "__dt__" + L(["sFStateFct_c<", name, ">"]) + "Fv",
        "build__" + L(["sFStateFct_c<", name, ">"]) + "FRC12sStateIDIf_c",
        "dispose__" + L(["sFStateFct_c<", name, ">"]) + "FRP10sStateIf_c",
    ]
)

sFState_c = Vtable(
    re.compile("__vt__[0-9]+sFState_c<(?P<class_name>[A-Za-z0-9_]+)>"),
    lambda name: [
        None,
        None,
        "__dt__" + L(["sFState_c<", name, ">"]) + "Fv",
        "initialize__" + L(["sFState_c<", name, ">"]) + "Fv",
        "execute__" + L(["sFState_c<", name, ">"]) + "Fv",
        "finalize__" + L(["sFState_c<", name, ">"]) + "Fv",
    ]
)

sFStateID_c = Vtable(
    re.compile("__vt__[0-9]+sFStateID_c<(?P<class_name>[A-Za-z0-9_]+)>"),
    lambda name: [
        None,
        None,
        "__dt__" + L(["sFStateID_c<", name, ">"]) + "Fv",
        None,
        None,
        None,
        None,
        "isSameName__" + L(["sFStateID_c<", name, ">"]) + "CFPCc",
        None,
        None,
        "initializeState__" + L(["sFStateID_c<", name, ">"]) + "CFR" + name,
        "executeState__" + L(["sFStateID_c<", name, ">"]) + "CFR" + name,
        "finalizeState__" + L(["sFStateID_c<", name, ">"]) + "CFR" + name,
    ]
)

vtables = [
    sFStateMgr_c,
    sStateMgr_c,
    sFStateFct_c,
    sFState_c,
    sFStateID_c,
]


def parse_data(file):
    renames = []
    lines: List[str] = file.splitlines()
    i = 0
    while i < len(lines):
        if lines[i].startswith(".obj"):
            obj_name = lines[i].rsplit(",", maxsplit=1)[0][5:]
            if obj_name.startswith('"') and obj_name.endswith('"'):
                obj_name = obj_name[1:-1]
            
            for tab in vtables:
                if (match := tab.vtable_pat.match(obj_name)):
                    class_name = match.group("class_name")
                    fns = tab.functions(class_name)

                    for new_name in fns:
                        i += 1
                        if new_name is not None:
                            old_name = lines[i].split(" ")[-1]
                            if old_name.startswith('"') and old_name.endswith('"'):
                                old_name = old_name[1:-1]
                            renames.append((old_name, new_name))
                    break
        i += 1

    return renames


def main():
    s_file = sys.argv[1]
    sym_file = sys.argv[2]
    s_text = pathlib.Path(s_file).read_text()
    renames = parse_data(s_text)

    sym_text = pathlib.Path(sym_file).read_text()
    for (old, new) in renames:
        if old != new:
            sym_text = sym_text.replace(old + " = .text", new + " = .text")

    pathlib.Path(sym_file).write_text(sym_text)


if __name__ == "__main__":
    main()
