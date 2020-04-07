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
 * DESCRIPTION: Dynamic-programming algorithm to compute Levenshtein alignment (edit)
 */

#ifndef EDIT_DP_H_
#define EDIT_DP_H_

#include "utils/commons.h"
#include "edit/edit_table.h"

/*
 * Edit distance computation using raw DP-Table
 */
void edit_dp_compute(
    edit_table_t* const edit_table,
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length);
/*
 * Edit distance computation using raw DP-Table (banded)
 */
void edit_dp_compute_banded(
    edit_table_t* const edit_table,
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length,
    const int bandwidth);

#endif /* EDIT_DP_H_ */
