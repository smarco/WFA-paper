###############################################################################
# Definitions
###############################################################################
FOLDER_ROOT=..
FOLDER_BUILD=../build

###############################################################################
# Modules
###############################################################################
MODULES=benchmark_bitpal \
        benchmark_edit \
        benchmark_gaba \
        benchmark_gap_affine \
        benchmark_gap_lineal \
        benchmark_ksw2 \
        benchmark_parasail \
        benchmark_seqan \
        benchmark_utils
        
SRCS=$(addsuffix .c, $(MODULES))
OBJS=$(addprefix $(FOLDER_BUILD)/, $(SRCS:.c=.o))

INCLUDE_SEQAN=$(FOLDER_ROOT)/resources/seqan/include/

###############################################################################
# Rules
###############################################################################
all: $(OBJS) $(FOLDER_BUILD)/benchmark_seqan_bridge.o

$(FOLDER_BUILD)/benchmark_gaba.o : benchmark_gaba.c
	$(CC) $(CC_FLAGS) -march=native -I$(FOLDER_ROOT) -c $< -o $@

$(FOLDER_BUILD)/benchmark_seqan_bridge.o : benchmark_seqan_bridge.cpp
	g++ -std=c++14 $(CC_FLAGS) -I$(FOLDER_ROOT) -I$(INCLUDE_SEQAN) -c $< -o $@

# General building rule
$(FOLDER_BUILD)/%.o : %.c
	$(CC) $(CC_FLAGS) -I$(FOLDER_ROOT) -c $< -o $@
	
