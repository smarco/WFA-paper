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

#ifndef BENCHMARK_KSW2
#define BENCHMARK_KSW2

#include "benchmark/benchmark_utils.h"

/*
 * ksw2
 */
void benchmark_ksw2_extz2_sse(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const bool approximate_max__drop,
    const int band_width,
    const int zdrop);
void benchmark_ksw2_extd2_sse(
    align_input_t* const align_input,
    affine_penalties_t* const penalties,
    const bool approximate_max__drop,
    const int band_width,
    const int zdrop);

#endif /* BENCHMARK_KSW2 */
