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
 * DESCRIPTION: Wavefront Alignments Algorithms benchmarking tool
 */

#include "utils/commons.h"
#include "system/profiler_timer.h"

#include "edit/edit_table.h"
#include "edit/edit_dp.h"
#include "gap_lineal/nw.h"
#include "gap_affine/affine_wavefront.h"
#include "gap_affine/swg.h"

#include "benchmark/benchmark_edit.h"
#include "benchmark/benchmark_gap_affine.h"
#include "benchmark/benchmark_gap_lineal.h"
#include "benchmark/benchmark_bitpal.h"
#include "benchmark/benchmark_gaba.h"
#include "benchmark/benchmark_ksw2.h"
#include "benchmark/benchmark_parasail.h"
#include "benchmark/benchmark_seqan.h"

/*
 * Algorithms
 */
typedef enum {
  alignment_edit_dp,
  alignment_edit_dp_banded,
  alignment_gap_lineal_nw,
  alignment_bitpal_edit,
  alignment_bitpal_scored,
  alignment_gap_affine_swg,
  alignment_gap_affine_swg_banded,
  alignment_gap_affine_wavefront,
  alignment_parasail_nw_stripped,
  alignment_parasail_nw_scan,
  alignment_parasail_nw_diag,
  alignment_parasail_nw_banded,
  alignment_aband_gaba,
  alignment_ksw2_extz2_sse,
  alignment_ksw2_extd2_sse,
  alignment_seqan_edit,
  alignment_seqan_edit_bpm,
  alignment_seqan_lineal,
  alignment_seqan_affine
} alg_algorithm_type;

/*
 * Generic parameters
 */
typedef struct {
  // Input
  char *algorithm;
  char *input;
  // Penalties
  lineal_penalties_t lineal_penalties;
  affine_penalties_t affine_penalties;
  // Specific parameters
  int bandwidth;
  wavefront_reduction_type reduction_type;
  int min_wavefront_length;
  int max_distance_threshold;
  bool ksw2_approx_max__drop;
  int ksw2_bandwidth;
  int ksw2_zdrop;
  // Profile
  profiler_timer_t timer_global;
  int progress;
  // Check
  bool check_correct;
  bool check_score;
  bool check_alignments;
  int check_metric;
  int check_bandwidth;
  bool verbose;
} benchmark_args;
benchmark_args parameters = {
  // Input
  .algorithm=NULL,
  .input=NULL,
  // Penalties
  .lineal_penalties = {
      .match = -1,
      .mismatch = 4,
      .insertion = 2,
      .deletion  = 2,
  },
  .affine_penalties = {
      .match = 0,
      .mismatch = 4,
      .gap_opening = 6,
      .gap_extension = 2,
  },
  // Specific parameters
  .bandwidth = 10,
  .reduction_type = wavefronts_reduction_none,
  .min_wavefront_length = 10,
  .max_distance_threshold = 50,
  .ksw2_approx_max__drop = false,
  .ksw2_bandwidth = -1,
  .ksw2_zdrop = -1,
  // Check
  .check_correct = false,
  .check_score = false,
  .check_alignments = false,
  .check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE,
  .check_bandwidth = -1,
  .progress = 10000,
  .verbose = false
};

/*
 * Benchmark UTest
 */
void align_pairwise_test() {
  // Patters & Texts
  char* pattern = "TCTTTACTCGCGCGTTGGAGAAATACAATAGT";
  char* text    = "TCTATACTGCGCGTTTGGAGAAATAAAATAGT";
  // Configure align_input
  mm_allocator_t* const mm_allocator = mm_allocator_new(BUFFER_SIZE_8M);
  align_input_t align_input = {
      .sequence_id = 0,
      .pattern = pattern,
      .pattern_length = strlen(pattern),
      .text = text,
      .text_length = strlen(text),
      .mm_allocator = mm_allocator,
      .debug_flags = /* ALIGN_DEBUG_DISPLAY_INFO | */ ALIGN_DEBUG_CHECK_SCORE | ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE,
      .verbose = true,
  };
  benchmark_align_input_clear(&align_input);
  /*
   * Edit
   */
  // Edit-DP
  //   benchmark_edit_dp(&align_input);
  //   benchmark_edit_dp_banded(&align_input,5);
  //   benchmark_seqan_global_edit(&align_input);
  /*
   * NW
   */
  //  benchmark_nw_dp(&align_input,&parameters.gap_lineal_penalties);
  /*
   * SWG
   */
  align_input.debug_flags = 0;
  align_input.check_lineal_penalties = &parameters.lineal_penalties;
  align_input.check_affine_penalties = &parameters.affine_penalties;

  parameters.affine_penalties.match = 0;
  parameters.affine_penalties.mismatch = 6;
  parameters.affine_penalties.gap_opening = 5;
  parameters.affine_penalties.gap_extension = 3;

  //  benchmark_gap_affine_swg(&align_input,&parameters.affine_penalties);
  //  benchmark_gap_affine_swg_banded(&align_input,&parameters.affine_penalties,30);
  benchmark_gap_affine_wavefront(
      &align_input,&parameters.affine_penalties,10,50);
  //  benchmark_ksw2_extz2_sse(&align_input,&parameters.affine_penalties,false,1000,100);
  //  benchmark_ksw2_extd2_sse(&align_input,&parameters.affine_penalties,false,-1,-1);
  // Free
  mm_allocator_delete(mm_allocator);
}
/*
 * Benchmark
 */
