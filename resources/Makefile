###############################################################################
# Compile rules
###############################################################################
BUILD_RULES=bitpal_build \
            ksw2_build \
            libgaba_build \
            parasail_build
            
CLEAN_RULES=libgaba_clean \
            parasail_clean

all: prepare $(BUILD_RULES)

prepare:
	-mkdir build

clean: $(CLEAN_RULES)
	-rm -rf build

###############################################################################
# Build rules
###############################################################################
bitpal_build:
	-$(MAKE) --directory=bitpal all

ksw2_build:
	-$(MAKE) --directory=ksw2 all
	-mv ksw2/kalloc.o ksw2/ksw2_gg.o ksw2/ksw2_gg2.o ksw2/ksw2_gg2_sse.o \
	    ksw2/ksw2_extz.o ksw2/ksw2_extz2_sse.o ksw2/ksw2_extd.o ksw2/ksw2_extd2_sse.o \
	    ksw2/ksw2_extf2_sse.o ksw2/ksw2_exts2_sse.o build

libgaba_build:
	-$(MAKE) --directory=libgaba all

parasail_build:
	-cd parasail; ./build.sh

###############################################################################
# Clean rules
###############################################################################
edlib_clean:
	-cd edlib; ./clean.sh;

libgaba_clean:
	-$(MAKE) --directory=libgaba clean

parasail_clean:
	-cd parasail; ./clean.sh;



