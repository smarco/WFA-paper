/*
 *                             The MIT License
 *
 * Wavefront Alignments Algorithms
 * Copyright (c) 2017 by Santiago Marco-Sola  <santiagomsola@gmail.com>
 *
 * This file is part of Wavefront Alignments Algorithms.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * PROJECT: Wavefront Alignments Algorithms
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Simple managed-memory allocator that reduces the overhead
 *   of using malloc/calloc/free functions by allocating slabs of memory
 *   and dispatching memory segments in order.
 */

#include "system/mm_allocator.h"

/*
 * Debug
 */
//#define MM_ALLOCATOR_MALLOC

/*
 * Constants
 */
#define MM_ALLOCATOR_SEGMENT_INITIAL_REQUESTS   10000
#define MM_ALLOCATOR_INITIAL_SEGMENTS              10
#define MM_ALLOCATOR_INITIAL_SEGMENTS_ALLOCATED     1
#define MM_ALLOCATOR_INITIAL_MALLOC_REQUESTS       10
#define MM_ALLOCATOR_INITIAL_STATES                10

/*
 * Allocator Segments Freed Cond
 */
#define MM_ALLOCATOR_REQUEST_IS_FREE(request)  ((request)->size & UINT32_ONE_LAST_MASK)
#define MM_ALLOCATOR_REQUEST_SET_FREE(request) ((request)->size |= UINT32_ONE_LAST_MASK)
#define MM_ALLOCATOR_REQUEST_SIZE(request)     ((request)->size & ~(UINT32_ONE_LAST_MASK))

/*
 * MM-Allocator Segments
 */
typedef struct {
  /* Request */
  uint32_t offset;
  uint32_t size;
  /* Log */
#ifdef MM_ALLOCATOR_LOG
  char* func_name;
  uint64_t line_no;
#endif
} mm_allocator_request_t;

/*
 * Allocator Segments Setup
 */
void mm_allocator_segment_allocate(
    mm_allocator_segment_t* const mm_allocator_segment,
    const uint64_t segment_id,
    const uint64_t mm_slab_size) {
  // Slab
  mm_allocator_segment->segment_id = segment_id;
  mm_allocator_segment->mm_slab = malloc(mm_slab_size);
  // Memory
  mm_allocator_segment->memory_base = mm_allocator_segment->mm_slab;
  mm_allocator_segment->segment_size = mm_slab_size;
  mm_allocator_segment->offset_available = 0;
  // Requests
  mm_allocator_segment->mem_requests = vector_new(MM_ALLOCATOR_SEGMENT_INITIAL_REQUESTS,mm_allocator_request_t);
  mm_allocator_segment->request_freed_idx = 0;
}
void mm_allocator_segment_clear(
    mm_allocator_segment_t* const mm_allocator_segment) {
  mm_allocator_segment->memory_base = mm_allocator_segment->mm_slab;
  mm_allocator_segment->offset_available = 0;
  vector_clear(mm_allocator_segment->mem_requests);
  mm_allocator_segment->request_freed_idx = 0;
}
void mm_allocator_segment_free(
    mm_allocator_segment_t* const mm_allocator_segment) {
  free(mm_allocator_segment->mm_slab);
  vector_delete(mm_allocator_segment->mem_requests);
}
/*
 * Allocator Setup
 */