void align_benchmark(const alg_algorithm_type alg_algorithm) {
  // Parameters
  FILE *input_file = NULL;
  char *line1 = NULL, *line2 = NULL;
  int line1_length=0, line2_length=0;
  size_t line1_allocated=0, line2_allocated=0;
  align_input_t align_input;
  // Init
  timer_restart(&(parameters.timer_global));
  input_file = fopen(parameters.input, "r");
  if (input_file==NULL) {
    fprintf(stderr,"Input file '%s' couldn't be opened\n",parameters.input);
    exit(1);
  }
  benchmark_align_input_clear(&align_input);
  align_input.debug_flags = 0;
  align_input.debug_flags |= parameters.check_metric;
  if (parameters.check_correct) align_input.debug_flags |= ALIGN_DEBUG_CHECK_CORRECT;
  if (parameters.check_score) align_input.debug_flags |= ALIGN_DEBUG_CHECK_SCORE;
  if (parameters.check_alignments) align_input.debug_flags |= ALIGN_DEBUG_CHECK_ALIGNMENT;
  align_input.check_lineal_penalties = &parameters.lineal_penalties;
  align_input.check_affine_penalties = &parameters.affine_penalties;
  align_input.check_bandwidth = parameters.check_bandwidth;
  align_input.verbose = parameters.verbose;
  align_input.mm_allocator = mm_allocator_new(BUFFER_SIZE_8M);
  timer_reset(&align_input.timer);
  // Read-align loop
  int reads_processed = 0, progress = 0;
  while (true) {
    // Read queries
    line1_length = getline(&line1,&line1_allocated,input_file);
    if (line1_length==-1) break;
    line2_length = getline(&line2,&line2_allocated,input_file);
    if (line1_length==-1) break;
    // Configure input
    align_input.sequence_id = reads_processed;
    align_input.pattern = line1+1;
    align_input.pattern_length = line1_length-2;
    align_input.pattern[align_input.pattern_length] = '\0';
    align_input.text = line2+1;
    align_input.text_length = line2_length-2;
    align_input.text[align_input.text_length] = '\0';
    // Align queries using DP
    switch (alg_algorithm) {
      case alignment_edit_dp:
        benchmark_edit_dp(&align_input);
        break;
      case alignment_edit_dp_banded:
        benchmark_edit_dp_banded(&align_input,parameters.bandwidth);
        break;
      case alignment_gap_lineal_nw:
        benchmark_gap_lineal_nw(&align_input,&parameters.lineal_penalties);
        break;
      case alignment_bitpal_edit:
        benchmark_bitpal_m0_x1_g1(&align_input);
        break;
      case alignment_bitpal_scored:
        benchmark_bitpal_m1_x4_g2(&align_input);
        break;
      case alignment_gap_affine_swg:
        benchmark_gap_affine_swg(&align_input,&parameters.affine_penalties);
        break;
      case alignment_gap_affine_swg_banded:
        benchmark_gap_affine_swg_banded(&align_input,
            &parameters.affine_penalties,parameters.bandwidth);
        break;
      case alignment_gap_affine_wavefront:
        benchmark_gap_affine_wavefront(
            &align_input,&parameters.affine_penalties,
            parameters.min_wavefront_length,
            parameters.max_distance_threshold);
        break;
      case alignment_parasail_nw_stripped:
        benchmark_parasail_nw_stripped(&align_input,&parameters.affine_penalties);
        break;
      case alignment_parasail_nw_scan:
        benchmark_parasail_nw_scan(&align_input,&parameters.affine_penalties);
        break;
      case alignment_parasail_nw_diag:
        benchmark_parasail_nw_diag(&align_input,&parameters.affine_penalties);
        break;
      case alignment_parasail_nw_banded:
        benchmark_parasail_nw_banded(&align_input,&parameters.affine_penalties,parameters.bandwidth);
        break;
      case alignment_aband_gaba:
        benchmark_aband_gaba(&align_input,&parameters.affine_penalties);
        break;
      case alignment_ksw2_extz2_sse:
        benchmark_ksw2_extz2_sse(
            &align_input,&parameters.affine_penalties,
            parameters.ksw2_approx_max__drop,
            parameters.ksw2_bandwidth,parameters.ksw2_zdrop);
        break;
      case alignment_ksw2_extd2_sse:
        benchmark_ksw2_extd2_sse(
            &align_input,&parameters.affine_penalties,
            parameters.ksw2_approx_max__drop,
            parameters.ksw2_bandwidth,parameters.ksw2_zdrop);
        break;
      case alignment_seqan_edit:
        benchmark_seqan_global_edit(&align_input);
        break;
      case alignment_seqan_edit_bpm:
        benchmark_seqan_global_edit_bpm(&align_input);
        break;
      case alignment_seqan_lineal:
        benchmark_seqan_global_lineal(&align_input,&parameters.lineal_penalties);
        break;
      case alignment_seqan_affine:
        benchmark_seqan_global_affine(&align_input,&parameters.affine_penalties);
        break;
      default:
        fprintf(stderr,"Algorithm unknown or not implemented\n");
        exit(1);
        break;
    }
    // Update progress
    ++reads_processed;
    // DEBUG mm_allocator_print(stderr,align_input.mm_allocator,true);
    if (++progress == parameters.progress) {
      progress = 0;
      // Compute speed
      const uint64_t time_elapsed_global = timer_elapsed_ns(&(parameters.timer_global));
      const float rate_global = (float)reads_processed/(float)TIMER_CONVERT_NS_TO_S(time_elapsed_global);
      const uint64_t time_elapsed_alg = timer_elapsed_ns(&(align_input.timer));
      const float rate_alg = (float)reads_processed/(float)TIMER_CONVERT_NS_TO_S(time_elapsed_alg);
      fprintf(stderr,"...processed %d reads (benchmark=%2.3f reads/s;alignment=%2.3f reads/s)\n",
          reads_processed,rate_global,rate_alg);
    }
  }
  timer_stop(&(parameters.timer_global));
  // Print benchmark results
  fprintf(stderr,"[Benchmark]\n");
  fprintf(stderr,"=> Total.reads            %d\n",reads_processed);
  fprintf(stderr,"=> Time.Benchmark      ");
  timer_print(stderr,&parameters.timer_global,NULL);
  fprintf(stderr,"  => Time.Alignment    ");
  timer_print(stderr,&align_input.timer,&parameters.timer_global);
  // Print Stats
  if (align_input.debug_flags!=0) {
    const bool print_wf_stats = (alg_algorithm == alignment_gap_affine_wavefront);
    benchmark_print_stats(stderr,&align_input,print_wf_stats);
  }
  // Free
  fclose(input_file);
  mm_allocator_delete(align_input.mm_allocator);
  free(line1);
  free(line2);
}
/*
 * Generic Menu
 */
