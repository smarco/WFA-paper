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
 * DESCRIPTION: WFA support functions for wavefront-reduction
 *   strategies (adaptive or banded strategies)
 */

#ifndef AFFINE_WAVEFRONT_REDUCTION_H_
#define AFFINE_WAVEFRONT_REDUCTION_H_

#include "utils/commons.h"

/*
 * Wavefront Reduction
 */
typedef enum {
  wavefronts_reduction_none,
  wavefronts_reduction_dynamic,
} wavefront_reduction_type;

/*
 * Wavefront Penalties
 */
typedef struct {
  wavefront_reduction_type reduction_strategy;     // Reduction strategy
  int min_wavefront_length;                        // Dynamic: Minimum wavefronts length to reduce
  int max_distance_threshold;                      // Dynamic: Maximum distance between offsets allowed
} affine_wavefronts_reduction_t;

/*
 * Setup
 */
void affine_wavefronts_reduction_set_none(
    affine_wavefronts_reduction_t* const wavefronts_reduction);
void affine_wavefronts_reduction_set_dynamic(
    affine_wavefronts_reduction_t* const wavefronts_reduction,
    const int min_wavefront_length,
    const int max_distance_threshold);

#endif /* AFFINE_WAVEFRONT_REDUCTION_H_ */
