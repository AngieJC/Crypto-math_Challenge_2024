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

OBJ = my_sampler.o sampler.o fpr.o rng.o shake.o util.o

all: test_time sampler2file test_acc

test: test_acc
	clear && ./test_acc

clean:
	-rm -f $(OBJ) test_time.o test_time sampler2file.o sampler2file test_acc test_acc.o

test_time: test_time.o $(OBJ)
	$(CC) $(LDFLAGS) -o test_time test_time.o $(OBJ) $(LIBS)

sampler2file: sampler2file.o $(OBJ)
	$(CC) $(LDFLAGS) -o sampler2file sampler2file.o $(OBJ) $(LIBS)

test_acc: test_acc.o $(OBJ)
	$(CPP) $(LDFLAGS) -o test_acc test_acc.o $(OBJ) $(LDTEST) $(LIBS)

fpr.o: fpr.c sampler.h fpr.h
	$(CC) $(CFLAGS) -c -o fpr.o fpr.c

rng.o: rng.c sampler.h fpr.h
	$(CC) $(CFLAGS) -c -o rng.o rng.c

shake.o: shake.c sampler.h fpr.h
	$(CC) $(CFLAGS) -c -o shake.o shake.c

sampler.o: sampler.c sampler.h
	$(CC) $(CFLAGS) -c -o sampler.o sampler.c

my_sampler.o: my_sampler.c my_sampler.h
	$(CC) $(CFLAGS) -c -o my_sampler.o my_sampler.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c -o util.o util.c

test_time.o: test_time.c sampler.h
	$(CC) $(CFLAGS) -c -o test_time.o test_time.c

sampler2file.o: sampler2file.c sampler.h
	$(CC) $(CFLAGS) -c -o sampler2file.o sampler2file.c

test_acc.o: test_acc.cpp sampler.h
	$(CPP) $(CPPFLAGS) -c -o test_acc.o test_acc.cpp

