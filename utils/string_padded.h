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
 * DESCRIPTION: Padded string module to avoid handling corner conditions
 */

#ifndef STRING_PADDED_H
#define STRING_PADDED_H

/*
 * Includes
 */
#include "utils/commons.h"
#include "system/mm_allocator.h"

/*
 * Strings Padded
 */
typedef struct {
  // Strings
  char* pattern_padded_buffer;
  char* pattern_padded;
  char* text_padded_buffer;
  char* text_padded;
  // MM
  mm_allocator_t* mm_allocator;
} strings_padded_t;

/*
 * Strings (text/pattern) padded
 */
strings_padded_t* strings_padded_new(
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length,
    const int padding_length,
    mm_allocator_t* const mm_allocator);
strings_padded_t* strings_padded_new_rhomb(
    const char* const pattern,
    const int pattern_length,
    const char* const text,
    const int text_length,
    const int padding_length,
    mm_allocator_t* const mm_allocator);
void strings_padded_delete(
    strings_padded_t* const strings_padded);

#endif /* STRING_PADDED_H */
