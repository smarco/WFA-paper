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
 * DESCRIPTION: DNA text encoding/decoding utils
 */

/*
 * Pragmas
 */
#ifdef __clang__
#pragma GCC diagnostic ignored "-Winitializer-overrides"
#endif

/*
 * Include
 */
#include "utils/dna_text.h"

/*
 * Tables/Conversions Implementation
 */
const uint8_t dna_encode_table[256] =
{
  [0 ... 255] = 4,
  ['A'] = 0, ['C'] = 1, ['G'] = 2,  ['T'] = 3, ['N'] = 4,
  ['a'] = 0, ['c'] = 1, ['g'] = 2,  ['t'] = 3, ['n'] = 4,
};
const char dna_decode_table[DNA_EXTENDED_RANGE] =
{
  [ENC_DNA_CHAR_A] = DNA_CHAR_A,
  [ENC_DNA_CHAR_C] = DNA_CHAR_C,
  [ENC_DNA_CHAR_G] = DNA_CHAR_G,
  [ENC_DNA_CHAR_T] = DNA_CHAR_T,
  [ENC_DNA_CHAR_N] = DNA_CHAR_N,
};

