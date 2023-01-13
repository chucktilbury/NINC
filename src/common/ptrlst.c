
#include <assert.h>

#include "system.h"
#include "memory.h"
#include "ptrlst.h"

PtrLst* createPtrLst() {

    PtrLst* lst = _alloc_ds(PtrLst);
    lst->cap = 1 << 3;
    lst->len = 0;
    lst->idx = 0;
    lst->list = _alloc_ds_array(void*, lst->cap);

    return lst;
}

PtrLstResult destroyPtrLst(PtrLst* lst) {

    if(lst != NULL) {
        _free(lst->list);
        _free(lst);
        return PTRLST_OK;
    }

    return PTRLST_ERROR;
}

PtrLstResult appendPtrLst(PtrLst* lst, void* data) {

    assert(lst != NULL);
    if(lst->len + 1 > lst->cap) {
        lst->cap <<= 1;
        lst->list = _realloc_ds_array(lst->list, void*, lst->cap);
    }

    lst->list[lst->len] = data;
    lst->len++;

    return PTRLST_OK;
}

void* getPtrLst(PtrLst* lst, int idx) {

    assert(lst != NULL);
    void* ptr = NULL;
    if(idx < lst->len) {
        ptr = lst->list[idx];
    }

    return ptr;
}

void* resetPtrLst(PtrLst* lst) {

    assert(lst != NULL);
    lst->idx = 0;

    if(lst->len > 0)
        return lst->list[0];
    else
        return NULL;
}

void* iteratePtrLst(PtrLst* lst) {

    assert(lst != NULL);
    void* ptr = getPtrLst(lst, lst->idx);
    if(ptr) {
        lst->idx++;
        return ptr;
    }
    else
        return NULL;
}

PtrLstResult pushPtrLst(PtrLst* lst, void* data) {

    return appendPtrLst(lst, data);
}

void* popPtrLst(PtrLst* lst) {

    assert(lst != NULL);

    void* ptr = NULL;
    if(lst->len > 0) {
        lst->len--;
        ptr = lst->list[lst->len];
    }

    return ptr;
}

void* peekPtrLst(PtrLst* lst) {

    assert(lst != NULL);

    void* ptr = NULL;
    if(lst->len > 0)
        ptr = lst->list[lst->len - 1];

    return ptr;
}
