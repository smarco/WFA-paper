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
 * DESCRIPTION: Benchmark utils
 */

#ifndef BENCHMARK_UTILS_H_
#define BENCHMARK_UTILS_H_

#include "utils/commons.h"
#include "system/mm_allocator.h"
#include "system/profiler_timer.h"
#include "edit/edit_table.h"
#include "gap_affine/wavefront_stats.h"

/*
 * Constants
 */
#define ALIGN_DEBUG_CHECK_CORRECT   0x00000001
#define ALIGN_DEBUG_CHECK_SCORE     0x00000002
#define ALIGN_DEBUG_CHECK_ALIGNMENT 0x00000004
#define ALIGN_DEBUG_DISPLAY_INFO    0x00000008

#define ALIGN_DEBUG_CHECK_DISTANCE_METRIC_EDIT       0x00000010
#define ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_LINEAL 0x00000040
#define ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE 0x00000080

/*
 * Alignment Input
 */
typedef struct {
  // Sequence ID
  int sequence_id;
  // Pattern
  char* pattern;
  int pattern_length;
  // Text
  char* text;
  int text_length;
  // Timer
  profiler_timer_t timer;
  // MM
  mm_allocator_t* mm_allocator;
  // Check
  lineal_penalties_t* check_lineal_penalties;
  affine_penalties_t* check_affine_penalties;
  int check_bandwidth;
  // STATS
  profiler_counter_t align;
  profiler_counter_t align_correct;
  profiler_counter_t align_score;
  profiler_counter_t align_score_total;
  profiler_counter_t align_score_diff;
  profiler_counter_t align_cigar;
  profiler_counter_t align_bases;
  profiler_counter_t align_matches;
  profiler_counter_t align_mismatches;
  profiler_counter_t align_del;
  profiler_counter_t align_ins;
  wavefronts_stats_t wavefronts_stats;
  // DEBUG
  int debug_flags;
  bool verbose;
} align_input_t;

/*
 * Setup
 */
void benchmark_align_input_clear(
    align_input_t* const align_input);

/*
 * Check
 */
void benchmark_check_alignment(
    align_input_t* const align_input,
    edit_cigar_t* const edit_cigar_computed);
void benchmark_check_alignment_using_template(
    align_input_t* const align_input,
    edit_cigar_t* const edit_cigar_computed,
    const int score_computed,
    edit_cigar_t* const edit_cigar_correct,
    const int score_correct);

/*
 * Display
 */
void benchmark_print_alignment(
    FILE* const stream,
    align_input_t* const align_input,
    const int score_computed,
    edit_cigar_t* const cigar_computed,
    const int score_correct,
    edit_cigar_t* const cigar_correct);

/*
 * Stats
 */
void benchmark_print_stats(
    FILE* const stream,
    align_input_t* const align_input,
    const bool print_wf_stats);

#endif /* BENCHMARK_UTILS_H_ */
