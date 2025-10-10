#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

#ifdef  DEBUG
#define DEBUG_PRINT( x ) printf x
#else
#define DEBUG_PRINT( x )
#endif

unsigned int map_hash( Map* map, void* key, size_t key_size ) {
    unsigned char* data = (unsigned char*) key;
    unsigned int   hash = 0;
    
    for( size_t i = 0; i < key_size; i++ ) {
        hash = ( hash * 31 ) + data[ i ];
    }
    
    return hash % map -> MAX_SIZE;
}

void map_insert( Map* map, void* key, void* value, size_t key_size, size_t value_size ) {
    if( load_factor( map ) < 0.75 ) {
        unsigned int iter = map_hash( map, key, key_size );
        List* bucket = map -> buckets[ iter ];
        for( int i = 0; i < bucket -> size( bucket ); i++ ) {
            item* current = (item*) bucket -> get( bucket, i );
            if( current -> key_size == key_size && memcmp( current -> key, key, key_size ) == 0 ) {
                free( current -> value );
                current -> value = malloc( value_size );
                if( !current -> value ) {
                    fprintf( stderr, "Error: Can't assign memory to insert.\n" );
                    exit( EXIT_FAILURE );
                }
                memcpy( current -> value, value, value_size );
                current -> value_size = value_size;
                return;
            }
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

        map -> buckets[ iter ] -> insert( map -> buckets[ iter ], map -> buckets[ iter ] -> size( map -> buckets[ iter ] ), newitem );
        map -> n++;
    } else {
        unsigned int newSize = 2 * map -> MAX_SIZE; 
        map_resize( map, newSize );
        map_insert( map, key, value, key_size, value_size );
    }

    return;
}   

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

unsigned int map_size( Map* map ) {
    return map -> n;
}

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

unsigned int map_empty( Map* map ) {
    return !map -> n;
}

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

float load_factor( Map* map ) {
    return (float) map -> n / (float) map -> MAX_SIZE;
}

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