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
 * DESCRIPTION: Gap-affine DP table
 */

#ifndef AFFINE_TABLE_H_
#define AFFINE_TABLE_H_

#include "utils/commons.h"
#include "edit/edit_cigar.h"

typedef struct {
  int M; // Alignment matching/mismatching
  int I; // Alignment ends with a gap in the reference (insertion)
  int D; // Alignment ends with a gap in the read (deletion)
} affine_cell_t;
typedef struct {
  // DP Table
  affine_cell_t** columns;
  int num_rows;
  int num_columns;
  // CIGAR
  edit_cigar_t edit_cigar;
} affine_table_t;

/*
 * Gap-Affine Table Setup
 */
void affine_table_allocate(
    affine_table_t* const table,
    const int pattern_length,
    const int text_length,
    mm_allocator_t* const mm_allocator);
void affine_table_free(
    affine_table_t* const table,
    mm_allocator_t* const mm_allocator);

/*
 * Display
 */
void affine_table_print(
    FILE* const stream,
    const affine_table_t* const table,
    const char* const pattern,
    const char* const text);
void affine_table_print_extended(
    FILE* const stream,
    const affine_table_t* const table,
    const char* const pattern,
    const char* const text);

#endif /* AFFINE_TABLE_H_ */
