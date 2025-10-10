#ifndef MAP_H
#define MAP_H

#include "linkedlist.h"

typedef struct Map Map;
typedef struct item item;

struct Map {
    unsigned int     n;
    unsigned int     MAX_SIZE;
    List**           buckets;
    void             (*insert)   ( Map*, void*, void*, size_t );
    unsigned int     (*contains) ( Map*, void*, size_t );
    void             (*erase)    ( Map*, void*, size_t );
    void*            (*get)      ( Map*, void*, size_t );
    void             (*resize)   ( Map*, unsigned int );
    unsigned int     (*size)     ( Map* );
    void             (*clear)    ( Map* );
    unsigned int     (*empty)    ( Map* );
};

struct item {
    void*   key;
    void*   value;
    size_t  key_size;
};

unsigned int map_hash( Map* map, void* key, size_t key_size );

void         map_insert   ( Map* map, void* key, void* value, size_t key_size );
unsigned int map_contains ( Map* map, void* value, size_t value_size );
void         map_erase    ( Map* map, void* key, size_t key_size );
void*        map_get      ( Map* map, void* key, size_t key_size );
void         map_resize   ( Map* map, unsigned int size );  
unsigned int map_size     ( Map* map );
void         map_clear    ( Map* map );
unsigned int map_empty    ( Map* map );
Map*         map_create   ( unsigned int size );
float        load_factor  ( Map* map );

void         destroy_map  ( Map* map );

#endif