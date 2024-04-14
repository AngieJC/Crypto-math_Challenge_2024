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

CC = clang
CFLAGS = -Wall -Wextra -Wshadow -Wundef -O3 -march=native #-pg -fno-pie
LD = clang
LDFLAGS = #-pg -no-pie
LIBS = #-lm

# =====================================================================

OBJ = sampler.o fpr.o rng.o shake.o util.o my_sampler.o

all: test_time sampler2file

clean:
	-rm -f $(OBJ) test_time.o test_time

test_time: test_time.o $(OBJ)
	$(LD) $(LDFLAGS) -o test_time test_time.o $(OBJ) $(LIBS)

sampler2file: sampler2file.o $(OBJ)
	$(LD) $(LDFLAGS) -o sampler2file sampler2file.o $(OBJ) $(LIBS)

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