void usage() {
  fprintf(stderr,
      "USE: ./align_benchmark -i input                                      \n"
      "      Options::                                                      \n"
      "        [Input]                                                      \n"
      "          --algorithm|a <algorithm>                                  \n"
      "            [edit]                                                   \n"
      "              edit-dp                                                \n"
      "              edit-dp-banded                                         \n"
      "            [gap-lineal]                                             \n"
      "              gap-lineal-nw                                          \n"
      "            [gap-affine]                                             \n"
      "              gap-affine-swg                                         \n"
      "              gap-affine-swg-banded                                  \n"
      "              gap-affine-wfa                                         \n"
      "              gap-affine-wfa-adaptive                                \n"
      "            [parasail]                                               \n"
      "              parasail-nw-stripped                                   \n"
      "              parasail-nw-scan                                       \n"
      "              parasail-nw-diag                                       \n"
      "              parasail-nw-banded                                     \n"
      "            [Adaptive Banded]                                        \n"
      "              aband-gaba                                             \n"
      "            [KSW2]                                                   \n"
      "              ksw2-extz2-sse                                         \n"
      "              ksw2-extd2-sse                                         \n"
      "            [BitPal]                                                 \n"
      "              bitpal-edit                                            \n"
      "              bitpal-scored                                          \n"
      "            [SeqAn]                                                  \n"
      "              seqan-edit                                             \n"
      "              seqan-edit-bpm                                         \n"
      "              seqan-lineal                                           \n"
      "              seqan-affine                                           \n"
      "          --input|i <File>                                           \n"
      "        [Penalties]                                                  \n"
      "          --lineal-penalties|p M,X,I,D                               \n"
      "          --affine-penalties|g M,X,O,E                               \n"
      "        [Specifics]                                                  \n"
      "          --bandwidth <INT>                                          \n"
      "          --ksw2-approximate                                         \n"
      "          --minimum-wavefront-length <INT>                           \n"
      "          --maximum-difference-distance <INT>                        \n"
      "        [Misc]                                                       \n"
      "          --progress|P <integer>                                     \n"
      "          --check|c 'correct'|'score'|'alignment'                    \n"
      "          --check-distance 'edit'|'gap-lineal'|'gap-affine'          \n"
      "          --check-bandwidth <INT>                                    \n"
      "          --help|h                                                   \n");
}
void parse_arguments(int argc,char** argv) {
  struct option long_options[] = {
    /* Input */
    { "algorithm", required_argument, 0, 'a' },
    { "input", required_argument, 0, 'i' },
    /* Penalties */
    { "lineal-penalties", required_argument, 0, 'p' },
    { "affine-penalties", required_argument, 0, 'g' },
    /* Specifics */
    { "bandwidth", required_argument, 0, 1000 },
    { "ksw2-approximate", no_argument, 0, 1001 },
    { "minimum-wavefront-length", required_argument, 0, 1002 },
    { "maximum-difference-distance", required_argument, 0, 1003 },
    /* Misc */
    { "progress", required_argument, 0, 'P' },
    { "check", optional_argument, 0, 'c' },
    { "check-distance", required_argument, 0, 2000 },
    { "check-bandwidth", required_argument, 0, 2001 },
    { "verbose", no_argument, 0, 'v' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 } };
  int c,option_index;
  if (argc <= 1) {
    usage();
    exit(0);
  }
  while (1) {
    c=getopt_long(argc,argv,"a:i:p:g:P:c:vh",long_options,&option_index);
    if (c==-1) break;
    switch (c) {
    /*
     * Input
     */
    case 'a':
      parameters.algorithm = optarg;
      break;
    case 'i':
      parameters.input = optarg;
      break;
    /*
     * Penalties
     */
    case 'p': { // --lineal-penalties
      char* sentinel = strtok(optarg,",");
      parameters.lineal_penalties.match = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.lineal_penalties.mismatch = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.lineal_penalties.insertion = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.lineal_penalties.deletion = atoi(sentinel);
      break;
    }
    case 'g': { // --affine-penalties
      char* sentinel = strtok(optarg,",");
      parameters.affine_penalties.match = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.affine_penalties.mismatch = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.affine_penalties.gap_opening = atoi(sentinel);
      sentinel = strtok(NULL,",");
      parameters.affine_penalties.gap_extension = atoi(sentinel);
      break;
    }
    /*
     * Specific parameters
     */
    case 1000: // --bandwidth
      parameters.bandwidth = atoi(optarg);
      break;
    case 1001: // --ksw2-approximate
      parameters.ksw2_bandwidth = 100;
      parameters.ksw2_zdrop = 100;
      break;
    case 1002: // --minimum-wavefront-length
      parameters.min_wavefront_length = atoi(optarg);
      break;
    case 1003: // --maximum-difference-distance
      parameters.max_distance_threshold = atoi(optarg);
      break;
    /*
     * Misc
     */
    case 'P':
      parameters.progress = atoi(optarg);
      break;
    case 'c':
      if (optarg ==  NULL) { // default = score
        parameters.check_correct = true;
        parameters.check_score = true;
        parameters.check_alignments = false;
      } else if (strcasecmp(optarg,"correct")==0) {
        parameters.check_correct = true;
        parameters.check_score = false;
        parameters.check_alignments = false;
      } else if (strcasecmp(optarg,"score")==0) {
        parameters.check_correct = true;
        parameters.check_score = true;
        parameters.check_alignments = false;
      } else if (strcasecmp(optarg,"alignment")==0) {
        parameters.check_correct = true;
        parameters.check_score = true;
        parameters.check_alignments = true;
      } else {
        fprintf(stderr,"Option '--check' must be in {'correct','score','alignment'}\n");
        exit(1);
      }
      break;
    case 2000: // --check-distance
      if (strcasecmp(optarg,"edit")==0) { // default = edit
        parameters.check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_EDIT;
      } else if (strcasecmp(optarg,"gap-lineal")==0) {
        parameters.check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_LINEAL;
      } else if (strcasecmp(optarg,"gap-affine")==0) {
        parameters.check_metric = ALIGN_DEBUG_CHECK_DISTANCE_METRIC_GAP_AFFINE;
      } else {
        fprintf(stderr,"Option '--check-distance' must be in {'edit','gap-lineal','gap-affine'}\n");
        exit(1);
      }
      break;
    case 2001: // --check-bandwidth
      parameters.check_bandwidth = atoi(optarg);
      break;
    case 'v':
      parameters.verbose = true;
      break;
    case 'h':
      usage();
      exit(1);
    // Other
    case '?': default:
      fprintf(stderr,"Option not recognized \n");
      exit(1);
    }
  }
  // Checks
  if (parameters.algorithm==NULL) {
    fprintf(stderr,"Option --algorithm is required \n");
    exit(1);
  }
  if (parameters.input==NULL) {
    fprintf(stderr,"Option --input is required \n");
    exit(1);
  }
}
int main(int argc,char* argv[]) {
  // Parsing command-line options
  parse_arguments(argc,argv);
  // Select option
  if (strcmp(parameters.algorithm,"test")==0) {
    align_pairwise_test();
  /* Edit */
  } else if (strcmp(parameters.algorithm,"edit-dp")==0) {
    align_benchmark(alignment_edit_dp);
  } else if (strcmp(parameters.algorithm,"edit-dp-banded")==0) {
    align_benchmark(alignment_edit_dp_banded);
  /* NW */
  } else if (strcmp(parameters.algorithm,"gap-lineal-nw")==0) {
    align_benchmark(alignment_gap_lineal_nw);
  } else if (strcmp(parameters.algorithm,"bitpal-edit")==0) {
    align_benchmark(alignment_bitpal_edit);
  } else if (strcmp(parameters.algorithm,"bitpal-scored")==0) {
    align_benchmark(alignment_bitpal_scored);
  /* SWG */
  } else if (strcmp(parameters.algorithm,"gap-affine-swg")==0) {
    align_benchmark(alignment_gap_affine_swg);
  } else if (strcmp(parameters.algorithm,"gap-affine-swg-banded")==0) {
    align_benchmark(alignment_gap_affine_swg_banded);
  } else if (strcmp(parameters.algorithm,"gap-affine-wfa")==0) {
    parameters.reduction_type = wavefronts_reduction_none;
    parameters.min_wavefront_length = -1;
    parameters.max_distance_threshold = -1;
    align_benchmark(alignment_gap_affine_wavefront);
  } else if (strcmp(parameters.algorithm,"gap-affine-wfa-adaptive")==0) {
    parameters.reduction_type = wavefronts_reduction_dynamic;
    align_benchmark(alignment_gap_affine_wavefront);
  /* Parasail */
  } else if (strcmp(parameters.algorithm,"parasail-nw-stripped")==0) {
    align_benchmark(alignment_parasail_nw_stripped);
  } else if (strcmp(parameters.algorithm,"parasail-nw-scan")==0) {
    align_benchmark(alignment_parasail_nw_scan);
  } else if (strcmp(parameters.algorithm,"parasail-nw-diag")==0) {
    align_benchmark(alignment_parasail_nw_diag);
  } else if (strcmp(parameters.algorithm,"parasail-nw-banded")==0) {
    align_benchmark(alignment_parasail_nw_banded);
  /* Adaptive Band */
  } else if (strcmp(parameters.algorithm,"aband-gaba")==0) {
    align_benchmark(alignment_aband_gaba);
  /* ksw2 */
  } else if (strcmp(parameters.algorithm,"ksw2-extz2-sse")==0) {
    align_benchmark(alignment_ksw2_extz2_sse);
  } else if (strcmp(parameters.algorithm,"ksw2-extd2-sse")==0) {
    align_benchmark(alignment_ksw2_extd2_sse);
  /* SeqAn */
  } else if (strcmp(parameters.algorithm,"seqan-edit")==0) {
    align_benchmark(alignment_seqan_edit);
  } else if (strcmp(parameters.algorithm,"seqan-edit-bpm")==0) {
    align_benchmark(alignment_seqan_edit_bpm);
  } else if (strcmp(parameters.algorithm,"seqan-lineal")==0) {
    align_benchmark(alignment_seqan_lineal);
  } else if (strcmp(parameters.algorithm,"seqan-affine")==0) {
    align_benchmark(alignment_seqan_affine);
  } else {
    fprintf(stderr,"Algorithm '%s' not recognized\n",parameters.algorithm);
    exit(1);
  }
}
