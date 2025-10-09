#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node Node;
typedef struct List List;

struct Node {
    void* item;
    Node* next;
};

struct List {
    int n;
    Node* firstNode;
    void* (*find)     ( List*, void*, int (*cmp_f) ( void*, void* ) );
    void  (*for_each) ( List*, void (*callback) ( void* ) );
    void* (*insert)   ( List*, int, void* );
    void* (*remove)   ( List*, int );
    void* (*get)      ( List*, int );
    void  (*display)  ( List* );
    int   (*size)     ( List* );
    void  (*clear)    ( List* );
    int   (*empty)    ( List* );
};


void* list_find     ( List* list, void* item, int (*cmp_f) ( void*, void* )  );
void  list_for_each ( List* list, void (*callback) ( void* ) );
void* list_insert   ( List* list, int index, void* item );
void* list_remove   ( List* list, int index );
void* list_get      ( List* list, int index );
void  list_display  ( List* list );
int   list_size     ( List* list );
void  list_clear    ( List* list );
int   list_empty    ( List* list );
List* list_create   ( );

void  destroy_list  ( List* list );

#endif