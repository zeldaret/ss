#pragma once

#include "types.h"

struct UnkFlagDefinition {
    u8 shift;
    u8 mask;
};

struct UnkFlagStuff {
    u16 definitionsCount;
    UnkFlagDefinition* definitions;
};

