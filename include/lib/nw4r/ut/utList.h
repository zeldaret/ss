#pragma once

// This file was ported from https://github.com/riidefi/mkw/blob/master/source/nw4r/ut/utList.hpp

#include <types.h>

// TODO: Doc file boundaries

namespace nw4r {
namespace ut {

//! Bidirectional list node
struct Node {
  void* pred;
  void* succ;
};

// Unlike modern "std::list"-like structures, list nodes are directly inherited
// by children, which saves a level of indirection.
struct List {
  void* head;
  void* tail;
  u16 count;
  u16 intrusion_offset;
};

void List_Init(List* pList, u16 intrusion_offset);
void List_Append(List* pList, void* pObj);
void List_Insert(List* pList, void* pTgt, void* pObj);
void List_Remove(List* pList, void* pObj);
void* List_GetNext(const List*, const void*);
void* List_GetPrev(const List*, const void*);
void* List_GetNth(const List* pList, unsigned short);
inline void* List_GetFirst(const List* pList) {
  return List_GetNext(pList, nullptr);
}
// Seems to be not included as a symbol. Only inlined.
// void List_Prepend(List* pList, void* pObj);

} // namespace ut
} // namespace nw4r
