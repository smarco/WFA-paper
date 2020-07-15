###############################################################################
# Flags & Folders
###############################################################################
FOLDER_BIN=bin
FOLDER_BUILD=build

CC=gcc
CPP=g++
LD_FLAGS=-lm -lrt
CC_FLAGS=-Wall -g
AR=ar

###############################################################################
# Compile rules
###############################################################################
SUBDIRS=benchmark \
        edit \
        gap_affine \
        gap_lineal \
        system \
        utils
       
LIB_WFA=$(FOLDER_BUILD)/libwfa.a

all: CC_FLAGS+=-O3
all: MODE=all
all: setup
all: $(SUBDIRS) tools $(LIB_WFA)

debug: setup
debug: MODE=all
debug: $(SUBDIRS) tools $(LIB_WFA)

$(LIB_WFA): $(FOLDER_BUILD)/*.o
	$(AR) -rcs $(LIB_WFA) $(FOLDER_BUILD)/*.o

setup:
	@mkdir -p $(FOLDER_BIN) $(FOLDER_BUILD)

clean:
	rm -rf $(FOLDER_BIN) $(FOLDER_BUILD)
	
resources: resources-all
	
resources-all:
	$(MAKE) --directory=resources all
	
resources-clean:
	$(MAKE) --directory=resources clean

###############################################################################
# Subdir rule
###############################################################################
export
$(SUBDIRS):
	$(MAKE) --directory=$@ all
	
tools:
	$(MAKE) --directory=$@ $(MODE)

.PHONY: $(SUBDIRS) tools

