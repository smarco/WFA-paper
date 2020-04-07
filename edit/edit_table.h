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
 * DESCRIPTION: DP-table to compute edit alignment
 */

#ifndef EDIT_TABLE_H_
#define EDIT_TABLE_H_

#include "utils/commons.h"
#include "system/mm_allocator.h"
#include "edit/edit_cigar.h"

/*
 * Constants
 */
#define SCORE_MAX (10000000)

/*
 * DP Table
 */
typedef struct {
  // DP Table
  int** columns;
  int num_rows;
  int num_columns;
  // Edit operations
  edit_cigar_t edit_cigar;
} edit_table_t;

/*
 * Setup
 */
void edit_table_allocate(
    edit_table_t* const edit_table,
    const int pattern_length,
    const int text_length,
    mm_allocator_t* const mm_allocator);
void edit_table_free(
    edit_table_t* const edit_table,
    mm_allocator_t* const mm_allocator);

/*
 * Display
 */
void edit_table_print(
    FILE* const stream,
    const edit_table_t* const edit_table,
    const char* const pattern,
    const char* const text);

#endif /* EDIT_TABLE_H_ */
