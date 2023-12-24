#ifndef NW4R_UT_LIST_H
#define NW4R_UT_LIST_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace ut {

struct List {
    void* first; // at 0x0
    void* last;  // at 0x4
    u16 size;    // at 0x8
    u16 offset;  // at 0xA
};

struct Node {
    void* prev; // at 0x0
    void* next; // at 0x4
};

void List_Init(List* list, u16 offset);
void List_Append(List* list, void* object);
void List_Prepend(List* list, void* object);
void List_Insert(List* list, void* next, void* object);
void List_Remove(List* list, void* object);
void* List_GetNext(const List* list, const void* object);
void* List_GetPrev(const List* list, const void* object);
void* List_GetNth(const List* list, u16 n);

inline void* List_GetFirst(const List* list) {
    return List_GetNext(list, NULL);
}

inline const void* List_GetFirstConst(const List* list) {
    return List_GetFirst(list);
}

inline void* List_GetLast(const List* list) { return List_GetPrev(list, NULL); }

inline const void* List_GetLastConst(const List* list) {
    return List_GetLast(list);
}

inline const void* List_GetNextConst(const List* list, const void* object) {
    return List_GetNext(list, object);
}

inline const void* List_GetNthConst(const List* list, u16 n) {
    return List_GetNth(list, n);
}

inline u16 List_GetSize(const List* list) { return list->size; }

} // namespace ut
} // namespace nw4r

#endif
