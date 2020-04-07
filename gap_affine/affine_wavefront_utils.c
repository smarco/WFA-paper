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
 * DESCRIPTION: WFA support utilities
 */

#include "gap_affine/affine_wavefront_utils.h"

/*
 * Accessors
 */
affine_wavefront_t* affine_wavefronts_get_source_mwavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int score) {
  return (score < 0 || affine_wavefronts->mwavefronts[score] == NULL) ?
      &affine_wavefronts->wavefront_null : affine_wavefronts->mwavefronts[score];
}
affine_wavefront_t* affine_wavefronts_get_source_iwavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int score) {
  return (score < 0 || affine_wavefronts->iwavefronts[score] == NULL) ?
      &affine_wavefronts->wavefront_null : affine_wavefronts->iwavefronts[score];
}
affine_wavefront_t* affine_wavefronts_get_source_dwavefront(
    affine_wavefronts_t* const affine_wavefronts,
    const int score) {
  return (score < 0 || affine_wavefronts->dwavefronts[score] == NULL) ?
      &affine_wavefronts->wavefront_null : affine_wavefronts->dwavefronts[score];
}
int affine_wavefronts_diagonal_length(
    affine_wavefronts_t* const affine_wavefronts,
    const int k) {
  if (k >= 0) {
    return MIN(affine_wavefronts->text_length-k,affine_wavefronts->pattern_length);
  } else {
    return MIN(affine_wavefronts->pattern_length+k,affine_wavefronts->text_length);
  }
}
int affine_wavefronts_compute_distance(
    const int pattern_length,
    const int text_length,
    const awf_offset_t offset,
    const int k) {
  const int v = AFFINE_WAVEFRONT_V(k,offset);
  const int h = AFFINE_WAVEFRONT_H(k,offset);
  const int left_v = pattern_length - v;
  const int left_h = text_length - h;
  return MAX(left_v,left_h);
}
/*
 * Initial Conditions and finalization
 */
void affine_wavefront_initialize(
    affine_wavefronts_t* const affine_wavefronts) {
  affine_wavefronts->mwavefronts[0] = affine_wavefronts_allocate_wavefront(affine_wavefronts,0,0);
  affine_wavefronts->mwavefronts[0]->offsets[0] = 0;
}
bool affine_wavefront_end_reached(
    affine_wavefronts_t* const affine_wavefronts,
    const int pattern_length,
    const int text_length,
    const int score) {
  // Parameters
  const int alignment_k = AFFINE_WAVEFRONT_DIAGONAL(text_length,pattern_length);
  const int alignment_offset = AFFINE_WAVEFRONT_OFFSET(text_length,pattern_length);
  // Fetch wavefront and check termination
  affine_wavefront_t* const mwavefront = affine_wavefronts->mwavefronts[score];
  if (mwavefront!=NULL) {
    awf_offset_t* const offsets = mwavefront->offsets;
    if (mwavefront->lo <= alignment_k &&
        alignment_k <= mwavefront->hi &&
        offsets[alignment_k] >= alignment_offset) {
      return true;
    }
  }
  return false;
}
