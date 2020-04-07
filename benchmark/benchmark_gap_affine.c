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
 * DESCRIPTION: Gap-affine alignment algorithms wrapper (including WFA)
 */

#include "benchmark/benchmark_gap_affine.h"

#include "gap_affine/affine_wavefront_align.h"
#include "gap_affine/affine_table.h"

// Gap affine modules
#include "gap_affine/affine_wavefront.h"
#include "gap_affine/affine_wavefront_display.h"
#include "gap_affine/swg.h"

/*
 * Benchmark SWG
 */
void benchmark_gap_affine_swg(
    align_input_t* const align_input,
    affine_penalties_t* const penalties) {
  // Allocate
  affine_table_t affine_table;
  affine_table_allocate(
      &affine_table,align_input->pattern_length,
      align_input->text_length,align_input->mm_allocator);
  // Align
  timer_start(&align_input->timer);
  swg_compute(&affine_table,penalties,
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&affine_table.edit_cigar);
  }
  // Free
  affine_table_free(&affine_table,align_input->mm_allocator);
}
void benchmark_gap_affine_swg_banded(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const int bandwidth) {
  // Allocate
  affine_table_t affine_table;
  affine_table_allocate(
      &affine_table,align_input->pattern_length,
      align_input->text_length,align_input->mm_allocator);
  // Align
  timer_start(&align_input->timer);
  swg_compute_banded(&affine_table,penalties,
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,bandwidth);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&affine_table.edit_cigar);
  }
  // Free
  affine_table_free(&affine_table,align_input->mm_allocator);
}
void benchmark_gap_affine_wavefront(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const int min_wavefront_length,
    const int max_distance_threshold) {
  // Allocate
  affine_wavefronts_t* affine_wavefronts;
  if (min_wavefront_length < 0) {
    affine_wavefronts = affine_wavefronts_new_complete(
        align_input->pattern_length,align_input->text_length,penalties,
        &(align_input->wavefronts_stats),align_input->mm_allocator);
  } else {
    affine_wavefronts = affine_wavefronts_new_reduced(
        align_input->pattern_length,align_input->text_length,penalties,
        min_wavefront_length,max_distance_threshold,
        &(align_input->wavefronts_stats),align_input->mm_allocator);
  }
  // Align
  timer_start(&align_input->timer);
  // affine_wavefronts_clear(affine_wavefronts);
  affine_wavefronts_align(affine_wavefronts,
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&affine_wavefronts->edit_cigar);
  }
  // Free
  affine_wavefronts_delete(affine_wavefronts);
}