mm_allocator_segment_t* mm_allocator_new_segment(mm_allocator_t* const mm_allocator) {
  // Allocate new segment
  const uint64_t segment_id = vector_get_used(mm_allocator->segments_index);
  mm_allocator_segment_t* const mm_allocator_segment = malloc(sizeof(mm_allocator_segment_t));
  mm_allocator_segment_allocate(mm_allocator_segment,segment_id,mm_allocator->mm_slab_size);
  // Insert
  vector_insert(mm_allocator->segments_index,mm_allocator_segment,mm_allocator_segment_t*);
  vector_insert(mm_allocator->segments_cbuffer,mm_allocator_segment,mm_allocator_segment_t*);
  // Return
  return mm_allocator_segment;
}
mm_allocator_t* mm_allocator_new(const uint64_t mm_slab_size) {
  static int no = 0;
  // Allocate handler
  mm_allocator_t* const mm_allocator = malloc(sizeof(mm_allocator_t));
  mm_allocator->allocator_id = no++;
  mm_allocator->mm_slab_size = mm_slab_size;
  // Initialize segments
  mm_allocator->segment_idx = 0;
  mm_allocator->segments_index = vector_new(MM_ALLOCATOR_INITIAL_SEGMENTS,mm_allocator_segment_t*);
  mm_allocator->segments_cbuffer = vector_new(MM_ALLOCATOR_INITIAL_SEGMENTS,mm_allocator_segment_t*);
  // Allocate initial segments
  uint64_t i;
  for (i=0;i<MM_ALLOCATOR_INITIAL_SEGMENTS_ALLOCATED;++i) {
    mm_allocator_new_segment(mm_allocator);
  }
  // Malloc Memory
  mm_allocator->malloc_requests = vector_new(MM_ALLOCATOR_INITIAL_MALLOC_REQUESTS,void*);
  // Allocator States
  mm_allocator->states = vector_new(MM_ALLOCATOR_INITIAL_STATES,mm_allocator_state_t);
  // Return
  return mm_allocator;
}
void mm_allocator_clear(mm_allocator_t* const mm_allocator) {
  // Clear segments
  mm_allocator->segment_idx = 0;
  VECTOR_ITERATE(mm_allocator->segments_index,segment_ptr,p,mm_allocator_segment_t*) {
    mm_allocator_segment_clear(*segment_ptr);
  }
  // Clear malloc memory
  VECTOR_ITERATE(mm_allocator->malloc_requests,malloc_request,m,void*) {
    free(*malloc_request); // Free malloc requests
  }
  vector_clear(mm_allocator->malloc_requests);
  // Clear states
  vector_clear(mm_allocator->states);
}
void mm_allocator_delete(mm_allocator_t* const mm_allocator) {
  // Free segments
  VECTOR_ITERATE(mm_allocator->segments_index,segment_ptr,p,mm_allocator_segment_t*) {
    mm_allocator_segment_free(*segment_ptr);
    free(*segment_ptr);
  }
  vector_delete(mm_allocator->segments_index);
  vector_delete(mm_allocator->segments_cbuffer);
  // Free malloc memory
  VECTOR_ITERATE(mm_allocator->malloc_requests,malloc_request,m,void*) {
    free(*malloc_request); // Free remaining requests
  }
  vector_delete(mm_allocator->malloc_requests);
  // Free states
  vector_delete(mm_allocator->states);
  // Free handler
  free(mm_allocator);
}
/*
 * Accessors
 */
mm_allocator_segment_t* mm_allocator_get_segment(
    mm_allocator_t* const mm_allocator,
    const uint64_t segment_idx) {
  return *(vector_get_elm(mm_allocator->segments_cbuffer,segment_idx,mm_allocator_segment_t*));
}
uint64_t mm_allocator_get_num_segments(
    mm_allocator_t* const mm_allocator) {
  return vector_get_used(mm_allocator->segments_cbuffer);
}
/*
 * Utils
 */
void mm_allocator_compute_occupation(
    mm_allocator_t* const mm_allocator,
    uint64_t* const total_used,
    uint64_t* const total_free,
    uint64_t* const begin_free) {
  // Init
  *total_used = 0;
  *total_free = 0;
  *begin_free = 0;
  // Traverse all segments
  uint64_t segment_idx, request_idx;
  for (segment_idx=0;segment_idx<=mm_allocator->segment_idx;++segment_idx) {
    mm_allocator_segment_t* const segment = mm_allocator_get_segment(mm_allocator,segment_idx);
    const uint64_t num_requests = vector_get_used(segment->mem_requests);
    mm_allocator_request_t* request = vector_get_mem(segment->mem_requests,mm_allocator_request_t);
    for (request_idx=0;request_idx<num_requests;++request_idx,++request) {
      const uint64_t size = MM_ALLOCATOR_REQUEST_SIZE(request);
      if (MM_ALLOCATOR_REQUEST_IS_FREE(request)) {
        *total_free += size;
        if (*total_used==0) {
          *begin_free += size;
        }
      } else {
        *total_used += size;
      }
    }
  }
}
/*
 * Allocator allocate
 */
