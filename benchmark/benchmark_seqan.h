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
 * DESCRIPTION: SeqAn library wrapper
 */

#ifndef BENCHMARK_SEQAN_H_
#define BENCHMARK_SEQAN_H_

#include "benchmark/benchmark_utils.h"

/*
 * Benchmark SeqAn
 */
void benchmark_seqan_global_edit(
    align_input_t* const align_input);
void benchmark_seqan_global_edit_bpm(
    align_input_t* const align_input);
void benchmark_seqan_global_lineal(
    align_input_t* const align_input,
    lineal_penalties_t* const penalties);
void benchmark_seqan_global_affine(
    align_input_t* const align_input,
    affine_penalties_t* const penalties);

#endif /* BENCHMARK_SEQAN_H_ */
