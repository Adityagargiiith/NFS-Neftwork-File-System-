#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "uthash.h"


#define MAX_CACHE_SIZE 5

struct CacheEntry
{
    char *key;
    char *value;
    UT_hash_handle hh;
};

struct CacheEntry *cache = NULL;

char *find_in_cache(char *key)
{
    struct CacheEntry *entry;
    HASH_FIND_STR(cache, key, entry);
    if (entry)
    {
        // remove it (so the subsequent add will throw it on the front of the list)
        HASH_DELETE(hh, cache, entry);
        HASH_ADD_KEYPTR(hh, cache, entry->key, strlen(entry->key), entry);
        return entry->value;
    }
    return NULL;
}

void add_to_cache(char *key, char *value)
{
    struct CacheEntry *entry, *tmp_entry;
    entry = malloc(sizeof(struct CacheEntry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    HASH_ADD_KEYPTR(hh, cache, entry->key, strlen(entry->key), entry);

    // prune the cache to MAX_CACHE_SIZE
    if (HASH_COUNT(cache) > MAX_CACHE_SIZE)
    {
        HASH_ITER(hh, cache, entry, tmp_entry)
        {
            // prune the first entry (loop is based on insertion order so this deletes the oldest item)
            HASH_DELETE(hh, cache, entry);
            free(entry->key);
            free(entry->value);
            free(entry);
            break;
        }
    }
}
