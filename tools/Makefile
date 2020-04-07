###############################################################################
# Definitions
###############################################################################
FOLDER_ROOT=..
FOLDER_BUILD=../build
FOLDER_BIN=../bin
FOLDER_RESOURCES=../resources

###############################################################################
# Tools
###############################################################################
TOOLS=generate_dataset align_benchmark
TOOLS_SRC=$(addsuffix .c, $(TOOLS))

###############################################################################
# Rules
###############################################################################
LIBS_PARASAIL=$(PWD)/resources/parasail/build/libparasail.a
LIBS_GABA=$(PWD)/resources/libgaba/libgaba.a

OBJS=$(FOLDER_BUILD)/*.o

all: LIBS+=$(LD_FLAGS)
all: LIBS+=$(LIBS_PARASAIL)
all: LIBS+=$(LIBS_GABA)
all: OBJS+=$(FOLDER_RESOURCES)/build/*.o
all: FLAGS=$(CC_FLAGS)
all: $(TOOLS)

align_benchmark: $(FOLDER_BUILD)/*.o align_benchmark.c
	$(CC) $(FLAGS) -I$(FOLDER_ROOT) align_benchmark.c $(OBJS) -o $(FOLDER_BIN)/align_benchmark $(LIBS) -lstdc++
	
generate_dataset: generate_dataset.c
	$(CC) $(CC_FLAGS) generate_dataset.c -o $(FOLDER_BIN)/generate_dataset $(LD_FLAGS)
