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
 * DESCRIPTION: SeqAn library wrapper
 */

#include "benchmark/benchmark_utils.h"
#include "benchmark/benchmark_edit.h"

/*
 * Benchmark SeqAn Bridge
 */
void benchmark_seqan_bridge_global_edit(
    char* const pattern,
    const int pattern_length,
    char* const text,
    const int text_length,
    char* const edit_operations,
    int* const num_edit_operations);
void benchmark_seqan_bridge_global_edit_bpm(
    char* const pattern,
    const int pattern_length,
    char* const text,
    const int text_length,
    char* const edit_operations,
    int* const num_edit_operations);
void benchmark_seqan_bridge_global_lineal(
    char* const pattern,
    const int pattern_length,
    char* const text,
    const int text_length,
    const int match,
    const int mismatch,
    const int insertion,
    const int deletion,
    char* const edit_operations,
    int* const num_edit_operations);
void benchmark_seqan_bridge_global_affine(
    char* const pattern,
    const int pattern_length,
    char* const text,
    const int text_length,
    const int match,
    const int mismatch,
    const int gap_opening,
    const int gap_extension,
    char* const edit_operations,
    int* const num_edit_operations);
/*
 * Benchmark SeqAn
 */
void benchmark_seqan_global_edit(
    align_input_t* const align_input) {
  // Parameters
  const int max_cigar_length = align_input->pattern_length + align_input->text_length;
  edit_cigar_t edit_cigar = {
      .operations = malloc(max_cigar_length),
      .begin_offset = 0,
      .end_offset = 0,
  };
  // Align
  timer_start(&align_input->timer);
  benchmark_seqan_bridge_global_edit(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      edit_cigar.operations,&edit_cigar.end_offset);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
}
void benchmark_seqan_global_edit_bpm(
    align_input_t* const align_input) {
  // Align
  timer_start(&align_input->timer);
  benchmark_seqan_bridge_global_edit_bpm(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      NULL,NULL);
  timer_stop(&align_input->timer);
  // NOTE:
  //   No CIGAR is produced, just score
}
void benchmark_seqan_global_lineal(
    align_input_t* const align_input,
    lineal_penalties_t* const penalties) {
  // Parameters
  const int max_cigar_length = align_input->pattern_length + align_input->text_length;
  edit_cigar_t edit_cigar = {
      .operations = malloc(max_cigar_length),
      .begin_offset = 0,
      .end_offset = 0,
  };
  // Align
  timer_start(&align_input->timer);
  benchmark_seqan_bridge_global_lineal(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties->match,penalties->mismatch,
      penalties->insertion,penalties->deletion,
      edit_cigar.operations,&edit_cigar.end_offset);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
}
void benchmark_seqan_global_affine(
    align_input_t* const align_input,
    affine_penalties_t* const penalties) {
  // Parameters
  const int max_cigar_length = align_input->pattern_length + align_input->text_length;
  edit_cigar_t edit_cigar = {
      .operations = malloc(max_cigar_length),
      .begin_offset = 0,
      .end_offset = 0,
  };
  // Align
  timer_start(&align_input->timer);
  benchmark_seqan_bridge_global_affine(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties->match,penalties->mismatch,
      penalties->gap_opening,penalties->gap_extension,
      edit_cigar.operations,&edit_cigar.end_offset);
  timer_stop(&align_input->timer);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
}
