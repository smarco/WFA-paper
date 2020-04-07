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
 * DESCRIPTION: KSW2 library wrapper
 */

#include "benchmark/benchmark_ksw2.h"
#include "resources/ksw2/ksw2.h"

/*
 * ksw2
 */
void benchmark_ksw2_adapt_cigar(
    align_input_t* const align_input,
    ksw_extz_t* const ez,
    edit_cigar_t* const edit_cigar) {
  // Adapt CIGAR
  edit_cigar->operations = malloc(2*(align_input->text_length+align_input->pattern_length));
  edit_cigar->begin_offset = 0;
  edit_cigar->end_offset = 0;
  int i, j;
  for (i=0;i<ez->n_cigar;++i) {
    const int length = ez->cigar[i]>>4;
    const char operation = "MDI"[ez->cigar[i]&0xf];
    for (j=0;j<length;++j) {
      edit_cigar->operations[(edit_cigar->end_offset)++] = operation;
    }
  }
  edit_cigar->operations[edit_cigar->end_offset] = '\0';
  // Add mismatches
  edit_cigar_add_mismatches(
      align_input->pattern,align_input->pattern_length,
      align_input->text,align_input->text_length,edit_cigar);
}
/*
 * ksw2
 */
void benchmark_ksw2_extz2_sse(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const bool approximate_max__drop,
    const int band_width,
    const int zdrop) {
  // Parameters
  const char* tseq = align_input->text;
  const char* qseq = align_input->pattern;
  const int sc_mch = -penalties->match;
  const int sc_mis = -penalties->mismatch;
  const int gapo = penalties->gap_opening;
  const int gape = penalties->gap_extension;
  // Prepare data
  void *km = 0;
  int flag = 0;
  int i, a = sc_mch, b = sc_mis < 0? sc_mis : -sc_mis; // a>0 and b<0
  int8_t mat[25] = { a,b,b,b,0, b,a,b,b,0, b,b,a,b,0, b,b,b,a,0, 0,0,0,0,0 };
  int tlen = strlen(tseq), qlen = strlen(qseq);
  uint8_t *ts, *qs, c[256];
  ksw_extz_t ez;
  memset(&ez, 0, sizeof(ksw_extz_t));
  memset(c, 4, 256);
  c['A'] = c['a'] = 0; c['C'] = c['c'] = 1;
  c['G'] = c['g'] = 2; c['T'] = c['t'] = 3; // build the encoding table
  ts = (uint8_t*)malloc(tlen);
  qs = (uint8_t*)malloc(qlen);
  for (i = 0; i < tlen; ++i) ts[i] = c[(uint8_t)tseq[i]]; // encode to 0/1/2/3
  for (i = 0; i < qlen; ++i) qs[i] = c[(uint8_t)qseq[i]];
  if (approximate_max__drop) flag |= KSW_EZ_APPROX_MAX | KSW_EZ_APPROX_DROP;
  // Align
  timer_start(&align_input->timer);
  ksw_extz2_sse(km, qlen, (uint8_t*)qseq, tlen, (uint8_t*)tseq, 5,
      mat, gapo, gape, band_width, zdrop, 0, flag, &ez);
  timer_stop(&align_input->timer);
  // Adapt CIGAR
  edit_cigar_t edit_cigar;
  benchmark_ksw2_adapt_cigar(align_input,&ez,&edit_cigar);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
  free(ez.cigar);
  free(ts);
  free(qs);
}
void benchmark_ksw2_extd2_sse(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const bool approximate_max__drop,
    const int band_width,
    const int zdrop) {
  // Parameters
  const char *tseq = align_input->text;
  const char *qseq = align_input->pattern;
  const int sc_mch = -penalties->match;
  const int sc_mis = -penalties->mismatch;
  const int gapo = penalties->gap_opening;
  const int gape = penalties->gap_extension;
  // Prepare data
  void *km = 0;
  int flag = 0;
  int i, a = sc_mch, b = sc_mis < 0? sc_mis : -sc_mis; // a>0 and b<0
  int8_t mat[25] = { a,b,b,b,0, b,a,b,b,0, b,b,a,b,0, b,b,b,a,0, 0,0,0,0,0 };
  int tlen = strlen(tseq), qlen = strlen(qseq);
  uint8_t *ts, *qs, c[256];
  ksw_extz_t ez;
  memset(&ez, 0, sizeof(ksw_extz_t));
  memset(c, 4, 256);
  c['A'] = c['a'] = 0; c['C'] = c['c'] = 1;
  c['G'] = c['g'] = 2; c['T'] = c['t'] = 3; // build the encoding table
  ts = (uint8_t*)malloc(tlen);
  qs = (uint8_t*)malloc(qlen);
  for (i = 0; i < tlen; ++i) ts[i] = c[(uint8_t)tseq[i]]; // encode to 0/1/2/3
  for (i = 0; i < qlen; ++i) qs[i] = c[(uint8_t)qseq[i]];
  if (approximate_max__drop) flag |= KSW_EZ_APPROX_MAX | KSW_EZ_APPROX_DROP;
  // Align
  timer_start(&align_input->timer);
  ksw_extd2_sse(km, qlen, (uint8_t*)qseq, tlen, (uint8_t*)tseq, 5,
      mat, gapo, gape, gapo, gape, band_width, zdrop, 0, flag, &ez);
  timer_stop(&align_input->timer);
  // Adapt CIGAR
  edit_cigar_t edit_cigar;
  benchmark_ksw2_adapt_cigar(align_input,&ez,&edit_cigar);
  // Debug alignment
  if (align_input->debug_flags) {
    benchmark_check_alignment(align_input,&edit_cigar);
  }
  // Free
  free(edit_cigar.operations);
  free(ez.cigar);
  free(ts);
  free(qs);
}

