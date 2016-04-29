#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

#define ARRAY_SIZE (2048)
#define SCALING_SIZE (ARRAY_SIZE / 2 + 1)
#define BUFFER_SIZE ARRAY_SIZE

#define IP_MAX_HEIGHT 16
#define NUM_FREQS 8
