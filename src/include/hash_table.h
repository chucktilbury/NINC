#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdlib.h>

typedef enum {
    HASH_OK,
    HASH_DUP,
    HASH_NF,
    HASH_ERROR,
} HashTabResult;

typedef struct {
    const char* key;
    void* data;
    size_t size;
} HashNode;


/*
 * If a node's key is NULL, but the bucket pointer in the table
 * is not NULL, then the bucket is a tombstone.
 */
struct _hash_tab_ {
    HashNode** table;
    int cap;
    int count;
    int tombstones;
};

HashTab* createHashTab();
void destroyHashTab(HashTab* table);
HashTabResult insertHashTab(HashTab* table, const char* key, void* data, size_t size);
HashTabResult findHashTab(HashTab* tab, const char* key, void* data, size_t size);
HashTabResult removeHashTab(HashTab* tab, const char* key);
void dumpHashTab(HashTab* tab);

void saveHashTab(HashTab* tab, FileBuf* fp);
HashTab* loadHashTab(FileBuf* fp);

#endif /* _HASH_TABLE_H */
