"""
REL Sieve finds RELs that are good candidates for matching.

RELs have a bunch of dependencies on the main DOL, not all of which are
decomped or have correct headers set up yet, so we can't start matching every
REL right now. The REL sieve finds rels where dependencies *are* set up already
and suggests good candidates for matching.

The approach is that we start with the assumption that every REL *can* be matched
right now, and then when you look at a REL you may find dependencies that block
this REL (usually in terms of a function call). You then add this function label
to the list in this file and re-run the script, at which point this REL and any others
that call this function will be filtered out. You then repeat the process until
you find a REL you can somewhat easily match.

When a system in the DOL becomes available (via headers or a full-on decomp) you
can remove the corresponding function label here and more RELs may become available.
Of course, some of these RELs may have other dependencies, which you can then add etc.

The hope is that this helps us keep an overview over which RELs can be matched and
which DOL systems are good candidates for decomping.
"""

import glob
import os
import pathlib

BLOCKING_SYMBOLS = [
    ['fn_80353D70', 'UnkCollider::ctor'],
    ['fn_80353F30', 'UnkCollider::ctor2'],
    ['fn_80353FF0', 'UnkCollider::init'],
    ['fn_800C3EC0', 'ActorEventFlowManagerRelated::checkEventFinished'],
    ['fn_802E6000', 'mdlAnmChr_c::create'],
    ['fn_802EB6F0', 'm3d::mdl_c::mdl_c'],
    ['fn_802EDF30', 'm3d::smdl_c::smdl_c'],
    ['fn_800275C0', 'EffectsStruct::ctor'],
    ['fn_80027610', 'EffectsStruct::ctor'],
    ['fn_803465D0', 'ActorCollision::ctor'],
    ['fn_80341A70', 'checkCollision'],
    ['fn_80016C10', 'AnimModelWrapper::ctor'],
    ['fn_802EBBD0', 'm3d::scnLeaf_c::scnLeaf_c'],
    ['fn_8009C910', 'ActorEventRelated::ctor'],
    ['fn_802E32B0', 'm2d::FrameCtrl_c::setFrame'],
    ['fn_802F04A0', 'mFader_c::draw'],
    ['fn_801695F0', 'LytCommonTitle::ctor'],
    ['fn_80067020', 'matrixCreateFromPosRotYScale'],
    ['fn_800C43D0', 'ActorEventFlowManagerRelated *FUN_800c43d0'],
    ['fn_801BB3C0', 'isCurrentStage'],
    ['fn_8037DCC0', 'EnemySoundMgr'],
    ['fn_800A6690', 'ActorOnRail::ctor'],
    ['fn_80355080', 'getColliderManager'],
    ['fn_800975D0', 'initializeDowsingTarget'],
    ['fn_800A0680', 'getCurrentEventActor'],
    ['fn_80390FE0', 'something harp sound'],
    ['fn_801BB6F0', 'getCamera'],
    ['fn_800225F0', 'something light'],
    ['fn_80179250', 'shutter fence list'],
]

def main():
    data = {}
    for folder in os.listdir('./build/SOUE01'):
        if folder.startswith('d_'):
            data[folder] = []
            s_files = glob.glob(f'./build/SOUE01/{folder}/asm/REL/d/**/*.s', recursive=True)
            # maybe won't be true at some point?
            assert len(s_files) == 1
            text = pathlib.Path(s_files[0]).read_text()
            for [sym, comment] in BLOCKING_SYMBOLS:
                if sym in text:
                    data[folder].append(comment)
    

    output = sorted([
        [rel, deps] for rel, deps in data.items()
    ], key=lambda x: -len(x[1]))

    for [rel, deps] in output:
        print(rel + ':', ", ".join(deps) if len(deps) else "No unidentified dependencies! Go decomp this REL, or add a blocker to this script")


if __name__ == "__main__":
    main()
