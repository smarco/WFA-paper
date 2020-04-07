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
 * DESCRIPTION: WFA support utilities
 */

#ifndef AFFINE_WAVEFRONT_UTILS_H_
#define AFFINE_WAVEFRONT_UTILS_H_

#include "gap_affine/affine_wavefront.h"

/*
 * Accessors
 */
affine_wavefront_t* affine_wavefronts_get_source_mwavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int score);
affine_wavefront_t* affine_wavefronts_get_source_iwavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int score);
affine_wavefront_t* affine_wavefronts_get_source_dwavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int score);
int affine_wavefronts_diagonal_length(
    affine_wavefronts_t* const affine_wavefronts,
    const int k);
int affine_wavefronts_compute_distance(
    const int pattern_length,
    const int text_length,
    const awf_offset_t offset,
    const int k);

/*
 * Initial Conditions and finalization
 */
void affine_wavefront_initialize(
    affine_wavefronts_t* const affine_wavefronts);
bool affine_wavefront_end_reached(
    affine_wavefronts_t* const affine_wavefronts,
    const int pattern_length,
    const int text_length,
    const int score);

#endif /* AFFINE_WAVEFRONT_UTILS_H_ */