mm_allocator_segment_t* mm_allocator_allocate_reserve(
    mm_allocator_t* const mm_allocator,
    uint64_t num_bytes) {
  // Fetch current segment
  mm_allocator_segment_t* const curr_segment =
      mm_allocator_get_segment(mm_allocator,mm_allocator->segment_idx);
  // Check available segment size
  if (curr_segment->offset_available + num_bytes <= curr_segment->segment_size) {
    return curr_segment;
  }
  // Check overall segment size
  if (num_bytes <= curr_segment->segment_size) {
    ++(mm_allocator->segment_idx);
    const uint64_t num_segments = mm_allocator_get_num_segments(mm_allocator);
    if (mm_allocator->segment_idx < num_segments) {
      mm_allocator_segment_t* const next_segment =
          mm_allocator_get_segment(mm_allocator,mm_allocator->segment_idx);
      mm_allocator_segment_clear(next_segment);
      return next_segment;
    } else {
      return mm_allocator_new_segment(mm_allocator);
    }
  } else {
    return NULL; // Memory request over segment size
  }
}
void* mm_allocator_allocate(
    mm_allocator_t* const mm_allocator,
    uint64_t num_bytes,
    const bool zero_mem
#ifdef MM_ALLOCATOR_LOG
    ,const char* func_name,
    uint64_t line_no
#endif
    ) {
  // Zero check
  if (num_bytes==0) {
    fprintf(stderr,"MM-Allocator. Requested zero bytes\n");
    exit(1);
  }
  // Add mm_reference payload
  num_bytes += sizeof(mm_allocator_reference_t);
  // Ensure alignment to 16 Bytes boundaries (128bits)
  if (num_bytes%16 != 0) {
    num_bytes += 16 - (num_bytes%16); // Serve more bytes
  }
  // Fetch segment
#ifdef MM_ALLOCATOR_MALLOC
  mm_allocator_segment_t* const segment = NULL; // Force malloc memory
#else
  mm_allocator_segment_t* const segment = mm_allocator_allocate_reserve(mm_allocator,num_bytes);
#endif
  if (segment != NULL) {
    // Serve Allocator Memory
    void* const memory = segment->memory_base + segment->offset_available;
    if (zero_mem) memset(memory,0,num_bytes); // Set zero
    // Set reference
    mm_allocator_reference_t* const mm_reference = memory;
    mm_reference->segment_id = segment->segment_id;
    mm_reference->request_offset = vector_get_used(segment->mem_requests);
    // Add request to segment
    mm_allocator_request_t* request;
    vector_alloc_new(segment->mem_requests,mm_allocator_request_t,request);
    request->offset = segment->offset_available;
    request->size = num_bytes;
#ifdef MM_ALLOCATOR_LOG
    request->func_name = (char*)func_name;
    request->line_no = line_no;
#endif
    segment->offset_available += num_bytes;
    // Return
    return memory + sizeof(mm_allocator_reference_t);
  } else {
    // Serve Malloc Memory
    void* const memory = malloc(num_bytes);
    if (zero_mem) memset(memory,0,num_bytes); // Set zero
    vector_insert(mm_allocator->malloc_requests,memory,void*);
    // Set reference
    mm_allocator_reference_t* const mm_reference = memory;
    mm_reference->segment_id = UINT32_MAX;
    // Return
    return memory + sizeof(mm_allocator_reference_t);
  }
}
/*
 * Allocator Free
 */
