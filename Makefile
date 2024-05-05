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
CFLAGS = -Wall -Wextra -Wshadow -Wundef -Ofast
CPP = g++
CPPFLAGS = -Wall -Wextra -Wshadow -Wundef -Ofast
LDTEST = -lCppUTest

PGFLAG = #-pg -no-pie
CFLAGS += $(PGFLAG)
LDFLAGS = $(PGFLAG)
LIBS = #-lm

# =====================================================================

OBJ = sampler_1.o sampler_2.o sampler_3.o sampler_4.o sampler.o fpr.o rng.o shake.o util.o

EXE = test_time sampler2file test_acc

all: $(EXE)

test: test_acc
	clear && ./test_acc

clean:
	-rm -f $(OBJ) test_time.o sampler2file.o test_acc.o test_time sampler2file test_acc

test_time: test_time.o $(OBJ)
	$(CC) $(LDFLAGS) -o test_time test_time.o $(OBJ) $(LIBS)

sampler2file: sampler2file.o $(OBJ)
	$(CC) $(LDFLAGS) -o sampler2file sampler2file.o $(OBJ) $(LIBS)

test_acc: test_acc.o $(OBJ)
	$(CPP) $(LDFLAGS) -o test_acc test_acc.o $(OBJ) $(LDTEST) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
