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
 * DESCRIPTION: Parasail library wrapper
 */

#include "benchmark/benchmark_parasail.h"
#include "resources/parasail/parasail.h"

/*
 * Parasail
 */
void benchmark_parasail_parse_cigar(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    parasail_result_t* const parasail_result,
    parasail_cigar_t* const parasail_cigar,
    edit_cigar_t* const edit_cigar) {
  // Allocate
  edit_cigar->operations = malloc(align_input->pattern_length+align_input->text_length);
  edit_cigar->begin_offset = 0;
  edit_cigar->end_offset = 0;
  // Decode all CIGAR operations
  int i;
  for (i=0;i<parasail_cigar->len;++i) {
    // Fetch operation and length
    char operation = parasail_cigar_decode_op(parasail_cigar->seq[i]);
    const uint32_t length = parasail_cigar_decode_len(parasail_cigar->seq[i]);
    // Adapt operation '='
    if (operation == '=') operation = 'M';
    else if (operation == 'D') operation = 'I';
    else if (operation == 'I') operation = 'D';
    // Dump into the cigar
    int j;
    for (j=0;j<length;++j) {
      edit_cigar->operations[(edit_cigar->end_offset)++] = operation;
    }
    edit_cigar->operations[edit_cigar->end_offset] = '\0';
  }
//  // DEBUG
//  const int score_parasail = parasail_result_get_score(parasail_result);
//  const int score_cigar = edit_cigar_score_gap_affine(edit_cigar,penalties);
//  if (score_parasail != score_cigar) {
//    char* const cigar_str = parasail_cigar_decode(parasail_cigar);
//    fprintf(stderr,"COMPUTED=%d\t",score_cigar);
//    edit_cigar_print(stderr,edit_cigar);
//    fprintf(stderr,"\nPARASAIL=%d\t%s\n",score_parasail,cigar_str);
//    free(cigar_str);
//  }
}
void benchmark_parasail_nw_stripped(
    align_input_t* const align_input,
    affine_penalties_t* const penalties) {
  // Create parasail-matrix
  parasail_matrix_t* const matrix =
      parasail_matrix_create("ACGT", -penalties->match, -penalties->mismatch);
  parasail_result_t* result = NULL;
  // Align
  timer_start(&align_input->timer);
  result = parasail_nw_trace_striped_avx2_256_16(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties->gap_opening + penalties->gap_extension,
      penalties->gap_extension,matrix);
  // Traceback
  parasail_cigar_t* parasail_cigar = parasail_result_get_cigar(
      result,align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,matrix);
  timer_stop(&align_input->timer);
  // Adapt CIGAR
  edit_cigar_t edit_cigar;
  benchmark_parasail_parse_cigar(
      align_input,penalties,result,parasail_cigar,&edit_cigar);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
  parasail_result_free(result);
  parasail_matrix_free(matrix);
}
void benchmark_parasail_nw_scan(
    align_input_t* const align_input,
    affine_penalties_t* const penalties) {
  // Create parasail-matrix
  parasail_matrix_t* const matrix =
      parasail_matrix_create("ACGT", -penalties->match, -penalties->mismatch);
  parasail_result_t* result = NULL;
  // Align
  timer_start(&align_input->timer);
  result = parasail_nw_trace_scan_avx2_256_16(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties->gap_opening + penalties->gap_extension,
      penalties->gap_extension,matrix);
  // Traceback
  parasail_cigar_t* parasail_cigar = parasail_result_get_cigar(
      result,align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,matrix);
  timer_stop(&align_input->timer);
  // Adapt CIGAR
  edit_cigar_t edit_cigar;
  benchmark_parasail_parse_cigar(
      align_input,penalties,result,
      parasail_cigar,&edit_cigar);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
  parasail_result_free(result);
  parasail_matrix_free(matrix);
}
void benchmark_parasail_nw_diag(
    align_input_t* const align_input,
    affine_penalties_t* const penalties) {
  // Create parasail-matrix
  parasail_matrix_t* const matrix =
      parasail_matrix_create("ACGT", -penalties->match, -penalties->mismatch);
  parasail_result_t* result = NULL;
  // Align
  timer_start(&align_input->timer);
  result = parasail_nw_trace_diag_avx2_256_16(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties->gap_opening + penalties->gap_extension,
      penalties->gap_extension,matrix);
  // Traceback
  parasail_cigar_t* parasail_cigar = parasail_result_get_cigar(
      result,align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,matrix);
  timer_stop(&align_input->timer);
  // Adapt CIGAR
  edit_cigar_t edit_cigar;
  benchmark_parasail_parse_cigar(
      align_input,penalties,result,
      parasail_cigar,&edit_cigar);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
  parasail_result_free(result);
  parasail_matrix_free(matrix);
}
void benchmark_parasail_nw_banded(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const int bandwidth) {
  // Create parasail-matrix
  parasail_matrix_t* const matrix =
      parasail_matrix_create("ACGT", -penalties->match, -penalties->mismatch);
  parasail_result_t* result = NULL;
  // Align
  timer_start(&align_input->timer);
  result = parasail_nw_banded(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,
      penalties->gap_opening + penalties->gap_extension,
      penalties->gap_extension,bandwidth,matrix);
  timer_stop(&align_input->timer);
  // NOTE:
  //   No CIGAR is produced, just score
  // Free
  parasail_result_free(result);
  parasail_matrix_free(matrix);
}

