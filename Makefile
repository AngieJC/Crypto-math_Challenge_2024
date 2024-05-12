# Falcon's Gaussian sampling over integers
#

# =====================================================================
#
# Configurable options:
#   CC       C compiler; GCC or Clang are fine; MSVC (2015+) works too.
#   CFLAGS   Compilation flags:
#             * Optimization level -O2 or higher is recommended
#            See config.h for some possible configuration macros.
#   LD       Linker; normally the same command as the compiler.
#   LDFLAGS  Linker options, not counting the extra libs.
#   LIBS     Extra libraries for linking:
#             * If using the native FPU, test_falcon and application
#               code that calls this library may need: -lm
#               (normally not needed on x86, both 32-bit and 64-bit)

CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -Wundef -O3 -march=native -mno-avx
CPP = g++
CPPFLAGS = $(CFLAGS)
LDTEST = -lCppUTest

PGFLAG = #-pg -no-pie
CFLAGS += $(PGFLAG)
LDFLAGS = $(PGFLAG)
LIBS = #-lm

# =====================================================================

.PHONY: all clean test debug

OBJ = sampler_1.o sampler_2.o sampler_3.o sampler_4.o sampler.o fpr.o rng.o shake.o util.o

EXE = test_time sampler2file test_acc benchmark

all: $(EXE)

test: test_acc
	clear && ./test_acc

test_time: test_time.o $(OBJ)
	$(CC) $(LDFLAGS) -o test_time test_time.o $(OBJ) $(LIBS)

sampler2file: sampler2file.o $(OBJ)
	$(CC) $(LDFLAGS) -o sampler2file sampler2file.o $(OBJ) $(LIBS)

test_acc: test_acc.o $(OBJ)
	$(CPP) $(LDFLAGS) -o test_acc test_acc.o $(OBJ) $(LDTEST) $(LIBS)

benchmark: libdgs.a benchmark.o
	$(CC) $(LDFLAGS) -o benchmark benchmark.o libdgs.a -lgmp -lmpfr -lm ./libdgs.a

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

libdgs.a: dgs/dgs_bern.o dgs/dgs_gauss_dp.o dgs/dgs_gauss_mp.o dgs/dgs_rround_dp.o dgs/dgs_rround_mp.o
	ar rcs $@ $^

clean:
	-rm -f *.o $(EXE) libdgs.a

debug:
	$(CC) fpr.c rng.c sampler_1.c sampler_2.c sampler_3.c sampler_4.c sampler.c shake.c util.c test_time.c -o test_time -g
