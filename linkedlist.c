#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

#ifdef  DEBUG
#define DEBUG_PRINT( x ) printf x
#else
#define DEBUG_PRINT( x )
#endif

void* list_find( List* list, void* item, int (*cmp_f) ( void*, void* ) ) {
    if( list_empty( list ) ) {
        fprintf( stderr, "Error: List is Empty\n" );
        return NULL;
    }

    Node* current = list -> firstNode;
    while( current ) {
        if( cmp_f( current -> item, item ) == 0 ) {
            DEBUG_PRINT( ( "Item found ( item ptr = %p )\n", current -> item ) );
            return current -> item;
        }
        current = current -> next;
    }

    DEBUG_PRINT( ( "Item not found\n" ) );
    return NULL;
}

void list_for_each( List* list, void (*callback) ( void* ) ) {
    if( list_empty( list ) ) {
        fprintf( stderr, "Error: List is empty, nothing to iterate\n" );
        return;
    }

    Node* current = list -> firstNode;
    while( current ) {
        callback( current -> item );
        current = current -> next;
    }

    DEBUG_PRINT( ( "Finished iterating through list\n" ) );
    return;
}

void* list_insert( List* list, int index, void* item ) {
    if( index < 0 || index > list -> n ) {
        fprintf( stderr, "Error: Invalid Index\n" );
        exit( EXIT_FAILURE );
    }

    Node* newNode = (Node*) malloc( sizeof( Node ) );
    if( newNode == NULL ) {
        fprintf( stderr, "Error: Memory Allocation Failed\n" );
        exit( EXIT_FAILURE );
    }
    newNode -> item = item;
    newNode -> next = NULL;
    
    if( index == 0 ) {
        newNode -> next = list -> firstNode;
        list -> firstNode = newNode;
    } else {
        Node* aux = list -> firstNode;
        for( int i = 0; i < index - 1; i++ ) {
            aux = aux -> next;
        }
        newNode -> next = aux -> next;
        aux -> next = newNode;
    }
    list -> n++;
    DEBUG_PRINT( ("Inserted item at index %d ( size = %d )\n", index, list -> n ) );
    return newNode -> item;
}

void* list_remove( List* list, int index ) {
    if( list_empty( list ) ) {
        fprintf( stderr, "Error: List is empty\n" );
        exit( EXIT_FAILURE );
    }
    if( index < 0 || index >= list -> n ) {
        fprintf( stderr, "Error: Invalid Index\n" );
        exit( EXIT_FAILURE );
    }
    Node* current = list -> firstNode;
    void* delete_item;

    if( index == 0 ) {
        delete_item = current -> item;
        list -> firstNode = current -> next;
        free( current );
    } else {
        Node* prev = NULL;
        for( int i = 0; i < index; i++ ) {
            prev = current;
            current = current -> next;
        }
        delete_item = current -> item;
        prev -> next = current -> next;
        free( current );
    }

    list -> n--;
    DEBUG_PRINT( ( "Item removed Succesfully ( item ptr = %p )\n", delete_item ) );
    return delete_item;
}

void* list_get( List* list, int index ) {
    if( index < 0 || index >= list -> n ) {
        fprintf( stderr, "Error: Invalid Index\n" );
        exit( EXIT_FAILURE );
    }
    Node* aux = list -> firstNode;
    for( int i = 0; i < index; i++ ) {
        aux = aux -> next;
    }

    return aux -> item;
}

void list_display( List* list ) {
    printf( "List: [ " );
    Node* aux = list -> firstNode;
    while( aux ) {
        printf( "%p", aux -> item );
        aux = aux -> next;
        if( aux ) {
            printf( ", " );
        }
    }
    printf( " ]\n" );
}

int list_size( List* list ) {
    return list -> n;
}

void list_clear( List* list ) {
    Node* current = list -> firstNode;
    Node* next;
    while( current ) {
        next = current -> next;
        free( current );
        current = next;
    }
    list -> firstNode = NULL;
    list -> n = 0;

    return;
}

int list_empty( List* list ) {
    return !list -> n;
}

List* list_create( ) {
    List* list = (List*) malloc( sizeof( List ) );
    if( list == NULL ) {
        fprintf( stderr, "Error: Can't assign memory to List\n" );
        exit( EXIT_FAILURE );
    }
    list -> n         = 0;
    list -> firstNode = NULL;
    list -> find      = &list_find;
    list -> for_each  = &list_for_each;
    list -> insert    = &list_insert;
    list -> remove    = &list_remove;
    list -> get       = &list_get;
    list -> display   = &list_display;
    list -> size      = &list_size; 
    list -> clear     = &list_clear;
    list -> empty     = &list_empty;

    return list;
}

void destroy_list( List* list ) {
    if ( list == NULL ) {
        return;
    }
    list_clear( list );
    free( list );
    DEBUG_PRINT( ( "List destroyed successfully.\n" ) );

    return;
}