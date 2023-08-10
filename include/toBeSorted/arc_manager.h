#pragma once

#include <types.h>

// Ghidra: ArcManagerEntry
//   size: 0x40
struct ArcManagerEntry {
    /* 0x00 */ char arc_name[0x20];
    /* 0x20 */ u16  ref_count;
    /* 0x24 */ int  dvd_req_ptr;
    /* 0x28 */ int  archive_ptr;
    /* 0x2C */ int  heap_ptr_0;
    /* 0x30 */ char field_0x30[0x3C - 0x30];
    /* 0x3C */ int  heap_ptr_1;
};

// Ghidra: ArcManagerEntryTable
//   size: 0xC
class ArcMangerEntryTable{
    public:
        /* 0x0 */ ArcManagerEntry* entries;
        /* 0x4 */ u16 count;
        /* 0x8 */ u32 field_0x8;
    public:
        ArcMangerEntryTable();
};

class OarcManager {
public:
    void* getDataFromOarc(char* oarcName, char* fileName);

};

OarcManager g_OarcManager;
