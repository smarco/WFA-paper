/*
 *  Wavefront Alignments Algorithms
 *  Copyright (c) 2017 by Santiago Marco-Sola  <santiagomsola@gmail.com>
 *
 *  This file is part of Wavefront Alignments Algorithms.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * PROJECT: Wavefront Alignments Algorithms
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Simple managed-memory allocator that reduces the overhead
 *   of using malloc/calloc/free functions by allocating slabs of memory
 *   and dispatching memory segments in order.
 */

#ifndef MM_ALLOCATOR_H_
#define MM_ALLOCATOR_H_

#include "utils/commons.h"
#include "utils/vector.h"

/*
 * Debug
 */
//#define MM_ALLOCATOR_LOG

/*
 * MM-Allocator State
 */
typedef struct {
  /* Allocator Memory */
  uint64_t segment_idx;         // Current segment
  uint64_t num_requests;        // Total memory requests
  uint64_t offset_available;    // Offset to memory available
  /* Malloc Memory */
  uint64_t num_malloc_requests; // Total malloc requests performed
  /* Log */
#ifdef MM_ALLOCATOR_LOG
  char* func_name;
  uint64_t line_no;
#endif
} mm_allocator_state_t;

/*
 * MM-Allocator
 */
typedef struct {
  /* ID */
  uint64_t segment_id;
  /* Memory */
  void* memory_base;            // Pointer to memory
  uint64_t segment_size;        // Total memory available
  uint64_t offset_available;    // Offset to memory available
  /* Requests */
  vector_t* mem_requests;       // Memory requests (mm_allocator_request_t)
  uint64_t request_freed_idx;   // Index of last request freed
  /* Slab-Unit */
  void* mm_slab;                // Slab-Unit
} mm_allocator_segment_t;

/*
 * MM-Allocator
 */
typedef struct {
  /* Allocator Memory */
  uint64_t allocator_id;        // Allocator ID
  uint64_t mm_slab_size;        // Memory slabs size (bytes)
  vector_t* segments_index;     // Sorted Memory segments (mm_allocator_segment_t*)
  vector_t* segments_cbuffer;   // Circular-Buffer of Memory segments (mm_allocator_segment_t*)
  uint64_t segment_idx;         // Current segment being used
  /* Malloc Memory */
  vector_t* malloc_requests;    // Malloc requests (void*)
  /* Allocator States */
  vector_t* states;             // Vector of states (mm_allocator_state_t)
} mm_allocator_t;

/*
 * MM-Allocator Reference (for fast deallocation)
 */
typedef struct {
  uint32_t segment_id;
  uint32_t request_offset;
} mm_allocator_reference_t;

/*
 * Setup
 */
mm_allocator_t* mm_allocator_new(const uint64_t mm_slab_size);
void mm_allocator_clear(mm_allocator_t* const mm_allocator);
void mm_allocator_delete(mm_allocator_t* const mm_allocator);

/*
 * Utils
 */
void mm_allocator_compute_occupation(
    mm_allocator_t* const mm_allocator,
    uint64_t* const total_used,
    uint64_t* const total_free,
    uint64_t* const begin_free);

/*
 * Allocate
 */
void* mm_allocator_allocate(
    mm_allocator_t* const mm_allocator,
    uint64_t num_bytes,
    const bool zero_mem
#ifdef MM_ALLOCATOR_LOG
    ,const char* func_name,
    uint64_t line_no
#endif
    );

#ifdef MM_ALLOCATOR_LOG
#define mm_allocator_alloc(mm_allocator,type) \
  ((type*)mm_allocator_allocate(mm_allocator,sizeof(type),false,__func__,(uint64_t)__LINE__))
#define mm_allocator_malloc(mm_allocator,num_bytes) \
  (mm_allocator_allocate(mm_allocator,(num_bytes),false,__func__,(uint64_t)__LINE__))
#define mm_allocator_calloc(mm_allocator,num_elements,type,clear_mem) \
  ((type*)mm_allocator_allocate(mm_allocator,(num_elements)*sizeof(type),clear_mem,__func__,(uint64_t)__LINE__))
#else
#define mm_allocator_alloc(mm_allocator,type) \
  ((type*)mm_allocator_allocate(mm_allocator,sizeof(type),false))
#define mm_allocator_malloc(mm_allocator,num_bytes) \
  (mm_allocator_allocate(mm_allocator,(num_bytes),false))
#define mm_allocator_calloc(mm_allocator,num_elements,type,clear_mem) \
  ((type*)mm_allocator_allocate(mm_allocator,(num_elements)*sizeof(type),clear_mem))
#endif

#define mm_allocator_uint64(mm_allocator) mm_allocator_malloc(mm_allocator,sizeof(uint64_t))
#define mm_allocator_uint32(mm_allocator) mm_allocator_malloc(mm_allocator,sizeof(uint32_t))
#define mm_allocator_uint16(mm_allocator) mm_allocator_malloc(mm_allocator,sizeof(uint16_t))
#define mm_allocator_uint8(mm_allocator)  mm_allocator_malloc(mm_allocator,sizeof(uint8_t))

/*
 * Free
 */
void mm_allocator_free(
    mm_allocator_t* const mm_allocator,
    void* const memory);

/*
 * State Push/Pop
 */
void mm_allocator_push_memory_state(
    mm_allocator_t* const mm_allocator
#ifdef MM_ALLOCATOR_LOG
    ,const char* func_name,
    uint64_t line_no
#endif
    );
void mm_allocator_pop_memory_state(
    mm_allocator_t* const mm_allocator);

#ifdef MM_ALLOCATOR_LOG
#define mm_allocator_push_state(mm_allocator) mm_allocator_push_memory_state(mm_allocator,__func__,(uint64_t)__LINE__)
#else
#define mm_allocator_push_state(mm_allocator) mm_allocator_push_memory_state(mm_allocator)
#endif
#define mm_allocator_pop_state(mm_allocator) mm_allocator_pop_memory_state(mm_allocator)

/*
 * Display
 */
void mm_allocator_print(
    FILE* const stream,
    mm_allocator_t* const mm_allocator,
    const bool display_requests);

#endif /* MM_ALLOCATOR_H_ */
