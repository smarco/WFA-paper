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


#ifndef DNA_TEXT_H_
#define DNA_TEXT_H_

#include "utils/commons.h"

/*
 * Range of DNA Nucleotides
 */
#define DNA_RANGE           4
#define DNA_EXTENDED_RANGE  5

#define DNA_RANGE_BITS 2

/*
 * DNA Nucleotides
 */
#define DNA_CHAR_A 'A'
#define DNA_CHAR_C 'C'
#define DNA_CHAR_G 'G'
#define DNA_CHAR_T 'T'
#define DNA_CHAR_N 'N'

/*
 * Encoded DNA Nucleotides
 */
#define ENC_DNA_CHAR_A 0
#define ENC_DNA_CHAR_C 1
#define ENC_DNA_CHAR_G 2
#define ENC_DNA_CHAR_T 3
#define ENC_DNA_CHAR_N 4

/*
 * Translation tables
 */
extern const uint8_t dna_encode_table[256];
extern const char dna_decode_table[DNA_EXTENDED_RANGE];

/*
 * Translation functions
 */
#define dna_encode(character)     (dna_encode_table[(int)(character)])
#define dna_decode(enc_char)      (dna_decode_table[(int)(enc_char)])

#endif /* DNA_TEXT_H_ */