bool mm_allocator_free_recycle_segment(mm_allocator_t* const mm_allocator) {
  // Fetch first segment & clean
  mm_allocator_segment_t** const segments =
      vector_get_mem(mm_allocator->segments_cbuffer,mm_allocator_segment_t*);
  mm_allocator_segment_t* const first_segment = segments[0];
  mm_allocator_segment_clear(first_segment);
  // Check segment
  const uint64_t num_segments = mm_allocator_get_num_segments(mm_allocator);
  if (mm_allocator->segment_idx==0) return false; // No need to recycle
  // Shift segments
  uint64_t i;
  for (i=0;i<num_segments-1;++i) {
    segments[i] = segments[i+1];
  }
  segments[num_segments-1] = first_segment;
  --(mm_allocator->segment_idx);
  return true; // Recycling done
}
bool mm_allocator_free_contiguous_segment(mm_allocator_segment_t* const segment) {
  // Continue to freed next requests in segment
  const uint64_t total_requests = vector_get_used(segment->mem_requests);
  mm_allocator_request_t* request =
      vector_get_mem(segment->mem_requests,mm_allocator_request_t) +
      segment->request_freed_idx;
  while (segment->request_freed_idx < total_requests) {
    if (!MM_ALLOCATOR_REQUEST_IS_FREE(request)) return false; // Request(s) pending
    ++(segment->request_freed_idx);
    ++request;
  }
  return true; // Segment fully freed
}
void mm_allocator_free_contiguous_memory(mm_allocator_t* const mm_allocator) {
  mm_allocator_segment_t* segment = mm_allocator_get_segment(mm_allocator,0);
  while (mm_allocator_free_contiguous_segment(segment)) {
    // Recycle free segment (first one)
    if (!mm_allocator_free_recycle_segment(mm_allocator)) break;
    // Fetch next segment
    segment = mm_allocator_get_segment(mm_allocator,0);
  }
}
void mm_allocator_free_malloc(
    mm_allocator_t* const mm_allocator,
    void* memory) {
  // Search for address in malloc requests
  const uint64_t num_malloc_requests = vector_get_used(mm_allocator->malloc_requests);
  void** const malloc_requests = vector_get_mem(mm_allocator->malloc_requests,void*);
  uint64_t i, j;
  for (i=0;i<num_malloc_requests;++i) {
    if (malloc_requests[i]==memory) {
      // Free memory
      free(memory);
      // Shift requests
      for (;i<num_malloc_requests-1;++i) {
        malloc_requests[i] = malloc_requests[i+1];
      }
      vector_dec_used(mm_allocator->malloc_requests);
      // Shift stacked states
      const uint64_t num_states = vector_get_used(mm_allocator->states);
      if (num_states > 0) {
        mm_allocator_state_t* const state =
            vector_get_mem(mm_allocator->states,mm_allocator_state_t);
        for (j=0;j<num_states;++j) {
          if (i < state[j].num_malloc_requests) {
            --(state[j].num_malloc_requests);
          }
        }
      }
      // Ok
      return;
    }
  }
  // Address not found. Raise error
  fprintf(stderr,"MM-Allocator. Invalid address freed (request not found)\n");
  exit(1);
}
void mm_allocator_free(
    mm_allocator_t* const mm_allocator,
    void* const memory) {
  // Fetch mm-reference
  void* const effective_memory = memory - sizeof(mm_allocator_reference_t);
  mm_allocator_reference_t* const mm_reference = effective_memory;
  // Malloc Memory
  if (mm_reference->segment_id==UINT32_MAX) {
    mm_allocator_free_malloc(mm_allocator,effective_memory);
    return;
  }
  // Allocator Memory. Fetch request
  mm_allocator_segment_t* const segment = *(vector_get_elm(
      mm_allocator->segments_index,mm_reference->segment_id,mm_allocator_segment_t*));
  mm_allocator_request_t* const request = vector_get_elm(
      segment->mem_requests,mm_reference->request_offset,mm_allocator_request_t);
  if (MM_ALLOCATOR_REQUEST_IS_FREE(request)) {
    fprintf(stderr,"MM-Allocator. Double free\n");
    exit(1);
  }
  // Free request
  MM_ALLOCATOR_REQUEST_SET_FREE(request);
  // Free contiguous request(s)
  const bool stacked_pending = (vector_get_used(mm_allocator->states)>0);
  if (!stacked_pending && segment->request_freed_idx==mm_reference->request_offset) {
    mm_allocator_free_contiguous_memory(mm_allocator);
  }
}
/*
 * State Push/Pop
 */
