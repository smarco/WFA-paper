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

#include "edit/edit_table.h"
#include "gap_affine/affine_table.h"

/*
 * Gap-Affine Table Setup
 */
void affine_table_allocate(
    affine_table_t* const table,
    const int pattern_length,
    const int text_length,
    mm_allocator_t* const mm_allocator) {
  // Allocate DP table
  int h;
  table->num_rows = pattern_length+1;
  const int num_columns = text_length+1;
  table->num_columns = num_columns;
  table->columns = mm_allocator_malloc(mm_allocator,(text_length+1)*sizeof(affine_cell_t*)); // Columns
  for (h=0;h<num_columns;++h) {
    table->columns[h] = mm_allocator_calloc(mm_allocator,pattern_length+1,affine_cell_t,false); // Rows
  }
  // CIGAR
  edit_cigar_allocate(&table->edit_cigar,pattern_length,text_length,mm_allocator);
}
void affine_table_free(
    affine_table_t* const table,
    mm_allocator_t* const mm_allocator) {
  const int num_columns = table->num_columns;
  int h;
  for (h=0;h<num_columns;++h) {
    mm_allocator_free(mm_allocator,table->columns[h]);
  }
  mm_allocator_free(mm_allocator,table->columns);
  // CIGAR
  edit_cigar_free(&table->edit_cigar,mm_allocator);
}
/*
 * Display
 */
#define AFFINE_TABLE_PRINT_VALUE(value)  \
  if (value >= 0 && value < SCORE_MAX) { \
    fprintf(stream,"%2d",value); \
  } else { \
    fprintf(stream," *"); \
  }
void affine_table_print(
    FILE* const stream,
    const affine_table_t* const table,
    const char* const pattern,
    const char* const text) {
  // Parameters
  affine_cell_t** const dp = table->columns;
  int i, j;
  // Print Header
  fprintf(stream,"     ");
  for (i=0;i<table->num_columns-1;++i) {
    fprintf(stream," %c ",text[i]);
  }
  fprintf(stream,"\n ");
  for (i=0;i<table->num_columns;++i) {
    fprintf(stream," ");
    AFFINE_TABLE_PRINT_VALUE(dp[i][0].M);
  }
  fprintf(stream,"\n");
  // Print Rows
  for (i=1;i<table->num_rows;++i) {
    fprintf(stream,"%c",pattern[i-1]);
    for (j=0;j<table->num_columns;++j) {
      fprintf(stream," ");
      AFFINE_TABLE_PRINT_VALUE(dp[j][i].M);
    }
    fprintf(stream,"\n");
  }
  fprintf(stream,"\n");
}
void affine_table_print_extended(
    FILE* const stream,
    const affine_table_t* const table,
    const char* const pattern,
    const char* const text) {
  // Parameters
  affine_cell_t** const dp = table->columns;
  int i, j;
  // Print Header
  fprintf(stream,"         ");
  for (i=0;i<table->num_columns-1;++i) {
    fprintf(stream,"     %c     ",text[i]);
  }
  fprintf(stream,"\n ");
  for (i=0;i<table->num_columns;++i) {
    fprintf(stream," ");
    AFFINE_TABLE_PRINT_VALUE(dp[i][0].M);
    fprintf(stream,"{");
    AFFINE_TABLE_PRINT_VALUE(dp[i][0].I);
    fprintf(stream,",");
    AFFINE_TABLE_PRINT_VALUE(dp[i][0].D);
    fprintf(stream,"} ");
  }
  fprintf(stream,"\n");
  // Print Rows
  for (i=1;i<table->num_rows;++i) {
    fprintf(stream,"%c",pattern[i-1]);
    for (j=0;j<table->num_columns;++j) {
      fprintf(stream," ");
      AFFINE_TABLE_PRINT_VALUE(dp[j][i].M);
      fprintf(stream,"{");
      AFFINE_TABLE_PRINT_VALUE(dp[j][i].I);
      fprintf(stream,",");
      AFFINE_TABLE_PRINT_VALUE(dp[j][i].D);
      fprintf(stream,"} ");
    }
    fprintf(stream,"\n");
  }
  fprintf(stream,"\n");
}
