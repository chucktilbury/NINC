#ifndef _PTRLST_H_
#define _PTRLST_H_

typedef enum {
    PTRLST_OK,
    PTRLST_ERROR,
    PTRLST_EMPTY,
} PtrLstResult;

typedef struct {
    void** list;
    int cap;
    int len;
    int idx;
} PtrLst;

PtrLst* createPtrLst();
PtrLstResult destroyPtrLst(PtrLst* lst);
PtrLstResult appendPtrLst(PtrLst* lst, void* data);
void* getPtrLst(PtrLst* lst, int idx);
void* resetPtrLst(PtrLst* lst);
void* iteratePtrLst(PtrLst* lst);
PtrLstResult pushPtrLst(PtrLst* lst, void* data);
void* popPtrLst(PtrLst* lst);
void* peekPtrLst(PtrLst* lst);

#endif
