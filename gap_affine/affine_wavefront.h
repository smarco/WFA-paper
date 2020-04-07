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
 * DESCRIPTION: Wavefront alignment algorithm for pairwise gap-affine
 *   alignment (Main module)
 */

#ifndef AFFINE_WAVEFRONT_H_
#define AFFINE_WAVEFRONT_H_


#include "utils/commons.h"
#include "system/profiler_counter.h"
#include "system/profiler_timer.h"

#include "gap_affine/affine_table.h"
#include "gap_affine/affine_wavefront_penalties.h"
#include "gap_affine/affine_wavefront_reduction.h"
#include "gap_affine/wavefront_stats.h"

/*
 * Debug
 */
//#define AFFINE_WAVEFRONT_DEBUG
//#define AFFINE_WAVEFRONT_STATS

/*
 * Constants
 */
#define AFFINE_WAVEFRONT_OFFSET_NULL (-10)
#define AFFINE_WAVEFRONT_K_NULL      (INT_MAX/2)

/*
 * Translate k and offset to coordinates h,v
 */
#define AFFINE_WAVEFRONT_V(k,offset) ((offset)-(k))
#define AFFINE_WAVEFRONT_H(k,offset) (offset)

#define AFFINE_WAVEFRONT_DIAGONAL(h,v) ((h)-(v))
#define AFFINE_WAVEFRONT_OFFSET(h,v)   (h)

/*
 * Offset size
 */
//#define AFFINE_WAVEFRONT_W8
//#define AFFINE_WAVEFRONT_W16
#define AFFINE_WAVEFRONT_W32

#ifdef AFFINE_WAVEFRONT_W8
  typedef int8_t awf_offset_t;
#else
  #ifdef AFFINE_WAVEFRONT_W16
    typedef int16_t awf_offset_t;
  #else // AFFINE_WAVEFRONT_W32
    typedef int32_t awf_offset_t;
  #endif
#endif

/*
 * Wavefront
 */
typedef struct {
  // Range
  bool null;                  // Is null interval?
  int lo;                     // Effective lowest diagonal (inclusive)
  int hi;                     // Effective highest diagonal (inclusive)
  int lo_base;                // Lowest diagonal before reduction (inclusive)
  int hi_base;                // Highest diagonal before reduction (inclusive)
  // Offsets
  awf_offset_t* offsets;      // Offsets
#ifdef AFFINE_WAVEFRONT_DEBUG
  awf_offset_t* offsets_base; // Offsets increment
#endif
} affine_wavefront_t;

/*
 * Gap-Affine Wavefronts
 */
typedef struct {
  // Dimensions
  int pattern_length;                          // Pattern length
  int text_length;                             // Text length
  int num_wavefronts;                          // Total number of allocatable wavefronts
  // Limits
  int max_penalty;                             // MAX(mismatch_penalty,single_gap_penalty)
  int max_k;                                   // Maximum diagonal k (used for null-wf, display, and banding)
  int min_k;                                   // Maximum diagonal k (used for null-wf, display, and banding)
  // Wavefronts
  affine_wavefront_t** mwavefronts;            // M-wavefronts
  affine_wavefront_t** iwavefronts;            // I-wavefronts
  affine_wavefront_t** dwavefronts;            // D-wavefronts
  affine_wavefront_t wavefront_null;           // Null wavefront (used to gain orthogonality)
  // Reduction
  affine_wavefronts_reduction_t reduction;     // Reduction parameters
  // Penalties
  affine_wavefronts_penalties_t penalties;     // Penalties parameters
  // CIGAR
  edit_cigar_t edit_cigar;                     // Alignment CIGAR
  // MM
  mm_allocator_t* mm_allocator;                // MM-Allocator
  affine_wavefront_t* wavefronts_mem;          // MM-Slab for affine_wavefront_t (base)
  affine_wavefront_t* wavefronts_current;      // MM-Slab for affine_wavefront_t (next)
  // STATS
  wavefronts_stats_t* wavefronts_stats; // Stats
  // DEBUG
#ifdef AFFINE_WAVEFRONT_DEBUG
  affine_table_t gap_affine_table;             // DP-Table encoded by the wavefronts
#endif
} affine_wavefronts_t;

/*
 * SWF Wavefront Computation Set
 */
typedef struct {
  /* In Wavefronts*/
  affine_wavefront_t* in_mwavefront_sub;
  affine_wavefront_t* in_mwavefront_gap;
  affine_wavefront_t* in_iwavefront_ext;
  affine_wavefront_t* in_dwavefront_ext;
  /* Out Wavefronts */
  affine_wavefront_t* out_mwavefront;
  affine_wavefront_t* out_iwavefront;
  affine_wavefront_t* out_dwavefront;
} affine_wavefront_set;

/*
 * Setup
 */
void affine_wavefronts_clear(
    affine_wavefronts_t* const affine_wavefronts);
void affine_wavefronts_delete(
    affine_wavefronts_t* const affine_wavefronts);

/*
 * Setup WF-modes
 */
affine_wavefronts_t* affine_wavefronts_new_complete(
    const int pattern_length,
    const int text_length,
    affine_penalties_t* const penalties,
    wavefronts_stats_t* const wavefronts_stats,
    mm_allocator_t* const mm_allocator);
affine_wavefronts_t* affine_wavefronts_new_reduced(
    const int pattern_length,
    const int text_length,
    affine_penalties_t* const penalties,
    const int min_wavefront_length,
    const int max_distance_threshold,
    wavefronts_stats_t* const wavefronts_stats,
    mm_allocator_t* const mm_allocator);

/*
 * Allocate individual wavefront
 */
affine_wavefront_t* affine_wavefronts_allocate_wavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int lo_base,
    const int hi_base);

#endif /* AFFINE_WAVEFRONT_H_ */
