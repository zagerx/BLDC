#ifndef __HEAP__H
#define __HEAP__H


#include "stdlib.h"
void *heap_malloc( size_t xSize ) ;
void heap_free( void *pv ) ;
void vPortInitialiseBlocks( void ) ;
size_t heap_get_freeheap_size( void ) ;
size_t heap_get_minimumever_freeheapsize( void ) ;

#endif