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
 * DESCRIPTION: Gap-affine alignment algorithms wrapper (including WFA)
 */

#ifndef BENCHMARK_GAP_AFFINE_H_
#define BENCHMARK_GAP_AFFINE_H_

#include "gap_affine/affine_wavefront.h"
#include "benchmark/benchmark_utils.h"

/*
 * Benchmark SWG
 */
void benchmark_gap_affine_swg(
    align_input_t* const align_input,
    affine_penalties_t* const penalties);
void benchmark_gap_affine_swg_banded(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const int bandwidth);
void benchmark_gap_affine_wavefront(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const int min_wavefront_length,
    const int max_distance_threshold);

#endif /* BENCHMARK_GAP_AFFINE_H_ */
