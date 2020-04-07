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
 * DESCRIPTION: BitPal wrapper
 */

#include "benchmark/benchmark_bitpal.h"
#include "gap_lineal/nw.h"

/*
 * BitPal
 */
int bitwise_alignment_m0_x1_g1(char * s1,char* s2,int words);
void benchmark_bitpal_m0_x1_g1(
    align_input_t* const align_input) {
  // Align
  timer_start(&align_input->timer);
  bitwise_alignment_m0_x1_g1(
      align_input->pattern,align_input->text,
      MAX(align_input->pattern_length,align_input->text_length)/63 + 1);
  timer_stop(&align_input->timer);
  // NOTE:
  //   No CIGAR is produced, just score
}
int bitwise_alignment_m1_x4_g2(char * s1,char* s2,int words);
void benchmark_bitpal_m1_x4_g2(
    align_input_t* const align_input) {
  // Align
  timer_start(&align_input->timer);
  bitwise_alignment_m1_x4_g2(
      align_input->pattern,align_input->text,
      MAX(align_input->pattern_length,align_input->text_length)/63 + 1);
  timer_stop(&align_input->timer);
  // NOTE:
  //   No CIGAR is produced, just score
}
