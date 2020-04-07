/*
 *  Wavefront Alignments Algorithms
 *  Copyright (c) 2019 by Santiago Marco-Sola  <santiagomsola@gmail.com>
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
 * DESCRIPTION: WFA Sample-Code
 */

#include "gap_affine/affine_wavefront_align.h"

int main(int argc,char* argv[]) {
  // Patter & Text
  char* pattern = "TCTTTACTCGCGCGTTGGAGAAATACAATAGT";
  char* text    = "TCTATACTGCGCGTTTGGAGAAATAAAATAGT";
  // Allocate MM
  mm_allocator_t* const mm_allocator = mm_allocator_new(BUFFER_SIZE_8M);
  // Set penalties
  affine_penalties_t affine_penalties = {
      .match = 0,
      .mismatch = 4,
      .gap_opening = 6,
      .gap_extension = 2,
  };
  // Init Affine-WFA
  affine_wavefronts_t* affine_wavefronts = affine_wavefronts_new_complete(
      strlen(pattern),strlen(text),&affine_penalties,NULL,mm_allocator);
  // Align
  affine_wavefronts_align(
      affine_wavefronts,pattern,strlen(pattern),text,strlen(text));
  // Display alignment
  const int score = edit_cigar_score_gap_affine(
      &affine_wavefronts->edit_cigar,&affine_penalties);
  fprintf(stderr,"  PATTERN  %s\n",pattern);
  fprintf(stderr,"  TEXT     %s\n",text);
  fprintf(stderr,"  SCORE COMPUTED %d\t",score);
  edit_cigar_print_pretty(stderr,
      pattern,strlen(pattern),text,strlen(text),
      &affine_wavefronts->edit_cigar,mm_allocator);
  // Free
  affine_wavefronts_delete(affine_wavefronts);
  mm_allocator_delete(mm_allocator);
}
