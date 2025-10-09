#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#ifdef  DEBUG
#define DEBUG_PRINT( x ) printf x
#else
#define DEBUG_PRINT( x )
#endif

void map_clear( Map* map ) {
    for( int i = 0; i < map -> MAX_SIZE; i++ ) {
        list_clear( map -> buckets[ i ] );
    }
    
    map -> n = 0;
    return;
}

unsigned int map_empty( Map* map ) {
    return !map -> n;
}

Map* map_create( unsigned int size ) {
    Map* map        = (Map*) malloc( sizeof( Map ) ) ;
    if( map == NULL ) {
        fprintf( stderr, "Error: Can't assign Memory to map\n" );
        exit( EXIT_FAILURE );
    }
    map -> n        = 0;
    map -> MAX_SIZE = size;
    map -> buckets  = (List**) malloc( map -> MAX_SIZE * sizeof( List* ) );
    for( int i = 0; i < map -> MAX_SIZE; i++ ) {
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

void destroy_map( Map* map ) {
    if( map == NULL ) {
        return;
    }
    for( int i = 0; i < map -> MAX_SIZE; i++ ) {
        destroy_list( map -> buckets[ i ] );
    }
    free( map -> buckets );
    free( map );

    DEBUG_PRINT( ( "Map destroyed succesfully\n" ) );
    return;
}