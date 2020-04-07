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
 * DESCRIPTION: Edit-distance alignment algorithms wrapper
 */

#include "benchmark/benchmark_utils.h"
#include "edit/edit_dp.h"

/*
 * Benchmark Edit
 */
void benchmark_edit_dp(
    align_input_t* const align_input) {
  // Parameters
  edit_table_t edit_table;
  // Allocate
  edit_table_allocate(
      &edit_table,align_input->pattern_length,
      align_input->text_length,align_input->mm_allocator);
  // Align
  timer_start(&align_input->timer);
  edit_dp_compute(&edit_table,
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_table.edit_cigar);
  }
  // Free
  edit_table_free(&edit_table,align_input->mm_allocator);
}
void benchmark_edit_dp_banded(
    align_input_t* const align_input,
    const int bandwidth) {
  // Parameters
  edit_table_t edit_table;
  // Allocate
  edit_table_allocate(
      &edit_table,align_input->pattern_length,
      align_input->text_length,align_input->mm_allocator);
  // Align
  timer_start(&align_input->timer);
  edit_dp_compute_banded(&edit_table,
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,bandwidth);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_table.edit_cigar);
  }
  // Free
  edit_table_free(&edit_table,align_input->mm_allocator);
}
