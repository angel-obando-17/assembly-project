#include <stdio.h>
#include "map.h"

void main( void ) {
    Map* map = map_create( 10 );
    printf( "The size of the map is: %d\n", map -> size( map ) );
} 