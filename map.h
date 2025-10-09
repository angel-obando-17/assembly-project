#ifndef MAP_H
#define MAP_H

typedef struct Node Node;
typedef struct map map;

struct Node {
    char*   key;
    int     element;
    Node*   next;
};

struct map {
    int   n;
    Node* firstNode;
    void  (*insert)  ( map*, char*, int );
    void  (*erase)   ( map*, char* );
    int   (*find)    ( map*, char* );
    int   (*count)   ( map*, char* );
    void  (*display) ( map* );
    int   (*size)    ( map* );
    int   (*empty)   ( map* );
};

void insert    ( map* table, char* key, int element );
void erase     ( map* table, char* key, int element );
int  find      ( map* table, char* key );
int  count     ( map* table, char* key );
void display   ( map* table );
int  size      ( map* table );
int  empty     ( map* table );
map  createMap ( );

#endif