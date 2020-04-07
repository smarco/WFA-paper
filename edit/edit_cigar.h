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
 * DESCRIPTION: Edit cigar data-structure (match/mismatch/insertion/deletion)
 */

#ifndef EDIT_CIGAR_H_
#define EDIT_CIGAR_H_

#include "utils/commons.h"
#include "system/mm_allocator.h"
#include "gap_lineal/lineal_penalties.h"
#include "gap_affine/affine_penalties.h"

/*
 * CIGAR
 */
typedef struct {
  char* operations;
  int max_operations;
  int begin_offset;
  int end_offset;
  int score;
} edit_cigar_t;

/*
 * Distance metrics
 */
typedef enum {
  edit,
  gap_lineal,
  gap_affine
} distance_metric_t;

/*
 * Setup
 */
void edit_cigar_allocate(
    edit_cigar_t* const edit_cigar,
    const int pattern_length,
    const int text_length,
    mm_allocator_t* const mm_allocator);
void edit_cigar_free(
    edit_cigar_t* const edit_cigar,
    mm_allocator_t* const mm_allocator);

/*
 * Accessors
 */
int edit_cigar_get_matches(
    edit_cigar_t* const edit_cigar);
void edit_cigar_add_mismatches(
    char* const pattern,
    const int pattern_length,
    char* const text,
    const int text_length,
    edit_cigar_t* const edit_cigar);

/*
 * Score
 */
int edit_cigar_score_edit(
    edit_cigar_t* const edit_cigar);
int edit_cigar_score_gap_lineal(
    edit_cigar_t* const edit_cigar,
    lineal_penalties_t* const penalties);
int edit_cigar_score_gap_affine(
    edit_cigar_t* const edit_cigar,
    affine_penalties_t* const penalties);

/*
 * Utils
 */
int edit_cigar_cmp(
    edit_cigar_t* const edit_cigar_a,
    edit_cigar_t* const edit_cigar_b);
void edit_cigar_copy(
    edit_cigar_t* const edit_cigar_dst,
    edit_cigar_t* const edit_cigar_src);
bool edit_cigar_check_alignment(
    FILE* const stream,
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length,
    edit_cigar_t* const edit_cigar,
    const bool verbose);

/*
 * Display
 */
void edit_cigar_print(
    FILE* const stream,
    edit_cigar_t* const edit_cigar);
void edit_cigar_print_pretty(
    FILE* const stream,
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length,
    edit_cigar_t* const edit_cigar,
    mm_allocator_t* const mm_allocator);

#endif /* EDIT_CIGAR_H_ */
