#include <nw4r/ut/ut_list.h>

namespace nw4r {
namespace ut {

#define NODE_PTR(list, object) ((Node *)(((char *)object) + list->offset))

void List_Init(List *list, u16 offset) {
    list->first = nullptr;
    list->last = nullptr;
    list->size = 0;
    list->offset = offset;
}

void List_Append(List *list, void *object) {
    if (list->first == nullptr) {
        Node *node = NODE_PTR(list, object);
        node->next = nullptr;
        node->prev = nullptr;
        list->first = object;
        list->last = object;
        list->size++;
    } else {
        Node *node = NODE_PTR(list, object);
        node->prev = list->last;
        node->next = nullptr;
        NODE_PTR(list, list->last)->next = object;
        list->last = object;
        list->size++;
    }
}

void List_Prepend(List *list, void *object) {
    if (list->first == nullptr) {
        Node *node = NODE_PTR(list, object);
        node->next = nullptr;
        node->prev = nullptr;
        list->first = object;
        list->last = object;
        list->size++;
    } else {
        Node *node = NODE_PTR(list, object);
        node->prev = nullptr;
        node->next = list->first;
        NODE_PTR(list, list->first)->prev = object;
        list->first = object;
        list->size++;
    }
}

void List_Insert(List *list, void *next, void *object) {
    if (next == nullptr) {
        List_Append(list, object);
    } else if (next == list->first) {
        List_Prepend(list, object);
    } else {
        Node *newNode = NODE_PTR(list, object);
        void *prevObj = NODE_PTR(list, next)->prev;
        Node *prevNode = NODE_PTR(list, prevObj);
        newNode->prev = prevObj;
        newNode->next = next;
        prevNode->next = object;
        NODE_PTR(list, next)->prev = object;
        list->size++;
    }
}

void List_Remove(List *list, void *object) {
    Node *node = NODE_PTR(list, object);
    if (node->prev == nullptr) {
        list->first = node->next;
    } else {
        NODE_PTR(list, node->prev)->next = node->next;
    }

    if (node->next == nullptr) {
        list->last = node->prev;
    } else {
        NODE_PTR(list, node->next)->prev = node->prev;
    }

    node->prev = nullptr;
    node->next = nullptr;
    list->size--;
}

void *List_GetNext(const List *list, const void *object) {
    if (object == nullptr) {
        return list->first;
    } else {
        return NODE_PTR(list, object)->next;
    }
}

void *List_GetPrev(const List *list, const void *object) {
    if (object == nullptr) {
        return list->last;
    } else {
        return NODE_PTR(list, object)->prev;
    }
}

void *List_GetNth(const List *list, u16 n) {
    void *object;
    int c;

    for (c = 0, object = nullptr; object = List_GetNext(list, object); c++) {
        if (n == c) {
            return object;
        }
    }

    return nullptr;
}

} // namespace ut
} // namespace nw4r
