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
 * DESCRIPTION: WFA extend backtrace component
 */

#ifndef AFFINE_WAVEFRONT_BACKTRACE_H_
#define AFFINE_WAVEFRONT_BACKTRACE_H_

#include "gap_affine/affine_wavefront.h"

/*
 * Sequences DTO
 */
typedef struct {
  char* pattern;
  int pattern_length;
  char* text;
  int text_length;
} alignment_sequences_t;

/*
 * WF type
 */
typedef enum {
  backtrace_wavefront_M = 0,
  backtrace_wavefront_I = 1,
  backtrace_wavefront_D = 2
} backtrace_wavefront_type;

/*
 * Backtrace
 */
void affine_wavefronts_backtrace(
    affine_wavefronts_t* const affine_wavefronts,
    char* const pattern,
    const int pattern_length,
    char* const text,
    const int text_length,
    const int alignment_score);

#endif /* AFFINE_WAVEFRONT_BACKTRACE_H_ */
