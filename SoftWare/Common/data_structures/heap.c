
#include <stdlib.h>
#include "_common.h"
#include "stdint.h"

#if 1

#define user_assert_param(expr) ((void)0U)

#define TOTAL_HEAP_SIZE                    ((size_t)4096)
#define BYTE_ALIGNMENT			8
#if BYTE_ALIGNMENT == 8
	#define BYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( heap_structSize << 1 ) )

#define BITS_PER_BYTE		( ( size_t ) 8 )


static uint8_t ucHeap[ TOTAL_HEAP_SIZE ];

typedef struct block_link
{
	struct block_link *p_next_freeblock;	
	size_t xBlockSize;						
} blocklink_t;


static void insert_block_into_freeList( blocklink_t *pblockto_insert );
static void heap_init( void );

static const size_t heap_structSize	= ( sizeof( blocklink_t ) + ( ( size_t ) ( BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK );

static blocklink_t pstart, *pend = NULL;

static size_t free_bytes_remaining = 0U;
static size_t minimum_everfreebytes_remaining = 0U;

static size_t block_allocatedbit = 0;

/*-----------------------------------------------------------*/

void *heap_malloc( size_t wanted_size )
{
    blocklink_t *pxBlock, *p_previous_block, *p_new_blocklink;
    void *p_return = NULL;
	// vTaskSuspendAll();
	{
		if( pend == NULL )
		{
			heap_init();
		}
		if( ( wanted_size & block_allocatedbit ) == 0 )
		{
			if( wanted_size > 0 )
			{
				wanted_size += heap_structSize;
				if( ( wanted_size & BYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					wanted_size += ( BYTE_ALIGNMENT - ( wanted_size & BYTE_ALIGNMENT_MASK ) );
					user_assert_param( ( wanted_size & BYTE_ALIGNMENT_MASK ) == 0 );
				}
			}

			if( ( wanted_size > 0 ) && ( wanted_size <= free_bytes_remaining ) )
			{
				p_previous_block = &pstart;
				pxBlock = pstart.p_next_freeblock;
				while( ( pxBlock->xBlockSize < wanted_size ) && ( pxBlock->p_next_freeblock != NULL ) )
				{
					p_previous_block = pxBlock;
					pxBlock = pxBlock->p_next_freeblock;
				}

				if( pxBlock != pend )
				{
					p_return = ( void * ) ( ( ( uint8_t * ) p_previous_block->p_next_freeblock ) + heap_structSize );

					p_previous_block->p_next_freeblock = pxBlock->p_next_freeblock;

					if( ( pxBlock->xBlockSize - wanted_size ) > heapMINIMUM_BLOCK_SIZE )
					{
						p_new_blocklink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + wanted_size );
						user_assert_param( ( ( ( size_t ) p_new_blocklink ) & BYTE_ALIGNMENT_MASK ) == 0 );

						p_new_blocklink->xBlockSize = pxBlock->xBlockSize - wanted_size;
						pxBlock->xBlockSize = wanted_size;

						insert_block_into_freeList( p_new_blocklink );
					}

					free_bytes_remaining -= pxBlock->xBlockSize;

					if( free_bytes_remaining < minimum_everfreebytes_remaining )
					{
						minimum_everfreebytes_remaining = free_bytes_remaining;
					}
					pxBlock->xBlockSize |= block_allocatedbit;
					pxBlock->p_next_freeblock = NULL;
				}
			}
		}
	}
	// ( void ) xTaskResumeAll();

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( p_return == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			
		}
	}
	#endif

	user_assert_param( ( ( ( size_t ) p_return ) & ( size_t ) BYTE_ALIGNMENT_MASK ) == 0 );
	return p_return;
}
/*-----------------------------------------------------------*/

void heap_free( void *pv )
{
uint8_t *puc = ( uint8_t * ) pv;
blocklink_t *pxLink;

	if( pv != NULL )
	{
		puc -= heap_structSize;

		pxLink = ( void * ) puc;

		user_assert_param( ( pxLink->xBlockSize & block_allocatedbit ) != 0 );
		user_assert_param( pxLink->p_next_freeblock == NULL );

		if( ( pxLink->xBlockSize & block_allocatedbit ) != 0 )
		{
			if( pxLink->p_next_freeblock == NULL )
			{
				pxLink->xBlockSize &= ~block_allocatedbit;

				// vTaskSuspendAll();
				{
					free_bytes_remaining += pxLink->xBlockSize;
					// traceFREE( pv, pxLink->xBlockSize );
					insert_block_into_freeList( ( ( blocklink_t * ) pxLink ) );
				}
				// ( void ) xTaskResumeAll();
			}
		}
	}
}
/*-----------------------------------------------------------*/

size_t heap_get_freeheap_size( void )
{
	return free_bytes_remaining;
}
/*-----------------------------------------------------------*/

size_t heap_get_minimumever_freeheapsize( void )
{
	return minimum_everfreebytes_remaining;
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks( void )
{
}
/*-----------------------------------------------------------*/

static void heap_init( void )
{
blocklink_t *p_first_freeblock;
uint8_t *p_ucaligned_heap;
size_t uxaddress;
size_t xTotalHeapSize = TOTAL_HEAP_SIZE;

	uxaddress = ( size_t ) ucHeap;

	if( ( uxaddress & BYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxaddress += ( BYTE_ALIGNMENT - 1 );
		uxaddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxaddress - ( size_t ) ucHeap;
	}

	p_ucaligned_heap = ( uint8_t * ) uxaddress;

	pstart.p_next_freeblock = ( void * ) p_ucaligned_heap;
	pstart.xBlockSize = ( size_t ) 0;


	uxaddress = ( ( size_t ) p_ucaligned_heap ) + xTotalHeapSize;
	uxaddress -= heap_structSize;
	uxaddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
	pend = ( void * ) uxaddress;
	pend->xBlockSize = 0;
	pend->p_next_freeblock = NULL;

	p_first_freeblock = ( void * ) p_ucaligned_heap;
	p_first_freeblock->xBlockSize = uxaddress - ( size_t ) p_first_freeblock;
	p_first_freeblock->p_next_freeblock = pend;

	minimum_everfreebytes_remaining = p_first_freeblock->xBlockSize;
	free_bytes_remaining = p_first_freeblock->xBlockSize;

	block_allocatedbit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * BITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void insert_block_into_freeList( blocklink_t *pblockto_insert )
{
    blocklink_t *pxIterator;
    uint8_t *puc;

	for( pxIterator = &pstart; pxIterator->p_next_freeblock < pblockto_insert; pxIterator = pxIterator->p_next_freeblock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pblockto_insert )
	{
		pxIterator->xBlockSize += pblockto_insert->xBlockSize;
		pblockto_insert = pxIterator;
	}
	puc = ( uint8_t * ) pblockto_insert;
	if( ( puc + pblockto_insert->xBlockSize ) == ( uint8_t * ) pxIterator->p_next_freeblock )
	{
		if( pxIterator->p_next_freeblock != pend )
		{
			/* Form one big block from the two blocks. */
			pblockto_insert->xBlockSize += pxIterator->p_next_freeblock->xBlockSize;
			pblockto_insert->p_next_freeblock = pxIterator->p_next_freeblock->p_next_freeblock;
		}
		else
		{
			pblockto_insert->p_next_freeblock = pend;
		}
	}
	else
	{
		pblockto_insert->p_next_freeblock = pxIterator->p_next_freeblock;
	}
	if( pxIterator != pblockto_insert )
	{
		pxIterator->p_next_freeblock = pblockto_insert;
	}
}
#else

void *heap_malloc( size_t wanted_size )
{
    return malloc(wanted_size);
}
void heap_free( void *pv )
{
    free(pv);
}

size_t heap_get_minimumever_freeheapsize( void )
{
    return 1;
}

size_t heap_get_freeheap_size( void )
{
    return 0;
}
#endif