void mm_allocator_push_memory_state(
    mm_allocator_t* const mm_allocator
#ifdef MM_ALLOCATOR_LOG
    ,const char* func_name,
    uint64_t line_no
#endif
    ) {
  // Allocate new state
  mm_allocator_state_t* state;
  vector_alloc_new(mm_allocator->states,mm_allocator_state_t,state);
  // Setup state
  state->segment_idx = mm_allocator->segment_idx;
  mm_allocator_segment_t* const curr_segment = mm_allocator_get_segment(mm_allocator,mm_allocator->segment_idx);
  state->num_requests = vector_get_used(curr_segment->mem_requests);
  state->offset_available = curr_segment->offset_available;
  state->num_malloc_requests = vector_get_used(mm_allocator->malloc_requests);
#ifdef MM_ALLOCATOR_LOG
  state->func_name = (char*)func_name;
  state->line_no = line_no;
#endif
}
void mm_allocator_pop_memory_state(
    mm_allocator_t* const mm_allocator) {
  // Pop state
  mm_allocator_state_t* const state = vector_get_last_elm(mm_allocator->states,mm_allocator_state_t);
  vector_dec_used(mm_allocator->states);
  // Restore allocated memory state
  mm_allocator->segment_idx = state->segment_idx;
  mm_allocator_segment_t* const curr_segment = mm_allocator_get_segment(mm_allocator,state->segment_idx);
  vector_set_used(curr_segment->mem_requests,state->num_requests);
  curr_segment->offset_available = state->offset_available;
  // Restore malloc memory state
  void** malloc_requests = vector_get_mem(mm_allocator->malloc_requests,void*);
  const uint64_t num_malloc_requests = vector_get_used(mm_allocator->malloc_requests);
  uint64_t i;
  for (i=state->num_malloc_requests;i<num_malloc_requests;++i) {
    free(malloc_requests[i]); // Free
  }
  vector_set_used(mm_allocator->malloc_requests,state->num_malloc_requests);
  // Free contiguous request(s)
  mm_allocator_free_contiguous_memory(mm_allocator);
}
/*
 * Display
 */
