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
 * DESCRIPTION: Gap-Affine penalties
 */

#ifndef AFFINE_PENALTIES_H_
#define AFFINE_PENALTIES_H_

#include "utils/commons.h"

typedef struct {
  int match;              // (Penalty representation; usually M <= 0)
  int mismatch;           // (Penalty representation; usually X > 0)
  int gap_opening;        // (Penalty representation; usually O > 0)
  int gap_extension;      // (Penalty representation; usually E > 0)
} affine_penalties_t;

#endif /* AFFINE_PENALTIES_H_ */
