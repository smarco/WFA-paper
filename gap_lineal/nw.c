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
 * DESCRIPTION: Dynamic-programming alignment algorithm for computing
 *   gap-lineal pairwise alignment (Needleman-Wunsch - NW)
 */

#include "gap_lineal/nw.h"

/*
 * NW Traceback
 */
void nw_traceback(
    edit_table_t* const edit_table,
    lineal_penalties_t* const penalties) {
  // Parameters
  int** const dp = edit_table->columns;
  char* const operations = edit_table->edit_cigar.operations;
  int op_sentinel = edit_table->edit_cigar.end_offset-1;
  int h, v;
  // Compute traceback
  h = edit_table->num_columns-1;
  v = edit_table->num_rows-1;
  while (h>0 && v>0) {
    if (dp[h][v] == dp[h][v-1]+penalties->deletion) {
      operations[op_sentinel--] = 'D';
      --v;
    } else if (dp[h][v] == dp[h-1][v]+penalties->insertion) {
      operations[op_sentinel--] = 'I';
      --h;
    } else {
      operations[op_sentinel--] =
          (dp[h][v] == dp[h-1][v-1]+penalties->mismatch) ? 'X' : 'M';
      --h;
      --v;
    }
  }
  while (h>0) {operations[op_sentinel--] = 'I'; --h;}
  while (v>0) {operations[op_sentinel--] = 'D'; --v;}
  edit_table->edit_cigar.begin_offset = op_sentinel+1;
}
void nw_compute(
    edit_table_t* const edit_table,
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length,
    lineal_penalties_t* const penalties) {
  // Parameters
  int** dp = edit_table->columns;
  int h, v;
  // Init DP (No ends-free)
  dp[0][0] = 0;
  for (v=1;v<=pattern_length;++v) {
    dp[0][v] = dp[0][v-1] + penalties->deletion;
  }
  for (h=1;h<=text_length;++h) {
    dp[h][0] = dp[h-1][0] + penalties->insertion;
  }
  // Compute DP
  for (h=1;h<=text_length;++h) {
    for (v=1;v<=pattern_length;++v) {
      int min = dp[h-1][v-1] + ((pattern[v-1]==text[h-1]) ? 0 : penalties->mismatch); // Misms
      min = MIN(min,dp[h-1][v]+penalties->insertion); // Ins
      min = MIN(min,dp[h][v-1]+penalties->deletion); // Del
      dp[h][v] = min;
    }
  }
  // Compute traceback
  nw_traceback(edit_table,penalties);
}