void mm_allocator_print_state(
    FILE* const stream,
    mm_allocator_t* const mm_allocator,
    mm_allocator_state_t* const mm_allocator_state,
    const uint64_t mm_allocator_state_id) {
//  // Compute total memory allocated to state
//  mm_stack_request_t* const mm_stack_request = vector_get_mem(mm_stack->mem_requests,mm_stack_request_t);
//  uint64_t i, total_memory = 0;
//  for (i=0;i<mm_stack_state->num_requests;++i) {
//    total_memory += mm_stack_request[i].num_bytes;
//  }
  // Display State
#ifdef MM_ALLOCATOR_LOG
  fprintf(stream,"  [%02lu] \t Requests=%04" PRIu64 " \t Allocated=%08uB \t %s:%" PRIu64 "\n",
      mm_allocator_state_id,(uint64_t)0,0,mm_allocator_state->func_name,mm_allocator_state->line_no);
#else
  fprintf(stream,"  [%02" PRIu64 "] \t Requests=%04" PRIu64 " \t Allocated=%08uB\n",
      mm_allocator_state_id,(uint64_t)0,0);
#endif
}
void mm_allocator_print_states(
    FILE* const stream,
    mm_allocator_t* const mm_allocator) {
  const uint64_t num_states = vector_get_used(mm_allocator->states);
  uint64_t i;
  fprintf(stream,"  => States.pushed\n");
  if (num_states==0) {
    fprintf(stream,"  -- None --\n");
  } else {
    for (i=0;i<num_states;++i) {
      mm_allocator_state_t* const mm_allocator_state =
          vector_get_elm(mm_allocator->states,i,mm_allocator_state_t);
      mm_allocator_print_state(stream,mm_allocator,mm_allocator_state,i);
    }
  }
}
void mm_allocator_print_request(
    FILE* const stream,
    const uint64_t segment_idx,
    mm_allocator_request_t* const request,
    const uint64_t request_id) {
#ifdef MM_ALLOCATOR_LOG
      fprintf(stream,"    [%03" PRIu64 ":%s \t @%" PRIu64 "/%08u \t Size=%" PRIu64 "B \t %s:%" PRIu64 "\n",
          request_id,MM_ALLOCATOR_REQUEST_IS_FREE(request) ? "Free]     " : "Allocated]",
          segment_idx,request->offset,MM_ALLOCATOR_REQUEST_SIZE(request),
          request->func_name,request->line_no);
#else
      fprintf(stream,"    [%03" PRIu64 ":%s \t @%" PRIu64 "/%08u \t Size=%" PRIu64 "B\n",
          request_id,MM_ALLOCATOR_REQUEST_IS_FREE(request) ? "Free]     " : "Allocated]",
          segment_idx,request->offset,MM_ALLOCATOR_REQUEST_SIZE(request));
#endif
}
void mm_allocator_print_requests(
    FILE* const stream,
    mm_allocator_t* const mm_allocator,
    const bool compact_free) {
  uint64_t segment_idx, local_request_idx, global_request_idx = 0;
  uint64_t free_block = 0;
  fprintf(stream,"  => Memory.requests\n");
  for (segment_idx=0;segment_idx<=mm_allocator->segment_idx;++segment_idx) {
    mm_allocator_segment_t* const segment = mm_allocator_get_segment(mm_allocator,segment_idx);
    const uint64_t num_requests = vector_get_used(segment->mem_requests);
    for (local_request_idx=0;local_request_idx<num_requests;++local_request_idx,++global_request_idx) {
      mm_allocator_request_t* const request =
          vector_get_elm(segment->mem_requests,local_request_idx,mm_allocator_request_t);
      if (compact_free) {
        if (MM_ALLOCATOR_REQUEST_IS_FREE(request)) {
          free_block += MM_ALLOCATOR_REQUEST_SIZE(request);
        } else {
          if (free_block > 0) {
            fprintf(stream,"    [n/a:Free]      \t Size=%" PRIu64 "B\n",free_block);
            free_block = 0;
          }
          mm_allocator_print_request(stream,segment_idx,request,global_request_idx);
        }
      } else {
        mm_allocator_print_request(stream,segment_idx,request,global_request_idx);
      }
    }
  }
  if (global_request_idx==0) {
    fprintf(stream,"    -- No requests --\n");
  }
}
void mm_allocator_print(
    FILE* const stream,
    mm_allocator_t* const mm_allocator,
    const bool display_requests) {
  // Print header
  fprintf(stream,"MM-Allocator Report\n");
  // Print segment information
  const uint64_t num_segments = mm_allocator_get_num_segments(mm_allocator);
  const uint64_t segment_size = mm_allocator_get_segment(mm_allocator,0)->segment_size;
  fprintf(stream,"  => Segments.allocated %" PRIu64 "\n",num_segments);
  fprintf(stream,"    => Segments.size %" PRIu64 " MB\n",CONVERT_B_TO_MB(segment_size));
  fprintf(stream,"    => Memory.available %" PRIu64 " MB\n",num_segments*CONVERT_B_TO_MB(segment_size));
  // Print memory information
  uint64_t total_used, total_free, begin_free;
  mm_allocator_compute_occupation(mm_allocator,&total_used,&total_free,&begin_free);
  fprintf(stream,"  => Memory.used %" PRIu64 "\n",total_used);
  fprintf(stream,"  => Memory.free %" PRIu64 "\n",total_free);
  fprintf(stream,"    => Memory.free.begin %" PRIu64 "\n",begin_free);
  fprintf(stream,"    => Memory.free.fragmented %" PRIu64 "\n",total_free-begin_free);
  // Print pushed states
  mm_allocator_print_states(stream,mm_allocator);
  // Print memory requests
  if (display_requests) {
    mm_allocator_print_requests(stream,mm_allocator,false);
  }
}



