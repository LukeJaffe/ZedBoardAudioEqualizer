CC=gcc
CFLAGS=-I. -I./kiss_fft130 -lsndfile -ggdb
DEPS = kiss_fft.h kiss_fftr.h audioTestbench.h frequencyScaling.h 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

equalizer: audioTestbench.o frequencyScaling.o kiss_fft.o kiss_fftr.o 
	gcc -o equalizer audioTestbench.o frequencyScaling.o kiss_fft.o kiss_fftr.o -I. -Ikiss_fft130 -lsndfile -lm -ggdb
