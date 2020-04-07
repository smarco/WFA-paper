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
  // Set parameters
  affine_penalties_t affine_penalties = {
      .match = 0,
      .mismatch = 4,
      .gap_opening = 6,
      .gap_extension = 2,
  };
  const int min_wavefront_length = 10;
  const int max_distance_threshold = 50;
  // Init Affine-WFA
  affine_wavefronts_t* affine_wavefronts = affine_wavefronts_new_reduced(
      strlen(pattern),strlen(text),&affine_penalties,
      min_wavefront_length,max_distance_threshold,NULL,mm_allocator);
  // Align
  affine_wavefronts_align(
      affine_wavefronts,pattern,strlen(pattern),text,strlen(text));
  // Count mismatches, deletions, and insertions
  int i, misms=0, ins=0, del=0;
  edit_cigar_t* const edit_cigar = &affine_wavefronts->edit_cigar;
  for (i=edit_cigar->begin_offset;i<edit_cigar->end_offset;++i) {
    switch (edit_cigar->operations[i]) {
      case 'M': break;
      case 'X': ++misms; break;
      case 'D': ++del; break;
      case 'I': ++ins; break;
    }
  }
  fprintf(stderr,
      "Alignment contains %d mismatches, %d insertions, "
      "and %d deletions\n",misms,ins,del);
  // Free
  affine_wavefronts_delete(affine_wavefronts);
  mm_allocator_delete(mm_allocator);
}
