The state system in this library was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
with differences/modifications outlined below:

## No inline destructors

The NSMBW code uses inlined destructors. In SS, these destructors are all part of the s_StateID.cpp(?) TU,
and pretty much every generic instantiation of the state manager (which for a large part are in RELs)
will call back into those destructors in the main DOL. As a result, the destructors were made not inline
and explicitly instantiated in s_StateID.cpp

## s_StateIDChk.hpp

In order for vtable ordering to match, some classes had to be extracted to a new header.

## sStateMethodIf_c

S_StateMethod_c's destructors calls back into another destructors. sStateMethodIf_c follows the example of
the other abstract interface classes and provides this dtor.

## operator== / != return BOOL instead of bool

We're observing a lot of word-to-bool casts in code after these
operators are invoked, and while there are ways to force the conversion,
this seems the most reasonable.

## s_FStateVirtualID

Implemented by SS based on NSMBW symbols. Probably missing some parts, sync with NSMBW if they get around to it.

## s_FPhase

SS-only. Symbols/names completely made up.

## s_Assert, s_Print

SS-only. Symbols/names completely made up.
