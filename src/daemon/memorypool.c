/*
     This file is part of libmicrohttpd
     (C) 2007 Daniel Pittman

     libmicrohttpd is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published
     by the Free Software Foundation; either version 2, or (at your
     option) any later version.

     libmicrohttpd is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with libmicrohttpd; see the file COPYING.  If not, write to the
     Free Software Foundation, Inc., 59 Temple Place - Suite 330,
     Boston, MA 02111-1307, USA.
*/

/**
 * @file memorypool.c
 * @brief memory pool
 * @author Christian Grothoff
 */

#include "memorypool.h"

struct MemoryPool {

  /**
   * Pointer to the pool's memory
   */ 
  char * memory;

  /**
   * Size of the pool.
   */
  unsigned int size;

  /**
   * Offset of the first unallocated byte.
   */
  unsigned int pos;

  /**
   * Offset of the last unallocated byte.
   */
  unsigned int end;

  /**
   * 0 if pool was malloc'ed, 1 if mmapped.
   */
  int is_mmap;
};

/**
 * Create a memory pool.
 * 
 * @param max maximum size of the pool
 */
struct MemoryPool * MHD_pool_create(unsigned int max) {
  struct MemoryPool * pool;

  pool = malloc(sizeof(struct MemoryPool));
  if (pool == NULL)
    return NULL;
  pool->memory = MMAP(NULL, max, PROT_READ | PROT_WRITE,
		      MAP_ANONYMOUS, -1, 0);
  if ( (pool->memory == MAP_FAILED) ||
       (pool->memory == NULL) ) {
    pool->memory = malloc(max);
    if (pool->memory == NULL) {
      free(pool);
      return NULL;
    }
    pool->is_mmap = 0;
  } else {
    pool->is_mmap = 1;
  }	   
  pool->pos = 0;
  pool->end = max;
  pool->size = max;
  return pool;
}

/**
 * Destroy a memory pool.
 */
void MHD_pool_destroy(struct MemoryPool * pool) {
  if (pool == NULL)
    return;
  if (pool->is_mmap == 0)
    free(pool->memory);
  else
    MUNMAP(pool->memory, pool->size);
  free(pool);
}

/**
 * Allocate size bytes from the pool.
 * @return NULL if the pool cannot support size more
 *         bytes
 */
void * MHD_pool_allocate(struct MemoryPool * pool,
			 unsigned int size,
			 int from_end) {
  void * ret;

  if ( (pool->pos + size > pool->end) ||
       (pool->pos + size < pool->pos) )
    return NULL;
  if (from_end == MHD_YES) {
    ret = &pool->memory[pool->end - size];
    pool->end -= size;
  } else {
    ret = &pool->memory[pool->pos];
    pool->pos += size;
  }
  return ret;
}

/**
 * Reallocate a block of memory obtained from the pool.
 * This is particularly efficient when growing or
 * shrinking the block that was last (re)allocated.
 * If the given block is not the most recenlty 
 * (re)allocated block, the memory of the previous
 * allocation may be leaked until the pool is 
 * destroyed (and copying the data maybe required).
 *
 * @param old the existing block
 * @param old_size the size of the existing block
 * @param new_size the new size of the block
 * @return new address of the block, or 
 *         NULL if the pool cannot support new_size 
 *         bytes (old continues to be valid for old_size)
 */
void * MHD_pool_reallocate(struct MemoryPool * pool,
			   void * old,
			   unsigned int old_size,
			   unsigned int new_size) {  
  void * ret;

  if ( (pool->end < old_size) ||
       (pool->end < new_size) ) 
    return NULL; /* unsatisfiable or bogus request */

  if ( (pool->pos >= old_size) &&
       (&pool->memory[pool->pos - old_size] == old) ) {
    /* was the previous allocation - optimize! */
    if (pool->pos + new_size - old_size <= pool->end) {
      /* fits */
      pool->pos += new_size - old_size;
      if (new_size < old_size) /* shrinking - zero again! */
	memset(&pool->memory[pool->pos],
	       0,
	       old_size - new_size);
      return old;
    }
    /* does not fit */
    return NULL;    
  }
  if (new_size <= old_size)
    return old; /* cannot shrink, no need to move */
  if ( (pool->pos + new_size >= pool->pos) &&
       (pool->pos + new_size <= pool->end) ) {
    /* fits */
    ret = &pool->memory[pool->pos];
    memcpy(ret,
	   old,
	   old_size);
    pool->pos += new_size;
    return ret;
  }
  /* does not fit */
  return NULL;
}

/* end of memorypool.c */