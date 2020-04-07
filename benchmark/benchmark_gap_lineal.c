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
 * DESCRIPTION: Gap-lineal alignment algorithms wrapper
 */

#include "benchmark/benchmark_gap_lineal.h"
#include "gap_lineal/nw.h"

/*
 * Benchmark NW
 */
void benchmark_gap_lineal_nw(
    align_input_t* const align_input,
    lineal_penalties_t* const penalties) {
  // Allocate
  edit_table_t edit_table;
  edit_table_allocate(
      &edit_table,align_input->pattern_length,
      align_input->text_length,align_input->mm_allocator);
  // Align
  timer_start(&align_input->timer);
  nw_compute(&edit_table,
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties);
  timer_stop(&align_input->timer);
  // Free
  edit_table_free(&edit_table,align_input->mm_allocator);
}
