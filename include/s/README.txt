This library was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
with differences/modifications outlined below:

## No inline destructors

The NSMBW code uses inlined destructors. In SS, these destructors are all part of the s_StateID.cpp(?) TU,
and pretty much every generic instantiation of the state manager (which for a large part are in RELs)
will call back into those destructors in the main DOL.

## s_StateIDChk.hpp

In order for vtable ordering to match, some classes had to be extracted to a new header.

## sStateMethodIf_c

S_StateMethod_c's destructors calls back into another destructors. sStateMethodIf_c follows the example of
the other abstract interface classes and provides this dtor.
