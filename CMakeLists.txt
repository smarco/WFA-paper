cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(wfa VERSION 1.0.0)

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Falling back to different standard it not allowed.
set(CMAKE_CXX_EXTENSIONS OFF)  # Make sure no compiler-specific features are used.

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -mcx16 -march=native -g -Wno-pointer-arith")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mcx16 -march=native -g -Wno-pointer-arith")

# Build wfa as static library by default
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libraries as shared")

# Define output directories for created binaries and libraries.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
  utils/commons.c
  utils/vector.c
  utils/string_padded.c
  gap_affine/affine_wavefront_backtrace.c
  gap_affine/affine_wavefront_reduction.c
  gap_affine/affine_wavefront_penalties.c
  gap_affine/affine_penalties.c
  gap_affine/affine_wavefront_utils.c
  gap_affine/affine_table.c
  gap_affine/wavefront_stats.c
  gap_affine/swg.c
  gap_affine/affine_wavefront_display.c
  gap_affine/affine_wavefront_align.c
  gap_affine/affine_wavefront.c
  gap_affine/affine_wavefront_extend.c
  gap_lineal/nw.c
  benchmark/benchmark_edit.c
  benchmark/benchmark_utils.c
  benchmark/benchmark_gap_lineal.c
  benchmark/benchmark_gap_affine.c
  system/profiler_timer.c
  system/mm_allocator.c
  system/profiler_counter.c
  edit/edit_dp.c
  edit/edit_cigar.c
  edit/edit_table.c)

# Create libraries.
add_library(wfa ${SOURCES})
set_target_properties(wfa
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION})

target_include_directories(wfa PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

