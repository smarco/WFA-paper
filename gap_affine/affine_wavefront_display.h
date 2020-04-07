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
 * DESCRIPTION: WFA display functions
 */

#ifndef AFFINE_WAVEFRONT_DISPLAY_H_
#define AFFINE_WAVEFRONT_DISPLAY_H_

/*
 * Includes
 */
#include "gap_affine/affine_table.h"
#include "gap_affine/affine_wavefront.h"
#include "utils/commons.h"

/*
 * Accessors
 */
void affine_wavefronts_set_edit_table(
    affine_wavefronts_t* const affine_wavefronts,
    const int pattern_length,
    const int text_length,
    const int k,
    const awf_offset_t offset,
    const int score);

/*
 * Display
 */
void affine_wavefronts_print_wavefront(
    FILE* const stream,
    affine_wavefronts_t* const affine_wavefronts,
    const int current_score);
void affine_wavefronts_print_wavefronts(
    FILE* const stream,
    affine_wavefronts_t* const affine_wavefronts,
    const int current_score);
void affine_wavefronts_print_wavefronts_pretty(
    FILE* const stream,
    affine_wavefronts_t* const affine_wavefronts,
    const int current_score);

/*
 * Debug
 */
void affine_wavefronts_debug_step(
    affine_wavefronts_t* const affine_wavefronts,
    const char* const pattern,
    const char* const text,
    const int score);

#endif /* AFFINE_WAVEFRONT_DISPLAY_H_ */
