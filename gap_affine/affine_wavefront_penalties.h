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
 * DESCRIPTION: WFA support functions for handling penalties scores
 */

#ifndef AFFINE_WAVEFRONT_PENALTIES_H_
#define AFFINE_WAVEFRONT_PENALTIES_H_

#include "utils/commons.h"
#include "gap_affine/affine_penalties.h"

/*
 * Wavefront Strategy
 */
typedef enum {
  wavefronts_penalties_match_zero,
  wavefronts_penalties_force_zero_match,
  wavefronts_penalties_shifted_penalties,
  wavefronts_penalties_odd_pair_penalties
} wavefronts_penalties_strategy;

/*
 * Wavefront Penalties
 */
typedef struct {
  affine_penalties_t base_penalties;                // Input base Gap-Affine penalties
  affine_penalties_t wavefront_penalties;           // Wavefront Gap-Affine penalties
  wavefronts_penalties_strategy penalties_strategy; // Penalties adaptation strategy
} affine_wavefronts_penalties_t;

/*
 * Setup
 */
void affine_wavefronts_penalties_init(
    affine_wavefronts_penalties_t* const wavefronts_penalties,
    affine_penalties_t* const penalties,
    const wavefronts_penalties_strategy penalties_strategy);

/*
 * Score Adjustment
 */
void affine_penalties_mzero(
    affine_penalties_t* const base_penalties,
    affine_penalties_t* const shifted_penalties);
void affine_penalties_shift(
    affine_penalties_t* const base_penalties,
    affine_penalties_t* const shifted_penalties,
    const bool pair_odd_heuristic);


#endif /* AFFINE_WAVEFRONT_PENALTIES_H_ */
