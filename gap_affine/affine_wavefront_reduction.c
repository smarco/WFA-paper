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

#include "gap_affine/affine_wavefront_reduction.h"

/*
 * Setup
 */
void affine_wavefronts_reduction_set_none(
    affine_wavefronts_reduction_t* const wavefronts_reduction) {
  wavefronts_reduction->reduction_strategy = wavefronts_reduction_none;
}
void affine_wavefronts_reduction_set_dynamic(
    affine_wavefronts_reduction_t* const wavefronts_reduction,
    const int min_wavefront_length,
    const int max_distance_threshold) {
  wavefronts_reduction->reduction_strategy = wavefronts_reduction_dynamic;
  wavefronts_reduction->min_wavefront_length = min_wavefront_length;
  wavefronts_reduction->max_distance_threshold = max_distance_threshold;
}
