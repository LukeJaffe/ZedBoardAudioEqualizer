################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/adau1761.c \
../src/audioPlayer.c \
../src/audioRxTx.c \
../src/audioSample.c \
../src/bufferPool_d.c \
../src/chunk_d.c \
../src/fprof.c \
../src/frequencyScaling.c \
../src/ipprof.c \
../src/kiss_fft.c \
../src/kiss_fftr.c \
../src/main.c \
../src/snd_sample.c 

OBJS += \
./src/adau1761.o \
./src/audioPlayer.o \
./src/audioRxTx.o \
./src/audioSample.o \
./src/bufferPool_d.o \
./src/chunk_d.o \
./src/fprof.o \
./src/frequencyScaling.o \
./src/ipprof.o \
./src/kiss_fft.o \
./src/kiss_fftr.o \
./src/main.o \
./src/snd_sample.o 

C_DEPS += \
./src/adau1761.d \
./src/audioPlayer.d \
./src/audioRxTx.d \
./src/audioSample.d \
./src/bufferPool_d.d \
./src/chunk_d.d \
./src/fprof.d \
./src/frequencyScaling.d \
./src/ipprof.d \
./src/kiss_fft.d \
./src/kiss_fftr.d \
./src/main.d \
./src/snd_sample.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../freertos_bsp_3_1_1/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


