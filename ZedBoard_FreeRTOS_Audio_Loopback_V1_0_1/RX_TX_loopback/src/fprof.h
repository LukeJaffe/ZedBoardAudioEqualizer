#ifndef FPROF_H
#define FPROF_H

#define CHUNK_BYTES 	(1024)
#define CHUNK_SAMPLES 	(CHUNK_BYTES/4)

#define ARRAY_SIZE		(CHUNK_SAMPLES)
#define SCALING_SIZE	(ARRAY_SIZE/2 + 1)

#define NUM_BANDS       (8) // anchor bands
#define NUM_SUB_BANDS	(ARRAY_SIZE/NUM_BANDS) // bands per anchor
#define MAX_BAND_VAL    (9)
#define MIN_BAND_VAL    (0)

float frequency_scalars[SCALING_SIZE];

// frequency profile struct
typedef struct
{
    int bands[NUM_BANDS];
    int selected;
} fprof_t;

void fprof_init(fprof_t* this);

void fprof_incSel(fprof_t* this);

void fprof_decSel(fprof_t* this);

void fprof_incBand(fprof_t* this);

void fprof_decBand(fprof_t* this);

void fprof_getBands(fprof_t* this, char buffer[12]);

void fprof_getScalars(fprof_t* this, float scalars[ARRAY_SIZE]);

#endif
