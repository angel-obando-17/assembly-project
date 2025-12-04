#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "map.h"

#ifdef  DEBUG
#define DEBUG_PRINT( x ) printf x
#else
#define DEBUG_PRINT( x )
#endif

typedef struct pair pair;

struct pair {
    item* first;
    int second;
};

/* Funcion hash muy simple. */
unsigned int map_hash( Map* map, void* key, size_t key_size ) {
    unsigned char* data = (unsigned char*) key;
    unsigned int   hash = 0;
    
    for( size_t i = 0; i < key_size; i++ ) {
        hash = ( hash * 31 ) + data[ i ];
    }
    
    return hash % map -> MAX_SIZE;
}

/* Retorna el item que pertenece a la key que se busca. */
pair* map_find_item( Map* map, void* key, size_t key_size ) {
    pair* object;
    unsigned int iter = map_hash( map, key, key_size );
    List* bucket = map -> buckets[ iter ];
    if( bucket -> empty( bucket ) ) {
        return NULL;
    } else {
        for( int i = 0; i < bucket -> size( bucket ); i++ ) {
            item* current = (item*) bucket -> get( bucket, i );
            if( current -> key_size == key_size && memcmp( current -> key, key, key_size ) == 0 ) {
                object -> first = current;
                object -> second = iter;
                return object;
            }
        }
    }

    return NULL;
}

/* Insertar un nuevo elemento [ key : value ] en el map, si el map no tiene suficiente
   espacio entonces hace resize( ). */
void map_insert( Map* map, void* key, void* value, size_t key_size, size_t value_size ) {
    if( load_factor( map ) < 0.75 ) {
        pair* current = map_find_item( map, key, key_size );
        if( (item*) current -> first != NULL ) {
            free( current -> first -> value );
            current -> first -> value = malloc( value_size );
            if( !current -> first -> value ) {
                fprintf( stderr, "Error: Can't assign memory to insert.\n" );
                exit( EXIT_FAILURE );
            }
            memcpy( current -> first -> value, value, value_size );
            current -> first -> value_size = value_size;
            return;
        }
        item* newitem = (item*) malloc( sizeof( item ) );
        if( !newitem ) {
            fprintf( stderr, "Error: Can't assign memory to New Item.\n" );
            exit( EXIT_FAILURE );
        }
        newitem -> key = malloc( key_size );
        newitem -> value = malloc( value_size );
        if( !newitem -> key || !newitem -> value ) {
            fprintf( stderr, "Error: Can't assign memory to New Item.\n" );
            free( newitem -> key );
            free( newitem -> value );
            free( newitem );
            exit( EXIT_FAILURE );
        }
        memcpy( newitem -> key, key, key_size );
        memcpy( newitem -> value, value, value_size );
        newitem -> key_size = key_size;
        newitem -> value_size = value_size;

        map -> buckets[ current -> second ] -> insert( map -> buckets[ current -> second ], map -> 
               buckets[ current -> second ] -> size( map -> buckets[ current -> second ] ), newitem );
        map -> n++;

    } else {
        unsigned int newSize = 2 * map -> MAX_SIZE; 
        map_resize( map, newSize );
        map_insert( map, key, value, key_size, value_size );
    }

    return;
}   

/* Funcion que retorna si el mapa contiene un value asociado la key [ key ]. */
unsigned int map_contains( Map* map, void* key, size_t key_size ) {
    if( map_find_item( map, key, key_size ) != NULL ) {
        return 1;
    }

    return 0;
}  

/* Aumenta el MAX_SIZE del map, solo cuando el load factor sea mayor a 0,75. */
void map_resize( Map* map, unsigned int size ) {
    unsigned int oldSize = map -> MAX_SIZE;
    map -> MAX_SIZE = size;
    List** newBuckets = (List**) malloc( map -> MAX_SIZE * sizeof( List* ) );
    if( !newBuckets ) {
        fprintf( stderr, "Error: Can't assign memory to new Buckets.\n");
        exit( EXIT_FAILURE );
    }
    for( unsigned int i = 0; i < map -> MAX_SIZE; i++ ) {
        newBuckets[ i ] = list_create( );
    }

    for( unsigned int i = 0; i < oldSize; i++ ) {
        for( int j = 0; j < map -> buckets[ i ] -> size( map -> buckets[ i ] ); j++ ) {
            item* iter = (item*) map -> buckets[ i ] -> get( map -> buckets[ i ], j );
            if( !iter ) {
                continue;
            }

            unsigned int newIndex = map_hash( map, iter -> key, iter -> key_size );
            newBuckets[ newIndex ] -> insert( newBuckets[ newIndex ], newBuckets[ newIndex ] -> size( newBuckets[ newIndex ] ), iter );
        }
        destroy_list( map -> buckets[ i ] );
    }
    free( map -> buckets );
    map -> buckets = newBuckets;

    DEBUG_PRINT( ( "Map resized successfully to %u buckets\n", size) );
    return;
}

/* Retorna el size del map. */
unsigned int map_size( Map* map ) {
    return map -> n;
}

/* Limpia el map. */
void map_clear( Map* map ) {
    for( unsigned int i = 0; i < map -> MAX_SIZE; i++ ) {
        for( int j = 0; j < map -> buckets[ i ] -> size( map -> buckets[ i ] ); j++ ) {
            item* iter = (item*) map -> buckets[ i ] -> get( map -> buckets[ i ], j );
            if( iter ) {
                free( iter -> key );
                free( iter -> value );
                free( iter );
            }
        }
        list_clear( map -> buckets[ i ] );
    }
    
    map -> n = 0;
    return;
}

/* Retorna si el map esta empty: 1 -> si, 0 -> no. */
unsigned int map_empty( Map* map ) {
    return !map -> n;
}

/* Crea un nuevo map, del tamaño size. */
Map* map_create( unsigned int size ) {
    Map* map = (Map*) malloc( sizeof( Map ) ) ;
    if( map == NULL ) {
        fprintf( stderr, "Error: Can't assign Memory to map\n" );
        exit( EXIT_FAILURE );
    }
    map -> n = 0;
    map -> MAX_SIZE = size;
    map -> buckets  = (List**) malloc( map -> MAX_SIZE * sizeof( List* ) );
    for( unsigned int i = 0; i < map -> MAX_SIZE; i++ ) {
        map -> buckets[ i ] = list_create( );
        if( map -> buckets[ i ] == NULL ) {
            fprintf( stderr, "Error: Can't assign memory to bucket in location %d = \n", i );
            destroy_map( map );
            exit( EXIT_FAILURE );
        }
    }
    map -> insert   = &map_insert;
    map -> contains = &map_contains;
    map -> erase    = &map_erase;
    map -> get      = &map_get;
    map -> resize   = &map_resize;
    map -> size     = &map_size;
    map -> clear    = &map_clear;
    map -> empty    = &map_empty;

    return map;
}

/* Retorna el load factor del map. */
float load_factor( Map* map ) {
    return (float) map -> n / (float) map -> MAX_SIZE;
}

/* Destructor del map, libera toda la memoria ocupada tanto por los buckets,
   y el propio map. */
void destroy_map( Map* map ) {
    if( map == NULL ) {
        return;
    }
    map_clear( map );
    for( unsigned int i = 0; i < map -> MAX_SIZE; i++ ) {
        destroy_list( map -> buckets[ i ] );
    }
    free( map -> buckets );
    free( map );

    DEBUG_PRINT( ( "Map destroyed succesfully\n" ) );
    return;
}